#ifndef DIALOG_USER_REGISTRATION_H
#define DIALOG_USER_REGISTRATION_H

#include <QDialog>

#include "dialog_user_login.h"

namespace Ui {
class UserRegistration;
}

class DialogUserRegistration : public QDialog
{
    Q_OBJECT

public:
    explicit DialogUserRegistration(QWidget *parent = nullptr);
    ~DialogUserRegistration();

private slots:
    //取消按钮
    void on_pushButton_2_clicked();

    //注册按钮
    void on_pushButton_clicked();

private:
    Ui::UserRegistration *ui;

    //创建数据库操作对象；
    QSqlDatabase database;

    //新用户信息；
    UserInfo newUser;
};

#endif // DIALOG_USER_REGISTRATION_H
