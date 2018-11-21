// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Commercium developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef COMMERCIUM_COMMERCIUMCONSENSUS_H
#define COMMERCIUM_COMMERCIUMCONSENSUS_H

#include <cstdint>

#if defined(BUILD_COMMERCIUM_INTERNAL) && defined(HAVE_CONFIG_H)
#include "config/commercium-config.h"
#if defined(_WIN32)
#if defined(DLL_EXPORT)
#if defined(HAVE_FUNC_ATTRIBUTE_DLLEXPORT)
#define EXPORT_SYMBOL __declspec(dllexport)
#else
#define EXPORT_SYMBOL
#endif
#endif
#elif defined(HAVE_FUNC_ATTRIBUTE_VISIBILITY)
#define EXPORT_SYMBOL __attribute__((visibility("default")))
#endif
#elif defined(MSC_VER) && !defined(STATIC_LIBCOMMERCIUMCONSENSUS)
#define EXPORT_SYMBOL __declspec(dllimport)
#endif

#ifndef EXPORT_SYMBOL
#define EXPORT_SYMBOL
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define COMMERCIUMCONSENSUS_API_VER 1

typedef enum commerciumconsensus_error_t {
    commerciumconsensus_ERR_OK = 0,
    commerciumconsensus_ERR_TX_INDEX,
    commerciumconsensus_ERR_TX_SIZE_MISMATCH,
    commerciumconsensus_ERR_TX_DESERIALIZE,
    commerciumconsensus_ERR_AMOUNT_REQUIRED,
    commerciumconsensus_ERR_INVALID_FLAGS,
} commerciumconsensus_error;

/** Script verification flags */
enum {
    commerciumconsensus_SCRIPT_FLAGS_VERIFY_NONE = 0,
    // evaluate P2SH (BIP16) subscripts
    commerciumconsensus_SCRIPT_FLAGS_VERIFY_P2SH = (1U << 0),
    // enforce strict DER (BIP66) compliance
    commerciumconsensus_SCRIPT_FLAGS_VERIFY_DERSIG = (1U << 2),
    // enforce NULLDUMMY (BIP147)
    commerciumconsensus_SCRIPT_FLAGS_VERIFY_NULLDUMMY = (1U << 4),
    // enable CHECKLOCKTIMEVERIFY (BIP65)
    commerciumconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY = (1U << 9),
    // enable CHECKSEQUENCEVERIFY (BIP112)
    commerciumconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY = (1U << 10),
    // enable WITNESS (BIP141)
    commerciumconsensus_SCRIPT_FLAGS_VERIFY_WITNESS_DEPRECATED = (1U << 11),
    // enable SIGHASH_FORKID replay protection
    commerciumconsensus_SCRIPT_ENABLE_SIGHASH_FORKID = (1U << 16),
    commerciumconsensus_SCRIPT_FLAGS_VERIFY_ALL =
        commerciumconsensus_SCRIPT_FLAGS_VERIFY_P2SH |
        commerciumconsensus_SCRIPT_FLAGS_VERIFY_DERSIG |
        commerciumconsensus_SCRIPT_FLAGS_VERIFY_NULLDUMMY |
        commerciumconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY |
        commerciumconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY,
};

/// Returns 1 if the input nIn of the serialized transaction pointed to by txTo
/// correctly spends the scriptPubKey pointed to by scriptPubKey under the
/// additional constraints specified by flags.
/// If not nullptr, err will contain an error/success code for the operation
EXPORT_SYMBOL int commerciumconsensus_verify_script(
    const uint8_t *scriptPubKey, unsigned int scriptPubKeyLen,
    const uint8_t *txTo, unsigned int txToLen, unsigned int nIn,
    unsigned int flags, commerciumconsensus_error *err);

EXPORT_SYMBOL int commerciumconsensus_verify_script_with_amount(
    const uint8_t *scriptPubKey, unsigned int scriptPubKeyLen, int64_t amount,
    const uint8_t *txTo, unsigned int txToLen, unsigned int nIn,
    unsigned int flags, commerciumconsensus_error *err);

EXPORT_SYMBOL unsigned int commerciumconsensus_version();

#ifdef __cplusplus
} // extern "C"
#endif

#undef EXPORT_SYMBOL

#endif // COMMERCIUM_COMMERCIUMCONSENSUS_H
