/*
 * Copyright (C) 2021 Sadeq Albana
 *
 * Licensed under the GNU Lesser General Public License v3.0 :
 * https://www.gnu.org/licenses/lgpl-3.0.html
 */

#include "networkedjsonmodel.h"
#include <QJsonDocument>
#include <QDebug>

NetworkedJsonModel::NetworkedJsonModel(QString Url,const JsonModelColumnList &columns, QObject *parent) : JsonModel(QJsonArray(),columns,parent), _url(Url)
{

}

NetworkedJsonModel::NetworkedJsonModel(const JsonModelColumnList &columns, QObject *parent) : JsonModel(QJsonArray(),columns,parent)
{

}


void NetworkedJsonModel::refresh()
{
    m_currentPage=0;
    requestData();
}


Qt::ItemFlags NetworkedJsonModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return JsonModel::flags(index);
}


bool NetworkedJsonModel::canFetchMore(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if(!m_hasPagination)
        return false;

    return (m_currentPage<_lastPage && !_busy);
}

void NetworkedJsonModel::fetchMore(const QModelIndex &parent)
{
    Q_UNUSED(parent);
    requestData();
}

void NetworkedJsonModel::setUrl(const QString &url)
{
    this->_url=url;
    emit urlChanged(url);
}

QString NetworkedJsonModel::url() const
{
    return _url;
}

void NetworkedJsonModel::setCurrentPage(int currentPage)
{
    m_currentPage = currentPage;
    emit currentPageChanged(m_currentPage);
}

int NetworkedJsonModel::currentPage() const
{
    return m_currentPage;
}

bool NetworkedJsonModel::hasPagination() const
{
    return m_hasPagination;
}
