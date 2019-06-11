#ifndef DATABASE_H
#define DATABASE_H
/*9*/
#include <QObject>
#include <QSqlError>
#include <QSqlRecord>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlTableModel>
#include <QDebug>

class Database : public QObject
{
    Q_OBJECT
    QSqlDatabase allclients_db;
public:
    explicit Database(QObject *parent = nullptr);
public slots:
    void add_newclient(const QString & name, const QString & password, const QString & email);
    void confirm_entrance(const QString & name, const QString & password);
    void confirm_chatname_password(const QString & chatname, const QString & chatpassword);
    void confirm_chatname(const QString & chatname);
signals:
    void connecting_checkAnswer_yes();
    void connecting_checkAnswer_no();

    void chatname_chatpassword_yes();
    void chatname_chatpassword_no();

    void chatname_yes();
    void chatname_no();

    void registrated();
    void registration_fault();
};

#endif // DATABASE_H
