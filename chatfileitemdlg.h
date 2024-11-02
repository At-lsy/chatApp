#ifndef CHATFILEITEMDLG_H
#define CHATFILEITEMDLG_H

#include <QWidget>
#include<QThread>
#include"sendfilesocket.h"

namespace Ui {
class ChatFileItemDlg;
}

class ChatFileItemDlg : public QWidget
{
    Q_OBJECT

public:
    explicit ChatFileItemDlg(bool isLeft,QString filePth,QString clientId,QString myId,qint64 fileSize,QWidget *parent = nullptr);
    ~ChatFileItemDlg();
    void initWidget();
    void sendFile();
    void clikedWin();
    void request_file();

    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::ChatFileItemDlg *ui;
    bool isLeft;
    bool isDownload=false;//是否在下载
    bool isPouse=false;//是否暂停
    bool isSend=true;//是否已经发送
    qint64 file_Size;
    QString file_path;
    QString client_id;
    QString my_id;
    QThread* thread;
    SendFileSocket* fileSocket=nullptr;
};

#endif // CHATFILEITEMDLG_H
