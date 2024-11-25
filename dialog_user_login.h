#ifndef DIALOG_USER_LOGIN_H
#define DIALOG_USER_LOGIN_H

#include <QDialog>
#include <QString>
#include <QMessageBox>
#include <QDateTime>
#include <QSqlDatabase>

//TODO:调整为单例模式，全局维持一个登录信息；
namespace Ui {
class UserLogin;
}

struct UserInfo
{
    //用户标识
    qint32 user_ID;
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

class DialogUserLogin : public QDialog
{
    Q_OBJECT

public:
    explicit DialogUserLogin(QWidget *parent = nullptr);
    ~DialogUserLogin();

    //检查登陆状态
    bool check_login_status();

    //更新登录时间
    static void update_login_time();

    static UserInfo get_current_user();

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
    //创建数据库操作对象
    static QSqlDatabase database;
    //创建当前用户信息
    static UserInfo current_user;
};

#endif // DIALOG_USER_LOGIN_H
