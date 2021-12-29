/*
 * Copyright (C) 2021 Sadeq Albana
 *
 * Licensed under the GNU Lesser General Public License v3.0 :
 * https://www.gnu.org/licenses/lgpl-3.0.html
 */

#include "jsonmodelfield.h"
#include <QDebug>
JsonModelField::JsonModelField(QString name, QJsonValue value)
{
    p_name=name;
    p_value=value;
}

QString JsonModelField::name() const
{
    return p_name;
}

void JsonModelField::setName(const QString &name)
{
    p_name = name;
}

QJsonValue JsonModelField::value() const
{
    return p_value;
}

void JsonModelField::setValue(const QJsonValue &value)
{
    p_value = value;
}

bool JsonModelField::operator ==(const JsonModelField &other) const
{
    return name()==other.name() && value()==other.value();
}

JsonModelField& JsonModelField::operator =(const JsonModelField &other)
{
    setName(other.name());
    setValue(other.value());
    return *this;
}


