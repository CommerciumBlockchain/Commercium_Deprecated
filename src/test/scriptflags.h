// Copyright (c) 2017 The Commercium developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef COMMERCIUM_TEST_SCRIPTFLAGS_H
#define COMMERCIUM_TEST_SCRIPTFLAGS_H

#include <string>

uint32_t ParseScriptFlags(std::string strFlags);
std::string FormatScriptFlags(uint32_t flags);

#endif // COMMERCIUM_TEST_SCRIPTFLAGS_H
