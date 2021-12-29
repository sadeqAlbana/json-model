/*
 * Copyright (C) 2021 Sadeq Albana
 *
 * Licensed under the GNU Lesser General Public License v3.0 :
 * https://www.gnu.org/licenses/lgpl-3.0.html
 */

#include "jsonmodelrecord.h"

#include <QDebug>
JsonModelRecord::JsonModelRecord()
{

}

JsonModelRecord::JsonModelRecord(const QJsonObject &object)
{
    m_object=object;
    for(const QString &key : object.keys()){
       fields << JsonModelField(key,object.value(key)); //change to something more appropriate
    }
}


JsonModelRecord::JsonModelRecord(QStringList keys)
{
    for(const QString &key : keys)
       fields << JsonModelField(key);
}

int JsonModelRecord::count() const
{
    return fields.count();
}

void JsonModelRecord::clear() //done
{
    fields.clear();
}

QVariant JsonModelRecord::value(int index) const //done
{
    return fields.value(index).value();
}

QVariant JsonModelRecord::value(const QString &name) const //done
{
    return value(indexOf(name));
}

bool JsonModelRecord::contains(const QString &name) const //done
{
    return (indexOf(name) >=0);
}
bool JsonModelRecord::contains(int index) const //done...bug was found here
{
    return (index >= 0) && ( index <= fields.size() );
}

QString JsonModelRecord::fieldName(int index) const //done
{
    return fields.value(index).name();
}

int JsonModelRecord::indexOf(const QString &name) const //done
{
    for(int i=0; i<fields.size() ; i++)
        if(fields.at(i).name()==name)
            return i;

    return -1;
}

bool JsonModelRecord::setValue(int index, const QJsonValue &value) //done
{
    if(!contains(index))
        return false;

    fields[index].setValue(value);
    return true;
}

bool JsonModelRecord::setValue(const QString &name, const QJsonValue &value) //done
{
    return setValue(indexOf(name),value);
}

bool JsonModelRecord::operator ==(const JsonModelRecord &other) const
{
    return fields==other.fields;
}

JsonModelRecord &JsonModelRecord::operator =(const JsonModelRecord &other)
{
    fields=other.fields;
    return *this;
}

JsonModelRecord::operator QJsonObject()
{
    return m_object;
}


