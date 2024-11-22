#ifndef DICTIONARYDATABASE_H
#define DICTIONARYDATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QHash>

class DictionaryDatabase
{
public:
    DictionaryDatabase()=delete;
    ~DictionaryDatabase()=delete;
    static QSqlDatabase get_database();
    static void close_database();
    static QString get_table_name(QString visible_name);
    static QHash<QString,QString> dictionary;
private:
    static QSqlDatabase db;

};

#endif // DICTIONARYDATABASE_H
