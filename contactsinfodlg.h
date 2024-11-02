#ifndef CONTACTSINFODLG_H
#define CONTACTSINFODLG_H

#include <QWidget>
#include<QString>

namespace Ui {
class ContactsInfoDlg;
}

class ContactsInfoDlg : public QWidget
{
    Q_OBJECT

public:
    explicit ContactsInfoDlg(QString clientId,QString name,QWidget *parent = nullptr);
    ~ContactsInfoDlg();
    void init();
    QString getId();
    QString getName();

signals:
    void enterSendMessage(QString id,QString name);

private slots:
    void on_but_message_clicked();

private:
    Ui::ContactsInfoDlg *ui;
    QString client_id;
    QString remark;
};

#endif // CONTACTSINFODLG_H
