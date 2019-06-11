#ifndef CHATROOM_H
#define CHATROOM_H

#include <QObject>
#include <QList>
#include "client.h"

class Chatroom : public QObject
{
    Q_OBJECT

    QList<Client *> clients_list;
public:
    explicit Chatroom(QObject *parent = nullptr);

signals:

public slots:
};

#endif // CHATROOM_H
