/*
 * Copyright (C) 2021 Sadeq Albana
 *
 * Licensed under the GNU Lesser General Public License v3.0 :
 * https://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef JSONMODELRECORD_H
#define JSONMODELRECORD_H

#include <QMap>
#include <QVariant>
#include <QJsonObject>
#include <QVector>
#include "jsonmodelfield.h"
class JsonModelRecord
{
public:
    JsonModelRecord();
    JsonModelRecord(const QJsonObject &object);
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
    QJsonObject m_object;
};

#endif // JSONMODELRECORD_H
