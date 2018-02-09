// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2012-2016 The Commercium Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef COMMERCIUM_UI_INTERFACE_H
#define COMMERCIUM_UI_INTERFACE_H

#include <cstdint>
#include <string>

#include <boost/signals2/last_value.hpp>
#include <boost/signals2/signal.hpp>

class CBasicKeyStore;
class CWallet;
class uint256;
class CBlockIndex;

/** General change type (added, updated, removed). */
enum ChangeType { CT_NEW, CT_UPDATED, CT_DELETED };

/** Signals for UI communication. */
class CClientUIInterface {
public:
    /** Flags for CClientUIInterface::ThreadSafeMessageBox */
    enum MessageBoxFlags {
        ICON_INFORMATION = 0,
        ICON_WARNING = (1U << 0),
        ICON_ERROR = (1U << 1),
        /**
         * Mask of all available icons in CClientUIInterface::MessageBoxFlags
         * This needs to be updated, when icons are changed there!
         */
        ICON_MASK = (ICON_INFORMATION | ICON_WARNING | ICON_ERROR),

        /**
         * These values are taken from qmessagebox.h "enum StandardButton" to be
         * directly usable.
         */
        CMM_OK = 0x00000400U,      // QMessageBox::Ok
        CMM_YES = 0x00004000U,     // QMessageBox::Yes
        CMM_NO = 0x00010000U,      // QMessageBox::No
        CMM_ABORT = 0x00040000U,   // QMessageBox::Abort
        CMM_RETRY = 0x00080000U,   // QMessageBox::Retry
        CMM_IGNORE = 0x00100000U,  // QMessageBox::Ignore
        CMM_CLOSE = 0x00200000U,   // QMessageBox::Close
        CMM_CANCEL = 0x00400000U,  // QMessageBox::Cancel
        CMM_DISCARD = 0x00800000U, // QMessageBox::Discard
        CMM_HELP = 0x01000000U,    // QMessageBox::Help
        CMM_APPLY = 0x02000000U,   // QMessageBox::Apply
        CMM_RESET = 0x04000000U,   // QMessageBox::Reset

        /**
         * Mask of all available buttons in CClientUIInterface::MessageBoxFlags
         * This needs to be updated, when buttons are changed there!
         */
        CMM_MASK = (CMM_OK | CMM_YES | CMM_NO | CMM_ABORT | CMM_RETRY |
                    CMM_IGNORE | CMM_CLOSE | CMM_CANCEL | CMM_DISCARD |
                    CMM_HELP | CMM_APPLY | CMM_RESET),

        /**
         * Force blocking, modal message box dialog (not just OS notification)
         */
        MODAL = 0x10000000U,

        /** Do not print contents of message to debug log */
        SECURE = 0x40000000U,

        /** Predefined combinations for certain default usage cases */
        MSG_INFORMATION = ICON_INFORMATION,
        MSG_WARNING = (ICON_WARNING | CMM_OK | MODAL),
        MSG_ERROR = (ICON_ERROR | CMM_OK | MODAL)
    };

    /** Show message box. */
    boost::signals2::signal<bool(const std::string &message,
                                 const std::string &caption,
                                 unsigned int style),
                            boost::signals2::last_value<bool>>
        ThreadSafeMessageBox;

    /**
     * If possible, ask the user a question. If not, falls back to
     * ThreadSafeMessageBox(noninteractive_message, caption, style) and returns
     * false.
     */
    boost::signals2::signal<bool(const std::string &message,
                                 const std::string &noninteractive_message,
                                 const std::string &caption,
                                 unsigned int style),
                            boost::signals2::last_value<bool>>
        ThreadSafeQuestion;

    /** Progress message during initialization. */
    boost::signals2::signal<void(const std::string &message)> InitMessage;

    /** Number of network connections changed. */
    boost::signals2::signal<void(int newNumConnections)>
        NotifyNumConnectionsChanged;

    /** Network activity state changed. */
    boost::signals2::signal<void(bool networkActive)>
        NotifyNetworkActiveChanged;

    /**
     * Status bar alerts changed.
     */
    boost::signals2::signal<void()> NotifyAlertChanged;

    /** A wallet has been loaded. */
    boost::signals2::signal<void(CWallet *wallet)> LoadWallet;

    /** Show progress e.g. for verifychain */
    boost::signals2::signal<void(const std::string &title, int nProgress)>
        ShowProgress;

    /** New block has been accepted */
    boost::signals2::signal<void(bool, const CBlockIndex *)> NotifyBlockTip;

    /** Best header has changed */
    boost::signals2::signal<void(bool, const CBlockIndex *)> NotifyHeaderTip;

    /** Banlist did change. */
    boost::signals2::signal<void(void)> BannedListChanged;
};

/** Show warning message **/
void InitWarning(const std::string &str);

/** Show error message **/
bool InitError(const std::string &str);

std::string AmountHighWarn(const std::string &optname);

std::string AmountErrMsg(const char *const optname,
                         const std::string &strValue);

extern CClientUIInterface uiInterface;

#endif // COMMERCIUM_UI_INTERFACE_H
