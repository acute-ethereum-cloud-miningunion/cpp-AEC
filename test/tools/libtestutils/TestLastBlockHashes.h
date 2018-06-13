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
 * Test implementation for LastBlockHahsesFace
 */

#pragma once

#include <libaecmio/LastBlockHashesFace.h>

namespace dev
{
namespace test
{

class TestLastBlockHashes: public aec::LastBlockHashesFace
{
public:
	explicit TestLastBlockHashes(h256s const& _hashes): m_hashes(_hashes) {}

	h256s precedingHashes(h256 const& /* _mostRecentHash */) const override { return m_hashes; }
	void clear() override {}

private:
	h256s const m_hashes;
};


}
}