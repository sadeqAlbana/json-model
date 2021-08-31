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

class JsonModelField
{
public:
    JsonModelField(QString name=QString(),QVariant value=QVariant());

    QString name() const;
    QVariant value() const;
    void setName(const QString &value);
    void setValue(const QVariant &value);

    bool operator ==(const JsonModelField &other) const;

    JsonModelField& operator =(const JsonModelField &other);

private:
    QString p_name;
    QVariant p_value;
};

#endif // JSONMODELFIELD_H
