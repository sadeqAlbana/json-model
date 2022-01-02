/*
 * Copyright (C) 2021 Sadeq Albana
 *
 * Licensed under the GNU Lesser General Public License v3.0 :
 * https://www.gnu.org/licenses/lgpl-3.0.html
 */

#include "jsonmodel.h"
#include <QDebug>


JsonModel::JsonModel(QJsonArray data, ColumnList columns, QObject *parent) : QAbstractTableModel(parent),m_columns(columns)
{
    if(!data.isEmpty())
        setupData(data);
    else{
        if(columns.size()){ //if columns are defined the use them
            QJsonObject record;
            for (const Column &column : columns) {
                record[column.accessKey]=QJsonValue();
            }
            m_record=record;
//            qDebug()<<record.keys();
//            for(int i=0; i<m_record.count(); i++){
//                qDebug()<<fieldName(i);
//            }
        }
    }
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
        return  columns().size() ? columns().at(section).accessKey : fieldName(section);
    }

    if(role==Qt::DisplayRole){
        return  columns().size() ? columns().at(section).displayName : fieldName(section);
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

    if(index.row() >= rowCount() || index.column() >= columnCount())
        return QVariant();


    if(role==Qt::UserRole && columns().size()){
        return columns().at(index.column()).type;
    }

    if(role==Qt::CheckStateRole && checkable()){
        return m_checkList.value(index.row(),Qt::Checked);
    }


    if(role==Qt::DisplayRole || role==Qt::EditRole )
    {
        if(columns().size()){
            QVariant value;
            Column column=columns().at(index.column());
            if(column.parentKey.isNull()){
                value=m_records.at(index.row()).toObject().value(column.accessKey);
            }
            else{
                value=m_records.at(index.row()).toObject().value(column.parentKey);
                if(value.type()==QMetaType::QVariantMap || value.type()==QMetaType::QJsonValue || value.type()==QMetaType::QJsonObject){
                    value=value.toMap().value(column.key);
                }
            }
            return value;

        }else{
            return m_records.at(index.row()).toObject().value(fieldName(index.column()));
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
    return m_records.at(row).toObject();
}

bool JsonModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.row()>=rowCount() || index.column() >=columnCount())
        return false;
    //qDebug()<<"set data: " << value;
    if (data(index, role) != value) {
        QString key=headerData(index.column(),Qt::Horizontal,Qt::EditRole).toString();
        //qDebug()<<"key: " <<key;
        //m_records[index.row()].setValue(key,value);


        if(role==Qt::CheckStateRole && checkable()){
            m_checkList[index.row()]=static_cast<Qt::CheckState>(value.toInt());
            emit dataChanged(index, index); //assume all roles have changed, inefficent !
            return true;
        }

        if(role==Qt::DisplayRole || role==Qt::EditRole )
        {
            if(columns().size()){
                Column column=columns().at(index.column());
                if(column.parentKey.isNull()){
                    //value=m_records.at(index.row()).value(column.accessKey);
                    m_records[index.row()].toObject()[column.accessKey]=value.toJsonValue();
                    QJsonObject obj=m_records[index.row()].toObject();
                    obj[column.accessKey]=value.toJsonValue();
                    m_records[index.row()]=obj;
                }
                else{
                    m_records.at(index.row()).toObject()[column.parentKey].toObject()[column.key]=value.toJsonValue();
                    QJsonObject obj=m_records[index.row()].toObject();
                    QJsonObject child=obj[column.parentKey].toObject();
                    child[column.key]=value.toJsonValue();
                    obj[column.parentKey]=child;
                    m_records[index.row()]=obj;

                    //object[column.key]=value.toJsonValue(); //might not work
                    //m_records[index.row()].setValue(column.parentKey,object);
                    //value=m_records.at(index.row()).value(column.parentKey);

                }

            }else{
                //m_records[index.row()].setValue(index.column(),value.toJsonValue());
                //m_records[index.row()].toObject()[fieldName(index.column())]=value.toJsonValue();
                QJsonObject obj=m_records[index.row()].toObject();
                obj[fieldName(index.column())]=value.toJsonValue();
                m_records[index.row()]=obj;
            }

            //emit dataChanged(index, index, QVector<int>() << role);
            emit dataChanged(index, index); //assume all roles have changed, inefficent !
            //must emit this signle with the the other data role eg

            return true;
        }

        if(role>Qt::UserRole){ //used for QML views
            int column=role-(Qt::UserRole+1);
            return setData(this->index(index.row(),column),value);
        }

        return false;
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

    //m_buffer.remove(0,count);

    for(int i=0; i<count; i++){
        m_buffer.removeAt(0);
    }

    endInsertRows();

    return true;
}

bool JsonModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if(count>rowCount() || row>=rowCount())
        return false;

    beginRemoveRows(parent,row,row+count-1);
    //m_records.remove(row,count);
    for(int i=row; i<=count; i++){
        m_records.removeAt(row);
    }

    endRemoveRows();

    return true;
}

/*bool JsonModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endInsertColumns();
}*/



/*bool JsonModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endRemoveColumns();
}*/

QJsonObject JsonModel::record() const
{
    return m_record;
}

QJsonObject JsonModel::record(int index) const
{
    if(index>=rowCount() || index<0)
        return QJsonObject();
    return m_records.at(index).toObject();
}

/*
must call beginInsertRows before inserting data and endInsertRows after inserting,
thus we use this method that uses a buffer to append json data
*/
bool JsonModel::appendData(const QJsonArray &data)
{
    for(const QJsonValue &row : data)
    {
        m_buffer << row.toObject();
    }
    insertRows(rowCount(),m_buffer.count());
    return true;
}

QString JsonModel::fieldName(int index) const
{
    //return fields.value(index).name();
    //must use a more efficient method !
    return m_record.keys().at(index);
}
QVariant JsonModel::data(int row, int column) const
{
    return data(index(row,column));
}


QVariant JsonModel::data(int row, QString key) const
{
    return data(index(row,indexOf(key)),Qt::EditRole); //may not work as intended !
}

bool JsonModel::setData(int row, int column, QVariant data)
{
    return setData(index(row,column),data);
}

bool JsonModel::setData(int row, QString key, QVariant data)
{
    return setData(row,indexOf(key),data);
}

bool JsonModel::removeRecord(int row)
{
    return JsonModel::removeRow(row);
}

void JsonModel::setupData(const QJsonArray &data)
{
    beginResetModel();

    m_record=QJsonObject();
    m_records=QJsonArray();
    m_buffer=QJsonArray();
    for(const QJsonValue &row : data)
    {
        m_records << row.toObject();
    }


    QJsonObject record;
    if(columns().size()){ //if columns are defined the use them
        for (const Column &column : columns()) {
            record[column.accessKey]=QJsonValue();
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

void JsonModel::appendRecord(const QJsonObject &record)
{
    m_buffer << record;
    insertRows(rowCount(),1);
}

int JsonModel::indexOf(const QString &key) const
{
    for(int i=0; i<columnCount(); i++){
        if(headerData(i,Qt::Horizontal,Qt::EditRole)==key)
            return i;
    }
    return -1;
}

QModelIndex JsonModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);

    return QModelIndex();
}



QHash<int, QByteArray> JsonModel::roleNames() const
{
    QHash<int,QByteArray> roles;
    roles.insert(Qt::DisplayRole, "display");

    if(checkable()){
        roles.insert(Qt::CheckStateRole,"checkState");
    }


    if(columns().size()){ //used by qml views to determine delegate
        roles.insert(Qt::UserRole,"delegateType");
    }



    for (int i=0;i<columnCount();i++) {
     QByteArray roleName=headerData(i,Qt::Horizontal,Qt::EditRole).toString().toUtf8();
     roles.insert(Qt::UserRole+1+i,roleName);
    }

    //qDebug()<<"rolenames: "<<roles;

    return roles;
}

QJsonArray JsonModel::filterData(QJsonArray data)
{
    return data;
}

QJsonArray JsonModel::toJsonArray() const
{
    return m_records;
}

bool JsonModel::checkable() const
{
    return m_checkable;
}

void JsonModel::setCheckable(bool checkable)
{
    m_checkable = checkable;
    emit checkableChanged(checkable);
}

QSet<int> JsonModel::checkedRows()
{
  QSet<int> checked;
  for(int i=0; i<rowCount(); i++){
      if(m_checkList.value(i,Qt::Unchecked)==Qt::Checked){
          checked.insert(i);
      }
  }

  return checked;
}
