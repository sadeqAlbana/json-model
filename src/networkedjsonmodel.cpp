/*
 * Copyright (C) 2021 Sadeq Albana
 *
 * Licensed under the GNU Lesser General Public License v3.0 :
 * https://www.gnu.org/licenses/lgpl-3.0.html
 */

#include "networkedjsonmodel.h"
#include <QJsonDocument>
#include <QDebug>

NetworkedJsonModel::NetworkedJsonModel(QString Url,const JsonModelColumnList &columns, QObject *parent) : JsonModel(QJsonArray(),columns,parent), m_url(Url)
{
    setupCanFetchMoreTimer();
}

NetworkedJsonModel::NetworkedJsonModel(const JsonModelColumnList &columns, QObject *parent) : JsonModel(QJsonArray(),columns,parent)
{
    setupCanFetchMoreTimer();
}


void NetworkedJsonModel::refresh()
{
    if(m_busy){
        qWarning()<<"NetworkedJsonModel::refresh: it's busy, returning...";
        return;
    }

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


    if(m_canFetchMoreTimerLimit){
        return false;
    }

    if(m_busy){
        return false;
    }

    return (m_currentPage<m_lastPage);
}

void NetworkedJsonModel::fetchMore(const QModelIndex &parent)
{
    Q_UNUSED(parent);
    requestData();
}

void NetworkedJsonModel::setUrl(const QString &url)
{
    if(m_url==url)
        return;

    m_url=url;
    emit urlChanged();
}

QString NetworkedJsonModel::url() const
{
    return m_url;
}

void NetworkedJsonModel::setCurrentPage(int currentPage)
{
    if(m_currentPage==currentPage){
        return;
    }

    m_currentPage = currentPage;
    emit currentPageChanged();
}

int NetworkedJsonModel::currentPage() const
{
    return m_currentPage;
}

bool NetworkedJsonModel::hasPagination() const
{
    return m_hasPagination;
}

void NetworkedJsonModel::setupCanFetchMoreTimer()
{
    m_canFetchMoreTimer.setInterval(1000);
    connect(&m_canFetchMoreTimer,&QTimer::timeout,this,[this](){m_canFetchMoreTimerLimit=false;});
}

void NetworkedJsonModel::activateCanFetchMoreLimiter()
{
    m_canFetchMoreTimerLimit=true;
    m_canFetchMoreTimer.start();
}
