#include "database_manager.h"

QHash<QString,QSqlDatabase>DatabaseManager::database_map;

QSqlDatabase DatabaseManager::get_database(QString database_name)
{
    //根据数据库名返回数据库对象
    if(database_map.contains(database_name))
    {
        return database_map[database_name];
    }
    //检查驱动是否存在
    if (QSqlDatabase::drivers().isEmpty())
    {
        qDebug() << "Error:No drivers!";
        return QSqlDatabase(); // 返回一个空的数据库对象
    }
    QSqlDatabase database_1=QSqlDatabase::addDatabase("QSQLITE","local_connection");
    //TODO:研究下Qt的资源路径表示
    database_1.setDatabaseName("./dictionary_database.db");
    if(database_1.open())
    {
        qDebug()<<"Succeeding in connecting to SQLite database successfully!";
        database_map.insert("dictionary_database",database_1);
    }
    else
    {
        // 获取和输出错误信息
        qDebug() << "Failed to connect to SQLite database!";
        // 输出数据库名称
        qDebug() << "Database Name: :/dictionary_database.db";
        // 输出连接名称
        qDebug() << "Connection Name: local_connection";
         // 输出具体的错误信息
        qDebug() << "Error: " << database_1.lastError().text();
    }
    QSqlDatabase database_2=QSqlDatabase::addDatabase("QODBC","remote_connection");
    database_2.setDatabaseName("magic_box_of_vocabulary");
    if(database_2.open())
    {
        qDebug()<<"Succeeding in connecting to MySql database successfully!";
        database_map.insert("basic_database",database_2);
    }
    else
    {
        qDebug() << "Failed to connect to SQLite database!";
        qDebug() << "Database Name: :/dictionary_database.db";
        qDebug() << "Connection Name: local_connection";
        qDebug() << "Error: " << database_2.lastError().text();
    }
    return database_map[database_name];
}

void DatabaseManager::close_database()
{
    QHash<QString,QSqlDatabase>::const_iterator iterator=database_map.constBegin();
    while(iterator!=database_map.constEnd())
    {
        const QString connectionName=iterator.key();
        //TODO:获取数据库连接名关闭数据库；
        QSqlDatabase temp=iterator.value();
        if(temp.isOpen())
        {
            temp.close();
            qDebug()<<"Closed database connection:"<<connectionName;
        }
        iterator++;
    }
}
