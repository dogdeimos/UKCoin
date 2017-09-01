// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "assert.h"

#include "chainparams.h"
#include "main.h"
#include "util.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

//
// Main network
//

// Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

class CMainParams : public CChainParams {
public:
    CMainParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xb7;
        pchMessageStart[1] = 0x5e;
        pchMessageStart[2] = 0x91;
        pchMessageStart[3] = 0x8c;
        vAlertPubKey = ParseHex("04aad5ca3cbe5616262ca3e7a6feef6a54765b96e9056bc6b132a04b94acefeac5d5257fe028e80695c62f7c2f81f85d251a216df3be197653f454852a2d08c631");
        nDefaultPort = 14354;
        nRPCPort = 14355;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 20);
        bnProofOfStakeLimit = CBigNum(~uint256(0) >> 20);

        const char* pszTimestamp = "http://www.bbc.co.uk/news/uk-41119870"; // Brexit: UK 'must not allow itself to be blackmailed'
        std::vector<CTxIn> vin;
        vin.resize(1);
        vin[0].scriptSig = CScript() << 0 << CBigNum(42) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        std::vector<CTxOut> vout;
        vout.resize(1);
        vout[0].SetEmpty();
        CTransaction txNew(1, 1504262400, vin, vout, 0); 
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1504262400; 
        genesis.nBits    = 0x1e0fffff;
        genesis.nNonce   = 527651;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x0883710680f5dc86ad7d800e34e165e7aeb4d9ca2a8aa99f91b8daea7fbf1133"));
        assert(genesis.hashMerkleRoot == uint256("0x04f5e7aac37e1d3676382841a851dbd5eb6c52e5da9c448389f50fca37dc4a2a"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,58);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,98);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,72);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x77)(0xE1)(0x7A).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x77)(0xDC)(0xF2).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        nTargetSpacing = 50; // Initially ~50 Sec during PoW
        if(nBestHeight > nLastPoWBlock) // Scaled down for PoS only phase
        {
          nTargetSpacing = 30;
        }
        if(nBestHeight > nStartPoSBlock) // Scaled down for PoW/PoS twin phase
        {
          if(nBestHeight <= nLastPoWBlock)
          {
            nTargetSpacing = 30;
          }
        }
        nTargetTimespan = 12 * nTargetSpacing;
        nLastPoWBlock = 50000;
        nStartPoSBlock = 3000;
    }

    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual Network NetworkID() const { return CChainParams::MAIN; }

    virtual const vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    vector<CAddress> vFixedSeeds;
};
static CMainParams mainParams;


//
// Testnet
//

class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0x42;
        pchMessageStart[1] = 0x8e;
        pchMessageStart[2] = 0x1f;
        pchMessageStart[3] = 0xa0;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 16);
        bnProofOfStakeLimit = CBigNum(~uint256(0) >> 16);

        vAlertPubKey = ParseHex("04aad5ca3cbe5616262ca3e7a6feef6b54765b96e9056bc6b132a04b94acefeac5d5257fe028e80695c62f7c2f81f85d251a216df3af197653f454852a2d08c631");
        nDefaultPort = 14355;
        nRPCPort = 24355;
        strDataDir = "testnet";

        // Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 84638;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0xe4ae7c27774ed294f3879f0799a6595aa50a2543cdea8dc2967a502939397a2e"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,71);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,116);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,53);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x55)(0x6A)(0x7E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x55)(0xFC)(0x5D).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        nTargetSpacing = 30;
        nLastPoWBlock = 5000;
        nStartPoSBlock = 300;
    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


//
// Regression test
//
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        pchMessageStart[0] = 0x94;
        pchMessageStart[1] = 0xc6;
        pchMessageStart[2] = 0x44;
        pchMessageStart[3] = 0xc8;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 1);
        genesis.nTime = 1504262400;
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 7094;
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 14356;
        strDataDir = "regtest";

        assert(hashGenesisBlock == uint256("0xce0bb70e91b94d98338648d913daf805aa520575de3e499e63b1cf1d9d4c348e"));

        vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.
    }

    virtual bool RequireRPCPassword() const { return false; }
    virtual Network NetworkID() const { return CChainParams::REGTEST; }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

void SelectParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        case CChainParams::REGTEST:
            pCurrentParams = &regTestParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

bool SelectParamsFromCommandLine() {
    bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet && fRegTest) {
        return false;
    }

    if (fRegTest) {
        SelectParams(CChainParams::REGTEST);
    } else if (fTestNet) {
        SelectParams(CChainParams::TESTNET);
    } else {
        SelectParams(CChainParams::MAIN);
    }
    return true;
}
