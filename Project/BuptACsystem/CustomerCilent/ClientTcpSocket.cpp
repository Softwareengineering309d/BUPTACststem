/************************************************************************
 * FILE				: ClientTcpSocket.h
 * AUTHOR			: Li Zhuo
 * DATE				: June 5, 2020
 * CHANGE HISTORY	:
 * Author       Date            Description of change
 * Li Zhuo		June 6, 2020	Solve sticking problem
 ************************************************************************/
#include "ClientTcpSocket.h"

ClientTcpSocket::ClientTcpSocket(QObject *parent)
	: QTcpSocket(parent)
{
	connect(this, &QTcpSocket::readyRead, this, &ClientTcpSocket::receiveData);
	connect(this, &QTcpSocket::disconnected, this, &ClientTcpSocket::serverDisconnected);
}

ClientTcpSocket::~ClientTcpSocket()
{
}

void ClientTcpSocket::serverDisconnected() {
	qDebug() << "Server disconnected.";
}

void ClientTcpSocket::receiveData() {
	while (isReadable()) {
		QByteArray qByteArray = this->readLine();

		if (qByteArray.size() == 0) {
			break;
		}

		QString str;
		if (!qByteArray.isEmpty()) {
			str += tr(qByteArray);
		}
		qDebug() << str;

		QJsonParseError jsonError;
		QJsonDocument jsonDoc = QJsonDocument::fromJson(qByteArray, &jsonError);
		if (jsonError.error != QJsonParseError::NoError)
		{
			qDebug() << "json Error!";
			return;
		}
		qDebug() << "josDoc" << jsonDoc << "\n";
		QJsonObject rootObj = jsonDoc.object();

		int request = -1;
		QJsonObject data;
		if (rootObj.contains("request") && rootObj["request"].isDouble()) {
			request = rootObj["request"].toDouble();
		}
		if (rootObj.contains("data") && rootObj["data"].isObject()) {
			data = rootObj["data"].toObject();
		}
		switch (request)
		{
		case 000:
			if (data.contains("TotalFee") && data["TotalFee"].isDouble() &&
				data.contains("CurrentTemp") && data["CurrentTemp"].isDouble()) {
				emit heartBeat(data["TotalFee"].toInt(), (float)data["CurrentTemp"].toDouble());
			}
			else {
				qDebug() << "Request Error 000: Bad TotalFee or CurrentTemp";
			}
			break;
		case 100:
			if (rootObj.contains("ACK") && rootObj["ACK"].isDouble()) {
				int ack = rootObj["ACK"].toInt();
				if (ack == 101) {
					if (data.contains("DefaultTemp") && data["DefaultTemp"].isDouble() &&
						data.contains("DefaultFanSpeed") && data["DefaultFanSpeed"].isDouble()) {
						float defaultTemp = (float)data["DefaultTemp"].toDouble();
						int defaultFanSpeed = data["DefaultFanSpeed"].toInt();
						emit turnOnAirConditionerOK(defaultTemp, defaultFanSpeed);
					}
					else {
						qDebug() << "Request Error 100: Bad defaultTemp or defaultFanSpeed";
					}
				}
				else if (ack == 102) {
					emit turnOnAirConditionerError();
				}
				else {
					qDebug() << "Request Error 100: Bad ACK: " << ack;
				}
			}
			else {
				qDebug() << "Request Error 100: Bad ACK";
			}
			break;
		case 200:
			if (rootObj.contains("ACK") && rootObj["ACK"].isDouble()) {
				int ack = rootObj["ACK"].toInt();
				if (ack == 201) {
					emit changeTempOK();
				}
				else if (ack == 202) {
					emit changeTempError();
				}
				else {
					qDebug() << "Request Error 100: Bad ACK: " << ack;
				}
			}
			else {
				qDebug() << "Request Error 100: Bad ACK";
			}
			break;
		case 300:
			if (rootObj.contains("ACK") && rootObj["ACK"].isDouble()) {
				int ack = rootObj["ACK"].toInt();
				if (ack == 201) {
					emit changeFanSpeedOK();
				}
				else if (ack == 202) {
					emit changeFanSpeedError();
				}
				else {
					qDebug() << "Request Error 100: Bad ACK: " << ack;
				}
			}
			else {
				qDebug() << "Request Error 100: Bad ACK";
			}
			break;
		case 400:
			if (rootObj.contains("ACK") && rootObj["ACK"].isDouble()) {
				int ack = rootObj["ACK"].toInt();
				if (ack == 201) {
					emit closeAirConditionerOK();
				}
				else if (ack == 202) {
					emit closeAirConditionerError();
				}
				else {
					qDebug() << "Request Error 100: Bad ACK: " << ack;
				}
			}
			else {
				qDebug() << "Request Error 100: Bad ACK";
			}
			break;
		case 500:
			if (data.contains("ServerId") && data["ServerId"].isDouble()) {
				emit serviceOn(data["ServerId"].toInt());
			}
			break;
		case 600:
			emit reachTargetTempStop();
			break;
		case 700:
			if (data.contains("WaitId") && data["WaitId"].isDouble() &&
				data.contains("WaitTime") && data["WaitTime"].isDouble()) {
				emit preemptedStop(data["WaitId"].toInt(), data["WaitTime"].toInt());
			}
			break;
		default:
			break;
		}
	}
}

void ClientTcpSocket::turnOnAirConditioner(int roomID) {
	QJsonObject rootObj, data;
	data.insert("RoomId", roomID);
	rootObj.insert("request", 100);
	rootObj.insert("data", data);
	write(QJsonDocument(rootObj).toJson(QJsonDocument::Compact) + "\n");
	//QString msg = "{\"request\":100,\"data\":{\"RoomId\":"
	//	+ QString::number(roomID)
	//	+ "}}";
	//write(msg.toUtf8().data());
	//flush();
	//Sleep(300);
	//return;
}
void ClientTcpSocket::changeTemp(int roomID, float targetTemp) {
	QJsonObject rootObj, data;
	data.insert("RoomId", roomID);
	data.insert("TargetTemp", targetTemp);
	rootObj.insert("request", 200);
	rootObj.insert("data", data);
	write(QJsonDocument(rootObj).toJson(QJsonDocument::Compact) + "\n");
	//QString msg = "{\"request\":200,\"data\":{\"RoomId\":"
	//	+ QString::number(roomID)
	//	+ ",\"TargetTemp\":"
	//	+ QString::number(targetTemp)
	//	+ "}}";
	//write(msg.toUtf8().data());
	//flush();
	//Sleep(300);
	//return;
}
void ClientTcpSocket::changeFanSpeed(int roomID, int fanSpeed) {
	QJsonObject rootObj, data;
	data.insert("RoomId", roomID);
	data.insert("FanSpeed", fanSpeed);
	rootObj.insert("request", 300);
	rootObj.insert("data", data);
	write(QJsonDocument(rootObj).toJson(QJsonDocument::Compact) + "\n");
	/*QString msg = "{\"request\":300,\"data\":{\"RoomId\":"
		+ QString::number(roomID)
		+ ",\"FanSpeed\":"
		+ QString::number(fanSpeed)
		+ "}}";
	write(msg.toUtf8().data());
	flush();
	Sleep(300);
	return;*/
}
void ClientTcpSocket::closeAirConditioner(int roomID) {
	QJsonObject rootObj, data;
	data.insert("RoomId", roomID);
	rootObj.insert("request", 400);
	rootObj.insert("data", data);
	write(QJsonDocument(rootObj).toJson(QJsonDocument::Compact) + "\n");
	//QString msg = "{\"request\":400,\"data\":{\"RoomId\":"
	//	+ QString::number(roomID)
	//	+ "}}";
	//write(msg.toUtf8().data());
	//flush();
	//Sleep(300);
	//return;
}
void ClientTcpSocket::serviceOnBack(int roomID, float currentTemp, bool succeed) {
	QJsonObject rootObj, data;
	data.insert("currentTemp", currentTemp);
	rootObj.insert("request", 500);
	rootObj.insert("ACK", 501);
	rootObj.insert("data", data);
	write(QJsonDocument(rootObj).toJson(QJsonDocument::Compact) + "\n");
	//QString msg = "{\"request\":500,\"ACK\":501,\"data\":{\"CurrentTemp\":"
	//	+ QString::number(currentTemp)
	//	+ "}}";
	//write(msg.toUtf8().data());
	//flush();
	//Sleep(300);
	//return;
}
void ClientTcpSocket::serviceOnBack(int roomID, bool succeed) {
	QJsonObject rootObj;
	rootObj.insert("request", 500);
	rootObj.insert("ACK", 502);
	write(QJsonDocument(rootObj).toJson(QJsonDocument::Compact) + "\n");
	//QString msg = "{\"request\":500,\"ACK\":502}";
	//write(msg.toUtf8().data());
	//flush();
	//Sleep(300);
	//return;
}
void ClientTcpSocket::reachTargetTempStopBack(bool succeed) {
	if (succeed) {
		QJsonObject rootObj;
		rootObj.insert("request", 600);
		rootObj.insert("ACK", 601);
		write(QJsonDocument(rootObj).toJson(QJsonDocument::Compact) + "\n");
		//QString msg = "{\"request\":600,\"ACK\":601}";
		//write(msg.toUtf8().data());
		//flush();
		//Sleep(300);
		//return;
	}
	QJsonObject rootObj;
	rootObj.insert("request", 600);
	rootObj.insert("ACK", 602);
	write(QJsonDocument(rootObj).toJson(QJsonDocument::Compact) + "\n");
	//QString msg = "{\"request\":600,\"ACK\":602}";
	//write(msg.toUtf8().data());
	//flush();
	//Sleep(300);
	//return;
}
void ClientTcpSocket::preemptedStopBack(bool succeed) {
	if (succeed) {
		QJsonObject rootObj;
		rootObj.insert("request", 700);
		rootObj.insert("ACK", 701);
		write(QJsonDocument(rootObj).toJson(QJsonDocument::Compact) + "\n");
		//QString msg = "{\"request\":700,\"ACK\":701}";
		//write(msg.toUtf8().data());
		//flush();
		//Sleep(300);
		//return;
	}
	QJsonObject rootObj;
	rootObj.insert("request", 700);
	rootObj.insert("ACK", 701);
	write(QJsonDocument(rootObj).toJson(QJsonDocument::Compact) + "\n");
	//QString msg = "{\"request\":700,\"ACK\":702}";
	//write(msg.toUtf8().data());
	//flush();
	//Sleep(300);
	//return;
}