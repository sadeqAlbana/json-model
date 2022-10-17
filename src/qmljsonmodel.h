#ifndef QMLJSONMODEL_H
#define QMLJSONMODEL_H

#include "jsonmodel.h"

class QmlJsonModel : public JsonModel
{
    Q_OBJECT
    QML_NAMED_ELEMENT(JsonModel)
public:
    explicit QmlJsonModel(QObject *parent = nullptr);
};

#endif // QMLJSONMODEL_H
