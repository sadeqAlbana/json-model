#include "demomodel.h"

DemoModel::DemoModel(QJsonArray data, QObject *parent) : JsonModel(data,ColumnList() <<
  Column{"name","Name",QString(),"text"} <<
  Column{"qty","Stock","stock","text"} << //nested key
  Column{"value","Value",QString(),"currency"},
         parent)
{

}
