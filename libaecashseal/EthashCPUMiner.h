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
/** @file
 * Determines the PoW algorithm.
 */

#pragma once

#include "aecashProofOfWork.h"

#include <libaecmio/GenericMiner.h>

namespace dev
{
namespace aec
{
class aecashCPUMiner : public GenericMiner<aecashProofOfWork>
{
public:
    explicit aecashCPUMiner(GenericMiner<aecashProofOfWork>::ConstructionInfo const& _ci);
    ~aecashCPUMiner() override;

    static unsigned instances()
    {
        return s_numInstances > 0 ? s_numInstances : std::thread::hardware_concurrency();
    }
    static std::string platformInfo();
    static void setNumInstances(unsigned _instances)
    {
        s_numInstances = std::min<unsigned>(_instances, std::thread::hardware_concurrency());
    }

protected:
    void kickOff() override;
    void pause() override;

private:
    static unsigned s_numInstances;

    void startWorking();
    void stopWorking();
    void minerBody();

    std::unique_ptr<std::thread> m_thread;
    std::atomic<bool> m_shouldStop;
};
}  // namespace aec
}  // namespace dev
