#include "jsonmodelrecord.h"

#include <QDebug>
JsonModelRecord::JsonModelRecord()
{

}

JsonModelRecord::JsonModelRecord(QJsonObject record)
{
    m_record=record;
    for(QString key : record.keys())
       fields << JsonModelField(key,record.value(key).toVariant()); //change to something more appropriate
}


JsonModelRecord::JsonModelRecord(QStringList keys)
{
    for(QString key : keys)
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
    return indexOf(name) >=0;
}
bool JsonModelRecord::contains(int index) const //done...bug was found here
{
    return index > 0 && index <= fields.size();
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

void JsonModelRecord::setValue(int index, const QVariant &value) //done
{
    if(!contains(index))
        return;

    fields[index].setValue(value);
}

void JsonModelRecord::setValue(const QString &name, const QVariant &value) //done
{
    setValue(indexOf(name),value);
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
    return m_record;
}


