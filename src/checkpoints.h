// Copyright (c) 2009-2016 The Commercium Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef COMMERCIUM_CHECKPOINTS_H
#define COMMERCIUM_CHECKPOINTS_H

#include "uint256.h"

#include <map>

class CBlockIndex;
struct CCheckpointData;

/**
 * Block-chain checkpoints are compiled-in sanity checks.
 * They are updated every release or three.
 */
namespace Checkpoints {

//! Returns last CBlockIndex* in mapBlockIndex that is a checkpoint
CBlockIndex *GetLastCheckpoint(const CCheckpointData &data);

} // namespace Checkpoints

#endif // COMMERCIUM_CHECKPOINTS_H
