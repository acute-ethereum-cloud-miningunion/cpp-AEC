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
/** @file BlockChainLoader.h
 * @author Marek Kotewicz <marek@aecdev.com>
 * @date 2015
 */

#pragma once
#include <json/json.h>
#include <libdevcore/TransientDirectory.h>
#include <libaecmio/BlockChain.h>
#include <libaecmio/Block.h>
#include <libaecashseal/GenesisInfo.h>

namespace dev
{
namespace test
{

/**
 * @brief Should be used to load test blockchain from json file
 * Loads the blockchain from json, creates temporary directory to store it, removes the directory on dealloc
 */
class BlockChainLoader
{
public:
	explicit BlockChainLoader(Json::Value const& _json, aec::Network _sealEngineNetwork = aec::Network::TransitionnetTest);
	aec::BlockChain const& bc() const { return *m_bc; }
	aec::State const& state() const { return m_block.state(); }	// TODO remove?
	aec::Block const& block() const { return m_block; }

private:
	TransientDirectory m_dir;
	std::unique_ptr<aec::BlockChain> m_bc;
	aec::Block m_block;
};

}
}
