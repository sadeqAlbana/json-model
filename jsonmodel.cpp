#include "jsonmodel.h"
#include <QDebug>
JsonModel::JsonModel(QObject *parent) : QAbstractTableModel(parent)
{

}

JsonModel::JsonModel(QJsonArray data, QObject *parent) : QAbstractTableModel(parent)
{
    setupData(data);
}

JsonModel::~JsonModel()
{

}

QVariant JsonModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return orientation==Qt::Horizontal && role==Qt::DisplayRole && section < columns().size() ?
                //m_record.fieldName(section) :
                columns().at(section).displayName :
                QVariant();
}

int JsonModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;

    return m_records.count();
}

int JsonModel::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;

    int size= columns().size();
    return  size ? size : m_record.count();
}

QVariant JsonModel::data(const QModelIndex &index, int role) const
{   
    if (!index.isValid())
        return QVariant();

    if(index.row() >= rowCount() || index.column() >= columns().count())
        return QVariant();

    if(role==Qt::DisplayRole || role==Qt::EditRole )
    {
        if(columns().size()){
            QVariant value;
            Column column=columns().at(index.column());
            if(column.parentKey.isNull()){
                value=m_records.at(index.row()).value(column.key);
            }
            else{
                value=m_records.at(index.row()).value(column.parentKey);
                if(value.type()==QMetaType::QVariantMap){
                    value=value.toMap().value(column.key);
                }
            }
            return value;
        }

    }
    if(role==Qt::TextAlignmentRole){
        return Qt::AlignCenter;
    }



    return QVariant();
}

QJsonObject JsonModel::data(const int &row) const
{
    return m_records.value(row);
}

bool JsonModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        m_records[index.row()].setValue(index.column(),value);
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags JsonModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    //return Qt::ItemIsEditable // FIXME: Implement me!

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

/*bool JsonModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
}*/

/*bool JsonModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endInsertColumns();
}*/

/*bool JsonModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me senpai!
    endRemoveRows();
}*/

/*bool JsonModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endRemoveColumns();
}*/

JsonModelRecord JsonModel::record() const
{
    return m_record;
}

void JsonModel::setupData(const QJsonArray &data)
{
    beginResetModel();
    m_record.clear(); // will cause memory leak if you don't clear the fields too !
    m_records.clear();
    for(QJsonValue row : data)
    {
        m_records << JsonModelRecord(row.toObject());
    }


    QJsonObject record;
    if(columns().size()){
        for (const Column &column : columns()) {
            record[column.key]=QJsonValue();
        }
    }
    else{
        record=data.at(0).toObject();
        for(QString key: record.keys())
            record.insert(key,QJsonValue());
    }
    m_record=record;
    endResetModel();
}


ColumnList JsonModel::columns() const
{
    return ColumnList();
}

QModelIndex JsonModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}
