#  JSON-MODEL

## A QAbstractItemModel subclass for json data
this model takes a json array to view it in Qt Item views

## Features
- supports Qt widgets views
- supports Qt Quick controls 1 views
- supports Qt Quick controls 2 views
- supports the new `DelegateChooser` using a special role
- custom header data
- ability to view nested data

## Usage
- you just pass the data to the constructor or call the `setupData()` method
- there is an optional `ColumnList` parameter that use can use to define custom header data, custom `DelegateChooser` role values and point to nested json keys
## sample code


 ```
 QJsonArray modelData;
    for(int i=1 ; i<1001; i++){
        QJsonObject row{
            {"name", QString("item %1").arg(i)},
            {"stock", QJsonObject{{"location", QString("wharehouse %1").arg(i)},{"qty", i}}},
            {"value", i*100},
        };
        modelData << row;
    }
    
    JsonModel model(modelData,ColumnList() <<
                    Column{"name","Name",QString(),"text"} <<
                    Column{"qty","Stock","stock","text"} << //nested key
                    Column{"value","Value",QString(),"currency"});
 ```



## License
GNU Lesser General Public License v3.0. See LICENSE file
