#include "jsonmodel.h"
#include <QDebug>


JsonModel::JsonModel(QJsonArray data, ColumnList columns, QObject *parent) : QAbstractTableModel(parent),m_columns(columns)
{
    if(!data.isEmpty())
        setupData(data);
}

JsonModel::~JsonModel()
{

}

QVariant JsonModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation==Qt::Vertical)
        return QVariant();

    if(columns().size() && section >= columns().size()){
        return QVariant();
    }else
        if(m_record.count() && section >= m_record.count()){
        return QVariant();
    }


    if(role==Qt::EditRole){
        return  columns().size() ? columns().at(section).key : m_record.fieldName(section);
    }

    if(role==Qt::DisplayRole){
        return  columns().size() ? columns().at(section).displayName : m_record.fieldName(section);
    }

    return QVariant();
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

    if(role>Qt::UserRole){ //used for QML views
        int column=role-(Qt::UserRole+1);
        return data(this->index(index.row(),column));
    }

    if(role==Qt::TextAlignmentRole){
        return Qt::AlignCenter;
    }

    return QVariant();
}

QJsonObject JsonModel::jsonObject(const int &row) const
{
    return m_records.value(row);
}

bool JsonModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.row()>=rowCount() || index.column() >=columnCount())
        return false;

    if (data(index, role) != value) {

        QString key=headerData(index.column(),Qt::Horizontal).toString();
        m_records[index.row()].setValue(key,value);
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}



Qt::ItemFlags JsonModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool JsonModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if(count>m_buffer.size())
        return false;

    beginInsertRows(parent, row, row + count - 1);

    for(int i=0; i<count; i++){
        m_records << m_buffer[i];
    }

    m_buffer.remove(0,count);

    endInsertRows();

    return true;
}

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

bool JsonModel::appendData(const QJsonArray &data)
{

    for(const QJsonValue &row : data)
    {
        m_buffer << JsonModelRecord(row.toObject());
    }
    insertRows(rowCount(),m_buffer.count());
    return true;
}

void JsonModel::setupData(const QJsonArray &data)
{
    beginResetModel();
    m_record.clear(); // will cause memory leak if you don't clear the fields too !
    m_records.clear();
    m_buffer.clear();
    for(const QJsonValue &row : data)
    {
        m_records << JsonModelRecord(row.toObject());
    }


    QJsonObject record;
    if(columns().size()){ //if columns are defined the use them
        for (const Column &column : columns()) {
            record[column.key]=QJsonValue();
        }
    }
    else{
        record=data.at(0).toObject(); //take the first record
        for(const QString &key: record.keys())
            record.insert(key,QJsonValue()); //clear it's values
    }
    m_record=record;
    endResetModel();
}


ColumnList JsonModel::columns() const
{
    return m_columns;
}

QModelIndex JsonModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}



QHash<int, QByteArray> JsonModel::roleNames() const
{
    QHash<int,QByteArray> roles;

    for (int i=0;i<columnCount();i++) {
     QByteArray roleName=headerData(i,Qt::Horizontal,Qt::EditRole).toString().toUtf8();
     roles.insert(Qt::UserRole+1+i,roleName);
    }

    //qDebug()<<"rolenames: "<<roles;

    return roles;
}
