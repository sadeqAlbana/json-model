#include "networkedjsonmodel.h"
#include <QJsonDocument>
#include <QDebug>

NetworkedJsonModel::NetworkedJsonModel(QString Url,const ColumnList &columns, QObject *parent) : JsonModel(QJsonArray(),columns,parent), url(Url)
{
    _currentPage=0;
    _lastPage=-1;
}


void NetworkedJsonModel::refresh()
{
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
    qDebug()<<"can fetch more: " << (_currentPage<_lastPage && !_busy);
    return (_currentPage<_lastPage && !_busy);
}

void NetworkedJsonModel::fetchMore(const QModelIndex &parent)
{
        requestData();
}







