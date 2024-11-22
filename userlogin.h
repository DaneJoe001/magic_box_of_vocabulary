#ifndef USERLOGIN_H
#define USERLOGIN_H

#include <QDialog>
#include <QString>
#include <QMessageBox>
#include <QDateTime>
#include "database_manager.h"

namespace Ui {
class UserLogin;
}

struct UserInfo
{
    //用户标识
    quint32 user_ID;
    //用户账户
    QString user_account;
    //用户密码
    QString user_password;
    //用户昵称
    QString user_nick_name;
    //用户图像
    QString user_avatar;
    //注册时间
    QDateTime registration_time;
    //上次登录时间
    QDateTime last_login_time;

};

class UserLogin : public QDialog
{
    Q_OBJECT

public:
    //创建数据库操作对象
    QSqlDatabase database;
    //创建当前用户信息
    UserInfo current_user;

public:
    explicit UserLogin(QWidget *parent = nullptr);
    ~UserLogin();

    //检查登陆状态
    bool check_login_status();

    //更新登录时间
    void update_login_time();

signals:
    //发送登录信号
    void login_signal(bool login_status);

private slots:

    //登录按钮
    void on_ButtonLogin_clicked();

    //注册按钮
    void on_pushButton_2_clicked();

private:
    Ui::UserLogin *ui;

    //存储输入框内容
    QString user_account;
    QString user_password;
};

#endif // USERLOGIN_H
