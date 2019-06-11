#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include "clientthread.h"

class Client : public QObject
{
    Q_OBJECT
    ClientThread * my_thread;

public:
    void SetThread(ClientThread *);

    Client();
    ~Client();
public slots:
    void MessageFromServer(const QString & msg);
    void MessageFromClient(const QString & msg);


    void Finished();
signals:
    void MsgFromServer(const QString & msg);
    void MsgFromClient(const QString & msg);

    void Disconnected();
};

#endif // CLIENT_H
