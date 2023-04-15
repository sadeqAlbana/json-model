/*
 * Copyright (C) 2021 Sadeq Albana
 *
 * Licensed under the GNU Lesser General Public License v3.0 :
 * https://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef JSONMODEL_H
#define JSONMODEL_H

#include <QAbstractTableModel>
#include <QJsonArray>
#include <QJsonObject>
#include <QQmlEngine>
#include "jsonmodelcolumn.h"

inline void swap(QJsonValueRef v1, QJsonValueRef v2)
{
    QJsonValue temp(v1);
    v1 = QJsonValue(v2);
    v2 = temp;
}

class JsonModelColumnList : public QList<JsonModelColumn>{
public:
    JsonModelColumnList() : QList<JsonModelColumn>(){}
    inline JsonModelColumnList(std::initializer_list<JsonModelColumn> args) : QList<JsonModelColumn>(args) { }

    bool contains(const QString &key){
        for(const JsonModelColumn &column : *this){
            if(column.m_accessKey==key){
                return true;
            }
        }
        return false;
    }
    JsonModelColumnList & operator <<(const JsonModelColumn &column){
        append(column);
        return *this;
    }

    operator QJsonArray(){
        QJsonArray array;
        for(const JsonModelColumn &column : *this)
            array << column.m_accessKey;

        return array;
    }
};

class JsonModel : public QAbstractTableModel
{
    Q_OBJECT
    QML_ELEMENT
public:
    Q_PROPERTY(bool checkable MEMBER m_checkable READ checkable WRITE setCheckable NOTIFY checkableChanged)

    Q_INVOKABLE explicit JsonModel(QObject *parent = nullptr);

    explicit JsonModel(const QJsonArray data, JsonModelColumnList columns=JsonModelColumnList(), QObject *parent = nullptr);
    // Header:
    ~JsonModel();
     virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override; //columns compatible

    //bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    // Basic functionality:
     int rowCount(const QModelIndex &parent = QModelIndex()) const override;
     int columnCount(const QModelIndex &parent = QModelIndex()) const override; //columns compatible

    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override; //columns compatible
    Q_INVOKABLE QJsonObject jsonObject(const int &row) const;

    // Editable:
    //this method should work but only from views
    virtual bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override; //columns compatible

    virtual Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    //bool insertJsonModelColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    //bool removeJsonModelColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    QJsonObject record() const;
    Q_INVOKABLE QJsonObject record(int index) const;
    bool appendData(const QJsonArray &data);
    QString fieldName(int index) const;


    Q_INVOKABLE QVariant data(int row,QString key) const;
    Q_INVOKABLE bool setData(int row, int column, QVariant data);
    Q_INVOKABLE bool setData(int row, QString key, QVariant data);
    Q_INVOKABLE bool removeRecord(int row);

    Q_INVOKABLE QVariant data(int row,int column) const;
    Q_INVOKABLE void setRecords(const QJsonArray &data);

    JsonModelColumnList columns() const;
    Q_INVOKABLE void appendRecord(const QJsonObject &record);

    Q_INVOKABLE int indexOf(const QString &key) const;

    virtual QModelIndex parent(const QModelIndex &child) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    virtual QJsonArray filterData(QJsonArray data);

    Q_INVOKABLE QJsonArray toJsonArray() const;


    bool checkable() const;
    void setCheckable(bool checkable);

    QSet<int> checkedRows();

    Q_INVOKABLE bool insertRecord(const QJsonObject &record);
    Q_INVOKABLE void uncheckAll();

    void setRecord(const QJsonObject &newRecord);
    void resetRecord();
    void resetRecords();

    const QJsonArray & records() const;

    Q_INVOKABLE void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
    Q_INVOKABLE void sort(const QString &key, Qt::SortOrder order = Qt::AscendingOrder);
    Q_INVOKABLE int matchChecked(const QJsonArray &checked, const QString &localKey, const QString &foriegnKey);

signals:
    void checkableChanged(bool checkable);

    void recordChanged();
    void recordsChanged();

protected:
    //    QVector<QMap<QString,QJsonValue>> m;
    QJsonArray m_records;
    QJsonArray m_buffer;

    QJsonObject m_record;
    JsonModelColumnList m_columns;
    bool m_checkable=false;
    QMap<int,Qt::CheckState> m_checkList;
    void loadRecord();
private:
    Q_PROPERTY(QJsonObject record READ record WRITE setRecord RESET resetRecord NOTIFY recordChanged)
    Q_PROPERTY(QJsonArray  records READ records WRITE setRecords RESET resetRecords NOTIFY recordsChanged)
};

#endif // JSONMODEL_H
