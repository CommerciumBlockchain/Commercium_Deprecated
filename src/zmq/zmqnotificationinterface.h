// Copyright (c) 2015-2016 The Commercium Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef COMMERCIUM_ZMQ_ZMQNOTIFICATIONINTERFACE_H
#define COMMERCIUM_ZMQ_ZMQNOTIFICATIONINTERFACE_H

#include "validationinterface.h"
#include <map>
#include <string>

class CBlockIndex;
class CZMQAbstractNotifier;

class CZMQNotificationInterface : public CValidationInterface {
public:
    virtual ~CZMQNotificationInterface();

    static CZMQNotificationInterface *Create();

protected:
    bool Initialize();
    void Shutdown();

    // CValidationInterface
    void SyncTransaction(const CTransaction &tx, const CBlockIndex *pindex,
                         int posInBlock);
    void UpdatedBlockTip(const CBlockIndex *pindexNew,
                         const CBlockIndex *pindexFork, bool fInitialDownload);

private:
    CZMQNotificationInterface();

    void *pcontext;
    std::list<CZMQAbstractNotifier *> notifiers;
};

#endif // COMMERCIUM_ZMQ_ZMQNOTIFICATIONINTERFACE_H
