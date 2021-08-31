#ifndef DEMOMODEL_H
#define DEMOMODEL_H

#include "jsonmodel.h"

class DemoModel : public JsonModel
{
    Q_OBJECT
public:
    explicit DemoModel(QJsonArray data, QObject *parent = nullptr);


};

#endif // DEMOMODEL_H
