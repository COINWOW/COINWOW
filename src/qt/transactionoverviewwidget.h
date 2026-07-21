// Copyright (C) 2009-2025 Bitcoin Core developers

// Copyright (C) 2026 COINWOW developers

// Distributed under the MIT software license

// Copyright (c) 2021-2022 The COINWOW Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef COINWOW_QT_TRANSACTIONOVERVIEWWIDGET_H
#define COINWOW_QT_TRANSACTIONOVERVIEWWIDGET_H

#include <QListView>
#include <QSize>

QT_BEGIN_NAMESPACE
class QShowEvent;
class QWidget;
QT_END_NAMESPACE

class TransactionOverviewWidget : public QListView
{
    Q_OBJECT

public:
    explicit TransactionOverviewWidget(QWidget* parent = nullptr);
    QSize sizeHint() const override;

protected:
    void showEvent(QShowEvent* event) override;
};

#endif // COINWOW_QT_TRANSACTIONOVERVIEWWIDGET_H
