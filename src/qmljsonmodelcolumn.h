#ifndef QMLJSONMODELCOLUMN_H
#define QMLJSONMODELCOLUMN_H

#include <QObject>
#include "jsonmodelcolumn.h"
class QmlJsonModelColumn : public QObject, JsonModelColumn
{
    Q_OBJECT
    QML_NAMED_ELEMENT(JsonModelColumn)

    Q_PROPERTY(QString key MEMBER m_key)
    Q_PROPERTY(QString displayName MEMBER m_displayName)
    Q_PROPERTY(QString parentKey MEMBER m_parentKey)
    Q_PROPERTY(QString type MEMBER m_type)
    Q_PROPERTY(QString accessKey MEMBER m_accessKey)

public:
    explicit QmlJsonModelColumn(QObject *parent = nullptr);

signals:

};

#endif // QMLJSONMODELCOLUMN_H
