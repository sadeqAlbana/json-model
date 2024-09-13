/*
 * Copyright (C) 2021 Sadeq Albana
 *
 * Licensed under the GNU Lesser General Public License v3.0 :
 * https://www.gnu.org/licenses/lgpl-3.0.html
 */

#include "jsonmodel.h"
#include <QDebug>
#include <QJsonValue>
#include <algorithm>
JsonModel::JsonModel(QObject *parent) : QAbstractTableModel(parent)
{

}

JsonModel::JsonModel(QJsonArray data, JsonModelColumnList columns, QObject *parent) : QAbstractTableModel(parent),m_columns(columns)
{
    if(!data.isEmpty())
        setRecords(data);
    else{
        if(columns.size()){ //if columns are defined the use them
            QJsonObject record;
            for (const JsonModelColumn &column : columns) {
                record[column.m_accessKey]=QJsonValue();
            }
            setRecord(record);
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
        return  columns().size() ? columns().at(section).m_accessKey : fieldName(section);
    }

    if(role==Qt::DisplayRole){
        return  columns().size() ? columns().at(section).m_displayName : fieldName(section);
    }

    return QVariant();
}

int JsonModel::rowCount(const QModelIndex &parent) const
{
    // if(parent.isValid())
    //     return 0;

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
        return columns().at(index.column()).m_type;
    }

    if(role==Qt::CheckStateRole && checkable()){
        return m_checkList.value(index.row(),Qt::Unchecked);
    }

    if(role>Qt::UserRole && columns().size()){

        JsonModelColumn column=columns().at(index.column());

        if(column.m_type=="link" && role==ItemDataRole::LinkRole){
            QUrl link=column.m_metadata["link"].toUrl();
            return link;
        }
        if(column.m_type=="link" && role==ItemDataRole::LinkKeyRole){
            QString key=column.m_metadata["linkKey"].toString();
            return key;
        }

        if(column.m_type=="link" && role==ItemDataRole::LinkKeyDataRole){
            QString key=column.m_metadata["linkKey"].toString();

            return m_records.at(index.row())[key];
        }
    }


    if(role==Qt::DisplayRole || role==Qt::EditRole )
    {
        if(columns().size()){
            QVariant value;
            JsonModelColumn column=columns().at(index.column());
            if(column.m_parentKey.isNull()){
                value=m_records.at(index.row()).toObject().value(column.m_accessKey);
            }
            else{
                QStringList keys=column.m_parentKey.split('.');

                if(!m_useNestedParentKeys || keys.count()<=1){
                    value=m_records.at(index.row()).toObject().value(column.m_parentKey);
                    QMetaType::Type type=static_cast<QMetaType::Type>(value.typeId());
                    if(type==QMetaType::QVariantMap || type==QMetaType::QJsonValue || type==QMetaType::QJsonObject){
                        value=value.toMap().value(column.m_key);
                    }

                }else{

                    if(keys.count()==2){
                        value=m_records.at(index.row()).toObject().value(keys[0]);
                        QMetaType::Type type=static_cast<QMetaType::Type>(value.typeId());
                        if(type==QMetaType::QVariantMap || type==QMetaType::QJsonValue || type==QMetaType::QJsonObject){
                            value=value.toMap().value(keys[1]);
                            value=value.toMap().value(column.m_key);
                        }
                    }
                }


            }
            return value;

        }else{
            return m_records.at(index.row()).toObject().value(fieldName(index.column()));
        }
    }

    if(role>Qt::UserRole){ //used for QML views

        if(role>(Qt::UserRole+2000+m_columns.size())){
            QString roleName=roleNames().value(role);
            if(!roleName.isEmpty()){
                roleName=roleName.removeFirst();
                return m_records.at(index.row())[roleName];
            }
        }

        int column=role-(Qt::UserRole+1000);
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
                JsonModelColumn column=columns().at(index.column());
                if(column.m_parentKey.isNull()){
                    //value=m_records.at(index.row()).value(column.accessKey);
                    m_records[index.row()].toObject()[column.m_accessKey]=value.toJsonValue();
                    QJsonObject obj=m_records[index.row()].toObject();
                    obj[column.m_accessKey]=value.toJsonValue();
                    m_records[index.row()]=obj;
                }
                else{
                    m_records.at(index.row()).toObject()[column.m_parentKey].toObject()[column.m_key]=value.toJsonValue();
                    QJsonObject obj=m_records[index.row()].toObject();
                    QJsonObject child=obj[column.m_parentKey].toObject();
                    child[column.m_key]=value.toJsonValue();
                    obj[column.m_parentKey]=child;
                    m_records[index.row()]=obj;

                    //object[column.key]=value.toJsonValue(); //might not work
                    //m_records[index.row()].setValue(column.m_parentKey,object);
                    //value=m_records.at(index.row()).value(column.m_parentKey);

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
            int column=role-(Qt::UserRole+1000);
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

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if(m_columns.size()){
        if(m_columns.at(index.column()).m_editable){
            flags = flags | Qt::ItemIsEditable;
        }
    }

    return flags;
}

bool JsonModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if(count>m_buffer.size())
        return false;

    beginInsertRows(parent, row, row + count - 1);

    for(int i=0; i<count; i++){
        if(row==rowCount()){
            m_records << m_buffer[i];
        }else{
            m_records.insert(row+i,m_buffer[i]);
        }
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
    if(row<0 || count<1)
        return false;

    if(row+count>rowCount())
        return false;

    beginRemoveRows(parent,row,row+count-1);
    //m_records.remove(row,count);
    for(int i=row; i<row+count; i++){
        m_records.removeAt(row);
    }

    endRemoveRows();

    return true;
}

/*bool JsonModel::insertJsonModelColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertJsonModelColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endInsertJsonModelColumns();
}*/



/*bool JsonModel::removeJsonModelColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveJsonModelColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endRemoveJsonModelColumns();
}*/

QJsonObject JsonModel::record() const
{
    return m_record;
}


QJsonObject JsonModel::recordAt(int index) const
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

void JsonModel::setRecords(const QJsonArray &data)
{
    beginResetModel();
    m_record=QJsonObject();
    m_buffer=QJsonArray();

    m_records=data;


    loadRecord();
    endResetModel();


    emit recordsChanged();
}


JsonModelColumnList JsonModel::columns() const
{
    return m_columns;
}

void JsonModel::appendRecord(const QJsonObject &record)
{
    if(!rowCount()){
        setRecords(QJsonArray{record});
        return;
    }

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
    roles.insert(Qt::EditRole, "edit");

    roles.insert(ItemDataRole::LinkRole,"__link");
    roles.insert(ItemDataRole::LinkKeyRole,"__linkKey");
    roles.insert(ItemDataRole::LinkKeyDataRole,"__linkKeyData");


    if(checkable()){
        roles.insert(Qt::CheckStateRole,"checkState");
    }


    if(columns().size()){ //used by qml views to determine delegate
        roles.insert(Qt::UserRole,"delegateType");
    }


    int lastRoleId=-1;
    for (int i=0;i<columnCount();i++) {
        lastRoleId=Qt::UserRole+1000+i;
        QByteArray roleName=headerData(i,Qt::Horizontal,Qt::EditRole).toString().toUtf8();
        roles.insert(lastRoleId,roleName);
    }


    if(columns().size()){
        lastRoleId+=1000;
        if(m_records.size()){
            QJsonObject firstRecord=m_records.first().toObject();
            QStringList keys=firstRecord.keys();
            for(int i=0; i<keys.size(); i++){
                roles.insert(++lastRoleId,QString("_%1").arg(keys.at(i)).toUtf8());
            }
        }
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

QJsonArray JsonModel::toJsonArray(const Qt::CheckState checkState) const
{
    QJsonArray records;
    for(int i=0; i<rowCount(); i++){
        if(data(index(i,0),Qt::CheckStateRole)==checkState){
            records << m_records.at(i);
        }
    }
    return records;
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

bool JsonModel::insertRecord(const QJsonObject &record)
{
    m_buffer << record;
    return insertRows(0,1);
}

bool JsonModel::uncheckAll()
{
    return setCheckStateForRecords(Qt::Unchecked);
}

bool JsonModel::checkAll()
{
    return setCheckStateForRecords(Qt::Checked);
}

void JsonModel::setRecord(const QJsonObject &newRecord)
{
    if (m_record == newRecord)
        return;
    m_record = newRecord;
    emit recordChanged();
}

void JsonModel::resetRecord()
{
    setRecord({});
}

void JsonModel::resetRecords()
{
    m_records=QJsonArray();
}

int JsonModel::roleFromKey(const QString &key) const
{
    auto roles =this->roleNames();
    return roles.key(key.toUtf8());
}

bool JsonModel::setCheckStateForRecords(Qt::CheckState checkState)
{
    if(!m_checkable){
        return false;
    }

    for(int i=0; i<rowCount(); i++){
        setData(index(i,0),checkState,Qt::CheckStateRole);
    }
    return true;
}

Qt::CheckState JsonModel::checkState() const
{
    if(!m_checkable){
        return Qt::Unchecked;
    }

    int counter=0;
    for(int i=0; i<rowCount(); i++){
        if(data(index(i,0),Qt::CheckStateRole)==Qt::Checked){
            counter++;
        }
    }
    return counter==0? Qt::Unchecked : counter==rowCount()? Qt::Checked : Qt::PartiallyChecked;
}

const QJsonArray &JsonModel::records() const
{
    return m_records;
}

void JsonModel::sort(int column, Qt::SortOrder order)
{
    std::sort(m_records.begin(),m_records.end(),[this,column](const QJsonValue &a, const QJsonValue &b){
        QString key=this->headerData(column,Qt::Horizontal,Qt::EditRole).toString();
        return a[key].toString()<b[key].toString();

    });

    emit layoutChanged();
}

void JsonModel::sort(const QString &key, Qt::SortOrder order)
{
    sort(indexOf(key),order);
}

void JsonModel::loadRecord()
{
    QJsonObject record;
    if(columns().size()){ //if columns are defined the use them
        for (const JsonModelColumn &column : columns()) {
            record[column.m_accessKey]=QJsonValue();
        }
    }
    else{
        record=m_records.at(0).toObject(); //take the first record
        for(const QString &key: record.keys())
            record.insert(key,QJsonValue()); //clear it's values
    }

    setRecord(record);
}

bool JsonModel::useNestedParentKeys() const
{
    return m_useNestedParentKeys;
}

void JsonModel::setUseNestedParentKeys(bool newUseNestedParentKeys)
{
    if (m_useNestedParentKeys == newUseNestedParentKeys)
        return;
    m_useNestedParentKeys = newUseNestedParentKeys;
    emit useNestedParentKeysChanged();
}


int JsonModel::matchChecked(const QJsonArray &checked, const QString &localKey, const QString &foriegnKey)
{
    if(!m_checkable)
        return -1;

    int  matched=0;
    for(int i=0; i<rowCount(); i++){
        QVariant localValue=data(i,localKey);
        for(int j=0; j<checked.size(); j++){
            QVariant foriegnValue=checked[j][foriegnKey];
            if(localValue==foriegnValue){
                matched++;
                setData(this->index(i,0),Qt::Checked,Qt::CheckStateRole);
                break;
            }

        }
    }
    return matched;
}




