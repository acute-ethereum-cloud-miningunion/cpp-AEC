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
/** @file FixedClient.h
 * @author Marek Kotewicz <marek@aecdev.com>
 * @date 2015
 */

#pragma once

#include <libaecmio/ClientBase.h>
#include <libaecmio/BlockChain.h>
#include <libaeccore/Common.h>

namespace dev
{
namespace test
{

/**
 * @brief mvp implementation of ClientBase
 * Doesn't support mining interface
 */
class FixedClient: public dev::aec::ClientBase
{
public:
    FixedClient(aec::BlockChain const& _bc, aec::Block const& _block) :  m_bc(_bc), m_block(_block) {}
    
    // stub
    void flushTransactions() override {}
    aec::Transactions pending() const override { aec::Transactions res; return res; }
    aec::BlockChain& bc() override
    {
        BOOST_THROW_EXCEPTION(InterfaceNotSupported() << errinfo_interface("FixedClient::bc()"));
    }
    aec::BlockChain const& bc() const override { return m_bc; }
    using ClientBase::block;
    aec::Block block(h256 const& _h) const override;
    aec::Block preSeal() const override { ReadGuard l(x_stateDB); return m_block; }
    aec::Block postSeal() const override { ReadGuard l(x_stateDB); return m_block; }
    void setAuthor(Address const& _us) override { WriteGuard l(x_stateDB); m_block.setAuthor(_us); }
    void prepareForTransaction() override {}
    std::pair<h256, Address> submitTransaction(aec::TransactionSkeleton const&, Secret const&) override { return {}; };
    aec::ImportResult injectTransaction(bytes const&, aec::IfDropped) override { return {}; }
    aec::ExecutionResult call(Address const&, u256, Address, bytes const&, u256, u256, aec::BlockNumber, aec::FudgeFactor) override { return {}; };

private:
    aec::BlockChain const& m_bc;
    aec::Block m_block;
    mutable SharedMutex x_stateDB;			///< Lock on the state DB, effectively a lock on m_postSeal.
};

}
}
