// Copyright (c) 2014 The Commercium developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef COMMERCIUM_ZMQ_ZMQCONFIG_H
#define COMMERCIUM_ZMQ_ZMQCONFIG_H

#if defined(HAVE_CONFIG_H)
#include "config/commercium-config.h"
#endif

#include <string>

#if ENABLE_ZMQ
#include <zmq.h>
#endif

#include "primitives/block.h"
#include "primitives/transaction.h"

void zmqError(const char *str);

#endif // COMMERCIUM_ZMQ_ZMQCONFIG_H
