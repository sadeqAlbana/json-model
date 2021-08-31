#ifndef NETWORKEDJSONMODEL_H
#define NETWORKEDJSONMODEL_H

#include "jsonmodel.h"
//#include <network/messagehandler.h>
class NetworkResponse;



class NetworkedJsonModel : public JsonModel
{
    Q_OBJECT
public:
    NetworkedJsonModel(QString Url, const ColumnList &columns=ColumnList(), QObject *parent=nullptr);

    void refresh();
    Q_INVOKABLE virtual void requestData()=0;
    virtual Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;


protected:
    QString url;
    int _lastPage;
    int _currentPage;
    bool _busy;

signals:
    void dataRecevied();
};

#endif // NETWORKEDJSONMODEL_H
