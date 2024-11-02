#ifndef CHATDLG_H
#define CHATDLG_H

#include <QWidget>
#include<QString>
#include<mydatabase.h>

namespace Ui {
class ChatDlg;
}

class ChatDlg : public QWidget
{
    Q_OBJECT

public:
    explicit ChatDlg(QString clientId,QString clientNmae,QString myId,QWidget *parent = nullptr);
    ~ChatDlg();
    void add_chatItem(quint16 type,QString id,QString message);
    void saveMessage(int type,QString saveId,QString message);
    void saveFileMessage(int type,QString saveId,QString fileName,quint64 fileSize);
    void readMessage();
    void setBottom();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void on_but_send_clicked();
    void add_TextMessage(QString clientid,QString message,bool isRead);
    void add_FileMessage(QString sendId,QString fileName,quint64 fileSize,bool isRead);
    void on_tbut_file_clicked();

signals:
    void text_change(QString clientId,QString text);
    void send_Text(qint16 type,QString clientId,QString message);

private:
    Ui::ChatDlg *ui;
    QString client_id;
    QString My_id;
    QString client_name;
    static MyDataBase* chatDlg_DB;
};
#endif // CHATDLG_H
