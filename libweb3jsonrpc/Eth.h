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
/** @file aec.h
 * @authors:
 *   Gav Wood <i@gavwood.com>
 *   Marek Kotewicz <marek@aecdev.com>
 * @date 2014
 */

#pragma once

#include <memory>
#include <iosfwd>
#include <jsonrpccpp/server.h>
#include <jsonrpccpp/common/exception.h>
#include <libdevcore/Common.h>
#include "SessionManager.h"
#include "aecFace.h"


namespace dev
{
class NetworkFace;
class KeyPair;
namespace aec
{
class AccountHolder;
struct TransactionSkeleton;
class Interface;
}

}

namespace dev
{

namespace rpc
{

/**
 * @brief JSON-RPC api implementation
 */
class aec: public dev::rpc::aecFace
{
public:
	aec(aec::Interface& _aec, aec::AccountHolder& _aecAccounts);

	virtual RPCModules implementedModules() const override
	{
		return RPCModules{RPCModule{"aec", "1.0"}};
	}

	aec::AccountHolder const& aecAccounts() const { return m_aecAccounts; }

	virtual std::string aec_protocolVersion() override;
	virtual std::string aec_hashrate() override;
	virtual std::string aec_coinbase() override;
	virtual bool aec_mining() override;
	virtual std::string aec_gasPrice() override;
	virtual Json::Value aec_accounts() override;
	virtual std::string aec_blockNumber()override;
	virtual std::string aec_getBalance(std::string const& _address, std::string const& _blockNumber) override;
	virtual std::string aec_getStorageAt(std::string const& _address, std::string const& _position, std::string const& _blockNumber) override;
	virtual std::string aec_getStorageRoot(std::string const& _address, std::string const& _blockNumber) override;
	virtual std::string aec_getTransactionCount(std::string const& _address, std::string const& _blockNumber) override;
	virtual Json::Value aec_pendingTransactions() override;
	virtual Json::Value aec_getBlockTransactionCountByHash(std::string const& _blockHash) override;
	virtual Json::Value aec_getBlockTransactionCountByNumber(std::string const& _blockNumber) override;
	virtual Json::Value aec_getUncleCountByBlockHash(std::string const& _blockHash) override;
	virtual Json::Value aec_getUncleCountByBlockNumber(std::string const& _blockNumber) override;
	virtual std::string aec_getCode(std::string const& _address, std::string const& _blockNumber) override;
	virtual std::string aec_sendTransaction(Json::Value const& _json) override;
	virtual std::string aec_call(Json::Value const& _json, std::string const& _blockNumber) override;
	virtual std::string aec_estimateGas(Json::Value const& _json) override;
	virtual bool aec_flush() override;
	virtual Json::Value aec_getBlockByHash(std::string const& _blockHash, bool _includeTransactions) override;
	virtual Json::Value aec_getBlockByNumber(std::string const& _blockNumber, bool _includeTransactions) override;
	virtual Json::Value aec_getTransactionByHash(std::string const& _transactionHash) override;
	virtual Json::Value aec_getTransactionByBlockHashAndIndex(std::string const& _blockHash, std::string const& _transactionIndex) override;
	virtual Json::Value aec_getTransactionByBlockNumberAndIndex(std::string const& _blockNumber, std::string const& _transactionIndex) override;
	virtual Json::Value aec_getTransactionReceipt(std::string const& _transactionHash) override;
	virtual Json::Value aec_getUncleByBlockHashAndIndex(std::string const& _blockHash, std::string const& _uncleIndex) override;
	virtual Json::Value aec_getUncleByBlockNumberAndIndex(std::string const& _blockNumber, std::string const& _uncleIndex) override;
	virtual std::string aec_newFilter(Json::Value const& _json) override;
	virtual std::string aec_newFilterEx(Json::Value const& _json) override;
	virtual std::string aec_newBlockFilter() override;
	virtual std::string aec_newPendingTransactionFilter() override;
	virtual bool aec_uninstallFilter(std::string const& _filterId) override;
	virtual Json::Value aec_getFilterChanges(std::string const& _filterId) override;
	virtual Json::Value aec_getFilterChangesEx(std::string const& _filterId) override;
	virtual Json::Value aec_getFilterLogs(std::string const& _filterId) override;
	virtual Json::Value aec_getFilterLogsEx(std::string const& _filterId) override;
	virtual Json::Value aec_getLogs(Json::Value const& _json) override;
	virtual Json::Value aec_getLogsEx(Json::Value const& _json) override;
	virtual Json::Value aec_getWork() override;
	virtual bool aec_submitWork(std::string const& _nonce, std::string const&, std::string const& _mixHash) override;
	virtual bool aec_submitHashrate(std::string const& _hashes, std::string const& _id) override;
	virtual std::string aec_register(std::string const& _address) override;
	virtual bool aec_unregister(std::string const& _accountId) override;
	virtual Json::Value aec_fetchQueuedTransactions(std::string const& _accountId) override;
	virtual std::string aec_signTransaction(Json::Value const& _transaction) override;
	virtual Json::Value aec_inspectTransaction(std::string const& _rlp) override;
	virtual std::string aec_sendRawTransaction(std::string const& _rlp) override;
	virtual bool aec_notePassword(std::string const&) override { return false; }
	virtual Json::Value aec_syncing() override;
	virtual std::string aec_chainId() override;
	
	void setTransactionDefaults(aec::TransactionSkeleton& _t);
protected:

	aec::Interface* client() { return &m_aec; }
	
	aec::Interface& m_aec;
	aec::AccountHolder& m_aecAccounts;

};

}
} //namespace dev
