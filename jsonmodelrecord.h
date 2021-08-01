#ifndef JSONMODELRECORD_H
#define JSONMODELRECORD_H

#include <QMap>
#include <QVariant>
#include <QJsonObject>
#include <QVector>
#include <models/jsonModel/jsonmodelfield.h>
class JsonModelRecord
{
public:
    JsonModelRecord();
    JsonModelRecord(QJsonObject record);
    JsonModelRecord(QStringList keys);
    int count() const;
    void clear();
    QVariant value(int index) const;
    QVariant value(const QString &Name) const;
    bool contains(const QString &name) const;
    bool contains(int index) const;
    QString fieldName(int index) const;
    int indexOf(const QString &Name) const;
    void setValue(int index, const QVariant &value);
    void setValue(const QString &name, const QVariant &value);


    bool operator ==(const JsonModelRecord &other) const;

    JsonModelRecord &operator =(const JsonModelRecord &other);
    operator QJsonObject();
private:
    QVector <JsonModelField> fields;
    QJsonObject m_record;
};

#endif // JSONMODELRECORD_H
