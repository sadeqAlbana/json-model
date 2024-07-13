/*
 * Copyright (C) 2021 Sadeq Albana
 *
 * Licensed under the GNU Lesser General Public License v3.0 :
 * https://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef NETWORKEDJSONMODEL_H
#define NETWORKEDJSONMODEL_H

#include "jsonmodel.h"
#include <QTimer>
//#include <network/messagehandler.h>
class NetworkResponse;



class NetworkedJsonModel : public JsonModel
{
    Q_OBJECT
    Q_PROPERTY(QString url  READ url NOTIFY urlChanged WRITE setUrl)
    Q_PROPERTY(int currentPage READ currentPage WRITE setCurrentPage NOTIFY currentPageChanged)

public:
    NetworkedJsonModel(QString Url, const JsonModelColumnList &columns=JsonModelColumnList(), QObject *parent=nullptr);
    NetworkedJsonModel(const JsonModelColumnList &columns=JsonModelColumnList(), QObject *parent=nullptr);

    Q_INVOKABLE void refresh();
    Q_INVOKABLE virtual void requestData()=0;
    virtual Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;
    Q_INVOKABLE void setUrl(const QString &url);
    Q_INVOKABLE QString url() const;
    void setCurrentPage(int currentPage);
    int currentPage() const;

    bool hasPagination() const;

signals:
    void urlChanged();
    void currentPageChanged();

protected:
    QString m_url;
    bool m_hasPagination=false;
    int m_lastPage=-1;
    bool m_busy=false;
    bool m_canFetchMoreTimerLimit=false;
    int m_currentPage=0;
    QTimer m_canFetchMoreTimer;
    void setupCanFetchMoreTimer();
    void activateCanFetchMoreLimiter();

signals:
    void dataRecevied();
};

#endif // NETWORKEDJSONMODEL_H
