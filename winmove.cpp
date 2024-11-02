#include "winmove.h"
#include<QWidget>
#include<QEvent>
#include<QMouseEvent>

WinMove::WinMove(QObject *parent) : QObject(parent)
{

}

bool WinMove::eventFilter(QObject *watched, QEvent *event)
{
    QWidget* w=static_cast<QWidget*>(watched);
    if(!w){
        return false;
    }
    if(event->type()==QEvent::MouseButtonPress){
        QMouseEvent* ev=static_cast<QMouseEvent*>(event);
        if(!ev){
            return false;
        }
        this->pos=ev->pos();
    }
    else if(event->type()==QEvent::MouseMove){
        QMouseEvent* ev=static_cast<QMouseEvent*>(event);
        if(!ev){
            return false;
        }
        if(ev->buttons()&Qt::MouseButton::LeftButton){
            w->move(ev->globalPos()-this->pos);
            return true;
        }
    }
    return QObject::eventFilter(watched,event);
}
