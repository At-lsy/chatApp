#ifndef WINMOVE_H
#define WINMOVE_H

#include <QObject>
#include<QPoint>

class WinMove : public QObject
{
    Q_OBJECT
public:
    explicit WinMove(QObject *parent = nullptr);
    bool eventFilter(QObject *watched, QEvent *event);
signals:

private:
    QPoint pos;
};

#endif // WINMOVE_H
