// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>

#include <chainparamsseeds.h>
#include <consensus/merkle.h>
#include <consensus/consensus.h>
#include <tinyformat.h>
#include <util/system.h>
#include <util/strencodings.h>
#include <util/convert.h>
#include <versionbitsinfo.h>

#include <assert.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

///////////////////////////////////////////// // credit
#include <libdevcore/SHA3.h>
#include <libdevcore/RLP.h>
#include "arith_uint256.h"
/////////////////////////////////////////////

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 00 << 488804799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    genesis.hashStateRoot = uint256(h256Touint(dev::h256("e965ffd002cd6ad0e2dc402b8044de833e06b23127ea8c3d80aec91410771495"))); // credit
    genesis.hashUTXORoot = uint256(h256Touint(dev::sha3(dev::rlp("")))); // credit
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "May 04, 2021 TerraCredit 3.0 started!";
    const CScript genesisOutputScript = CScript() << ParseHex("042bdb58f730ffa8fc0a134dac165cc1a76cac9603b6e13a8790c54ecae41d31274ab87f36589fb2f2ecbff5e3422468679423bb94f34ed879daf2f4c789b08cd5") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

// static bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
// {
//     bool fNegative;
//     bool fOverflow;
//     arith_uint256 bnTarget;

//     bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

//     // Check range
//     if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
//         return false;

//     // Check proof of work matches claimed amount
//     if (UintToArith256(hash) > bnTarget)
//         return false;

//     return true;
// }

/**
 * Main network
 */
class CMainParams : public CChainParams {
public:
    CMainParams() {
        
        strNetworkID = CBaseChainParams::MAIN;
        consensus.nSubsidyHalvingInterval = 525600; // credit halving every 4 years
        consensus.BIP16Exception = uint256S("0x000075aef83cf2853580f8ae8ce6f8c3096cfa21d98334d6e3f95e5582ed986c");
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x000075aef83cf2853580f8ae8ce6f8c3096cfa21d98334d6e3f95e5582ed986c");
        consensus.BIP65Height = 0; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        consensus.BIP66Height = 0; // 00000000000000000379eaa19dce8c9b722d46ae6a57c2f1a988119488b50931
        consensus.CSVHeight = 6048; // 000000000000000004a1b34462cb8aeebd5799177f7a29cf28f2d1961716b5b5
        consensus.SegwitHeight = 6048; // 0000000000000000001c8018d9cb3b742ef25114f27563e3fc4a1902167f9893
        consensus.MinBIP9WarningHeight = 8064; // segwit activation height + miner confirmation window
        consensus.CIP5Height = 0;
        consensus.CIP6Height = 0;
        consensus.CIP7Height = 0;
        consensus.CIP9Height = 0;
        consensus.nOfflineStakeHeight = 1;
        consensus.nReduceBlocktimeHeight = 0;
        consensus.nMuirGlacierHeight = 0;
        consensus.powLimit = uint256S("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimit = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.CIP9PosLimit = uint256S("0000000000001fffffffffffffffffffffffffffffffffffffffffffffffffff"); // The new POS-limit activated after CIP9
        consensus.RBTPosLimit = uint256S("0000000000003fffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 60 * 60; // 60 minutes
        consensus.nPowTargetTimespanV2 = 60 * 60;
        consensus.nRBTPowTargetTimespan = 60 * 60;
        consensus.nPowTargetSpacing = 60 * 2;
        consensus.nRBTPowTargetSpacing = 60 * 2;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = true;
        consensus.fPoSNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 28; // 95% of 2016
        consensus.nMinerConfirmationWindow = 30; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000"); // credit

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x23c66194def65cfea20d32a71f23807a93a0b207b3d7251246e2c351204fe9d3"); // 708000

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xef;
        pchMessageStart[1] = 0xce;
        pchMessageStart[2] = 0xa5;
        pchMessageStart[3] = 0xd2;
        nDefaultPort = 4000;
        nPruneAfterHeight = 100000;
        m_assumed_blockchain_size = 8;
        m_assumed_chain_state_size = 1;

        genesis = CreateGenesisBlock(1620093300, 81470, 0x1f00ffff, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x0000fa4acfb8b65f9be0b19d88836a165788a4cc80c92e7ac98a333dab956cd4"));
        assert(genesis.hashMerkleRoot == uint256S("0x059f12a764f464f8606ca61831505f7d5a613eb930c34b9632986f95c4c9de8c"));

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as a oneshot if they don't support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        vSeeds.emplace_back("seed-01.terra-credit.com"); // Credit mainnet
        vSeeds.emplace_back("seed-02.terra-credit.com"); // Credit mainnet
        vSeeds.emplace_back("seed-03.terra-credit.com"); // Credit mainnet
        vSeeds.emplace_back("seed-04.terra-credit.com"); // Credit mainnet
        vSeeds.emplace_back("seed-05.terra-credit.com"); // Credit mainnet
        vSeeds.emplace_back("explorer.terra-credit.com"); // Credit mainnet

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,66);  // 'T'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,28);  // 'C'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,129);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x05, 0x89, 0xB3, 0x1F};
        base58Prefixes[EXT_SECRET_KEY] = {0x05, 0x89, 0xAE, 0xE5};

        bech32_hrp = "tc";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        m_is_test_chain = false;
        m_is_mockable_chain = false;

        checkpointData = {
            {
                { 0, uint256S("0000fa4acfb8b65f9be0b19d88836a165788a4cc80c92e7ac98a333dab956cd4")},
                { 1000, uint256S("0000db1fbdc01c72c91c3ee0ea62fd39933a2c5c22abf9155af846d86d8e1dfe")}, //last PoW block
                { 2000, uint256S("07c8bf936ce0987811abb84784c69ab3b9dbc0bb30ac7c92c22e986975830d9a")},
            }
        };

        chainTxData = ChainTxData{
            // Data as of block 76b1e67fcff0fcfd078d499c65494cee4319f256da09f5fdefa574433f7d4e3c (height 709065)
            1629728544, // * UNIX timestamp of last known number of transactions
            3121, // * total number of transactions between genesis and that timestamp
            //   (the tx=... number in the SetBestChain debug.log lines)
            0.01869545410952335 // * estimated number of transactions per second after that timestamp
        };

        consensus.nBlocktimeDownscaleFactor = 2;
        consensus.nCoinbaseMaturity = 10;
        consensus.nRBTCoinbaseMaturity = consensus.nBlocktimeDownscaleFactor * 5;
        consensus.nSubsidyHalvingIntervalV2 = consensus.nBlocktimeDownscaleFactor*525600; // credit halving every 4 years (nSubsidyHalvingInterval * nBlocktimeDownscaleFactor)

        consensus.nLastPOWBlock = 1000;
        consensus.nLastBigReward = 1;
        consensus.nMPoSRewardRecipients = 10;
        consensus.nFirstMPoSBlock = consensus.nLastPOWBlock + 
                                    consensus.nMPoSRewardRecipients + 
                                    consensus.nCoinbaseMaturity;
        consensus.nLastMPoSBlock = 1000;

        consensus.nFixUTXOCacheHFHeight = 100000;
        consensus.nEnableHeaderSignatureHeight = 399100;
        consensus.nCheckpointSpan = consensus.nCoinbaseMaturity;
        consensus.nRBTCheckpointSpan = consensus.nRBTCoinbaseMaturity;
        consensus.delegationsAddress = uint160(ParseHex("0000000000000000000000000000000000000086")); // Delegations contract for offline staking
        consensus.nStakeTimestampMask = 15;
        consensus.nRBTStakeTimestampMask = 3;
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = CBaseChainParams::TESTNET;
        consensus.nSubsidyHalvingInterval = 525600; // credit halving every 4 years
        consensus.BIP16Exception = uint256S("0x0000e803ee215c0684ca0d2f9220594d3f828617972aad66feb2ba51f5e14222");
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x0000e803ee215c0684ca0d2f9220594d3f828617972aad66feb2ba51f5e14222");
        consensus.BIP65Height = 0; // 00000000007f6655f22f98e72ed80d8b06dc761d5da09df0fa1dc4be4f861eb6
        consensus.BIP66Height = 0; // 000000002104c8c45e99a8853285a3b592602a3ccde2b832481da85e9e4ba182
        consensus.CSVHeight = 6048; // 00000000025e930139bac5c6c31a403776da130831ab85be56578f3fa75369bb
        consensus.SegwitHeight = 6048; // 00000000002b980fcd729daaa248fd9316a5200e9b367f4ff2c42453e84201ca
        consensus.MinBIP9WarningHeight = 8064; // segwit activation height + miner confirmation window
        consensus.CIP5Height = 0;
        consensus.CIP6Height = 0;
        consensus.CIP7Height = 0;
        consensus.CIP9Height = 0;
        consensus.nOfflineStakeHeight = 1;
        consensus.nReduceBlocktimeHeight = 0;
        consensus.nMuirGlacierHeight = 0;
        consensus.powLimit = uint256S("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimit = uint256S("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.CIP9PosLimit = uint256S("0000000000001fffffffffffffffffffffffffffffffffffffffffffffffffff"); // The new POS-limit activated after CIP9
        consensus.RBTPosLimit = uint256S("0000000000003fffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 60 * 60; // 60 minutes
        consensus.nPowTargetTimespanV2 = 60 * 60;
        consensus.nRBTPowTargetTimespan = 60 * 60;
        consensus.nPowTargetSpacing = 60 * 2;
        consensus.nRBTPowTargetSpacing = 60 * 2;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = true;
        consensus.fPoSNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 22; // 75% for testchains
        consensus.nMinerConfirmationWindow = 30; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000"); // credit

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x89b010b5333fa9d22c7fcf157c7eeaee1ccfe80c435390243b3d782a1fc1eff7"); // 690000

        pchMessageStart[0] = 0x0e;
        pchMessageStart[1] = 0x23;
        pchMessageStart[2] = 0x16;
        pchMessageStart[3] = 0x07;
        nDefaultPort = 14000;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 4;
        m_assumed_chain_state_size = 1;

        genesis = CreateGenesisBlock(1620093301, 7020, 0x1f00ffff, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x0000ee183b69b8798ab94d25ad44ccc23beae0d175218a2cf2b394f384f71003"));
        assert(genesis.hashMerkleRoot == uint256S("0x059f12a764f464f8606ca61831505f7d5a613eb930c34b9632986f95c4c9de8c"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("seed-01.terra-credit.com"); // Credit mainnet
        vSeeds.emplace_back("seed-02.terra-credit.com"); // Credit mainnet
        vSeeds.emplace_back("seed-03.terra-credit.com"); // Credit mainnet
        vSeeds.emplace_back("seed-04.terra-credit.com"); // Credit mainnet
        vSeeds.emplace_back("seed-05.terra-credit.com"); // Credit mainnet
        vSeeds.emplace_back("explorer.terra-credit.com"); // Credit mainnet


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,127); // 't'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,87);  // 'c'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,240);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x05, 0x36, 0x88, 0xD0};
        base58Prefixes[EXT_SECRET_KEY] = {0x05, 0x36, 0x84, 0x95};

        bech32_hrp = "tt";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        m_is_test_chain = true;
        m_is_mockable_chain = false;

        checkpointData = {
            {
                {0, uint256S("0000ee183b69b8798ab94d25ad44ccc23beae0d175218a2cf2b394f384f71003")},
                {100, uint256S("0000f67d7dec953994bc036f16aacee2b468d5e339ff02317046e6004c08ee2a")}, //last PoW block
                {15880, uint256S("2f908106f567015d1fd1cbb2ba8583495b771de6e3ada2dbee72926f2134c1a1")},
            }
        };

        chainTxData = ChainTxData{
            // Data as of block 0d4230381e4987a569abb30de5f9141e62ac9f23b95174d2226f7f04bd9b5058 (height 38983)
            1628612092,
            78035,
            0.01620918090405545
        };

        consensus.nBlocktimeDownscaleFactor = 2;
        consensus.nCoinbaseMaturity = 10;
        consensus.nRBTCoinbaseMaturity = consensus.nBlocktimeDownscaleFactor * 5;
        consensus.nSubsidyHalvingIntervalV2 = consensus.nBlocktimeDownscaleFactor*525600; // credit halving every 4 years (nSubsidyHalvingInterval * nBlocktimeDownscaleFactor)

        consensus.nLastPOWBlock = 100;
        consensus.nLastBigReward = 1;
        consensus.nMPoSRewardRecipients = 10;
        consensus.nFirstMPoSBlock = consensus.nLastPOWBlock + 
                                    consensus.nMPoSRewardRecipients + 
                                    consensus.nCoinbaseMaturity;
        consensus.nLastMPoSBlock = 100;

        consensus.nFixUTXOCacheHFHeight = 84500;
        consensus.nEnableHeaderSignatureHeight = 391993;
        consensus.nCheckpointSpan = consensus.nCoinbaseMaturity;
        consensus.nRBTCheckpointSpan = consensus.nRBTCoinbaseMaturity;
        consensus.delegationsAddress = uint160(ParseHex("0000000000000000000000000000000000000086")); // Delegations contract for offline staking
        consensus.nStakeTimestampMask = 15;
        consensus.nRBTStakeTimestampMask = 3;
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    explicit CRegTestParams(const ArgsManager& args) {
        strNetworkID =  CBaseChainParams::REGTEST;
        consensus.nSubsidyHalvingInterval = 2500;
        consensus.BIP16Exception = uint256S("0x665ed5b402ac0b44efc37d8926332994363e8a7278b7ee9a58fb972efadae943");
        consensus.BIP34Height = 0; // BIP34 activated on regtest (Used in functional tests)
        consensus.BIP34Hash = uint256S("0x665ed5b402ac0b44efc37d8926332994363e8a7278b7ee9a58fb972efadae943");
        consensus.BIP65Height = 0; // BIP65 activated on regtest (Used in functional tests)
        consensus.BIP66Height = 0; // BIP66 activated on regtest (Used in functional tests)
        consensus.CSVHeight = 432; // CSV activated on regtest (Used in rpc activation tests)
        consensus.SegwitHeight = 0; // SEGWIT is always activated on regtest unless overridden
        consensus.MinBIP9WarningHeight = 0;
        consensus.CIP5Height = 0;
        consensus.CIP6Height = 0;
        consensus.CIP7Height = 0;
        consensus.CIP9Height = 0;
        consensus.nOfflineStakeHeight = 1;
        consensus.nReduceBlocktimeHeight = 0;
        consensus.nMuirGlacierHeight = 0;
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.CIP9PosLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // The new POS-limit activated after CIP9
        consensus.RBTPosLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 60 * 60; // 60 minutes (960 = 832 + 128; multiplier is 832)
        consensus.nPowTargetTimespanV2 = 60 * 60;
        consensus.nRBTPowTargetTimespan = 60 * 60;
        consensus.nPowTargetSpacing = 60 * 2;
        consensus.nRBTPowTargetSpacing = 60 * 2;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.fPoSNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 22; // 75% for testchains
        consensus.nMinerConfirmationWindow = 30; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xfd;
        pchMessageStart[1] = 0xdd;
        pchMessageStart[2] = 0xc6;
        pchMessageStart[3] = 0xe1;
        nDefaultPort = 24000;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 0;
        m_assumed_chain_state_size = 0;

        UpdateActivationParametersFromArgs(args);

        // for (int i = 1; i < 999999999; i++) {
        //     genesis = CreateGenesisBlock(1620093302, i, 0x207fffff, 1, 0 * COIN);
        //     unsigned int nBits;
        //     arith_uint256 nBits_arith = UintToArith256(consensus.powLimit);
        //     // nBits_arith *= 2;
        //     nBits = nBits_arith.GetCompact();
        //     if (CheckProofOfWork(genesis.GetHash(), nBits, consensus)) {
        //         std::cout << i << std::endl;
        //         std::cout << genesis.GetHash().ToString() << std::endl;
        //         std::cout << genesis.hashMerkleRoot.ToString() << std::endl;
        //         break;
        //     }
        // }
        genesis = CreateGenesisBlock(1620093302, 1, 0x207fffff, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x7f737a302ff68b04f59f62eda305003642ce8958e5e7550c85df64a4d223f98b"));
        assert(genesis.hashMerkleRoot == uint256S("0x059f12a764f464f8606ca61831505f7d5a613eb930c34b9632986f95c4c9de8c"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = true;
        fMineBlocksOnDemand = true;
        m_is_test_chain = true;
        m_is_mockable_chain = true;

        checkpointData = {
            {
                {0, uint256S("7f737a302ff68b04f59f62eda305003642ce8958e5e7550c85df64a4d223f98b")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        consensus.nBlocktimeDownscaleFactor = 2;
        consensus.nCoinbaseMaturity = 10;
        consensus.nRBTCoinbaseMaturity = consensus.nBlocktimeDownscaleFactor * 5;
        consensus.nSubsidyHalvingIntervalV2 = consensus.nBlocktimeDownscaleFactor * consensus.nSubsidyHalvingInterval; // credit halving every 4 years (nSubsidyHalvingInterval * nBlocktimeDownscaleFactor)

        consensus.nLastPOWBlock = 50;
        consensus.nLastBigReward = 1;
        consensus.nMPoSRewardRecipients = 10;
        consensus.nFirstMPoSBlock = 50;
        consensus.nLastMPoSBlock = 0;

        consensus.nFixUTXOCacheHFHeight=0;
        consensus.nEnableHeaderSignatureHeight = 0;

        consensus.nCheckpointSpan = consensus.nCoinbaseMaturity;
        consensus.nRBTCheckpointSpan = consensus.nRBTCoinbaseMaturity;
        consensus.delegationsAddress = uint160(ParseHex("0000000000000000000000000000000000000086")); // Delegations contract for offline staking
        consensus.nStakeTimestampMask = 15;
        consensus.nRBTStakeTimestampMask = 3;

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,127); // 't'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,87);  // 'c'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,240);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x05, 0x36, 0x88, 0xD0};
        base58Prefixes[EXT_SECRET_KEY] = {0x05, 0x36, 0x84, 0x95};

        bech32_hrp = "tcrt";
    }

    /**
     * Allows modifying the Version Bits regtest parameters.
     */
    void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
    void UpdateActivationParametersFromArgs(const ArgsManager& args);
};

void CRegTestParams::UpdateActivationParametersFromArgs(const ArgsManager& args)
{
    if (gArgs.IsArgSet("-segwitheight")) {
        int64_t height = gArgs.GetArg("-segwitheight", consensus.SegwitHeight);
        if (height < -1 || height >= std::numeric_limits<int>::max()) {
            throw std::runtime_error(strprintf("Activation height %ld for segwit is out of valid range. Use -1 to disable segwit.", height));
        } else if (height == -1) {
            LogPrintf("Segwit disabled for testing\n");
            height = std::numeric_limits<int>::max();
        }
        consensus.SegwitHeight = static_cast<int>(height);
    }

    if (!args.IsArgSet("-vbparams")) return;

    for (const std::string& strDeployment : args.GetArgs("-vbparams")) {
        std::vector<std::string> vDeploymentParams;
        boost::split(vDeploymentParams, strDeployment, boost::is_any_of(":"));
        if (vDeploymentParams.size() != 3) {
            throw std::runtime_error("Version bits parameters malformed, expecting deployment:start:end");
        }
        int64_t nStartTime, nTimeout;
        if (!ParseInt64(vDeploymentParams[1], &nStartTime)) {
            throw std::runtime_error(strprintf("Invalid nStartTime (%s)", vDeploymentParams[1]));
        }
        if (!ParseInt64(vDeploymentParams[2], &nTimeout)) {
            throw std::runtime_error(strprintf("Invalid nTimeout (%s)", vDeploymentParams[2]));
        }
        bool found = false;
        for (int j=0; j < (int)Consensus::MAX_VERSION_BITS_DEPLOYMENTS; ++j) {
            if (vDeploymentParams[0] == VersionBitsDeploymentInfo[j].name) {
                UpdateVersionBitsParameters(Consensus::DeploymentPos(j), nStartTime, nTimeout);
                found = true;
                LogPrintf("Setting version bits activation parameters for %s to start=%ld, timeout=%ld\n", vDeploymentParams[0], nStartTime, nTimeout);
                break;
            }
        }
        if (!found) {
            throw std::runtime_error(strprintf("Invalid deployment (%s)", vDeploymentParams[0]));
        }
    }
}

/**
 * Regression network parameters overwrites for unit testing
 */
class CUnitTestParams : public CRegTestParams
{
public:
    explicit CUnitTestParams(const ArgsManager& args)
    : CRegTestParams(args)
    {
        // Activate the the BIPs for regtest as in Bitcoin
        consensus.BIP16Exception = uint256();
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = consensus.nBlocktimeDownscaleFactor*500 + 851; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = consensus.nBlocktimeDownscaleFactor*500 + 751; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.CIP6Height = consensus.nBlocktimeDownscaleFactor*500 + 500;
        consensus.CIP7Height = 0; // CIP7 activated on regtest

        // CREDIT have 500 blocks of maturity, increased values for regtest in unit tests in order to correspond with it
        consensus.nSubsidyHalvingInterval = 750;
        consensus.nSubsidyHalvingIntervalV2 = consensus.nBlocktimeDownscaleFactor*750;
        consensus.nRuleChangeActivationThreshold = consensus.nBlocktimeDownscaleFactor*558; // 75% for testchains
        consensus.nMinerConfirmationWindow = consensus.nBlocktimeDownscaleFactor*744; // Faster than normal for regtest (744 instead of 2016)

        consensus.nBlocktimeDownscaleFactor = 2;
        consensus.nCoinbaseMaturity = 10;
        consensus.nRBTCoinbaseMaturity = consensus.nBlocktimeDownscaleFactor * 5;

        consensus.nCheckpointSpan = consensus.nCoinbaseMaturity*2; // Increase the check point span for the reorganization tests from 500 to 1000
        consensus.nRBTCheckpointSpan = consensus.nRBTCoinbaseMaturity*2; // Increase the check point span for the reorganization tests from 500 to 1000
    }
};

static std::unique_ptr<const CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<const CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams(gArgs));
    else if (chain == CBaseChainParams::UNITTEST)
        return std::unique_ptr<CChainParams>(new CUnitTestParams(gArgs));
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

std::string CChainParams::EVMGenesisInfo() const
{
    std::string genesisInfo = dev::eth::genesisInfo(GetEVMNetwork());
    ReplaceInt(consensus.CIP7Height,         "CIP7_STARTING_BLOCK", genesisInfo);
    ReplaceInt(consensus.CIP6Height,         "CIP6_STARTING_BLOCK", genesisInfo);
    ReplaceInt(consensus.nMuirGlacierHeight, "MUIR_STARTING_BLOCK", genesisInfo);
    return genesisInfo;
}

std::string CChainParams::EVMGenesisInfo(int nHeight) const
{
    std::string genesisInfo = dev::eth::genesisInfo(GetEVMNetwork());
    ReplaceInt(nHeight, "CIP7_STARTING_BLOCK", genesisInfo);
    ReplaceInt(nHeight, "CIP6_STARTING_BLOCK", genesisInfo);
    ReplaceInt(nHeight, "MUIR_STARTING_BLOCK", genesisInfo);
    return genesisInfo;
}

dev::eth::Network CChainParams::GetEVMNetwork() const
{
    return dev::eth::Network::creditMainNetwork;
}

void CChainParams::UpdateOpSenderBlockHeight(int nHeight)
{
    consensus.CIP5Height = nHeight;
}

void UpdateOpSenderBlockHeight(int nHeight)
{
    const_cast<CChainParams*>(globalChainParams.get())->UpdateOpSenderBlockHeight(nHeight);
}

void CChainParams::UpdateBtcEcrecoverBlockHeight(int nHeight)
{
    consensus.CIP6Height = nHeight;
}

void UpdateBtcEcrecoverBlockHeight(int nHeight)
{
    const_cast<CChainParams*>(globalChainParams.get())->UpdateBtcEcrecoverBlockHeight(nHeight);
}

void CChainParams::UpdateConstantinopleBlockHeight(int nHeight)
{
    consensus.CIP7Height = nHeight;
}

void UpdateConstantinopleBlockHeight(int nHeight)
{
    const_cast<CChainParams*>(globalChainParams.get())->UpdateConstantinopleBlockHeight(nHeight);
}

void CChainParams::UpdateDifficultyChangeBlockHeight(int nHeight)
{
    consensus.nSubsidyHalvingInterval = 525600; // credit halving every 4 years
    consensus.nSubsidyHalvingIntervalV2 = consensus.nBlocktimeDownscaleFactor*525600; // credit halving every 4 years (nSubsidyHalvingInterval * nBlocktimeDownscaleFactor)
    consensus.posLimit = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
    consensus.CIP9PosLimit = uint256S("0000000000001fffffffffffffffffffffffffffffffffffffffffffffffffff");
    consensus.RBTPosLimit = uint256S("0000000000003fffffffffffffffffffffffffffffffffffffffffffffffffff");
    consensus.CIP9Height = nHeight;
    consensus.fPowAllowMinDifficultyBlocks = false;
    consensus.fPowNoRetargeting = true;
    consensus.fPoSNoRetargeting = false;
    consensus.nLastPOWBlock = 1000;
    consensus.nMPoSRewardRecipients = 10;
    consensus.nFirstMPoSBlock = consensus.nLastPOWBlock + 
                                consensus.nMPoSRewardRecipients + 
                                consensus.nCoinbaseMaturity;
    consensus.nLastMPoSBlock = 0;
}

void UpdateDifficultyChangeBlockHeight(int nHeight)
{
    const_cast<CChainParams*>(globalChainParams.get())->UpdateDifficultyChangeBlockHeight(nHeight);
}

void CChainParams::UpdateOfflineStakingBlockHeight(int nHeight)
{
    consensus.nOfflineStakeHeight = nHeight;
}

void UpdateOfflineStakingBlockHeight(int nHeight)
{
    const_cast<CChainParams*>(globalChainParams.get())->UpdateOfflineStakingBlockHeight(nHeight);
}

void CChainParams::UpdateDelegationsAddress(const uint160& address)
{
    consensus.delegationsAddress = address;
}

void UpdateDelegationsAddress(const uint160& address)
{
    const_cast<CChainParams*>(globalChainParams.get())->UpdateDelegationsAddress(address);
}

void CChainParams::UpdateLastMPoSBlockHeight(int nHeight)
{
    consensus.nLastMPoSBlock = nHeight;
}

void UpdateLastMPoSBlockHeight(int nHeight)
{
    const_cast<CChainParams*>(globalChainParams.get())->UpdateLastMPoSBlockHeight(nHeight);
}

void CChainParams::UpdateReduceBlocktimeHeight(int nHeight)
{
    consensus.nReduceBlocktimeHeight = nHeight;
}

void UpdateReduceBlocktimeHeight(int nHeight)
{
    const_cast<CChainParams*>(globalChainParams.get())->UpdateReduceBlocktimeHeight(nHeight);
}

void CChainParams::UpdatePowAllowMinDifficultyBlocks(bool fValue)
{
    consensus.fPowAllowMinDifficultyBlocks = fValue;
}

void UpdatePowAllowMinDifficultyBlocks(bool fValuet)
{
    const_cast<CChainParams*>(globalChainParams.get())->UpdatePowAllowMinDifficultyBlocks(fValuet);
}

void CChainParams::UpdatePowNoRetargeting(bool fValue)
{
    consensus.fPowNoRetargeting = fValue;
}

void UpdatePowNoRetargeting(bool fValuet)
{
    const_cast<CChainParams*>(globalChainParams.get())->UpdatePowNoRetargeting(fValuet);
}

void CChainParams::UpdatePoSNoRetargeting(bool fValue)
{
    consensus.fPoSNoRetargeting = fValue;
}

void UpdatePoSNoRetargeting(bool fValuet)
{
    const_cast<CChainParams*>(globalChainParams.get())->UpdatePoSNoRetargeting(fValuet);
}

void CChainParams::UpdateMuirGlacierHeight(int nHeight)
{
    consensus.nMuirGlacierHeight = nHeight;
}

void UpdateMuirGlacierHeight(int nHeight)
{
    const_cast<CChainParams*>(globalChainParams.get())->UpdateMuirGlacierHeight(nHeight);
}
