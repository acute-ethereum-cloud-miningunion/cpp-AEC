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
/** @file aecashProofOfWork.cpp
 * @author Gav Wood <i@gavwood.com>
 * @date 2014
 *
 * Determines the PoW algorithm.
 */

#include "aecashProofOfWork.h"
#include "aecash.h"
using namespace std;
using namespace chrono;
using namespace dev;
using namespace aec;

const unsigned aecashProofOfWork::defaultLocalWorkSize = 64;
const unsigned aecashProofOfWork::defaultGlobalWorkSizeMultiplier = 4096; // * CL_DEFAULT_LOCAL_WORK_SIZE
const unsigned aecashProofOfWork::defaultMSPerBatch = 0;
const aecashProofOfWork::WorkPackage aecashProofOfWork::NullWorkPackage = aecashProofOfWork::WorkPackage();

aecashProofOfWork::WorkPackage::WorkPackage(BlockHeader const& _bh):
	boundary(aecash::boundary(_bh)),
	seedHash(aecash::seedHash(_bh)),
	m_headerHash(_bh.hash(WithoutSeal))
{}

aecashProofOfWork::WorkPackage::WorkPackage(WorkPackage const& _other):
	boundary(_other.boundary),
	seedHash(_other.seedHash),
	m_headerHash(_other.headerHash())
{}

aecashProofOfWork::WorkPackage& aecashProofOfWork::WorkPackage::operator=(aecashProofOfWork::WorkPackage const& _other)
{
	if (this == &_other)
		return *this;
	boundary = _other.boundary;
	seedHash = _other.seedHash;
	h256 headerHash = _other.headerHash();
	{
		Guard l(m_headerHashLock);
		m_headerHash = std::move(headerHash);
	}
	return *this;
}
