/************************************************************************
 * FILE				: TcpServer.cpp
 * AUTHOR			: Li Zhuo
 * DATE				: June 5, 2020
 * CHANGE HISTORY	:
 * Author       Date            Description of change
 * Li Zhuo		June 6, 2020	change to a QJson way to pack json msg
 ************************************************************************/
#include "TcpServer.h"

TcpServer::TcpServer(QObject *parent, quint16 port)
	: QTcpServer(parent)
{
	if (!listen(QHostAddress::Any, port)) {
		qDebug() << this->errorString();
	}
	qDebug() << "Listening at " << port;
}

TcpServer::~TcpServer()
{
}

void TcpServer::incomingConnection(qintptr socketDescriptor) {

	qDebug() << "Client " << socketDescriptor << " is coming.\n";

	TcpSocket* newClient = new TcpSocket(this);
	newClient->setSocketDescriptor(socketDescriptor);
	clientSocketList.append(newClient);

	connect(newClient, &TcpSocket::receiveDataRaw, this, &TcpServer::receiveData);
	connect(newClient, &TcpSocket::clientDisconnected, this, &TcpServer::clientDisconnected);
}

void TcpServer::receiveData(QByteArray data, qintptr socketDescriptor) {
	//在Debug栏里输出
	QString str;
	if (!data.isEmpty()) {
		str += tr(data);
	}
	qDebug() << str;

	////echo
	//for (size_t i = 0; i < clientSocketList.count(); i++)
	//{
	//	TcpSocket* item = clientSocketList.at(i);
	//	if (item->socketDescriptor() == socketDescriptor) {
	//		item->write(data);
	//		item->flush();
	//	}
	//}

	//json解析
	QJsonParseError jsonError;
	QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &jsonError);
	if (jsonError.error != QJsonParseError::NoError)
	{
		qDebug() << "json Error!";
		return;
	}
	qDebug() << "josDoc" << jsonDoc << "\n";
	QJsonObject rootObj = jsonDoc.object();

	if (rootObj.contains("request") && rootObj["request"].isDouble()) {
		int request = rootObj["request"].toInt();
		QJsonObject data;
		if (rootObj.contains("data") && rootObj["data"].isObject()) {
			data = rootObj["data"].toObject();
		}
		switch (request)
		{
		case 100:
			if (data.contains("RoomId") && data["RoomId"].isDouble()) {
				int roomID = data["RoomId"].toInt();
				roomIDToSocketDescriptor.insert(roomID, socketDescriptor);
				socketDescriptorToRoomID.insert(socketDescriptor, roomID);
				emit turnOnAirConditioner(roomID);
			}
			break;
		case 200:
			if (data.contains("RoomId") && data["RoomId"].isDouble() &&
				data.contains("TargetTemp") && data["TargetTemp"].isDouble()) {
				emit changeTemp(data["RoomId"].toInt(), (float)data["TargetTemp"].toDouble());
			}
			break;
		case 300:
			if (data.contains("RoomId") && data["RoomId"].isDouble() &&
				data.contains("FanSpeed") && data["FanSpeed"].isDouble()) {
				emit changeFanSpeed(data["RoomId"].toInt(), data["FanSpeed"].toInt());
			}
			break;
		case 400:
			if (data.contains("RoomId") && data["RoomId"].isDouble()) {
				emit turnOffAirConditioner(data["RoomId"].toInt());
			}
			break;
		case 500:
			if (rootObj.contains("ACK") && rootObj["ACK"].isDouble()) {
				int ack = rootObj["ACK"].toInt();
				if (ack == 501) {
					if (data.contains("CurrentTemp") && data["CurrentTemp"].isDouble()) {
						int roomID = socketDescriptorToRoomID.value(socketDescriptor, -1);
						if (roomID != -1)
							emit serviceOnOK(roomID, (float)data["CurrentTemp"].toDouble());
						else
							qDebug() << "Requset Error: roomID not exist.";
					}
				}
				else if (ack == 502) {
					int roomID = socketDescriptorToRoomID.value(socketDescriptor, -1);
					if (roomID != -1)
						emit serviceOnError(roomID);
					else
						qDebug() << "Requset Error: roomID not exist.";
				}
			}
			break;
		case 600:
			if (rootObj.contains("ACK") && rootObj["ACK"].isDouble()) {
				int ack = rootObj["ACK"].toInt();
				if (ack == 601) {
					int roomID = socketDescriptorToRoomID.value(socketDescriptor, -1);
					if (roomID != -1)
						emit reachTargetTempStopOK(roomID);
					else
						qDebug() << "Requset Error: roomID not exist.";
				}
				else if (ack == 602) {
					int roomID = socketDescriptorToRoomID.value(socketDescriptor, -1);
					if (roomID != -1)
						emit reachTargetTempStopError(roomID);
					else
						qDebug() << "Requset Error: roomID not exist.";
				}
			}
			break;
		case 700:
			if (rootObj.contains("ACK") && rootObj["ACK"].isDouble()) {
				int ack = rootObj["ACK"].toInt();
				if (ack == 701) {
					int roomID = socketDescriptorToRoomID.value(socketDescriptor, -1);
					if (roomID != -1)
						emit preemptedStopOK(roomID);
					else
						qDebug() << "Requset Error: roomID not exist.";
				}
				else if (ack == 701) {
					if (ack == 701) {
						int roomID = socketDescriptorToRoomID.value(socketDescriptor, -1);
						if (roomID != -1)
							emit preemptedStopError(roomID);
						else
							qDebug() << "Requset Error: roomID not exist.";
					}
				}
			}
			break;
		default:
			break;
		}
	}
	emit receiveDataRaw(data);
}

void TcpServer::clientDisconnected(qintptr socketDescriptor) {
	for (size_t i = 0; i < clientSocketList.count(); i++)
	{
		TcpSocket* item = clientSocketList.at(i);
		if (item->socketDescriptor() == socketDescriptor) {
			clientSocketList.removeAt(i);
			break;
		}
	}
	return;
}

void TcpServer::turnOnAirConditionerBack(int roomID, float defaultTemp, int defaultFanSpeed, bool succeed) {
	TcpSocket* socket = findSocketByRoomID(roomID);
	QJsonObject rootObj, data;
	data.insert("DefaultTemp", defaultTemp);
	data.insert("DefaultFanSpeed", defaultFanSpeed);
	rootObj.insert("request", 100);
	rootObj.insert("ACK", 101);
	rootObj.insert("data", data);
	socket->write(QJsonDocument(rootObj).toJson(QJsonDocument::Compact) + "\n");
	/*QString msg = "{\"request\":100,\"ACK\":101,\"data\":{\"DefaultTemp\":"
		+ QString::number(defaultTemp)
		+ ",\"DefaultFanSpeed\":"
		+ QString::number(defaultFanSpeed)
		+ "}}";
	socket->write(msg.toUtf8().data());
	socket->flush();
	Sleep(300);*/
	return;
}
void TcpServer::turnOnAirConditionerBack(int roomID, bool succeed) {
	TcpSocket* socket = findSocketByRoomID(roomID);
	QJsonObject rootObj;
	rootObj.insert("request", 100);
	rootObj.insert("ACK", 102);
	socket->write(QJsonDocument(rootObj).toJson(QJsonDocument::Compact) + "\n");

	//QString msg = "{\"request\":100,\"ACK\":102}}";
	//socket->write(msg.toUtf8().data());
	//socket->flush();
	//Sleep(300);
	return;
}
void TcpServer::changeTempBack(int roomID, bool succeed) {
	TcpSocket* socket = findSocketByRoomID(roomID);
	QJsonObject rootObj;
	if (succeed) {
		rootObj.insert("request", 200);
		rootObj.insert("ACK", 201);
		socket->write(QJsonDocument(rootObj).toJson(QJsonDocument::Compact) + "\n");
		//QString msg = "{\"request\":200,\"ACK\":201}}";
		//socket->write(msg.toUtf8().data());
		//socket->flush();
		//Sleep(300);
		return;
	}
	rootObj.insert("request", 200);
	rootObj.insert("ACK", 202);
	socket->write(QJsonDocument(rootObj).toJson(QJsonDocument::Compact) + "\n");
	//QString msg = "{\"request\":200,\"ACK\":202}}";
	//socket->write(msg.toUtf8().data());
	//socket->flush();
	//Sleep(300);
	return;
}
void TcpServer::changeFanSpeedBack(int roomID, bool succeed) {
	TcpSocket* socket = findSocketByRoomID(roomID);
	QJsonObject rootObj;
	if (succeed) {
		rootObj.insert("request", 300);
		rootObj.insert("ACK", 301);
		socket->write(QJsonDocument(rootObj).toJson(QJsonDocument::Compact) + "\n");
		/*QString msg = "{\"request\":300,\"ACK\":301}}";
		socket->write(msg.toUtf8().data());
		socket->flush();
		Sleep(300);*/
		return;
	}
	rootObj.insert("request", 300);
	rootObj.insert("ACK", 301);
	socket->write(QJsonDocument(rootObj).toJson(QJsonDocument::Compact) + "\n");
	//QString msg = "{\"request\":300,\"ACK\":302}}";
	//socket->write(msg.toUtf8().data());
	//socket->flush();
	//Sleep(300);
	return;
}
void TcpServer::turnOffAirConditionerBack(int roomID, bool succeed) {
	TcpSocket* socket = findSocketByRoomID(roomID);
	QJsonObject rootObj;
	if (succeed) {
		rootObj.insert("request", 400);
		rootObj.insert("ACK", 401);
		socket->write(QJsonDocument(rootObj).toJson(QJsonDocument::Compact) + "\n");
		/*QString msg = "{\"request\":400,\"ACK\":401}}";
		socket->write(msg.toUtf8().data());
		socket->flush();
		Sleep(300);*/
		return;
	}
	rootObj.insert("request", 400);
	rootObj.insert("ACK", 402);
	socket->write(QJsonDocument(rootObj).toJson(QJsonDocument::Compact) + "\n");
	/*QString msg = "{\"request\":400,\"ACK\":402}}";
	socket->write(msg.toUtf8().data());
	socket->flush();
	Sleep(300);*/
	return;
}
void TcpServer::serviceOn(int roomID, int serverID) {
	TcpSocket* socket = findSocketByRoomID(roomID);
	QJsonObject rootObj, data;
	data.insert("ServerId", serverID);
	rootObj.insert("request", 500);
	rootObj.insert("data", data);
	socket->write(QJsonDocument(rootObj).toJson(QJsonDocument::Compact) + "\n");
	/*QString msg = "{\"request\":500,\"data\":{\"ServerId\":"
		+ QString::number(serverID)
		+ "}}";
	socket->write(msg.toUtf8().data());
	socket->flush();
	Sleep(300);*/
	return;
}
void TcpServer::reachTargetTempStop(int roomID) {
	TcpSocket* socket = findSocketByRoomID(roomID);
	QJsonObject rootObj;
	rootObj.insert("request", 600);
	socket->write(QJsonDocument(rootObj).toJson(QJsonDocument::Compact) + "\n");
	//QString msg = "{\"request\":600}}";
	//socket->write(msg.toUtf8().data());
	//socket->flush();
	//Sleep(300);
	return;
}
void TcpServer::preemptedStop(int roomID, int waitID, int waitTime) {
	TcpSocket* socket = findSocketByRoomID(roomID);
	QJsonObject rootObj, data;
	data.insert("WaitId", waitID);
	data.insert("waitTime", waitTime);
	rootObj.insert("request", 700);
	rootObj.insert("data", data);
	socket->write(QJsonDocument(rootObj).toJson(QJsonDocument::Compact) + "\n");
	//QString msg = "{\"request\":700,\"data\":{\"WaitId\":"
	//	+ QString::number(waitID)
	//	+ ",\"WaitTime\":"
	//	+ QString::number(waitTime)
	//	+ "}}";
	//socket->write(msg.toUtf8().data());
	//socket->flush();
	//Sleep(300);
	return;
}
void TcpServer::keepAlive(int roomID, int totalFee, float CurrentTemp) {
	TcpSocket* socket = findSocketByRoomID(roomID);
	QJsonObject rootObj, data;
	data.insert("TotalFee", totalFee);
	data.insert("CurrentTemp", CurrentTemp);
	rootObj.insert("request", 000);
	rootObj.insert("data", data);
	socket->write(QJsonDocument(rootObj).toJson(QJsonDocument::Compact) + "\n");
	/*QString msg = "{\"request\":000,\"data\":{\"TotalFee\":"
		+ QString::number(totalFee)
		+ ",\"CurrentTemp\":"
		+ QString::number(CurrentTemp)
		+ "}}";
	socket->write(msg.toUtf8().data());
	socket->flush();
	Sleep(300);*/
	return;
}
TcpSocket* TcpServer::findSocketByRoomID(int roomID) {
	
	qintptr descriptor = roomIDToSocketDescriptor.value(roomID, -1);
	if (descriptor == -1) {
		qDebug() << "RoomID: " << roomID << "dose not exist.";
		return NULL;
	}
	for (size_t i = 0; i < clientSocketList.count(); i++)
	{
		TcpSocket* item = clientSocketList.at(i);
		if (descriptor == item->socketDescriptor()) {
			return item;
		}
	}
	qDebug() << "RoomID: " << roomID << "dose not have a corresponding socket";
	return NULL;
}