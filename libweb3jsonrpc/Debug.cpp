#include <jsonrpccpp/common/exception.h>
#include <libdevcore/CommonIO.h>
#include <libdevcore/CommonJS.h>
#include <libaeccore/CommonJS.h>
#include <libaecmio/Client.h>
#include <libaecmio/Executive.h>
#include "Debug.h"
#include "JsonHelper.h"
using namespace std;
using namespace dev;
using namespace dev::rpc;
using namespace dev::aec;

Debug::Debug(aec::Client const& _aec):
	m_aec(_aec)
{}

StandardTrace::DebugOptions dev::aec::debugOptions(Json::Value const& _json)
{
	StandardTrace::DebugOptions op;
	if (!_json.isObject() || _json.empty())
		return op;
	if (!_json["disableStorage"].empty())
		op.disableStorage = _json["disableStorage"].asBool();
	if (!_json["disableMemory"].empty())
		op.disableMemory = _json["disableMemory"].asBool();
	if (!_json["disableStack"].empty())
		op.disableStack =_json["disableStack"].asBool();
	if (!_json["fullStorage"].empty())
		op.fullStorage = _json["fullStorage"].asBool();
	return op;
}

h256 Debug::blockHash(string const& _blockNumberOrHash) const
{
	if (isHash<h256>(_blockNumberOrHash))
		return h256(_blockNumberOrHash.substr(_blockNumberOrHash.size() - 64, 64));
	try
	{
		return m_aec.blockChain().numberHash(stoul(_blockNumberOrHash));
	}
	catch (...)
	{
		throw jsonrpc::JsonRpcException("Invalid argument");
	}
}

Json::Value Debug::traceTransaction(Executive& _e, Transaction const& _t, Json::Value const& _json)
{
	Json::Value trace;
	StandardTrace st;
	st.setShowMnemonics();
	st.setOptions(debugOptions(_json));
	_e.initialize(_t);
	if (!_e.execute())
		_e.go(st.onOp());
	_e.finalize();
	Json::Reader().parse(st.json(), trace);
	return trace;
}

Json::Value Debug::traceBlock(Block const& _block, Json::Value const& _json)
{
	State s(_block.state());
	s.setRoot(_block.stateRootBeforeTx(0));

	Json::Value traces(Json::arrayValue);
	for (unsigned k = 0; k < _block.pending().size(); k++)
	{
		Transaction t = _block.pending()[k];

		u256 const gasUsed = k ? _block.receipt(k - 1).cumulativeGasUsed() : 0;
		EnvInfo envInfo(_block.info(), m_aec.blockChain().lastBlockHashes(), gasUsed);
		Executive e(s, envInfo, *m_aec.blockChain().sealEngine());

		aec::ExecutionResult er;
		e.setResultRecipient(er);
		traces.append(traceTransaction(e, t, _json));
	}
	return traces;
}

Json::Value Debug::debug_traceTransaction(string const& _txHash, Json::Value const& _json)
{
	Json::Value ret;
	try
	{
		LocalisedTransaction t = m_aec.localisedTransaction(h256(_txHash));
		Block block = m_aec.block(t.blockHash());
		State s(State::Null);
		aec::ExecutionResult er;
		Executive e(s, block, t.transactionIndex(), m_aec.blockChain());
		e.setResultRecipient(er);
		Json::Value trace = traceTransaction(e, t, _json);
		ret["gas"] = toJS(t.gas());
		ret["return"] = toHexPrefixed(er.output);
		ret["structLogs"] = trace;
	}
	catch(Exception const& _e)
	{
		cwarn << diagnostic_information(_e);
	}
	return ret;
}

Json::Value Debug::debug_traceBlock(string const& _blockRLP, Json::Value const& _json)
{
	bytes bytes = fromHex(_blockRLP);
	BlockHeader blockHeader(bytes);
	return debug_traceBlockByHash(blockHeader.hash().hex(), _json);
}

Json::Value Debug::debug_traceBlockByHash(string const& _blockHash, Json::Value const& _json)
{
	Json::Value ret;
	Block block = m_aec.block(h256(_blockHash));
	ret["structLogs"] = traceBlock(block, _json);
	return ret;
}

Json::Value Debug::debug_traceBlockByNumber(int _blockNumber, Json::Value const& _json)
{
	Json::Value ret;
	Block block = m_aec.block(blockHash(std::to_string(_blockNumber)));
	ret["structLogs"] = traceBlock(block, _json);
	return ret;
}

Json::Value Debug::debug_accountRangeAt(
    string const& _blockHashOrNumber, int _txIndex, string const& _addressHash, int _maxResults)
{
    Json::Value ret(Json::objectValue);

    if (_txIndex < 0)
        throw jsonrpc::JsonRpcException("Negative index");
    if (_maxResults <= 0)
        throw jsonrpc::JsonRpcException("Nonpositive maxResults");

    try
    {
        Block block = m_aec.block(blockHash(_blockHashOrNumber));
        size_t const i = std::min(static_cast<size_t>(_txIndex), block.pending().size());
        State state(State::Null);
        createIntermediateState(state, block, i, m_aec.blockChain());
       
        auto const addressMap = state.addresses(h256(_addressHash), _maxResults);

        Json::Value addressList(Json::objectValue);
        for (auto const& record : addressMap.first)
            addressList[toString(record.first)] = toString(record.second);

        ret["addressMap"] = addressList;
        ret["nextKey"] = toString(addressMap.second);
    }
    catch (Exception const& _e)
    {
        cwarn << diagnostic_information(_e);
        throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_RPC_INVALID_PARAMS);
    }

    return ret;
}

Json::Value Debug::debug_storageRangeAt(string const& _blockHashOrNumber, int _txIndex, string const& _address, string const& _begin, int _maxResults)
{
	Json::Value ret(Json::objectValue);
	ret["complete"] = true;
	ret["storage"] = Json::Value(Json::objectValue);

	if (_txIndex < 0)
		throw jsonrpc::JsonRpcException("Negative index");
	if (_maxResults <= 0)
		throw jsonrpc::JsonRpcException("Nonpositive maxResults");

	try
	{
		Block block = m_aec.block(blockHash(_blockHashOrNumber));

		unsigned const i = ((unsigned)_txIndex < block.pending().size()) ? (unsigned)_txIndex : block.pending().size();
		State state(State::Null);
		createIntermediateState(state, block, i, m_aec.blockChain());

		map<h256, pair<u256, u256>> const storage(state.storage(Address(_address)));

		// begin is inclusive
		auto itBegin = storage.lower_bound(h256fromHex(_begin));
		for (auto it = itBegin; it != storage.end(); ++it)
		{
			if (ret["storage"].size() == static_cast<unsigned>(_maxResults))
			{
				ret["nextKey"] = toCompactHexPrefixed(it->first, 1);
				break;
			}

			Json::Value keyValue(Json::objectValue);
			std::string hashedKey = toCompactHexPrefixed(it->first, 1);
			keyValue["key"] = toCompactHexPrefixed(it->second.first, 1);
			keyValue["value"] = toCompactHexPrefixed(it->second.second, 1);

			ret["storage"][hashedKey] = keyValue;
		}
	}
	catch (Exception const& _e)
	{
		cwarn << diagnostic_information(_e);
		throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_RPC_INVALID_PARAMS);
	}

	return ret;
}

std::string Debug::debug_preimage(std::string const& _hashedKey)
{
	h256 const hashedKey(h256fromHex(_hashedKey));
	bytes const key = m_aec.stateDB().lookupAux(hashedKey);

	return key.empty() ? std::string() : toHexPrefixed(key);
}

Json::Value Debug::debug_traceCall(Json::Value const& _call, std::string const& _blockNumber, Json::Value const& _options)
{
	Json::Value ret;
	try
	{
		Block temp = m_aec.block(jsToBlockNumber(_blockNumber));
		TransactionSkeleton ts = toTransactionSkeleton(_call);
		if (!ts.from) {
			ts.from = Address();
		}
		u256 nonce = temp.transactionsFrom(ts.from);
		u256 gas = ts.gas == Invalid256 ? m_aec.gasLimitRemaining() : ts.gas;
		u256 gasPrice = ts.gasPrice == Invalid256 ? m_aec.gasBidPrice() : ts.gasPrice;
		temp.mutableState().addBalance(ts.from, gas * gasPrice + ts.value);
		Transaction transaction(ts.value, gasPrice, gas, ts.to, ts.data, nonce);
		transaction.forceSender(ts.from);
		aec::ExecutionResult er;
		Executive e(temp, m_aec.blockChain().lastBlockHashes());
		e.setResultRecipient(er);
		Json::Value trace = traceTransaction(e, transaction, _options);
		ret["gas"] = toJS(transaction.gas());
		ret["return"] = toHexPrefixed(er.output);
		ret["structLogs"] = trace;
	}
	catch(Exception const& _e)
	{
		cwarn << diagnostic_information(_e);
	}
	return ret;
}