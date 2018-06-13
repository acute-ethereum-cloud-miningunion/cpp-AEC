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
/** @file aecashClient.cpp
 * @author Gav Wood <i@gavwood.com>
 * @date 2015
 */

#include "aecashClient.h"
#include "aecash.h"
#include <boost/filesystem/path.hpp>
using namespace std;
using namespace dev;
using namespace dev::aec;
using namespace p2p;
namespace fs = boost::filesystem;

aecashClient& dev::aec::asaecashClient(Interface& _c)
{
    if (dynamic_cast<aecash*>(_c.sealEngine()))
        return dynamic_cast<aecashClient&>(_c);
    throw InvalidSealEngine();
}

aecashClient* dev::aec::asaecashClient(Interface* _c)
{
    if (dynamic_cast<aecash*>(_c->sealEngine()))
        return &dynamic_cast<aecashClient&>(*_c);
    throw InvalidSealEngine();
}

DEV_SIMPLE_EXCEPTION(ChainParamsNotaecash);

aecashClient::aecashClient(ChainParams const& _params, int _networkID, p2p::Host* _host,
    std::shared_ptr<GasPricer> _gpForAdoption, fs::path const& _dbPath,
    fs::path const& _snapshotPath, WithExisting _forceAction,
    TransactionQueue::Limits const& _limits)
  : Client(
        _params, _networkID, _host, _gpForAdoption, _dbPath, _snapshotPath, _forceAction, _limits)
{
    // will throw if we're not an aecash seal engine.
    asaecashClient(*this);
}

aecashClient::~aecashClient()
{
    m_signalled.notify_all(); // to wake up the thread from Client::doWork()
    terminate();
}

aecash* aecashClient::aecash() const
{
    return dynamic_cast<aecash*>(Client::sealEngine());
}

bool aecashClient::isMining() const
{
    return aecash()->farm().isMining();
}

WorkingProgress aecashClient::miningProgress() const
{
    if (isMining())
        return aecash()->farm().miningProgress();
    return WorkingProgress();
}

u256 aecashClient::hashrate() const
{
    u256 r = externalHashrate();
    if (isMining())
        r += miningProgress().rate();
    return r;
}

std::tuple<h256, h256, h256> aecashClient::getaecashWork()
{
    // lock the work so a later submission isn't invalidated by processing a transaction elsewhere.
    // this will be reset as soon as a new block arrives, allowing more transactions to be processed.
    bool oldShould = shouldServeWork();
    m_lastGetWork = chrono::system_clock::now();

    if (!sealEngine()->shouldSeal(this))
        return std::tuple<h256, h256, h256>();

    // if this request has made us bother to serve work, prep it now.
    if (!oldShould && shouldServeWork())
        onPostStateChanged();
    else
        // otherwise, set this to true so that it gets prepped next time.
        m_remoteWorking = true;
    aecash()->manuallySetWork(m_sealingInfo);
    return std::tuple<h256, h256, h256>(m_sealingInfo.hash(WithoutSeal), aecash::seedHash(m_sealingInfo), aecash::boundary(m_sealingInfo));
}

bool aecashClient::submitaecashWork(h256 const& _mixHash, h64 const& _nonce)
{
    aecash()->manuallySubmitWork(_mixHash, _nonce);
    return true;
}

void aecashClient::submitExternalHashrate(u256 const& _rate, h256 const& _id)
{
    WriteGuard writeGuard(x_externalRates);
    m_externalRates[_id] = make_pair(_rate, chrono::steady_clock::now());
}

u256 aecashClient::externalHashrate() const
{
    u256 ret = 0;
    WriteGuard writeGuard(x_externalRates);
    for (auto i = m_externalRates.begin(); i != m_externalRates.end();)
        if (chrono::steady_clock::now() - i->second.second > chrono::seconds(5))
            i = m_externalRates.erase(i);
        else
            ret += i++->second.first;
    return ret;
}
