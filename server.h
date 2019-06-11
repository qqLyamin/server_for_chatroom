#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <iostream>
#include "client.h"
#include "database.h"
#include "chatroom.h"
#include "clientthread.h"

class Server : public QTcpServer
{
    Q_OBJECT
    int m_count;
    QTcpServer my_server;
    QList<Client *> my_clients;
    QList<Chatroom *> rooms;
    Database * db;
public:
    explicit Server(QObject *parent = nullptr);
    bool listen(const QHostAddress &address = QHostAddress::Any, quint16 port = 33333);/*3*/
    QAbstractSocket::SocketError serverError() const;
protected:
    virtual void incomingConnection(qintptr) override;
private slots:
    void new_Connection();
    void Disconnected_signal();
    void ExchangeWithClient(QTcpSocket *);

    void MsgFromClient(const QString &);
public slots:
    void DatabaseConnectingUser_true();
    void DatabaseConnectingUser_false();

    void DB_chatname_chatpassword_true();
    void DB_chatname_chatpassword_false();

    void DB_chatname_true();
    void DB_chatname_false();

    void DB_registration_true();
    void DB_registration_false();
signals:
    void NewClient(const QString &, const QString &, const QString &);
    void Check_isChatnameCorrect(const QString &);
    void CheckingCorrect_Chatname_Password(const QString &, const QString &);
    void CheckingSignIn_toDatabase(const QString &, const QString &);
    void SendToClient(const QString &);
};

#endif // SERVER_H
