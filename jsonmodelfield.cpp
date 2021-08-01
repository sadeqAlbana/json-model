#include "jsonmodelfield.h"

JsonModelField::JsonModelField(QString name, QVariant value)
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

QVariant JsonModelField::value() const
{
    return p_value;
}

void JsonModelField::setValue(const QVariant &value)
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


