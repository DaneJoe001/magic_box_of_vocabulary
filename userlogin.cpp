#include "userlogin.h"
#include "ui_userlogin.h"
#include "userregistration.h"

UserLogin::UserLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserLogin)
{
    ui->setupUi(this);
    //获取数据库对象；
    database=databaseOperation.returnDatabase();
}

UserLogin::~UserLogin()
{
    delete ui;
}

bool UserLogin::checkLoginStatus()
{
    QSqlQuery sql=QSqlQuery(database);
    //在数据库查询对应账号信息；
    QString sqlStatement=QString("select * from user_info where user_account='%1';").arg(userAccount);
    if(!sql.exec(sqlStatement))
    {
        qDebug()<<"Sql query failed!";
        return false;
    }
    //未查询到结果即返回；
    if(!sql.next())
    {
        qDebug()<<QString("Not found user %1!").arg(userAccount);
        return false;
    }
    currentUser.userPassword=sql.value(2).toString();
    //检查密码是否正确，正确则填充当前用户信息；
    if(currentUser.userPassword==userPassword)
    {
        currentUser.userID=sql.value(0).toUInt();
        currentUser.userAccount=userAccount;
        currentUser.userNickName=sql.value(3).toString();
        currentUser.userAvatar=sql.value(4).toString();
        currentUser.registrationTime=sql.value(5).toDateTime();
        currentUser.lastLoginTime=QDateTime::currentDateTime();
        updateLoginTime();
        return true;
    }
    else
    {
        return false;
    }
}

void UserLogin::updateLoginTime()
{
    QSqlQuery sql(database);

    QString sqlStatement =
    QString("UPDATE user_info SET last_login_time = :lastLoginTime WHERE user_id = :userId");

    // 准备查询
    if (!sql.prepare(sqlStatement)) {
        qDebug() << "Failed to prepare SQL statement!";
        return;
    }

    qDebug()<<sqlStatement;
    // 绑定参数值
    sql.bindValue(":lastLoginTime", QDateTime::currentDateTime().toString(Qt::ISODate));
    sql.bindValue(":userId", currentUser.userID);

    // 执行查询
    if (!sql.exec()) {
        qDebug() << "SQL query failed: " << sql.lastError().text();
        return;
    }
}

void UserLogin::on_ButtonLogin_clicked()
{
    //获取输入框内容；
    userAccount=ui->AccountLineEdit->text();
    userPassword=ui->PasswordLineEdit->text();
    qDebug()<<QString("userAccount: %1").arg(userAccount);
    qDebug()<<QString("userPassword: %1").arg(userPassword);
    if(checkLoginStatus())
    {
        QMessageBox::information(this,"状态","登陆成功");
        emit loginSignal(true);
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

