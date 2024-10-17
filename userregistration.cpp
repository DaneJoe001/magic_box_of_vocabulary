#include "userregistration.h"
#include "ui_userregistration.h"

UserRegistration::UserRegistration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserRegistration)
{
    ui->setupUi(this);

}

UserRegistration::~UserRegistration()
{
    delete ui;
}

void UserRegistration::on_pushButton_2_clicked()
{
    this->close();
}


void UserRegistration::on_pushButton_clicked()
{
    //获取输入框注册信息；
    QSqlQuery sql=QSqlQuery(databaseOperation.returnDatabase());
    newUser.userAccount=ui->AccountLineEdit->text();
    //TODO:检查账号密码位数是否低于8位
    newUser.userPassword=ui->PasswordLineEdit->text();
    newUser.userNickName=ui->NickNameLineEdit->text();
    newUser.lastLoginTime=QDateTime::currentDateTime();
    newUser.registrationTime=QDateTime::currentDateTime();
    QString passwordAgain=ui->CheckPasswordLineEdit->text();
    qDebug()<<newUser.userPassword;
    qDebug()<<passwordAgain;
    //检查两次输入密码是否一致；
    if(newUser.userPassword!=passwordAgain)
    {
        QMessageBox::information(this,"状态","两次密码不匹配！");
        return;
    }
    //检查账号是否已存在；
    QString sqlStatement=QString("select * from user_info where user_account='%1'").
                           arg(newUser.userAccount);
    if(!sql.exec(sqlStatement))
    {
        qDebug("SqlQuery Error!");
        return;
    }
    if(sql.next()==true)
    {
        QMessageBox::information(this,"状态","账号已存在！");
        return;
    }

    // 假设 newUser 的各个属性已经以安全的方式设置
    sqlStatement = QString("INSERT INTO user_info(user_account, user_password, user_nickname, user_avatar, registration_time, last_login_time) values");
    sqlStatement+=QString("('%1','%2','%3','%4','%5','%6')")
                       .arg(newUser.userAccount) // 应该确保这是安全的，但最好是使用参数化查询
                       .arg(newUser.userPassword) //TODO:对密码进行哈希处理
                       .arg(newUser.userNickName) // 应该确保这是安全的
                       .arg(newUser.userAvatar.isEmpty() ? "NULL" : "'" + newUser.userAvatar.replace("'", "''") + "'") // 如果 avatar 是字符串，需要转义单引号
                       .arg(newUser.registrationTime.toString(Qt::ISODate)) // 使用 ISO 日期格式
                       .arg(newUser.lastLoginTime.toString(Qt::ISODate));   // 使用 ISO 日期格式


    qDebug()<<sqlStatement;
    if(!sql.exec(sqlStatement))
    {
        qDebug()<<"Sql query Error!";
    }
    QMessageBox::information(this,"状态","注册成功！");
    this->close();
}

