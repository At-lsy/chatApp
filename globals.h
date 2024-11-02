#ifndef GLOBALS_H
#define GLOBALS_H
#include<QString>

extern QString MAIN_PATH;//文件存储
extern QString SERVER_IP;//服务器ip地址
extern QString SERVER_PORT;//服务器port端口
extern QString MYID;//该用户ID
extern int TOP_GAP;//头像与顶部边距
extern int BORDER_GAP;//头像与左右边框边距
extern int MID_GAP;//头像与聊天框边距
extern int HEAD_WIDTH;//头像宽度
extern int TEXT2BACK_GAP;//文字与背景间隙
extern int CHATLOG_NUM;//默认聊天记录读取数量
extern bool ISSHOW_SEARCHFRIEND;//寻找成功好友名片是否显示

enum MessageType{
    LOGIN_MESSAGE,
    TEXT_MESSAGE,
    PICTURE_MESSAGE,
    FILE_MESSAGE,
    FILE_SEND,
    FILE_REQUEST,
    UNREAD_MESSAGE,
    CONTACTS_MESSAGE,
    HEADIMAGE_MESSAGE,
    SEARCHFRIEND_MESSAGE,
    ADDFRIEND_MESSAGE,
    AGREEFRIEND_MESSAGE,
    UPDATE_NEWFRIEND,
    UPDATE_CONTACT,
    UPDATE_REQUEST_CONTACT,
    CREAT_NEWADMIN,
    GETIMAGE_NEWADMIN
};

enum status{
    HAS_ADD,    //已经添加
    WAIT_ARGEE, //等待同意
    WAIT_VERIFY //等待验证
};


#endif // GLOBALS_H
