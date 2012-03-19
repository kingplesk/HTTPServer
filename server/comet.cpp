#include <QTimer>

#include "qdebug.h"
#include "comet.h"

Comet::Comet(QObject *parent) :
    QObject(parent)
{
    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(update()));
    timer_->setSingleShot(true);
    timer_->start(10000);
}

void Comet::update() {
    qDebug() << "UPDATE";
}
