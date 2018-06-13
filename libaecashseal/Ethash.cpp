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

#include "aecash.h"
#include "aecashCPUMiner.h"

#include <libaeccore/ChainOperationParams.h>
#include <libaeccore/CommonJS.h>
#include <libaecmio/Interface.h>

#include <aecash/aecash.hpp>

using namespace std;
using namespace dev;
using namespace aec;

void aecash::init()
{
    aec_REGISTER_SEAL_ENGINE(aecash);
}

aecash::aecash()
{
    map<string, GenericFarm<aecashProofOfWork>::SealerDescriptor> sealers;
    sealers["cpu"] = GenericFarm<aecashProofOfWork>::SealerDescriptor{&aecashCPUMiner::instances, [](GenericMiner<aecashProofOfWork>::ConstructionInfo ci){ return new aecashCPUMiner(ci); }};
    m_farm.setSealers(sealers);
    m_farm.onSolutionFound([=](aecashProofOfWork::Solution const& sol)
    {
        std::unique_lock<Mutex> l(m_submitLock);
//        cdebug << m_farm.work().seedHash << m_farm.work().headerHash << sol.nonce << aecashAux::eval(m_farm.work().seedHash, m_farm.work().headerHash, sol.nonce).value;
        setMixHash(m_sealing, sol.mixHash);
        setNonce(m_sealing, sol.nonce);
        if (!quickVerifySeal(m_sealing))
            return false;

        if (m_onSealGenerated)
        {
            RLPStream ret;
            m_sealing.streamRLP(ret);
            l.unlock();
            m_onSealGenerated(ret.out());
        }
        return true;
    });
}

aecash::~aecash()
{
    // onSolutionFound closure sometimes has references to destroyed members.
    m_farm.onSolutionFound({});
}

strings aecash::sealers() const
{
    return {"cpu"};
}

h256 aecash::seedHash(BlockHeader const& _bi)
{
    // FIXME: Use aecash lib for this (function not exposed in 0.3).

    unsigned epoch = static_cast<unsigned>(_bi.number()) / aecASH_EPOCH_LENGTH;

    h256 seed;
    for (unsigned n = 0; n < epoch; ++n)
        seed = sha3(seed);
    return seed;
}

StringHashMap aecash::jsInfo(BlockHeader const& _bi) const
{
    return { { "nonce", toJS(nonce(_bi)) }, { "seedHash", toJS(seedHash(_bi)) }, { "mixHash", toJS(mixHash(_bi)) }, { "boundary", toJS(boundary(_bi)) }, { "difficulty", toJS(_bi.difficulty()) } };
}

void aecash::verify(Strictness _s, BlockHeader const& _bi, BlockHeader const& _parent, bytesConstRef _block) const
{
    SealEngineFace::verify(_s, _bi, _parent, _block);

    if (_s != CheckNothingNew)
    {
        if (_bi.difficulty() < chainParams().minimumDifficulty)
            BOOST_THROW_EXCEPTION(InvalidDifficulty() << RequirementError(bigint(chainParams().minimumDifficulty), bigint(_bi.difficulty())) );

        if (_bi.gasLimit() < chainParams().minGasLimit)
            BOOST_THROW_EXCEPTION(InvalidGasLimit() << RequirementError(bigint(chainParams().minGasLimit), bigint(_bi.gasLimit())) );

        if (_bi.gasLimit() > chainParams().maxGasLimit)
            BOOST_THROW_EXCEPTION(InvalidGasLimit() << RequirementError(bigint(chainParams().maxGasLimit), bigint(_bi.gasLimit())) );

        if (_bi.number() && _bi.extraData().size() > chainParams().maximumExtraDataSize)
            BOOST_THROW_EXCEPTION(ExtraDataTooBig() << RequirementError(bigint(chainParams().maximumExtraDataSize), bigint(_bi.extraData().size())) << errinfo_extraData(_bi.extraData()));

        u256 const& daoHardfork = chainParams().daoHardforkBlock;
        if (daoHardfork != 0 && daoHardfork + 9 >= daoHardfork && _bi.number() >= daoHardfork && _bi.number() <= daoHardfork + 9)
            if (_bi.extraData() != fromHex("0x64616f2d686172642d666f726b"))
                BOOST_THROW_EXCEPTION(ExtraDataIncorrect() << errinfo_comment("Received block from the wrong fork (invalid extradata)."));
    }

    if (_parent)
    {
        // Check difficulty is correct given the two timestamps.
        auto expected = calculateDifficulty(_bi, _parent);
        auto difficulty = _bi.difficulty();
        if (difficulty != expected)
            BOOST_THROW_EXCEPTION(InvalidDifficulty() << RequirementError((bigint)expected, (bigint)difficulty));

        auto gasLimit = _bi.gasLimit();
        auto parentGasLimit = _parent.gasLimit();
        if (
            gasLimit < chainParams().minGasLimit ||
            gasLimit > chainParams().maxGasLimit ||
            gasLimit <= parentGasLimit - parentGasLimit / chainParams().gasLimitBoundDivisor ||
            gasLimit >= parentGasLimit + parentGasLimit / chainParams().gasLimitBoundDivisor)
            BOOST_THROW_EXCEPTION(
                InvalidGasLimit()
                << errinfo_min((bigint)((bigint)parentGasLimit - (bigint)(parentGasLimit / chainParams().gasLimitBoundDivisor)))
                << errinfo_got((bigint)gasLimit)
                << errinfo_max((bigint)((bigint)parentGasLimit + parentGasLimit / chainParams().gasLimitBoundDivisor))
            );
    }

    // check it hashes according to proof of work or that it's the genesis block.
    if (_s == CheckEverything && _bi.parentHash() && !verifySeal(_bi))
    {
        aecash::result result =
            aecash::hash(aecash::get_global_epoch_context(aecash::get_epoch_number(_bi.number())),
                toaecash(_bi.hash(WithoutSeal)), toaecash(nonce(_bi)));

        h256 mix{result.mix_hash.bytes, h256::ConstructFromPointer};
        h256 final{result.final_hash.bytes, h256::ConstructFromPointer};

        InvalidBlockNonce ex;
        ex << errinfo_nonce(nonce(_bi));
        ex << errinfo_mixHash(mixHash(_bi));
        ex << errinfo_seedHash(seedHash(_bi));
        ex << errinfo_aecashResult(make_tuple(final, mix));
        ex << errinfo_hash256(_bi.hash(WithoutSeal));
        ex << errinfo_difficulty(_bi.difficulty());
        ex << errinfo_target(boundary(_bi));
        BOOST_THROW_EXCEPTION(ex);
    }
    else if (_s == QuickNonce && _bi.parentHash() && !quickVerifySeal(_bi))
    {
        InvalidBlockNonce ex;
        ex << errinfo_hash256(_bi.hash(WithoutSeal));
        ex << errinfo_difficulty(_bi.difficulty());
        ex << errinfo_nonce(nonce(_bi));
        BOOST_THROW_EXCEPTION(ex);
    }
}

void aecash::verifyTransaction(ImportRequirements::value _ir, TransactionBase const& _t, BlockHeader const& _header, u256 const& _startGasUsed) const
{
    SealEngineFace::verifyTransaction(_ir, _t, _header, _startGasUsed);

    if (_ir & ImportRequirements::TransactionSignatures)
    {
        if (_header.number() >= chainParams().EIP158ForkBlock)
        {
            int chainID = chainParams().chainID;
            _t.checkChainId(chainID);
        }
        else
            _t.checkChainId(-4);
    }
}

u256 aecash::childGasLimit(BlockHeader const& _bi, u256 const& _gasFloorTarget) const
{
    u256 gasFloorTarget = _gasFloorTarget == Invalid256 ? 3141562 : _gasFloorTarget;
    u256 gasLimit = _bi.gasLimit();
    u256 boundDivisor = chainParams().gasLimitBoundDivisor;
    if (gasLimit < gasFloorTarget)
        return min<u256>(gasFloorTarget, gasLimit + gasLimit / boundDivisor - 1);
    else
        return max<u256>(gasFloorTarget, gasLimit - gasLimit / boundDivisor + 1 + (_bi.gasUsed() * 6 / 5) / boundDivisor);
}

void aecash::manuallySubmitWork(const h256& _mixHash, Nonce _nonce)
{
    m_farm.submitProof(aecashProofOfWork::Solution{_nonce, _mixHash}, nullptr);
}

u256 aecash::calculateDifficulty(BlockHeader const& _bi, BlockHeader const& _parent) const
{
    const unsigned c_expDiffPeriod = 100000;

    if (!_bi.number())
        throw GenesisBlockCannotBeCalculated();
    auto const& minimumDifficulty = chainParams().minimumDifficulty;
    auto const& difficultyBoundDivisor = chainParams().difficultyBoundDivisor;
    auto const& durationLimit = chainParams().durationLimit;

    bigint target;    // stick to a bigint for the target. Don't want to risk going negative.
    if (_bi.number() < chainParams().homesteadForkBlock)
        // Frontier-era difficulty adjustment
        target = _bi.timestamp() >= _parent.timestamp() + durationLimit ? _parent.difficulty() - (_parent.difficulty() / difficultyBoundDivisor) : (_parent.difficulty() + (_parent.difficulty() / difficultyBoundDivisor));
    else
    {
        bigint const timestampDiff = bigint(_bi.timestamp()) - _parent.timestamp();
        bigint const adjFactor = _bi.number() < chainParams().byzantiumForkBlock ?
            max<bigint>(1 - timestampDiff / 10, -99) : // Homestead-era difficulty adjustment
            max<bigint>((_parent.hasUncles() ? 2 : 1) - timestampDiff / 9, -99); // Byzantium-era difficulty adjustment

        target = _parent.difficulty() + _parent.difficulty() / 2048 * adjFactor;
    }

    bigint o = target;
    unsigned exponentialIceAgeBlockNumber = unsigned(_parent.number() + 1);

    // EIP-649 modifies exponentialIceAgeBlockNumber
    if (_bi.number() >= chainParams().byzantiumForkBlock)
    {
        if (exponentialIceAgeBlockNumber >= 3000000)
            exponentialIceAgeBlockNumber -= 3000000;
        else
            exponentialIceAgeBlockNumber = 0;
    }

    unsigned periodCount = exponentialIceAgeBlockNumber / c_expDiffPeriod;
    if (periodCount > 1)
        o += (bigint(1) << (periodCount - 2));    // latter will eventually become huge, so ensure it's a bigint.

    o = max<bigint>(minimumDifficulty, o);
    return u256(min<bigint>(o, std::numeric_limits<u256>::max()));
}

void aecash::populateFromParent(BlockHeader& _bi, BlockHeader const& _parent) const
{
    SealEngineFace::populateFromParent(_bi, _parent);
    _bi.setDifficulty(calculateDifficulty(_bi, _parent));
    _bi.setGasLimit(childGasLimit(_parent));
}

bool aecash::quickVerifySeal(BlockHeader const& _blockHeader) const
{
    h256 const h = _blockHeader.hash(WithoutSeal);
    h256 const b = boundary(_blockHeader);
    Nonce const n = nonce(_blockHeader);
    h256 const m = mixHash(_blockHeader);

    return aecash::verify_final_hash(toaecash(h), toaecash(m), toaecash(n), toaecash(b));
}

bool aecash::verifySeal(BlockHeader const& _blockHeader) const
{
    h256 const h = _blockHeader.hash(WithoutSeal);
    h256 const b = boundary(_blockHeader);
    Nonce const n = nonce(_blockHeader);
    h256 const m = mixHash(_blockHeader);

    auto& context =
        aecash::get_global_epoch_context(aecash::get_epoch_number(_blockHeader.number()));
    return aecash::verify(context, toaecash(h), toaecash(m), toaecash(n), toaecash(b));
}

void aecash::generateSeal(BlockHeader const& _bi)
{
    Guard l(m_submitLock);
    m_sealing = _bi;
    m_farm.setWork(m_sealing);
    m_farm.start(m_sealer);
    m_farm.setWork(m_sealing);
}

bool aecash::shouldSeal(Interface*)
{
    return true;
}
