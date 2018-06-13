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
/** @file aecashCPUMiner.cpp
 * @author Gav Wood <i@gavwood.com>
 * @date 2014
 *
 * Determines the PoW algorithm.
 */

#include "aecashCPUMiner.h"
#include "aecash.h"

#include <aecash/aecash.hpp>

#include <thread>
#include <chrono>
#include <random>

using namespace std;
using namespace dev;
using namespace aec;

unsigned aecashCPUMiner::s_numInstances = 0;


aecashCPUMiner::aecashCPUMiner(GenericMiner<aecashProofOfWork>::ConstructionInfo const& _ci)
  : GenericMiner<aecashProofOfWork>(_ci)
{
}

aecashCPUMiner::~aecashCPUMiner()
{
    stopWorking();
}

void aecashCPUMiner::kickOff()
{
    stopWorking();
    startWorking();
}

void aecashCPUMiner::pause()
{
    stopWorking();
}

void aecashCPUMiner::startWorking()
{
    if (!m_thread)
    {
        m_shouldStop = false;
        m_thread.reset(new thread(&aecashCPUMiner::minerBody, this));
    }
}

void aecashCPUMiner::stopWorking()
{
    if (m_thread)
    {
        m_shouldStop = true;
        m_thread->join();
        m_thread.reset();
    }
}


void aecashCPUMiner::minerBody()
{
    setThreadName("miner" + toString(index()));

    auto tid = std::this_thread::get_id();
    static std::mt19937_64 s_eng((utcTime() + std::hash<decltype(tid)>()(tid)));

    uint64_t tryNonce = s_eng();

    // FIXME: Use epoch number, not seed hash in the work package.
    WorkPackage w = work();

    int epoch = aecash::find_epoch_number(toaecash(w.seedHash));
    auto& aecashContext = aecash::get_global_epoch_context_full(epoch);

    h256 boundary = w.boundary;
    for (unsigned hashCount = 1; !m_shouldStop; tryNonce++, hashCount++)
    {
        auto result = aecash::hash(aecashContext, toaecash(w.headerHash()), tryNonce);
        h256 value = h256(result.final_hash.bytes, h256::ConstructFromPointer);
        if (value <= boundary && submitProof(aecashProofOfWork::Solution{(h64)(u64)tryNonce,
                                     h256(result.mix_hash.bytes, h256::ConstructFromPointer)}))
            break;
        if (!(hashCount % 100))
            accumulateHashes(100);
    }
}

std::string aecashCPUMiner::platformInfo()
{
    string baseline = toString(std::thread::hardware_concurrency()) + "-thread CPU";
    return baseline;
}
