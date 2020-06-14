/************************************************************************
 * FILE				: TcpServer.cpp
 * AUTHOR			: Li Zhuo
 * DATE				: June 5, 2020
 * CHANGE HISTORY	:
 * Author       Date            Description of change
 * Li Zhuo		June 6, 2020	solve sticking problem
 ************************************************************************/
#include "TcpSocket.h"

TcpSocket::TcpSocket(QObject *parent)
{
	connect(this, &TcpSocket::readyRead, this, &TcpSocket::receiveData);
	connect(this, &TcpSocket::disconnected, this, &TcpSocket::receiveData);
}

TcpSocket::~TcpSocket()
{
}

void TcpSocket::receiveData() {
	while (isReadable()) {
		QByteArray data = this->readLine();
		if (data.size() == 0) {
			break;
		}
		emit receiveDataRaw(data, this->socketDescriptor());
	}
}

void TcpSocket::clientDisconnect(){
	emit clientDisconnected(this->socketDescriptor());
}