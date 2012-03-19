#ifndef COMET_H
#define COMET_H

#include <QObject>
#include <QTimer>

class Comet : public QObject
{
    Q_OBJECT
public:
    explicit Comet(QObject *parent = 0);

signals:

public slots:
    void update();

private:
    QTimer * timer_;
};

#endif // COMET_H
