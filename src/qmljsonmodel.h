#ifndef QMLJSONMODEL_H
#define QMLJSONMODEL_H

#include "jsonmodel.h"
#include "qmljsonmodelcolumn.h"
class QmlJsonModel : public JsonModel, public QQmlParserStatus
{
    Q_OBJECT
    QML_NAMED_ELEMENT(JsonModel)
    Q_CLASSINFO("DefaultProperty", "columnList")
public:
    explicit QmlJsonModel(QObject *parent = nullptr);

    QList<QmlJsonModelColumn *> m_qmlColumnList;
    QQmlListProperty<QmlJsonModelColumn> qmlColumnList();
    static void columnList_append(QQmlListProperty<QmlJsonModelColumn> *property, QmlJsonModelColumn *value);
    static qsizetype columnList_count(QQmlListProperty<QmlJsonModelColumn> *property);
    static QmlJsonModelColumn *columnList_at(QQmlListProperty<QmlJsonModelColumn> *property, qsizetype index);
    static void columnList_clear(QQmlListProperty<QmlJsonModelColumn> *property);
    static void columnList_replace(QQmlListProperty<QmlJsonModelColumn> *property, qsizetype index, QmlJsonModelColumn *value);
    static void columnList_removeLast(QQmlListProperty<QmlJsonModelColumn> *property);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override; //columns compatible
    void classBegin() override;
    void componentComplete() override;
private:
    Q_PROPERTY(QQmlListProperty<QmlJsonModelColumn> columnList READ qmlColumnList CONSTANT FINAL)
};

#endif // QMLJSONMODEL_H
