#ifndef SENDFILESOCKET_H
#define SENDFILESOCKET_H

#include <QObject>
#include <QWidget>
#include<QString>
#include<QTcpSocket>

class SendFileSocket : public QObject
{
    Q_OBJECT
public:
    explicit SendFileSocket(QString filePath,QString clientId,QString myId,QObject *parent = nullptr);
    ~SendFileSocket();
    void recv_file(quint64 size);
    void hasRecv();

public slots:
    void initSocket();
    void send_File();
    void request_file();

signals:
    void current_send(int predcent);
    void send_over();
    void connect_result(bool isConnect);
    void recv_over();

private:
    bool isPouse=false;//是否暂停
    QString file_path;//发送文件时需要的路径（接收文件时需要的文件名）
    QString client_id;
    QString my_id;
    QTcpSocket* fileSocket=nullptr;
    bool readFile=false;
};

#endif // SENDFILESOCKET_H
