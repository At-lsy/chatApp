#ifndef CHATLISTITEMDLG_H
#define CHATLISTITEMDLG_H

#include <QWidget>
#include<QString>
#include<QDateTime>
#include<QMouseEvent>
#include<QMenu>

namespace Ui {
class ChatListItemDlg;
}

class ChatListItemDlg : public QWidget
{
    Q_OBJECT

public:
    explicit ChatListItemDlg(QString clientId, QString name, QDateTime time, QString message, int unreadNum,QWidget *parent = nullptr);
    ~ChatListItemDlg();
    void init();
    QString get_clientName();
    QDateTime get_time();
    QString get_message();
    int get_unreadNum();
    void set_message(QString msg);
    void set_time(QDateTime c_time);
    void set_remark(QString name);
    void set_unreadNum(int num);
    void add_unreadNum(QString msg);
    void transform_checked();

protected:
    void mousePressEvent(QMouseEvent* event);
    void contextMenuEvent(QContextMenuEvent* event);

signals:
    void hasCliked(QString clientId,QString name);

private:
    Ui::ChatListItemDlg *ui;
    QString client_Id;
    QString client_name;
    QString message;
    QDateTime send_time;
    QMenu* menu=nullptr;
    int unreadNum;
};

#endif // CHATLISTITEMDLG_H
