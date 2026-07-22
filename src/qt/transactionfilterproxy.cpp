// Copyright (C) 2009-2025 Bitcoin Core developers

// Copyright (C) 2026 COINWOW developers

// Distributed under the MIT software license

// Copyright (c) 2011-2021 The COINWOW Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/transactionfilterproxy.h>

#include <qt/transactiontablemodel.h>
#include <qt/transactionrecord.h>

#include <algorithm>
#include <cstdlib>
#include <optional>

TransactionFilterProxy::TransactionFilterProxy(QObject* parent)
    : QSortFilterProxyModel(parent),
      m_search_string(),
      typeFilter(ALL_TYPES)
{
}

bool TransactionFilterProxy::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    int status = index.data(TransactionTableModel::StatusRole).toInt();
    if (!showInactive && status == TransactionStatus::Conflicted)
        return false;

    int type = index.data(TransactionTableModel::TypeRole).toInt();
    if (!(TYPE(type) & typeFilter))
        return false;

    QDateTime datetime = index.data(TransactionTableModel::DateRole).toDateTime();
    if (dateFrom && datetime < *dateFrom) return false;
    if (dateTo && datetime > *dateTo) return false;

    QString address = index.data(TransactionTableModel::AddressRole).toString();
    QString label = index.data(TransactionTableModel::LabelRole).toString();
    QString txid = index.data(TransactionTableModel::TxHashRole).toString();
    if (!address.contains(m_search_string, Qt::CaseInsensitive) &&
        !  label.contains(m_search_string, Qt::CaseInsensitive) &&
        !   txid.contains(m_search_string, Qt::CaseInsensitive)) {
        return false;
    }

    qint64 amount = llabs(index.data(TransactionTableModel::AmountRole).toLongLong());
    if (amount < minAmount)
        return false;

    return true;
}

void TransactionFilterProxy::setDateRange(const std::optional<QDateTime>& from, const std::optional<QDateTime>& to)
{
    beginFilterChange();
    dateFrom = from;
    dateTo = to;
    endFilterChange(QSortFilterProxyModel::Direction::Rows);
}

void TransactionFilterProxy::setSearchString(const QString &search_string)
{
    if (m_search_string == search_string) return;
    beginFilterChange();
    m_search_string = search_string;
    endFilterChange(QSortFilterProxyModel::Direction::Rows);
}

void TransactionFilterProxy::setTypeFilter(quint32 modes)
{
    beginFilterChange();
    this->typeFilter = modes;
    endFilterChange(QSortFilterProxyModel::Direction::Rows);
}

void TransactionFilterProxy::setMinAmount(const CAmount& minimum)
{
    beginFilterChange();
    this->minAmount = minimum;
    endFilterChange(QSortFilterProxyModel::Direction::Rows);
}

void TransactionFilterProxy::setShowInactive(bool _showInactive)
{
    beginFilterChange();
    this->showInactive = _showInactive;
    endFilterChange(QSortFilterProxyModel::Direction::Rows);
}
