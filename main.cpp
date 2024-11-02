#include "logindlg.h"

#include <QApplication>
#include"logindlg.h"

#include"newfrienddlg.h"
#include"registerdlg.h"
#include"cutimagedlg.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginDlg w;
    w.show();

//    CutImageDlg c;
//    c.show();

//    RegisterDlg r;
//    r.show();
//    NewFriendDlg n;
//    n.show();
    return a.exec();
}
