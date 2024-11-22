#include "dictionarydatabase.h"

// 静态成员变量的定义
QSqlDatabase DictionaryDatabase::db = QSqlDatabase();
QHash<QString, QString> DictionaryDatabase::dictionary; // 假设你需要一个字典来存储映射

QSqlDatabase DictionaryDatabase::get_database()
{
    // 检查数据库是否已经打开
    if (db.isOpen())
    {
        return db;
    }

    // 检查数据库驱动是否可用
    if (QSqlDatabase::drivers().isEmpty())
    {
        qDebug() << "Error! No drivers!";
        return QSqlDatabase(); // 返回一个空的数据库对象
    }

    // 检查是否已经添加了同名连接
    if (QSqlDatabase::connectionNames().contains("sqlite_connection"))
    {
        db = QSqlDatabase::database("sqlite_connection");
    }
    else
    {
        // 添加数据库连接
        db = QSqlDatabase::addDatabase("QSQLITE", "sqlite_connection");
    }

    // 设置数据库名称
    db.setDatabaseName("./dictionary_database.db"); // 使用当前路径

    // 打开数据库
    if (db.open())
    {
        qDebug() << "Connected to SQLite database successfully!";

    }
    else
    {
        qDebug() << "Failed to connect to SQLite database!";
    }
    qDebug() << "数据库文件路径:" << db.databaseName();

    return db;
}


void DictionaryDatabase::close_database()
{
    if(db.isOpen())
    {
        db.close();
    }
}

QString DictionaryDatabase::get_table_name(QString visible_name)
{
    if(dictionary.contains(visible_name))
    {
        return dictionary.value(visible_name);
    }
    return QString();
}
