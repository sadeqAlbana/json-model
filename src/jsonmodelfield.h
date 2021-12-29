/*
 * Copyright (C) 2021 Sadeq Albana
 *
 * Licensed under the GNU Lesser General Public License v3.0 :
 * https://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef JSONMODELFIELD_H
#define JSONMODELFIELD_H
#include <QString>
#include <QVariant>
#include <QJsonValue>
class JsonModelField
{
public:
    JsonModelField(QString name=QString(),QJsonValue value=QJsonValue());

    QString name() const;
    QJsonValue value() const;
    void setName(const QString &value);
    void setValue(const QJsonValue &value);

    bool operator ==(const JsonModelField &other) const;

    JsonModelField& operator =(const JsonModelField &other);

private:
    QString p_name;
    QJsonValue p_value;
};

#endif // JSONMODELFIELD_H
