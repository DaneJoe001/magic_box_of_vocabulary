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

    QSqlDatabase returnDatabase();

signals:
private:
    QSqlDatabase database;

};

#endif // DATABASEOPERATION_H
