/************************************************************************
 * FILE				: TcpServer.cpp
 * AUTHOR			: Li Zhuo
 * DATE				: June 5, 2020
 * CHANGE HISTORY	:
 * Author       Date            Description of change
 * Li Zhuo		June 6, 2020	solve sticking problem
 ************************************************************************/
#pragma once

#include <QObject>
#include <QtNetwork/QTcpSocket.h>

class TcpSocket : public QTcpSocket
{
	Q_OBJECT

public:
	TcpSocket(QObject *parent);
	~TcpSocket();

private slots:
	void receiveData();
	void clientDisconnect();

signals:
	void receiveDataRaw(QByteArray, qintptr);
	void clientDisconnected(qintptr);
};
