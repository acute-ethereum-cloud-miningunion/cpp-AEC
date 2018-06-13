#pragma once
#include "AdminaecFace.h"

namespace dev
{

namespace aec
{
class Client;
class TrivialGasPricer;
class KeyManager;
}

namespace rpc
{

class SessionManager;

class Adminaec: public AdminaecFace
{
public:
	Adminaec(aec::Client& _aec, aec::TrivialGasPricer& _gp, aec::KeyManager& _keyManager, SessionManager& _sm);

	virtual RPCModules implementedModules() const override
	{
		return RPCModules{RPCModule{"admin", "1.0"}, RPCModule{"miner", "1.0"}};
	}

	virtual bool admin_aec_setMining(bool _on, std::string const& _session) override;
	virtual Json::Value admin_aec_blockQueueStatus(std::string const& _session) override;
	virtual bool admin_aec_setAskPrice(std::string const& _wei, std::string const& _session) override;
	virtual bool admin_aec_setBidPrice(std::string const& _wei, std::string const& _session) override;
	virtual Json::Value admin_aec_findBlock(std::string const& _blockHash, std::string const& _session) override;
	virtual std::string admin_aec_blockQueueFirstUnknown(std::string const& _session) override;
	virtual bool admin_aec_blockQueueRetryUnknown(std::string const& _session) override;
	virtual Json::Value admin_aec_allAccounts(std::string const& _session) override;
	virtual Json::Value admin_aec_newAccount(const Json::Value& _info, std::string const& _session) override;
	virtual bool admin_aec_setMiningBenefactor(std::string const& _uuidOrAddress, std::string const& _session) override;
	virtual Json::Value admin_aec_inspect(std::string const& _address, std::string const& _session) override;
	virtual Json::Value admin_aec_reprocess(std::string const& _blockNumberOrHash, std::string const& _session) override;
	virtual Json::Value admin_aec_vmTrace(std::string const& _blockNumberOrHash, int _txIndex, std::string const& _session) override;
	virtual Json::Value admin_aec_getReceiptByHashAndIndex(std::string const& _blockNumberOrHash, int _txIndex, std::string const& _session) override;
	virtual bool miner_start(int _threads) override;
	virtual bool miner_stop() override;
	virtual bool miner_setaecerbase(std::string const& _uuidOrAddress) override;
	virtual bool miner_setExtra(std::string const& _extraData) override;
	virtual bool miner_setGasPrice(std::string const& _gasPrice) override;
	virtual std::string miner_hashrate() override;

	virtual void setMiningBenefactorChanger(std::function<void(Address const&)> const& _f) { m_setMiningBenefactor = _f; }
private:
	aec::Client& m_aec;
	aec::TrivialGasPricer& m_gp;
	aec::KeyManager& m_keyManager;
	SessionManager& m_sm;
	std::function<void(Address const&)> m_setMiningBenefactor;

	h256 blockHash(std::string const& _blockNumberOrHash) const;
};

}
}
