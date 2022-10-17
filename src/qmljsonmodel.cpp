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

    QmlJsonModelColumn *column = qobject_cast<QmlJsonModelColumn*>(value);
    if (column){
        model->m_qmlColumnList.append(column);
        model->m_columns.append(*column);
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
    return model->m_qmlColumnList.clear(); //needs fix
}

void QmlJsonModel::columnList_replace(QQmlListProperty<QmlJsonModelColumn> *property, qsizetype index, QmlJsonModelColumn *value)
{
    QmlJsonModel *model = static_cast<QmlJsonModel*>(property->object);
     if (QmlJsonModelColumn *column = qobject_cast<QmlJsonModelColumn*>(value))
         return model->m_qmlColumnList.replace(index, column); //needs fix
}

void QmlJsonModel::columnList_removeLast(QQmlListProperty<QmlJsonModelColumn> *property)
{
    QmlJsonModel *model = static_cast<QmlJsonModel*>(property->object);
    model->m_qmlColumnList.removeLast();
}
