#include "server.h"

Server::Server(QObject *parent) : QTcpServer (parent)
{
    m_count = 0;
    connect(&my_server, &QTcpServer::newConnection, this, &Server::new_Connection); // появился сокет - обработал его nextPendingConnection

    db = new Database;
    connect(this, &Server::CheckingSignIn_toDatabase, db, &Database::confirm_entrance);
    connect(this, &Server::CheckingCorrect_Chatname_Password, db, &Database::confirm_chatname_password);
    connect(this, &Server::Check_isChatnameCorrect, db, &Database::confirm_chatname);
    connect(this, &Server::NewClient, db, &Database::add_newclient);

    connect(db, &Database::connecting_checkAnswer_yes, this, &Server::DatabaseConnectingUser_true);
    connect(db, &Database::connecting_checkAnswer_no, this, &Server::DatabaseConnectingUser_false);

    connect(db, &Database::chatname_chatpassword_yes, this, &Server::DB_chatname_chatpassword_true);
    connect(db, &Database::chatname_chatpassword_no, this, &Server::DB_chatname_chatpassword_false);

    connect(db, &Database::chatname_yes, this, &Server::DB_chatname_true);
    connect(db, &Database::chatname_no, this, &Server::DB_chatname_false);

    connect(db, &Database::registrated, this, &Server::DB_registration_true);
    connect(db, &Database::registration_fault, this, &Server::DB_registration_false);
}

bool Server::listen(const QHostAddress &address, quint16 port)
{
    return my_server.listen(address, port);
}

QAbstractSocket::SocketError Server::serverError() const
{
    return my_server.serverError();
}

void Server::ExchangeWithClient(QTcpSocket * ClientSocket)
{
    qDebug() << "EXCHANGE";
    char buffer[1024]{0};
    qint64 rr = ClientSocket->read(buffer, ClientSocket->bytesAvailable());
    sprintf(buffer, "%s - from client %d", buffer, m_count);

    qint64 rw = ClientSocket->write("check:ok");
    std::cout <<"SERVER: "<< buffer <<" ,write bytes:" << rw << std::endl;
}

void Server::MsgFromClient(const QString & msg)
{
    if(msg.startsWith("check_user:"))
    {
        QString msg_name, msg_password;
        int firstposition = msg.indexOf(':');
        int starposition = msg.indexOf('*');
        msg_name = msg.mid(firstposition + 1, starposition - firstposition - 1);
        msg_password = msg.mid(starposition + 1);

        emit CheckingSignIn_toDatabase(msg_name, msg_password);
    }

    if(msg.startsWith("check_connecting:"))
    {
        QString msg_chatname, msg_chatpassword;
        int firstposition = msg.indexOf(':');
        int starposition = msg.indexOf('*');
        msg_chatname = msg.mid(firstposition + 1, starposition - firstposition - 1);
        msg_chatpassword = msg.mid(starposition + 1);

        emit CheckingCorrect_Chatname_Password(msg_chatname, msg_chatpassword);
    }

    if(msg.startsWith("check_creating:"))
    {
        QString msg_chatname;
        int firstposition = msg.indexOf(':');
        int starposition = msg.indexOf('*');
        msg_chatname = msg.mid(firstposition + 1, starposition - firstposition - 1);

        emit Check_isChatnameCorrect(msg_chatname);
    }

    if(msg.startsWith("registrate:"))
    {
        QString msg_name, msg_password, msg_email;
        int firstposition = msg.indexOf(':');
        int starposition = msg.indexOf('*');
        int waveposition = msg.indexOf('~');
        msg_name = msg.mid(firstposition + 1, starposition - firstposition - 1);
        msg_password = msg.mid(starposition + 1, waveposition - starposition - 1);
        msg_email = msg.mid(waveposition + 1);

        emit NewClient(msg_name, msg_password, msg_email);
    }
}

void Server::DatabaseConnectingUser_true()
{
    emit SendToClient("check:ok");
}

void Server::DatabaseConnectingUser_false()
{
    emit SendToClient("check:nope");
}

void Server::DB_chatname_chatpassword_true()
{
    emit SendToClient("check_connecting:ok");
}

void Server::DB_chatname_chatpassword_false()
{
    emit SendToClient("check_connecting:nope");
}

void Server::DB_chatname_true()
{
    emit SendToClient("check_creating:ok");
}

void Server::DB_chatname_false()
{
    emit SendToClient("check_creating:nope");
}

void Server::DB_registration_true()
{
    emit SendToClient("registrate:ok");
}

void Server::DB_registration_false()
{
    emit SendToClient("registrate:nope");
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    m_count++;

    Client * new_client = new Client;
    connect(new_client, &Client::Disconnected, this, &Server::Disconnected_signal);
    connect(this, &Server::SendToClient, new_client, &Client::MessageFromServer);
    connect(new_client, &Client::MsgFromClient, this, &Server::MsgFromClient);

    my_clients.push_back(new_client);

    ClientThread * thread = new ClientThread(socketDescriptor, m_count, this);
    new_client->SetThread(thread);

    connect(thread, &QThread::finished, thread, &QObject::deleteLater);
}

void Server::new_Connection()//зарегался
{
    qDebug() << "newConnection";
    incomingConnection(my_server.nextPendingConnection()->socketDescriptor());
}

void Server::Disconnected_signal()
{
    //Client * Any_client = reinterpret_cast<Client*>(QObject::sender()); // почему не съел статик каст????????????
    auto Any_client = QObject::sender();
    for (auto i = my_clients.begin(); i != my_clients.end(); ++i)
    {
        if (Any_client == * i)
        {
            my_clients.removeOne(*i);
            delete Any_client;
        }
    }
}
