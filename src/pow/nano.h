#ifndef COMMERCIUM_NANO_H
#define COMMERCIUM_NANO_H

#include "primitives/block.h"
#include "chainparams.h"
#include <string>

bool equihash_(std::string solver, CBlock *pblock, int n, int k);

#endif
