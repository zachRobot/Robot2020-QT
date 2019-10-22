/*
 * @Author: xingzhang.Wu 
 * @Date: 2019-10-21 16:07:52 
 * @Last Modified by: xingzhang.Wu
 * @Last Modified time: 2019-10-22 11:04:06
 */
#include "remotecontrol.h"
#include "debug.h"

RemoteControl::RemoteControl(QDialog *parent) : QDialog(parent)
{
    ac = new AutoControl();
}

RemoteControl::~RemoteControl()
{
    delete qmlControl1;
    delete qmlControl2;
    delete ac;
}

/**
 * @brief 显示遥控器界面
 * 
 */
void RemoteControl::show()
{
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    QObject *item = engine.rootObjects().first(); //component->create();
    QObject *qmlLeftObject = item->findChild<QObject *>("qmlLeftJoySignal");
    QObject *qmlRightObject = item->findChild<QObject *>("qmlRightJoySignal");
    QObject *qmlGamepadObject = item->findChild<QObject *>("qmlGamepadSignal");

    QObject::connect(qmlLeftObject, SIGNAL(qmlLeftJoySignal(double)), this, SLOT(leftJoySlot(double)));
    QObject::connect(qmlRightObject, SIGNAL(qmlRightJoySignal(double)), this, SLOT(rightJoySlot(double)));
    QObject::connect(qmlGamepadObject, SIGNAL(qmlStartSignal(void)), this, SLOT(qmlStartSlot(void)));
    QObject::connect(qmlGamepadObject, SIGNAL(qmlL1Signal(void)), this, SLOT(qmlL1Slot(void)));
    QObject::connect(qmlGamepadObject, SIGNAL(qmlASignal(bool)), this, SLOT(qmlASlot(bool)));
    QObject::connect(qmlGamepadObject, SIGNAL(qmlBSignal(bool)), this, SLOT(qmlBSlot(bool)));

    qmlControl2 = item->findChild<QObject *>("qmlcontrol2");
    qmlControl1 = item->findChild<QObject *>("qmlcontrol1");
}

void RemoteControl::leftJoySlot(const double &speed)
{
    double changePos[3] = {0, 0, 1};
    ac->moveLeg(0, changePos, 0.1);
}

void RemoteControl::rightJoySlot(const double &angle)
{
    double changePos[3] = {0, 0, -1};
    ac->moveLeg(0, changePos, 0.1);
}

void RemoteControl::qmlStartSlot()
{
}

void RemoteControl::qmlL1Slot()
{
}

void RemoteControl::qmlASlot(bool press)
{
}

void RemoteControl::qmlBSlot(bool press)
{
}