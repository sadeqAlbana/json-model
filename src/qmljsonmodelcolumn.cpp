#include "qmljsonmodelcolumn.h"
#include <QDebug>
void QmlJsonModelColumn::setKey(const QString &key)
{
    m_key=key;
    m_accessKey= m_parentKey.isEmpty() ?  key : QString("%1.%2").arg(m_parentKey,key);
}



QmlJsonModelColumn::QmlJsonModelColumn(QObject *parent)
    : QObject{parent}
{

}
