#ifndef JSONMODEL_H
#define JSONMODEL_H

#include <QAbstractTableModel>
#include "jsonmodelrecord.h"
#include <QJsonArray>
#include <QJsonObject>

struct Column{
    Column(const QString &key, const QString &displayName,const QString &parentKey=QString()):
    key(key),displayName(displayName),parentKey(parentKey){}
    QString key;
    QString displayName;
    QString parentKey;
};

class ColumnList : public QList<Column>{
public:
    ColumnList(){}
    bool contains(const QString &key){
        for(const Column &column : *this){
            if(column.key==key){
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
            array << column.key;

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

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override; //columns compatible
    Q_INVOKABLE QJsonObject jsonObject(const int &row) const;

    // Editable:
    //this method should work but only from views
    bool setData(const QModelIndex &index, const QVariant &value,
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

    Q_INVOKABLE void setupData(const QJsonArray &data);

    ColumnList columns() const;

    virtual QModelIndex parent(const QModelIndex &child) const override;
    QHash<int, QByteArray> roleNames() const override;




protected:
    QVector<QMap<QString,QJsonValue>> m;
    QVector<JsonModelRecord> m_records;
    QVector<JsonModelRecord> m_buffer;

    JsonModelRecord m_record;
    ColumnList m_columns;
};

#endif // JSONMODEL_H
