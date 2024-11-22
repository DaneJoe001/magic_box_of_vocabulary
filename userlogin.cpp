#include "userlogin.h"
#include "ui_userlogin.h"
#include "userregistration.h"

UserLogin::UserLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserLogin)
{
    ui->setupUi(this);
    //获取数据库对象；
    database=DatabaseManager::get_database("basic_database");
}

UserLogin::~UserLogin()
{
    delete ui;
}

bool UserLogin::check_login_status()
{
    QSqlQuery sql=QSqlQuery(database);
    //在数据库查询对应账号信息；
    QString sqlStatement=QString("select * from user_info where user_account='%1';").arg(user_account);
    if(!sql.exec(sqlStatement))
    {
        qDebug()<<"Sql query failed!";
        return false;
    }
    //未查询到结果即返回；
    if(!sql.next())
    {
        qDebug()<<QString("Not found user %1!").arg(user_account);
        return false;
    }
    current_user.user_password=sql.value(2).toString();
    //检查密码是否正确，正确则填充当前用户信息；
    if(current_user.user_password==user_password)
    {
        current_user.user_ID=sql.value(0).toUInt();
        current_user.user_account=user_account;
        current_user.user_nick_name=sql.value(3).toString();
        current_user.user_avatar=sql.value(4).toString();
        current_user.registration_time=sql.value(5).toDateTime();
        current_user.last_login_time=QDateTime::currentDateTime();
        update_login_time();
        return true;
    }
    else
    {
        return false;
    }
}

void UserLogin::update_login_time()
{
    QSqlQuery sql(database);

    QString sqlStatement =
    QString("UPDATE user_info SET last_login_time = :last_login_time WHERE user_id = :userId");

    // 准备查询
    if (!sql.prepare(sqlStatement)) {
        qDebug() << "Failed to prepare SQL statement!";
        return;
    }

    qDebug()<<sqlStatement;
    // 绑定参数值
    sql.bindValue(":last_login_time", QDateTime::currentDateTime().toString(Qt::ISODate));
    sql.bindValue(":userId", current_user.user_ID);

    // 执行查询
    if (!sql.exec()) {
        qDebug() << "SQL query failed: " << sql.lastError().text();
        return;
    }
}

void UserLogin::on_ButtonLogin_clicked()
{
    //获取输入框内容；
    user_account=ui->AccountLineEdit->text();
    user_password=ui->PasswordLineEdit->text();
    qDebug()<<QString("user_account: %1").arg(user_account);
    qDebug()<<QString("user_password: %1").arg(user_password);
    if(check_login_status())
    {
        QMessageBox::information(this,"状态","登陆成功");
        emit login_signal(true);
    }
    else
    {
        QMessageBox::information(this,"状态","登陆失败");
    }
}


void UserLogin::on_pushButton_2_clicked()
{
    //打开注册窗口；
    UserRegistration userRegistration;
    userRegistration.show();
    userRegistration.exec();
}

