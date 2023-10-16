#ifndef JSONMODELCOLUMN_H
#define JSONMODELCOLUMN_H

#include <QObject>
#include <QQmlEngine>
#include <QJsonObject>
struct  JsonModelColumn{
//https://doc.qt.io/qt-6/positioning-cpp-qml.html
//https://stackoverflow.com/questions/42797359/how-to-create-new-instance-of-a-q-gadget-struct-in-qml
public:
   JsonModelColumn();
    JsonModelColumn(const QString &key, const QString &displayName,const QString &parentKey=QString(),const QString &type=QStringLiteral("text"),
                   const QVariantMap metadata=QVariantMap()):
       m_key(key),m_displayName(displayName),m_parentKey(parentKey),m_type(type),m_metadata(metadata){
        m_accessKey= parentKey.isEmpty() ?  key : QString("%1.%2").arg(parentKey,key);
    }
    QString m_key;
    QString m_displayName;
    QString m_parentKey;
    QString m_type;
    QString m_accessKey;
    QVariantMap m_metadata;
};

#endif // JSONMODELCOLUMN_H
