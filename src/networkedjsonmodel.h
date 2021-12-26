/*
 * Copyright (C) 2021 Sadeq Albana
 *
 * Licensed under the GNU Lesser General Public License v3.0 :
 * https://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef NETWORKEDJSONMODEL_H
#define NETWORKEDJSONMODEL_H

#include "jsonmodel.h"
//#include <network/messagehandler.h>
class NetworkResponse;



class NetworkedJsonModel : public JsonModel
{
    Q_OBJECT
    Q_PROPERTY(QString url MEMBER _url READ url NOTIFY urlChanged WRITE setUrl)
    Q_PROPERTY(int currentPage MEMBER m_currentPage READ currentPage NOTIFY currentPageChanged WRITE setCurrentPage)

public:
    NetworkedJsonModel(QString Url, const ColumnList &columns=ColumnList(), QObject *parent=nullptr);

    Q_INVOKABLE void refresh();
    Q_INVOKABLE virtual void requestData()=0;
    virtual Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;
    Q_INVOKABLE void setUrl(const QString &url);
    Q_INVOKABLE QString url() const;
    void setCurrentPage(int currentPage);
    int currentPage() const;

signals:
    void urlChanged(QString url);
    void currentPageChanged(int page);

protected:
    QString _url;
    int _lastPage;
    int _currentPage;
    bool _busy;
    int m_currentPage;

signals:
    void dataRecevied();
};

#endif // NETWORKEDJSONMODEL_H
