#include "dialog_user_registration.h"
//#include "ui_userregistration.h"
#include"database_manager.h"
#include"ui_dialog_user_registration.h"

DialogUserRegistration::DialogUserRegistration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserRegistration)
{
    database=DatabaseManager::get_database("basic_database");
    ui->setupUi(this);

}

DialogUserRegistration::~DialogUserRegistration()
{
    delete ui;
}

void DialogUserRegistration::on_pushButton_2_clicked()
{
    this->close();
}


void DialogUserRegistration::on_pushButton_clicked()
{
    //获取输入框注册信息；
    QSqlQuery sql=QSqlQuery(database);
    newUser.user_account=ui->AccountLineEdit->text();
    //TODO:检查账号密码位数是否低于8位
    newUser.user_password=ui->PasswordLineEdit->text();
    newUser.user_nick_name=ui->NickNameLineEdit->text();
    newUser.last_login_time=QDateTime::currentDateTime();
    newUser.registration_time=QDateTime::currentDateTime();
    QString passwordAgain=ui->CheckPasswordLineEdit->text();
    qDebug()<<newUser.user_password;
    qDebug()<<passwordAgain;
    //检查两次输入密码是否一致；
    if(newUser.user_password!=passwordAgain)
    {
        QMessageBox::information(this,"状态","两次密码不匹配！");
        return;
    }
    //检查账号是否已存在；
    QString sqlStatement=QString("select * from user_info where user_account='%1'").
                           arg(newUser.user_account);
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
                       .arg(newUser.user_account) // 应该确保这是安全的，但最好是使用参数化查询
                       .arg(newUser.user_password) //TODO:对密码进行哈希处理
                       .arg(newUser.user_nick_name) // 应该确保这是安全的
                       .arg(newUser.user_avatar.isEmpty() ? "NULL" : "'" + newUser.user_avatar.replace("'", "''") + "'") // 如果 avatar 是字符串，需要转义单引号
                       .arg(newUser.registration_time.toString(Qt::ISODate)) // 使用 ISO 日期格式
                       .arg(newUser.last_login_time.toString(Qt::ISODate));   // 使用 ISO 日期格式


    qDebug()<<sqlStatement;
    if(!sql.exec(sqlStatement))
    {
        qDebug()<<"Sql query Error!";
    }
    QMessageBox::information(this,"状态","注册成功！");
    this->close();
}

