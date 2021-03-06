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
/** @file FixedClient.cpp
 * @author Marek Kotewicz <marek@aecdev.com>
 * @date 2015
 */

#include "FixedClient.h"

using namespace dev;
using namespace dev::aec;
using namespace dev::test;

Block FixedClient::block(h256 const& _h) const
{
	ReadGuard l(x_stateDB);
	Block ret(bc(), m_block.db());
	ret.populateFromChain(bc(), _h);
	return ret;
}
