/*
    This file is part of cpp-aecmio.

    cpp-aecmio is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    cpp-aecmio is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with cpp-aecmio.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file aecmioHost.h
 * @author Gav Wood <i@gavwood.com>
 * @date 2014
 */

#pragma once

#include <mutex>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <memory>
#include <utility>
#include <thread>

#include <libdevcore/Guards.h>
#include <libdevcore/Worker.h>
#include <libaeccore/Common.h>
#include <libp2p/Common.h>
#include <libdevcore/OverlayDB.h>
#include <libaeccore/BlockHeader.h>
#include <libaecmio/BlockChainSync.h>
#include "CommonNet.h"
#include "aecmioPeer.h"

namespace dev
{

class RLPStream;

namespace aec
{

class TransactionQueue;
class BlockQueue;
class BlockChainSync;

/**
 * @brief The aecmioHost class
 * @warning None of this is thread-safe. You have been warned.
 * @doWork Syncs to peers and sends new blocks and transactions.
 */
class aecmioHost: public p2p::HostCapability<aecmioPeer>, Worker
{
public:
    /// Start server, but don't listen.
    aecmioHost(BlockChain const& _ch, OverlayDB const& _db, TransactionQueue& _tq, BlockQueue& _bq, u256 _networkId);

    /// Will block on network process events.
    virtual ~aecmioHost();

    unsigned protocolVersion() const { return c_protocolVersion; }
    u256 networkId() const { return m_networkId; }
    void setNetworkId(u256 _n) { m_networkId = _n; }

    void reset();
    /// Don't sync further - used only in test mode
    void completeSync();

    bool isSyncing() const;
    bool isBanned(p2p::NodeID const& _id) const { return !!m_banned.count(_id); }

    void noteNewTransactions() { m_newTransactions = true; }
    void noteNewBlocks() { m_newBlocks = true; }
    void onBlockImported(BlockHeader const& _info) { m_sync->onBlockImported(_info); }

    BlockChain const& chain() const { return m_chain; }
    OverlayDB const& db() const { return m_db; }
    BlockQueue& bq() { return m_bq; }
    BlockQueue const& bq() const { return m_bq; }
    SyncStatus status() const;
    h256 latestBlockSent() { return m_latestBlockSent; }
    static char const* stateName(SyncState _s) { return s_stateNames[static_cast<int>(_s)]; }

    static unsigned const c_oldProtocolVersion;
    void foreachPeer(std::function<bool(std::shared_ptr<aecmioPeer>)> const& _f) const;

protected:
    std::shared_ptr<p2p::Capability> newPeerCapability(std::shared_ptr<p2p::SessionFace> const& _s, unsigned _idOffset, p2p::CapDesc const& _cap) override;

private:
    static char const* const s_stateNames[static_cast<int>(SyncState::Size)];

    std::tuple<std::vector<std::shared_ptr<aecmioPeer>>, std::vector<std::shared_ptr<aecmioPeer>>, std::vector<std::shared_ptr<p2p::SessionFace>>> randomSelection(unsigned _percent = 25, std::function<bool(aecmioPeer*)> const& _allow = [](aecmioPeer const*){ return true; });

    /// Sync with the BlockChain. It might contain one of our mined blocks, we might have new candidates from the network.
    virtual void doWork() override;

    void maintainTransactions();
    void maintainBlocks(h256 const& _currentBlock);
    void onTransactionImported(ImportResult _ir, h256 const& _h, h512 const& _nodeId);

    ///	Check to see if the network peer-state initialisation has happened.
    bool isInitialised() const { return (bool)m_latestBlockSent; }

    /// Initialises the network peer-state, doing the stuff that needs to be once-only. @returns true if it really was first.
    bool ensureInitialised();

    virtual void onStarting() override { startWorking(); }
    virtual void onStopping() override { stopWorking(); }

    BlockChain const& m_chain;
    OverlayDB const& m_db;					///< References to DB, needed for some of the aecmio Protocol responses.
    TransactionQueue& m_tq;					///< Maintains a list of incoming transactions not yet in a block on the blockchain.
    BlockQueue& m_bq;						///< Maintains a list of incoming blocks not yet on the blockchain (to be imported).

    u256 m_networkId;

    h256 m_latestBlockSent;
    h256Hash m_transactionsSent;

    std::unordered_set<p2p::NodeID> m_banned;

    bool m_newTransactions = false;
    bool m_newBlocks = false;

    mutable Mutex x_transactions;
    std::shared_ptr<BlockChainSync> m_sync;
    std::atomic<time_t> m_lastTick = { 0 };

    std::shared_ptr<aecmioHostDataFace> m_hostData;
    std::shared_ptr<aecmioPeerObserverFace> m_peerObserver;

    Logger m_logger{createLogger(VerbosityDebug, "host")};
};

}
}