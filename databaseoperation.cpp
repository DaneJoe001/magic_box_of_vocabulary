#include <QDebug>
#include <QSqlError>
#include "databaseoperation.h"


DatabaseOperation::DatabaseOperation(QObject *parent)
    : QObject{parent}
{
    //TODO:turn to use Sqlite3 database;
    //TODO:get data from locol file;
    if(QSqlDatabase::drivers().isEmpty())
    {
        qDebug()<<"Error! No drivers!";
    }
    database=QSqlDatabase::addDatabase("QODBC");
    database.setDatabaseName("magic_box_of_vocabulary");
    if(database.open())
    {
        qDebug()<<"Connect to MySql database successfully!";
    }
    else
    {
        qDebug()<<"Faild to connect to MySql database!";

    }
}

QSqlDatabase DatabaseOperation::returnDatabase()
{
    return database;
}
