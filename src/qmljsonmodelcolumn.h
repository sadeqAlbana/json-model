#ifndef QMLJSONMODELCOLUMN_H
#define QMLJSONMODELCOLUMN_H

#include <QObject>
#include "jsonmodelcolumn.h"
class QmlJsonModelColumn : public QObject, public JsonModelColumn
{
    Q_OBJECT
    QML_NAMED_ELEMENT(JsonModelColumn)

    void setKey(const QString &key);
    void setMetaData(const QVariantMap &data);
    Q_PROPERTY(QString key MEMBER m_key WRITE setKey)
    Q_PROPERTY(QString displayName MEMBER m_displayName)
    Q_PROPERTY(QString parentKey MEMBER m_parentKey)
    Q_PROPERTY(QString type MEMBER m_type)
    Q_PROPERTY(QVariantMap metadata MEMBER m_metadata)

public:
    explicit QmlJsonModelColumn(QObject *parent = nullptr);

signals:

};

#endif // QMLJSONMODELCOLUMN_H
