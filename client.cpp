#include "client.h"

Client::Client()
{

}

Client::~Client()
{

}

void Client::SetThread(ClientThread * thread)
{
    my_thread = thread;
    connect(my_thread, &ClientThread::finished, this, &Client::Finished);
    connect(this, &Client::MsgFromServer, my_thread, &ClientThread::MsgFromServer_toQueue);
    connect(my_thread, &ClientThread::MsgFromClient, this, &Client::MessageFromClient);
    my_thread->start();
}

void Client::Finished()
{
    delete my_thread;
    emit Disconnected();
}

void Client::MessageFromServer(const QString & msg)
{
    emit MsgFromServer(msg);
}

void Client::MessageFromClient(const QString & msg)
{
    emit MsgFromClient(msg);
}

