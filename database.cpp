#include "database.h"

Database::Database(QObject *parent) : QObject(parent)
{
    allclients_db = QSqlDatabase::addDatabase("QMYSQL");
    allclients_db.setHostName("localhost");
    allclients_db.setDatabaseName("allclients");
    allclients_db.setUserName("root");
    allclients_db.setPassword("2708tjlfe");

    if (!allclients_db.open())
    {
        qDebug() << "db openning error " << allclients_db.lastError().text();
    }
    else {
        qDebug() << "Success!";
    }
}

void Database::add_newclient(const QString & name, const QString & password, const QString & email)
{
    QSqlQuery query;
    query.prepare("INSERT INTO clients(name, email, password)" "VALUES(?, ?, ?)");
    query.addBindValue(name);
    query.addBindValue(password);
    query.addBindValue(email);
    emit (query.exec() ? registrated() : registration_fault());
}

void Database::confirm_entrance(const QString & name, const QString & password)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM clients WHERE NAME = (?) AND PASSWORD = (?)");
    query.addBindValue(name);
    query.addBindValue(password);
    if (query.exec())
    {
        emit (query.first() ? connecting_checkAnswer_yes() : connecting_checkAnswer_no());
    }
}

void Database::confirm_chatname_password(const QString & chatname, const QString & chatpassword)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM hosts WHERE CHAT_NAME = (?) AND CHAT_PASSWORD = (?)");
    query.addBindValue(chatname);
    query.addBindValue(chatpassword);
    if (query.exec())
    {
        qDebug() << query.executedQuery();
        emit (query.first() ? chatname_chatpassword_yes() : chatname_chatpassword_no());
    }
}

void Database::confirm_chatname(const QString & chatname)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM hosts WHERE CHAT_NAME = (?)");
    query.addBindValue(chatname);
    if (query.exec())
    {
        qDebug() << query.executedQuery();
        emit (query.first() ? chatname_no() : chatname_yes());
    }
}

