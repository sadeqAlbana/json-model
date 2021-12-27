/*
 * Copyright (C) 2021 Sadeq Albana
 *
 * Licensed under the GNU Lesser General Public License v3.0 :
 * https://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef JSONMODEL_H
#define JSONMODEL_H

#include <QAbstractTableModel>
#include "jsonmodelrecord.h"
#include <QJsonArray>
#include <QJsonObject>

struct Column{
    Column(const QString &key, const QString &displayName,const QString &parentKey=QString(),const QString &type=QStringLiteral("text")):
    key(key),displayName(displayName),parentKey(parentKey),type(type){
        accessKey= parentKey.isEmpty() ?  key : QString("%1.%2").arg(parentKey).arg(key);
    }

    QString key;
    QString displayName;
    QString parentKey;
    QString type;
    QString accessKey;
};

class ColumnList : public QList<Column>{
public:
    ColumnList() : QList<Column>(){}
    inline ColumnList(std::initializer_list<Column> args) : QList<Column>(args) { }

    bool contains(const QString &key){
        for(const Column &column : *this){
            if(column.accessKey==key){
                return true;
            }
        }
        return false;
    }
    ColumnList & operator <<(const Column &column){
        append(column);
        return *this;
    }

    operator QJsonArray(){
        QJsonArray array;
        for(const Column &column : *this)
            array << column.accessKey;

        return array;
    }
};

class JsonModel : public QAbstractTableModel
{
    Q_OBJECT
public:
//    explicit JsonModel(QObject *parent = nullptr);
    explicit JsonModel(QJsonArray data=QJsonArray(), ColumnList columns=ColumnList(), QObject *parent = nullptr);
    // Header:
    ~JsonModel();
     QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override; //columns compatible

    //bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    // Basic functionality:
     int rowCount(const QModelIndex &parent = QModelIndex()) const override;
     int columnCount(const QModelIndex &parent = QModelIndex()) const override; //columns compatible

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override; //columns compatible
    Q_INVOKABLE QJsonObject jsonObject(const int &row) const;

    // Editable:
    //this method should work but only from views
    virtual bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override; //columns compatible

    virtual Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    //bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    //bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    //bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    JsonModelRecord record() const;
    JsonModelRecord record(int index) const{return m_records.at(index);}
    bool appendData(const QJsonArray &data);

    Q_INVOKABLE QVariant data(int row,QString key) const;
    Q_INVOKABLE bool setData(int row, int column, QVariant data);
    Q_INVOKABLE bool setData(int row, QString key, QVariant data);

    Q_INVOKABLE QVariant data(int row,int column) const;
    Q_INVOKABLE void setupData(const QJsonArray &data);

    ColumnList columns() const;
    Q_INVOKABLE void appendRecord(const QJsonObject &record);

    Q_INVOKABLE int indexOf(const QString &key) const;

    virtual QModelIndex parent(const QModelIndex &child) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    virtual QJsonArray filterData(QJsonArray data);


protected:
    QVector<QMap<QString,QJsonValue>> m;
    QVector<JsonModelRecord> m_records;
    QVector<JsonModelRecord> m_buffer;

    JsonModelRecord m_record;
    ColumnList m_columns;
};

#endif // JSONMODEL_H
