#ifndef DATABASEOPERATION_H
#define DATABASEOPERATION_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class DatabaseOperation : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseOperation(QObject *parent = nullptr);

    //返回私有成员QSqlDatabase对象database;
    //TODO:考虑是否改为公有成员
    QSqlDatabase returnDatabase();

signals:
private:
    QSqlDatabase database;

};

#endif // DATABASEOPERATION_H
