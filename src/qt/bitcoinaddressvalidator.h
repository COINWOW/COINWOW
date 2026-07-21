// Copyright (C) 2009-2025 Bitcoin Core developers

// Copyright (C) 2026 COINWOW developers

// Distributed under the MIT software license

// Copyright (c) 2011-2020 The COINWOW Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef COINWOW_QT_COINWOWADDRESSVALIDATOR_H
#define COINWOW_QT_COINWOWADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class COINWOWAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit COINWOWAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

/** COINWOW address widget validator, checks for a valid coinwow address.
 */
class COINWOWAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit COINWOWAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

#endif // COINWOW_QT_COINWOWADDRESSVALIDATOR_H
