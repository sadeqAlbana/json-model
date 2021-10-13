#include <QtTest>

// add necessary includes here

class jsonmodel : public QObject
{
    Q_OBJECT

public:
    jsonmodel();
    ~jsonmodel();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();

};

jsonmodel::jsonmodel()
{

}

jsonmodel::~jsonmodel()
{

}

void jsonmodel::initTestCase()
{

}

void jsonmodel::cleanupTestCase()
{

}

void jsonmodel::test_case1()
{

}

QTEST_APPLESS_MAIN(jsonmodel)

#include "tst_jsonmodel.moc"
