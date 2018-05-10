// Copyright (c) 2011-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef COMMERCIUM_QT_COMMERCIUMADDRESSVALIDATOR_H
#define COMMERCIUM_QT_COMMERCIUMADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class CommerciumAddressEntryValidator : public QValidator {
    Q_OBJECT

public:
    explicit CommerciumAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Commercium address widget validator, checks for a valid commercium address.
 */
class CommerciumAddressCheckValidator : public QValidator {
    Q_OBJECT

public:
    explicit CommerciumAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // COMMERCIUM_QT_COMMERCIUMADDRESSVALIDATOR_H
