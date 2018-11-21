// Copyright (c) 2017 The Bitcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef COMMERCIUM_RPCBLOCKCHAIN_H
#define COMMERCIUM_RPCBLOCKCHAIN_H

#include <univalue.h>

class Config;
class JSONRPCRequest;

UniValue getblockchaininfo(const Config &config, const JSONRPCRequest &request);

#endif // COMMERCIUM_RPCBLOCKCHAIN_H
