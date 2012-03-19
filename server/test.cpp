#include "test.h"

Test::Test(QObject *parent) :
    QObject(parent)
{
}

QString Test::test() {
    return "testOBJECTstringy";
}
