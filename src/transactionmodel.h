/*
    This file is part of etherwall.
    etherwall is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    etherwall is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with etherwall. If not, see <http://www.gnu.org/licenses/>.
*/
/** @file transactionmodel.h
 * @author Ales Katona <almindor@gmail.com>
 * @date 2015
 *
 * Transaction model header
 */


#ifndef TRANSACTIONMODEL_H
#define TRANSACTIONMODEL_H


#include <QAbstractListModel>
#include "types.h"
#include "etheripc.h"
#include "accountmodel.h"

namespace Etherwall {

    class TransactionModel : public QAbstractListModel
    {
        Q_OBJECT
        Q_PROPERTY(quint64 blockNumber READ getBlockNumber NOTIFY blockNumberChanged FINAL)
        Q_PROPERTY(QString gasPrice READ getGasPrice NOTIFY gasPriceChanged FINAL)
    public:
        TransactionModel(EtherIPC& ipc, const AccountModel& accountModel);
        quint64 getBlockNumber() const;
        const QString& getGasPrice() const;
        QHash<int, QByteArray> roleNames() const;
        int rowCount(const QModelIndex & parent = QModelIndex()) const;
        QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
        int containsTransaction(const QString& hash);
        Q_INVOKABLE void loadHistory();
        double getHistoryProgress() const;
    public slots:
        void connectToServerDone();
        void getBlockNumberDone(quint64 num);
        void getGasPriceDone(const QString& num);
        void sendTransactionDone(const QString& hash);
        void sendTransaction(const QString& from, const QString& to, double value, double gas = -1.0);
        void newTransaction(const TransactionInfo& info);
        void newBlock(const QJsonObject& block);
        void refresh();
    signals:
        void blockNumberChanged(quint64 num);
        void gasPriceChanged(const QString& price);
        void historyChanged();
    private:
        EtherIPC& fIpc;
        const AccountModel& fAccountModel;
        TransactionList fTransactionList;
        quint64 fBlockNumber;
        QString fGasPrice;
        TransactionInfo fQueuedTransaction;

        void addTransaction(const TransactionInfo& info);
        void storeTransaction(const TransactionInfo& info);
    };

}


#endif // TRANSACTIONMODEL_H