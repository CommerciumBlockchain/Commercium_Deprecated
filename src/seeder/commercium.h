#ifndef COMMERCIUM_SEEDER_COMMERCIUM_H
#define COMMERCIUM_SEEDER_COMMERCIUM_H

#include "protocol.h"

bool TestNode(const CService &cip, int &ban, int &client, std::string &clientSV,
              int &blocks, std::vector<CAddress> *vAddr);

#endif
