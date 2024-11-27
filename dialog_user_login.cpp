#include "database_manager.h"
#include "dialog_user_login.h"
#include "dialog_user_login.h"
#include "dialog_user_registration.h"
#include "ui_dialog_user_login.h"


UserInfo DialogUserLogin::current_user={-1,nullptr,nullptr,nullptr,nullptr,QDateTime(),QDateTime()};
QSqlDatabase DialogUserLogin::database=DatabaseManager::get_database("basic_database");


DialogUserLogin::DialogUserLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserLogin)
{
    ui->setupUi(this);
    //获取数据库对象；

}

DialogUserLogin::~DialogUserLogin()
{
    delete ui;
}

bool DialogUserLogin::check_login_status()
{
    QSqlQuery sql=QSqlQuery(database);
    //在数据库查询对应账号信息；
    QString sql_statement=QString("select * from user_info where user_account='%1';").arg(current_user.user_account);
    if(!sql.exec(sql_statement))
    {
        qDebug()<<"Failed to execute sql query!";
        return false;
    }
    //未查询到结果即返回；
    if(!sql.next())
    {
        qDebug()<<QString("Not found user %1!").arg(current_user.user_account);
        QMessageBox::information(this,"状态","用户不存在");
        return false;
    }
    QString temp_password=sql.value(2).toString();
    //检查密码是否正确，正确则填充当前用户信息；
    if(current_user.user_password==temp_password)
    {
        current_user.user_ID=sql.value(0).toUInt();
        current_user.user_nick_name=sql.value(3).toString();
        current_user.user_avatar=sql.value(4).toString();
        current_user.registration_time=sql.value(5).toDateTime();
        current_user.last_login_time=sql.value(6).toDateTime();
        //update_login_time();
        return true;
    }
    else
    {
        return false;
    }
}

void DialogUserLogin::update_login_time()
{
    if(current_user.user_ID==-1)
    {
        qDebug()<<"Failed to update login time!";
    }
    QSqlQuery sql(database);

    QString sql_statement =
    QString("UPDATE user_info SET last_login_time = :last_login_time WHERE user_id = :userId");

    // 准备查询
    if (!sql.prepare(sql_statement)) {
        qDebug() << "Failed to prepare SQL statement!";
        return;
    }
    // 绑定参数值
    sql.bindValue(":last_login_time", QDateTime::currentDateTime().toString(Qt::ISODate));
    sql.bindValue(":userId", current_user.user_ID);

    // 执行查询
    if (!sql.exec()) {
        qDebug()<<"The statement executing now:"<<sql_statement;
        qDebug() << "SQL query failed: " << sql.lastError().text();
        return;
    }
}

UserInfo DialogUserLogin::get_current_user()
{
    return current_user;
}

void DialogUserLogin::on_ButtonLogin_clicked()
{
    //获取输入框内容；
    current_user.user_account=ui->AccountLineEdit->text();
    current_user.user_password=ui->PasswordLineEdit->text();
    qDebug()<<QString("user_account: %1").arg(current_user.user_account);
    qDebug()<<QString("user_password: %1").arg(current_user.user_password);
    if(check_login_status())
    {
        QMessageBox::information(this,"状态","登陆成功");
        emit login_signal(true);
    }

}


void DialogUserLogin::on_pushButton_2_clicked()
{
    //打开注册窗口；
    DialogUserRegistration userRegistration;
    userRegistration.show();
    userRegistration.exec();
}

