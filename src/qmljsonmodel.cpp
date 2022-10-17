#include "qmljsonmodel.h"

QmlJsonModel::QmlJsonModel(QObject *parent)
    : JsonModel{parent}
{

}

const QQmlListProperty<QmlJsonModelColumn> &QmlJsonModel::qmlColumnList() const
{
    return m_qmlColumnList;
}

void QmlJsonModel::setQmlColumnList(const QQmlListProperty<QmlJsonModelColumn> &newQmlColumnList)
{
    if (m_qmlColumnList == newQmlColumnList)
        return;
    m_qmlColumnList = newQmlColumnList;


    emit qmlColumnListChanged();
}

void QmlJsonModel::resetQmlColumnList()
{
    setQmlColumnList({}); // TODO: Adapt to use your actual default value
}
