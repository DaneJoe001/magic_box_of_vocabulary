#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include<QSqlDatabase>
#include<QHash>
#include<QString>
#include<QSqlQuery>
#include<QSqlError>

class DatabaseManager
{
public:
    DatabaseManager()=delete;
    ~DatabaseManager()=delete;
    //获取静态数据库对象
    static QSqlDatabase get_database(QString database_name);
    //关闭所有数据库对象
    static void close_database();
private:
    //存储数据库名和数据库对象对应关系
    static QHash<QString,QSqlDatabase>database_map;

};

#endif // DATABASEMANAGER_H
