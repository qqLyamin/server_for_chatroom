#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>
#include <QCoreApplication>
#include <QQueue>

class ClientThread : public QThread
{
    Q_OBJECT

    int m_count;
    QTcpServer * server;
    QTcpSocket * socket;
    qintptr socketDescriptor;
    QQueue<QString> myq;

public:
    virtual void run();
    ClientThread(qintptr socketDescriptor, int m_count, QTcpServer * server);
private:
    void exchangeWithClient();
signals:
    void MsgFromClient(const QString &);
    void CheckSignIn(const QString &, const QString &);
public slots:
    void MsgFromServer_toQueue(const QString &);
};

#endif // CLIENTTHREAD_H
