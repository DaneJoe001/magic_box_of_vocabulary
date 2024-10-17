#include <QDebug>
#include <QSqlError>
#include "databaseoperation.h"


DatabaseOperation::DatabaseOperation(QObject *parent)
    : QObject{parent}
{
    //TODO:turn to use Sqlite3 database;
    //TODO:get data from locol file;
    //判断驱动是否为空；
    if(QSqlDatabase::drivers().isEmpty())
    {
        qDebug()<<"Error! No drivers!";
    }
    //添加数据库；
    database=QSqlDatabase::addDatabase("QODBC");
    //设置数据库名称；
    database.setDatabaseName("magic_box_of_vocabulary");
    //打开数据库；
    if(database.open())
    {
        qDebug()<<"Connect to MySql database successfully!";
    }
    else
    {
        qDebug()<<"Faild to connect to MySql database!";

    }
}

//返回数据库对象；
QSqlDatabase DatabaseOperation::returnDatabase()
{
    return database;
}
