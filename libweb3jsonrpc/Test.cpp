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
/** @file Test.cpp
 * @authors:
 *   Dimitry Khokhlov <dimitry@aecdev.com>
 * @date 2016
 */

#include "Test.h"
#include <jsonrpccpp/common/errors.h>
#include <jsonrpccpp/common/exception.h>
#include <libdevcore/CommonJS.h>
#include <libaecmio/ChainParams.h>
#include <libaecmio/ClientTest.h>

using namespace std;
using namespace dev;
using namespace dev::aec;
using namespace dev::rpc;
using namespace jsonrpc;

Test::Test(aec::Client& _aec): m_aec(_aec) {}

namespace
{
string logEntriesToLogHash(aec::LogEntries const& _logs)
{
    RLPStream s;
    s.appendList(_logs.size());
    for (aec::LogEntry const& l : _logs)
        l.streamRLP(s);
    return toJS(sha3(s.out()));
}
}

string Test::test_getLogHash(string const& _txHash)
{
    try
    {
        h256 txHash;
        try
        {
            txHash = h256(_txHash);
        }
        catch (BadHexCharacter const&)
        {
            throw JsonRpcException(Errors::ERROR_RPC_INVALID_PARAMS);
        }

        if (m_aec.blockChain().isKnownTransaction(txHash))
        {
            LocalisedTransaction t = m_aec.localisedTransaction(txHash);
            BlockReceipts const& blockReceipts = m_aec.blockChain().receipts(t.blockHash());
            if (blockReceipts.receipts.size() != 0)
                return logEntriesToLogHash(blockReceipts.receipts[t.transactionIndex()].log());
        }
        return toJS(dev::EmptyListSHA3);
    }
    catch (std::exception const& ex)
    {
        cwarn << ex.what();
        throw JsonRpcException(Errors::ERROR_RPC_INTERNAL_ERROR);
    }
}

bool Test::test_setChainParams(Json::Value const& param1)
{
    try
    {
        Json::FastWriter fastWriter;
        std::string output = fastWriter.write(param1);
        asClientTest(m_aec).setChainParams(output);
        asClientTest(m_aec).completeSync();  // set sync state to idle for mining
    }
    catch (std::exception const&)
    {
        BOOST_THROW_EXCEPTION(JsonRpcException(Errors::ERROR_RPC_INTERNAL_ERROR));
    }

    return true;
}

bool Test::test_mineBlocks(int _number)
{
    try
    {
        asClientTest(m_aec).mineBlocks(_number);
    }
    catch (std::exception const&)
    {
        BOOST_THROW_EXCEPTION(JsonRpcException(Errors::ERROR_RPC_INTERNAL_ERROR));
    }

    return true;
}

bool Test::test_modifyTimestamp(int _timestamp)
{
    // FIXME: Fix year 2038 issue.
    try
    {
        asClientTest(m_aec).modifyTimestamp(_timestamp);
    }
    catch (std::exception const&)
    {
        BOOST_THROW_EXCEPTION(JsonRpcException(Errors::ERROR_RPC_INTERNAL_ERROR));
    }
    return true;
}

bool Test::test_addBlock(std::string const& _rlp)
{
    try
    {
        asClientTest(m_aec).addBlock(_rlp);
    }
    catch (std::exception const&)
    {
        BOOST_THROW_EXCEPTION(JsonRpcException(Errors::ERROR_RPC_INTERNAL_ERROR));
    }
    return true;
}

bool Test::test_rewindToBlock(int _number)
{
    try
    {
        m_aec.rewind(_number);
        asClientTest(m_aec).completeSync();
    }
    catch (std::exception const&)
    {
        BOOST_THROW_EXCEPTION(JsonRpcException(Errors::ERROR_RPC_INTERNAL_ERROR));
    }
    return true;
}
