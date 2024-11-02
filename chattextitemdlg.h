#ifndef CHATTEXTITEMDLG_H
#define CHATTEXTITEMDLG_H

#include <QWidget>
#include<QString>

namespace Ui {
class chatTextItemDlg;
}

class chatTextItemDlg : public QWidget
{
    Q_OBJECT

public:
    explicit chatTextItemDlg(QString clientid,bool isLeft,QString message,QWidget *parent = nullptr);
    ~chatTextItemDlg();

    void paintEvent(QPaintEvent *event);

signals:
    void sendHight(int hight);

private:
    Ui::chatTextItemDlg *ui;
    QString head_path;
    QString My_Id;
    QString message;
    bool isLeft;
};

#endif // CHATTEXTITEMDLG_H
