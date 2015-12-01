#include <QtTest/QTest>
#include "writertest.h"

WriteTest::WriteTest(QObject *parent) : QObject(parent)
{

}

void WriteTest::testDummy() {
    QVERIFY2( 2 == 1 + 1, "Expect two to be one plus one");
}
