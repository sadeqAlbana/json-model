#include "qmljsonmodel.h"

QmlJsonModel::QmlJsonModel(QObject *parent)
    : JsonModel{parent}
{

}

QQmlListProperty<QmlJsonModelColumn> QmlJsonModel::qmlColumnList()
{
    return QQmlListProperty<QmlJsonModelColumn>(this, nullptr,
         &QmlJsonModel::columnList_append,
         &QmlJsonModel::columnList_count,
         &QmlJsonModel::columnList_at,
         &QmlJsonModel::columnList_clear,
         &QmlJsonModel::columnList_replace,
         &QmlJsonModel::columnList_removeLast);
}



void QmlJsonModel::columnList_append(QQmlListProperty<QmlJsonModelColumn> *property, QmlJsonModelColumn *value)
{
    QmlJsonModel *model = static_cast<QmlJsonModel*>(property->object);
//    QmlJsonModelColumn *column = qobject_cast<QmlJsonModelColumn*>(value);
    if (value){
        model->m_qmlColumnList.append(value);
    }
}

qsizetype QmlJsonModel::columnList_count(QQmlListProperty<QmlJsonModelColumn> *property)
{
    const QmlJsonModel *model = static_cast<QmlJsonModel*>(property->object);
    return model->m_qmlColumnList.count();
}

QmlJsonModelColumn *QmlJsonModel::columnList_at(QQmlListProperty<QmlJsonModelColumn> *property, qsizetype index)
{
    const QmlJsonModel *model = static_cast<QmlJsonModel*>(property->object);
    return model->m_qmlColumnList.at(index);
}

void QmlJsonModel::columnList_clear(QQmlListProperty<QmlJsonModelColumn> *property)
{
    QmlJsonModel *model = static_cast<QmlJsonModel*>(property->object);
    model->m_qmlColumnList.clear();
    model->m_columns.clear();
}

void QmlJsonModel::columnList_replace(QQmlListProperty<QmlJsonModelColumn> *property, qsizetype index, QmlJsonModelColumn *value)
{
    QmlJsonModel *model = static_cast<QmlJsonModel*>(property->object);
     if (QmlJsonModelColumn *column = qobject_cast<QmlJsonModelColumn*>(value)){
          model->m_qmlColumnList.replace(index, column);
          model->m_columns.replace(index, *column);

     }
}

void QmlJsonModel::columnList_removeLast(QQmlListProperty<QmlJsonModelColumn> *property)
{
    QmlJsonModel *model = static_cast<QmlJsonModel*>(property->object);
    model->m_qmlColumnList.removeLast();
    model->m_columns.removeLast();

}

QVariant QmlJsonModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation==Qt::Vertical)
        return QVariant();

    if(m_qmlColumnList.size() && section >= m_qmlColumnList.size()){
        return QVariant();
    }else
        if(m_record.count() && section >= m_record.count()){
        return QVariant();
    }


    if(role==Qt::EditRole){
        return  m_qmlColumnList.size() ? m_qmlColumnList.at(section)->m_accessKey : fieldName(section);
    }

    if(role==Qt::DisplayRole){
        return  m_qmlColumnList.size() ? m_qmlColumnList.at(section)->m_displayName : fieldName(section);
    }

    return QVariant();
}

void QmlJsonModel::classBegin()
{

}

void QmlJsonModel::componentComplete()
{
    for(int i=0; i<m_qmlColumnList.size(); i++){
        m_columns.append(*m_qmlColumnList.at(i));
    }
    loadRecord();
}

