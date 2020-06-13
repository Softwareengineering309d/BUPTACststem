#pragma once

#include <QObject>
#include<QtSql/qsqldatabase.h>
#include<QtSql/qsqlquery.h>
#include<QtSql/qsqlerror.h>
#include<qvector.h>
#include<qdebug.h>
#include<time.h>
#include<string.h>
#include"Report.h"

class DBFacade : public QObject
{
	Q_OBJECT

public:
	DBFacade(QObject *parent);
	~DBFacade();
	void connectDB();
	void createStatTable();
	void createServiceTable();
	void createOptTable();
	char* tmtostring(tm* time);
	//获取系统时间，返回tm结构
	tm* gettime();
	void insertRoom(int RoomID, int Mode, int stat, float CurrentTemp,float targetTemp,int speed,float fee,int feerate);
	void insertOpt(int RoomID, int optID, int opt, tm* time);
	void insertService(int RoomID, tm* time1, tm* time2, time_t time3, float fee,int speed);
	void updatecurtemp(int RoomID, float temp);
	void updatetartemp(int RoomID, float temp);
	void updatespeed(int RoomID, int speed);
	void updateFee(int RoomID, float fee);
	void updateRoom(int RoomId, int state, float cur_temp, float dist_temp, int speed, float fee);
	void addscheduleCount(int RoomID);
	float queryFee(int RoomID);
	time_t queryServiceTime(int RoomID);
	QVector<QString> queryInvoice(int RoomID);
	QVector<QString> queryRecption(int RoomID);
	QVector<RoomData> queryRoomData(RoomInfo roomlist);

private:
	QSqlDatabase db;
	QString sqlsentence;
	QSqlQuery query;
};
