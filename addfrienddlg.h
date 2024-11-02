#ifndef ADDFRIENDDLG_H
#define ADDFRIENDDLG_H

#include <QWidget>
#include<QDialog>

namespace Ui {
class AddFriendDlg;
}

class AddFriendDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AddFriendDlg(QString clientId,QString clientName,QWidget *parent = nullptr);
    ~AddFriendDlg();

private slots:
    void on_but_cancel_clicked();
    void on_but_ack_clicked();

signals:
    void request_addFriend(quint16 add_type,QString id,QString name,QString msg);

private:
    Ui::AddFriendDlg *ui;
    QString client_id;
    QString client_name;
};

#endif // ADDFRIENDDLG_H
