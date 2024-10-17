#ifndef USERREGISTRATION_H
#define USERREGISTRATION_H

#include <QDialog>

#include "userlogin.h"

namespace Ui {
class UserRegistration;
}

class UserRegistration : public QDialog
{
    Q_OBJECT

public:
    explicit UserRegistration(QWidget *parent = nullptr);
    ~UserRegistration();

private slots:
    //取消按钮
    void on_pushButton_2_clicked();

    //注册按钮
    void on_pushButton_clicked();

private:
    Ui::UserRegistration *ui;

    //创建数据库操作对象；
    DatabaseOperation databaseOperation;

    //新用户信息；
    UserInfo newUser;
};

#endif // USERREGISTRATION_H
