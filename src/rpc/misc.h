// Copyright (c) 2017 The Commercium developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef COMMERCIUM_RPCMISC_H
#define COMMERCIUM_RPCMISC_H

#include "script/script.h"

class UniValue;

CScript createmultisig_redeemScript(const UniValue &params);

#endif // COMMERCIUM_RPCBLOCKCHAIN_H
