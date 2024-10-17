#ifndef USERLOGIN_H
#define USERLOGIN_H

#include <QDialog>
#include <QString>
#include <QMessageBox>
#include <QDateTime>

#include "databaseoperation.h"

namespace Ui {
class UserLogin;
}

struct UserInfo
{
    //用户标识
    quint32 userID;
    //用户账户
    QString userAccount;
    //用户密码
    QString userPassword;
    //用户昵称
    QString userNickName;
    //用户图像
    QString userAvatar;
    //注册时间
    QDateTime registrationTime;
    //上次登录时间
    QDateTime lastLoginTime;

};

class UserLogin : public QDialog
{
    Q_OBJECT

public:
    //创建数据库操作对象
    DatabaseOperation databaseOperation;
    //创建当前用户信息
    UserInfo currentUser;

public:
    explicit UserLogin(QWidget *parent = nullptr);
    ~UserLogin();

    //检查登陆状态
    bool checkLoginStatus();

    //更新登录时间
    void updateLoginTime();

signals:
    //发送登录信号
    void loginSignal(bool loginStatus);

private slots:

    //登录按钮
    void on_ButtonLogin_clicked();

    //注册按钮
    void on_pushButton_2_clicked();

private:
    Ui::UserLogin *ui;

    //存储输入框内容
    QString userAccount;
    QString userPassword;

    //创建数据库对象
    QSqlDatabase database;
};

#endif // USERLOGIN_H
