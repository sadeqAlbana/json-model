#ifndef QMLJSONMODEL_H
#define QMLJSONMODEL_H

#include "jsonmodel.h"
#include "qmljsonmodelcolumn.h"
class QmlJsonModel : public JsonModel
{
    Q_OBJECT
    QML_NAMED_ELEMENT(JsonModel)
    Q_CLASSINFO("DefaultProperty", "columnList")
public:
    explicit QmlJsonModel(QObject *parent = nullptr);

    QQmlListProperty<QmlJsonModelColumn> m_qmlColumnList;
    const QQmlListProperty<QmlJsonModelColumn> &qmlColumnList() const;
    void setQmlColumnList(const QQmlListProperty<QmlJsonModelColumn> &newQmlColumnList);
    void resetQmlColumnList();
signals:
    void qmlColumnListChanged();
private:
    Q_PROPERTY(QQmlListProperty<QmlJsonModelColumn> columnList READ qmlColumnList CONSTANT FINAL)
};

#endif // QMLJSONMODEL_H
