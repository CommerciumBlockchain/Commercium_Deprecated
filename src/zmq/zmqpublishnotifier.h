// Copyright (c) 2009-2016 The Commercium developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef COMMERCIUM_ZMQ_ZMQPUBLISHNOTIFIER_H
#define COMMERCIUM_ZMQ_ZMQPUBLISHNOTIFIER_H

#include "zmqabstractnotifier.h"

class CBlockIndex;

class CZMQAbstractPublishNotifier : public CZMQAbstractNotifier {
private:
    //!< upcounting per message sequence number
    uint32_t nSequence;

public:
    /* send zmq multipart message
       parts:
          * command
          * data
          * message sequence number
    */
    bool SendMessage(const char *command, const void *data, size_t size);

    bool Initialize(void *pcontext);
    void Shutdown();
};

class CZMQPublishHashBlockNotifier : public CZMQAbstractPublishNotifier {
public:
    bool NotifyBlock(const CBlockIndex *pindex);
};

class CZMQPublishHashTransactionNotifier : public CZMQAbstractPublishNotifier {
public:
    bool NotifyTransaction(const CTransaction &transaction);
};

class CZMQPublishRawBlockNotifier : public CZMQAbstractPublishNotifier {
public:
    bool NotifyBlock(const CBlockIndex *pindex);
};

class CZMQPublishRawTransactionNotifier : public CZMQAbstractPublishNotifier {
public:
    bool NotifyTransaction(const CTransaction &transaction);
};

#endif // COMMERCIUM_ZMQ_ZMQPUBLISHNOTIFIER_H
