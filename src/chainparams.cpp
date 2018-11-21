// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Commercium developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

// Equihash
#include "crypto/equihash.h"
#include "validation.h"
#include "net.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <cassert>
#include "chainparamsseeds.h"
#include "pow/nano.h"

// turn on to generate genesis
//#define PRINTS_OUT

#ifdef PRINTS_OUT
#include <fstream>
using namespace std;

void mery(CBlock *pblock, std::string net, int n, int k, std::string solver)
{ 
    ofstream fout; 
    fout.open("/root/cmm_genesis", std::fstream::in | std::fstream::out | std::fstream::app);
    arith_uint256 hashTarget = arith_uint256().SetCompact(pblock->nBits);

    cout << "calculating net = " << net << ", n = " << n
         << ", k = " << k << ", using solver = " << solver << endl;
    fout << "calculating net = " << net << ", n = " << n
         << ", k = " << k << ", using solver = " << solver << endl;

    cout << "hash target = " << hashTarget.ToString() << endl;                                                                                                                                                                                                                
    fout << "hash target = " << hashTarget.ToString() << endl;

    while (true)
    {
	cout << "nonce = " << pblock->nNonce.ToString() << endl;
	fout << "nonce = " << pblock->nNonce.ToString() << endl;
        if (equihash_(solver, pblock, n, k)) break;
        pblock->nNonce = ArithToUint256(UintToArith256(pblock->nNonce) + 1);
    }

    cout << "\t\t\t" << net << " genesis" << endl;
    fout << "\t\t\t" << net << " genesis" << endl;
    cout << pblock->ToString()<< endl;
    fout << pblock->ToString()<< endl;
	
    fout.close();
}
#endif

typedef std::vector<unsigned char> vuc;

// Far into the future.
static const std::string ANTI_REPLAY_COMMITMENT =
    "Commercium: Commerce Made Modern";

static std::vector<uint8_t> GetAntiReplayCommitment() {
    return std::vector<uint8_t>(std::begin(ANTI_REPLAY_COMMITMENT),
                                std::end(ANTI_REPLAY_COMMITMENT));
}

static CBlock CreateGenesisBlock(const char *pszTimestamp,
                                 const CScript &genesisOutputScript,
                                 uint32_t nTime, uint256 nNonce,
                                 uint32_t nBits, int32_t nVersion,
                                 const Amount genesisReward, vuc sol) {
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig =
        CScript() << 486604799 << CScriptNum(4)
                  << std::vector<uint8_t>((const uint8_t *)pszTimestamp,
                                          (const uint8_t *)pszTimestamp +
                                              strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime = nTime;
    genesis.nBits = nBits;
    genesis.nNonce = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashReserved.SetNull();
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    genesis.nSolution = sol;
    return genesis;
}

static CBlock CreateGenesisBlock(uint32_t nTime, uint256 nNonce,
                                 uint32_t nBits, int32_t nVersion,
                                 const Amount genesisReward, vuc sol) {
    const char *pszTimestamp = "Commercium5b983e3f258fb03419cbb777bf926eacc96a9f1affd5294d11cf0c351a6785fd";
    const CScript genesisOutputScript =
        CScript() << ParseHex("04ab060cb20fad3b8e05fc2b689ab6"
			      "7492792c3aabef71c25523672e2c6e539bfce9e287eaa292eb1caec867f3"
							  "97dff9ddf5fe7e207b523fc2c8bf96"
							  "ada92cde96")
                  << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce,
                              nBits, nVersion, genesisReward, sol);
}

arith_uint256 maxUint = UintToArith256(uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */
class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
//        consensus.nSubsidySlowStopInterval = 1260000; // slow start mining in roughly a month
	consensus.nSubsidyHalvingInterval = 420000;
        consensus.BIP34Height = 9999999;
        consensus.BIP34Hash = uint256S("0x000478a689198ed6d7ef574d5a3f2b224ebe8f87507fa1bd40c61785bdf8d53c");
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;
        consensus.antiReplayOpReturnSunsetHeight = 530000;
        consensus.antiReplayOpReturnCommitment = GetAntiReplayCommitment();
        consensus.powLimit = uint256S(
            "0007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowAveragingWindow = 30;                                                                                                                                                                                                                                   
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.nPowMaxAdjustDown = 32; // 32% adjustment down
        consensus.nPowMaxAdjustUp = 16; // 16% adjustment up

        // two weeks
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60;
        consensus.nPowTargetSpacing = 1 * 30;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        // 95% of 2016
        consensus.nRuleChangeActivationThreshold = 1916;
        // nPowTargetTimespan / nPowTargetSpacing
        consensus.nMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        // January 1, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime =
            1518061090;
        // January 1, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout =
            1546272000;

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        // January 1, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime =
			1518061090;
        // January 1, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout =
			1546272000;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0");

        // By default assume that the signatures in ancestors of this block are
        // valid.
        consensus.defaultAssumeValid =
            uint256S("0x00");

        //
        consensus.uahfHeight = 0;

        /**
         * The message start string is designed to be unlikely to occur in
         * normal data. The characters are rarely used upper ASCII, not valid as
         * UTF-8, and produce a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xde;
        pchMessageStart[1] = 0xad;
        pchMessageStart[2] = 0xbe;
        pchMessageStart[3] = 0xef;
	pchCmmMessageStart[0] = 0xde;
        pchCmmMessageStart[1] = 0xad;
        pchCmmMessageStart[2] = 0xbe;
        pchCmmMessageStart[3] = 0xef;

        nDefaultPort = 2018;
        nPruneAfterHeight = 100000;

	// Equihash params
        const size_t N = 200, K = 9;
        BOOST_STATIC_ASSERT(equihash_parameters_acceptable(N, K));
        nEquihashN = N;
        nEquihashK = K;

		vuc sol = {0,10,23,118,116,225,23,85,20,62,18,119,134,32,109,141,4,53,239,185,4,19,128,225,253,227,153,192,167,37,6,99,246,133,105,27,94,51,116,31,68,161,2,97,7,222,174,211,32,223,64,222,224,176,4,216,153,15,37,90,28,32,163,25,238,129,62,12,185,175,217,243,17,150,222,215,223,74,13,243,70,152,45,172,1,4,182,4,230,83,234,27,79,12,67,249,192,108,29,174,46,168,242,207,50,22,250,179,8,183,8,58,144,123,28,114,116,73,188,5,208,201,199,201,29,2,24,122,187,99,178,82,21,31,234,168,83,168,77,206,187,68,247,17,74,215,165,39,178,249,151,190,46,221,93,149,176,3,84,58,193,131,1,137,213,222,73,31,0,147,92,47,237,67,183,137,223,85,160,188,6,60,17,48,255,148,54,123,52,30,168,234,144,31,144,35,191,228,174,37,165,228,127,148,229,226,79,177,78,119,59,25,134,237,76,213,200,162,225,125,244,181,134,214,183,153,147,213,178,1,128,75,199,150,140,188,153,166,95,35,145,85,244,34,213,108,234,146,1,58,110,18,9,212,112,186,117,70,138,178,95,234,67,112,183,207,230,34,73,146,59,20,73,35,158,37,145,147,21,211,253,146,27,18,91,104,187,76,168,156,243,197,71,51,11,150,97,105,106,155,185,17,61,74,193,201,250,60,41,60,134,80,146,6,63,14,130,56,128,54,7,139,188,205,102,34,5,5,51,160,176,131,124,72,108,255,3,236,85,243,168,203,93,34,196,231,35,205,35,109,45,125,154,168,53,245,98,18,194,107,41,196,84,42,227,37,3,152,152,233,121,93,215,203,59,30,125,32,6,228,244,134,1,173,249,33,170,171,241,129,179,215,66,56,112,99,232,235,75,14,52,231,65,31,75,128,49,181,80,162,67,82,215,118,50,227,228,223,189,86,8,213,236,189,8,155,14,227,89,73,17,217,240,218,230,220,189,215,145,234,214,30,47,251,109,15,144,148,189,126,190,98,9,180,70,85,230,236,25,58,22,148,22,53,114,111,149,145,224,146,201,109,66,163,84,241,165,224,226,66,158,88,22,23,239,23,158,207,15,151,23,110,75,162,84,34,34,55,236,159,95,140,147,27,3,240,166,175,11,22,114,127,166,0,177,52,125,18,109,234,50,181,84,11,209,76,249,196,105,19,32,155,247,188,183,86,235,140,221,222,57,224,31,118,155,167,7,26,229,188,158,117,201,31,191,253,167,34,90,91,111,111,46,82,63,32,126,15,86,177,179,6,51,125,43,225,58,182,160,94,215,34,194,6,232,241,53,231,10,96,211,16,187,18,17,169,170,85,247,97,13,232,164,74,64,155,212,69,255,29,241,166,156,186,218,255,187,163,29,131,56,1,79,137,162,199,23,156,190,96,91,114,190,143,204,224,197,151,249,194,121,236,148,125,253,155,23,111,223,143,218,101,132,189,41,218,221,72,191,54,240,202,105,194,96,179,171,15,28,58,21,78,0,161,77,229,103,141,154,38,228,98,53,101,6,99,23,61,214,81,214,223,202,79,125,31,72,45,163,210,51,43,26,69,220,4,110,195,125,165,108,49,251,82,8,219,79,243,43,12,132,121,199,70,177,172,66,161,80,249,18,203,156,129,35,74,83,74,114,59,163,214,53,41,60,164,183,253,96,121,42,132,53,25,240,149,5,189,134,88,235,145,132,123,29,57,144,199,214,61,81,162,26,209,23,85,71,23,182,229,114,150,15,227,123,255,7,114,219,13,254,164,101,37,76,110,160,73,37,13,127,59,154,111,201,219,133,225,121,65,242,245,47,38,233,205,27,163,95,70,149,27,114,220,249,58,185,218,233,244,241,174,219,69,190,68,63,156,28,220,1,216,219,223,129,150,87,107,159,132,194,72,156,125,70,205,94,240,75,30,89,9,161,26,48,103,139,135,187,32,219,225,18,191,218,172,164,111,89,78,140,183,7,6,199,181,128,104,42,125,125,216,81,141,223,241,179,90,86,85,19,12,6,19,98,233,132,215,105,142,195,223,190,233,24,181,86,133,182,104,252,189,255,24,3,109,177,173,4,28,122,251,111,91,160,99,168,152,12,119,15,25,223,174,7,39,10,228,232,175,147,36,51,3,249,216,85,57,93,191,30,165,160,86,90,129,5,209,1,167,47,4,89,45,100,109,227,137,62,231,247,177,12,166,169,203,215,26,102,0,229,156,206,105,191,223,149,229,231,153,201,225,67,17,252,217,190,239,3,106,20,159,162,144,186,115,124,216,128,75,191,65,228,169,25,62,22,205,78,11,242,243,61,77,205,18,99,40,148,106,100,180,109,194,218,216,28,223,144,27,12,25,254,192,138,202,225,116,142,36,85,142,38,113,59,173,170,157,177,184,35,31,15,113,182,248,216,72,247,42,9,75,17,110,218,230,10,224,151,254,61,157,20,200,43,12,163,204,78,219,87,127,226,46,80,165,248,150,39,116,18,211,176,22,43,173,220,56,164,245,241,238,183,117,236,162,67,130,17,198,14,62,205,0,27,15,172,150,184,180,25,135,203,99,25,141,179,254,134,62,174,231,255,59,91,35,41,204,172,77,106,0,181,157,42,164,245,74,242,237,145,158,215,157,161,79,5,78,104,51,218,42,84,143,249,139,0,140,127,105,216,96,185,49,254,218,104,29,15,190,98,41,217,18,37,42,30,238,21,70,118,35,19,162,157,126,39,48,47,195,2,182,181,13,59,6,186,134,3,13,255,100,216,61,120,201,57,78,151,52,28,238,128,39,182,186,57,243,30,246,189,46,69,122,139,102,180,28,84,61,26,83,123,204,75,228,25,139,236,7,68,209,118,189,196,202,113,248,27,245,103,72,152,45,143,114,168,182,1,171,135,57,26,220,201,216,127,121,241,31,110,21,42,62,100,112,188,223,219,229,193,245,119,102,151,91,148,151,29,188,253,66,128,52,43,135,99,172,20,7,185,69,246,4,208,92,71,25,115,136,146,61,179,114,};
        genesis = CreateGenesisBlock(1518061090, uint256S("8568"), 0x1f07ffff, 1, 32 * COIN, sol);

        consensus.hashGenesisBlock = genesis.GetHash();
#ifdef PRINTS_OUT
		mery(&genesis, "main", nEquihashN, nEquihashK, "tromp");
#endif
        assert(consensus.hashGenesisBlock ==
               uint256S("0x00045ada91749106ec529f0544ad15e9f0c2855fea37bf2952971de06e6251d2"));
        assert(genesis.hashMerkleRoot ==
               uint256S("0x1150d688019fcca73ab1136cc1ed51f41641b9919bbb22da571d2f755dc2cf99"));


        // Note that of those with the service bits flag, most only support a
        // subset of possible options.
        // Commercium seeder
/*        vSeeds.push_back(
            CDNSSeedData("commercium.org", "seed.commercium.org", true));*/


        vSeeds.push_back(
            CDNSSeedData("commercium.net", "seed1.commercium.net", true));
        vSeeds.push_back(
            CDNSSeedData("commercium.net", "seed2.commercium.net", true));
        vSeeds.push_back(
            CDNSSeedData("miningspeed.com", "comseeder.miningspeed.com", true));

		// start with 'C'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<uint8_t>(1, 28);
		// start with 'M'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<uint8_t>(1, 50);
        // the first character, when base58 encoded, is "5" or "K" or "L" (as in Commercium)
        base58Prefixes[SECRET_KEY] = std::vector<uint8_t>(1, 140);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        vFixedSeeds = std::vector<SeedSpec6>(
            pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = {
            .mapCheckpoints = {
				{0, uint256S("0x0x00045ada91749106ec529f0544ad15e9f0c2855fea37bf2952971de06e6251d2")}
                                  }};

        chainTxData = ChainTxData{
            // UNIX timestamp of last known number of transactions.
            1518061090,
            // Total number of transactions between genesis and that timestamp
            // (the tx=... number in the SetBestChain debug.log lines)
            0,
            // Estimated number of transactions per second after that timestamp.
            0};
    }
};

static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
//	consensus.nSubsidySlowStartInterval = 4370;
        consensus.nSubsidyHalvingInterval = 210000;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x06b10fdee77b868501c6c06708cb3bf2a7baab1939a2f931882ad7e43bf247ad");
        // 00000000007f6655f22f98e72ed80d8b06dc761d5da09df0fa1dc4be4f861eb6
        consensus.BIP65Height = 0;
        // 000000002104c8c45e99a8853285a3b592602a3ccde2b832481da85e9e4ba182
        consensus.BIP66Height = 0;
        consensus.antiReplayOpReturnSunsetHeight = 1250000;
        consensus.antiReplayOpReturnCommitment = GetAntiReplayCommitment();
        consensus.powLimit = uint256S(
            "07ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowAveragingWindow = 30;
		// Equihash
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.nPowMaxAdjustDown = 32; // 32% adjustment down
        consensus.nPowMaxAdjustUp = 16; // 16% adjustment up
        // two weeks
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60;
        consensus.nPowTargetSpacing = 1 * 30;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        // 75% for testchains
        consensus.nRuleChangeActivationThreshold = 1512;
        // nPowTargetTimespan / nPowTargetSpacing
        consensus.nMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        // January 1, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime =
            1518061090;
        // January 1, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout =
            1546272000;

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        // January 1, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime =
            1518061090;
        // January 1, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout =
			1546272000;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork =
            uint256S("0x0");

        // By default assume that the signatures in ancestors of this block are
        // valid.
        consensus.defaultAssumeValid =
            uint256S("0x0");

        consensus.uahfHeight = 0;

        pchMessageStart[0] = 0x0b;
        pchMessageStart[1] = 0x12;
        pchMessageStart[2] = 0x09;
        pchMessageStart[3] = 0x07;
        pchCmmMessageStart[0] = 0xde;                                                                                                                                                                                                                                        
        pchCmmMessageStart[1] = 0x1e;
        pchCmmMessageStart[2] = 0xab;
        pchCmmMessageStart[3] = 0xcd;
        nDefaultPort = 12018;
        nPruneAfterHeight = 1000;

        // Equihash params
        const size_t N = 200, K = 9;
        BOOST_STATIC_ASSERT(equihash_parameters_acceptable(N, K));
        nEquihashN = N;
        nEquihashK = K;		
		
		vuc sol = {0,184,140,21,128,180,44,235,214,38,82,254,104,243,227,177,12,208,204,235,122,8,61,38,98,223,199,228,204,83,113,242,156,9,106,46,146,28,122,24,11,126,21,138,34,168,145,213,67,171,13,12,179,81,221,175,139,126,210,204,189,148,167,47,224,228,114,164,209,204,137,55,154,165,7,32,93,223,18,119,151,116,170,104,6,89,34,45,190,36,52,81,74,116,0,194,218,157,103,224,90,230,191,89,118,82,154,188,226,122,161,229,21,154,15,185,225,231,84,19,54,255,185,61,226,10,6,208,98,69,165,209,224,243,145,249,150,9,61,53,128,190,226,110,57,9,70,71,6,124,168,223,147,68,135,220,141,166,19,58,187,139,115,102,160,124,171,141,1,82,170,200,3,32,141,163,133,210,112,158,233,67,202,190,127,246,246,253,231,60,190,19,18,241,171,207,183,255,96,5,106,193,248,59,249,181,8,150,152,27,2,8,167,70,136,209,203,160,185,164,160,153,186,123,121,253,104,30,218,231,43,53,190,130,50,119,22,120,247,190,243,215,34,56,75,116,158,207,173,182,173,177,25,68,129,24,20,9,3,180,141,116,25,35,154,89,142,71,44,220,123,141,106,33,147,109,113,42,91,189,137,207,188,194,43,94,224,229,60,145,54,174,47,6,41,62,241,221,20,32,132,45,168,150,90,154,78,90,237,182,223,234,88,217,23,61,24,199,109,252,101,30,243,185,210,36,188,226,103,11,153,171,26,220,66,176,11,246,147,139,1,100,161,57,91,119,18,206,251,188,29,94,204,214,55,41,58,32,229,33,239,97,251,110,137,228,34,4,151,155,167,116,1,136,230,118,41,193,18,252,239,177,71,51,31,129,188,24,34,140,155,158,225,213,142,64,243,38,237,95,232,115,31,168,48,7,249,190,243,24,109,85,240,53,86,245,54,63,14,162,22,250,226,225,106,9,78,15,70,155,33,117,143,58,247,242,219,155,254,167,76,31,77,35,160,104,243,192,181,229,46,121,218,252,212,115,138,163,26,60,232,10,28,249,73,90,30,181,207,149,250,94,244,150,95,109,215,205,83,241,191,25,200,47,50,143,245,48,155,127,61,147,206,98,255,143,68,123,29,101,252,116,219,90,12,11,87,9,78,140,135,58,102,190,227,243,238,179,27,34,6,163,120,73,175,63,213,158,251,183,92,133,63,165,95,104,234,237,254,199,6,117,66,211,254,2,12,181,4,23,17,6,208,43,129,222,98,24,213,213,146,173,133,64,146,103,121,44,252,239,71,241,99,36,9,53,9,163,106,250,126,61,249,89,156,209,125,174,14,176,164,231,116,165,207,249,141,204,129,186,110,198,120,104,144,177,70,231,23,60,186,118,23,145,213,4,99,24,186,169,6,106,118,194,110,111,105,120,74,154,75,24,114,196,44,227,105,63,28,5,118,166,8,202,160,49,224,219,23,87,183,98,234,43,200,38,115,126,227,251,32,139,174,142,121,22,134,247,228,152,61,2,1,135,120,77,255,16,39,17,157,188,50,216,70,81,194,73,243,135,85,145,231,10,92,229,199,18,99,223,71,235,165,51,138,217,110,60,29,157,97,111,221,236,10,123,116,231,3,198,152,15,136,44,97,214,32,170,41,0,195,146,169,67,87,26,209,215,139,96,78,210,146,174,162,227,179,119,80,170,241,49,215,118,255,233,4,61,160,115,236,69,61,213,160,179,183,84,24,71,236,163,108,188,223,137,33,18,185,249,51,195,88,58,219,22,25,249,51,14,233,2,115,170,128,63,184,113,15,102,204,196,93,223,133,149,236,216,228,235,223,118,54,209,113,144,179,51,143,63,137,206,118,228,234,227,111,133,86,89,133,111,220,148,147,33,184,153,13,242,3,111,130,160,107,108,74,215,178,226,166,203,76,243,114,254,147,76,94,150,250,8,240,113,187,255,98,5,13,174,219,241,128,143,178,6,109,150,17,83,93,151,5,175,30,152,235,14,152,124,202,153,192,201,158,181,97,142,124,33,251,185,203,63,33,246,37,111,244,248,253,248,162,69,67,7,49,143,178,14,175,55,142,218,4,143,21,129,4,168,238,5,240,20,164,183,163,177,190,26,218,142,60,184,68,87,120,5,97,237,27,122,85,212,130,165,196,197,217,210,1,114,238,94,251,1,10,196,185,53,194,212,200,99,105,15,39,245,170,221,58,186,162,169,186,15,141,29,230,183,18,243,82,250,192,233,28,210,234,72,113,252,45,200,222,58,243,66,2,27,23,122,251,209,96,249,56,213,97,38,251,55,117,48,91,107,35,171,200,10,134,144,212,124,130,178,16,226,69,181,26,169,44,116,190,15,128,115,229,132,9,114,164,121,207,50,15,125,160,195,82,156,247,149,56,18,20,217,19,213,156,64,15,199,245,78,151,13,253,225,7,56,231,19,82,174,67,241,110,127,231,109,18,199,238,188,227,15,212,87,216,225,145,91,251,204,63,85,151,38,223,193,136,22,82,93,162,224,29,84,9,203,71,162,103,45,240,180,13,160,57,157,86,209,22,48,161,184,163,136,63,144,205,109,231,39,153,121,249,242,20,138,17,219,231,33,165,111,142,70,191,164,87,253,228,85,67,56,82,115,113,142,97,237,19,147,3,176,233,108,117,142,181,201,107,145,179,18,64,246,26,137,138,15,78,11,27,6,69,230,196,8,8,142,102,169,146,160,163,21,66,6,216,75,18,122,93,123,14,55,55,12,166,211,180,139,88,106,161,113,189,25,22,162,6,38,176,106,236,39,42,250,1,4,155,18,231,244,48,152,196,114,249,1,42,122,229,248,16,79,18,33,211,0,25,7,246,203,219,101,19,41,74,204,41,53,117,43,151,125,167,24,133,181,30,194,202,48,33,89,97,202,42,140,244,131,191,95,154,62,157,80,66,163,149,47,64,17,68,85,91,190,198,56,151,123,61,90,27,177,85,215,0,118,108,5,139,220,242,65,63,178,148,119,198,39,237,84,254,31,8,241,92,187,};
        genesis = CreateGenesisBlock(1518061090, uint256S("a"), 0x2007ffff, 1, 32 * COIN, sol);
        consensus.hashGenesisBlock = genesis.GetHash();
#ifdef PRINTS_OUT
		mery(&genesis, "test", nEquihashN, nEquihashK, "tromp");
#endif
        assert(consensus.hashGenesisBlock ==
               uint256S("0x056362c5cdd33f1a351fbda301ed20cf15d2c8e777a3226904b9eb065a18188d"));
        assert(genesis.hashMerkleRoot ==
               uint256S("0x1150d688019fcca73ab1136cc1ed51f41641b9919bbb22da571d2f755dc2cf99"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        // Commercium seeder
/*        vSeeds.push_back(CDNSSeedData(
			"commercium.org", "testnet-seed.commercium.org", true));*/
        vSeeds.push_back(
            CDNSSeedData("commercium.net", "testnet-seed.commercium.net", true));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<uint8_t>(1, 127);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<uint8_t>(1, 196);
        base58Prefixes[SECRET_KEY] = std::vector<uint8_t>(1, 239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
        vFixedSeeds = std::vector<SeedSpec6>(
            pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        checkpointData = {
            .mapCheckpoints = {
				{0, uint256S("0x056362c5cdd33f1a351fbda301ed20cf15d2c8e777a3226904b9eb065a18188d")}
            }};

        // Data as of block
        // 00000000c2872f8f8a8935c8e3c5862be9038c97d4de2cf37ed496991166928a
        // (height 1063660)
        chainTxData = ChainTxData{1518061090, 0, 0};
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
//	consensus.nSubsidySlowStartInterval = 0;
        consensus.nSubsidyHalvingInterval = 150;
        // BIP34 has not activated on regtest (far in the future so block v1 are
        // not rejected in tests)
        consensus.BIP34Height = 100000000;
        consensus.BIP34Hash = uint256();
        // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP65Height = 1351;
        // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251;
        consensus.antiReplayOpReturnSunsetHeight = 530000;
        consensus.antiReplayOpReturnCommitment = GetAntiReplayCommitment();
        consensus.powLimit = uint256S(
            "0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f");
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.nPowAveragingWindow = 30;
        consensus.nPowMaxAdjustDown = 0; // Turn off adjustment down
        consensus.nPowMaxAdjustUp = 0; // Turn off adjustment up
        // two weeks
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60;
        consensus.nPowTargetSpacing = 1 * 30;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        // 75% for testchains
        consensus.nRuleChangeActivationThreshold = 108;
        // Faster than normal for regtest (144 instead of 2016)
        consensus.nMinerConfirmationWindow = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout =
            999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout =
            999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are
        // valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        // Hard fork is always enabled on regtest.
        consensus.uahfHeight = 0;

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xbe;
        pchMessageStart[3] = 0xda;
        pchCmmMessageStart[0] = 0xfd;                                                                                                                                                                                                                                        
        pchCmmMessageStart[1] = 0xbb;
        pchCmmMessageStart[2] = 0x5b;
        pchCmmMessageStart[3] = 0xde;
        nDefaultPort = 22018;
        nPruneAfterHeight = 1000;

		// Equihash
        const size_t N = 48, K = 5;
        BOOST_STATIC_ASSERT(equihash_parameters_acceptable(N, K));
        nEquihashN = N;
        nEquihashK = K;

		vuc sol = {2,137,21,111,81,161,49,25,87,4,84,202,44,240,86,32,211,237,22,184,103,31,19,51,249,53,197,27,181,221,52,129,217,158,173,243,};
        genesis = CreateGenesisBlock(1518061090, uint256S("2d"), 0x200f0f0f, 1, 32 * COIN, sol);
        consensus.hashGenesisBlock = genesis.GetHash();

#ifdef PRINTS_OUT
		mery(&genesis, "reg", nEquihashN, nEquihashK, "default");
#endif
        assert(consensus.hashGenesisBlock ==
               uint256S("0x0b3ef0495fc91be636a065995d3953eb0fab2039da10d0b0c8c97abf5293ec41"));
        assert(genesis.hashMerkleRoot ==
               uint256S("0x1150d688019fcca73ab1136cc1ed51f41641b9919bbb22da571d2f755dc2cf99"));

        //!< Regtest mode doesn't have any fixed seeds.
        vFixedSeeds.clear();
        //!< Regtest mode doesn't have any DNS seeds.
        vSeeds.clear();

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = {.mapCheckpoints = {
                              {0, uint256S("0x0b3ef0495fc91be636a065995d3953eb0fab2039da10d0b0c8c97abf5293ec41")},
                          }};

        chainTxData = ChainTxData{0, 0, 0};

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<uint8_t>(1, 111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<uint8_t>(1, 196);
        base58Prefixes[SECRET_KEY] = std::vector<uint8_t>(1, 239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
    }

    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime,
                              int64_t nTimeout) {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
};

static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams &Params(const std::string &chain) {
    if (chain == CBaseChainParams::MAIN) {
        return mainParams;
    }

    if (chain == CBaseChainParams::TESTNET) {
        return testNetParams;
    }

    if (chain == CBaseChainParams::REGTEST) {
        return regTestParams;
    }

    throw std::runtime_error(
        strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string &network) {
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime,
                                 int64_t nTimeout) {
    regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}
