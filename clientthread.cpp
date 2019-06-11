#include "clientthread.h"

ClientThread::ClientThread(qintptr socketDescriptor, int m_count, QTcpServer * server)
{
    this->m_count = m_count;
    this->server = server;
    this->socketDescriptor = socketDescriptor;
}

void ClientThread::exchangeWithClient()
{
    QString msg = socket->readAll();
    emit MsgFromClient(msg);
}

void ClientThread::MsgFromServer_toQueue(const QString & msg)
{
    myq.enqueue(msg);
}

void ClientThread::run()
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        socket->write("connected:ok");
    }
    else
    {
        return;
    }
    while(socket->state() == QAbstractSocket::ConnectedState)
    {
        if (socket->waitForReadyRead(100))
        {
           exchangeWithClient();
        }
        else
        {
            QCoreApplication::processEvents();
            while (!myq.isEmpty())
            {
                socket->write(myq.dequeue().toUtf8());
            }
        }
    }
}
