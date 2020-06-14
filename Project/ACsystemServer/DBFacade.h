#pragma once

#include <QObject>
#include<QtSql/qsqldatabase.h>
#include<QtSql/qsqlquery.h>
#include<QtSql/qsqlerror.h>
#include<qvector.h>
#include<qdebug.h>
#include<time.h>
#include<string.h>
#include<qtimer.h>
#include"Report.h"

// 房间信息表roomstat:
//+---------------+-------------+------+-----+---------+-------+
//| Field         | Type        | Null | Key | Default | Extra |
//+---------------+-------------+------+-----+---------+-------+
//| RoomID        | int         | NO   | PRI | NULL    |       |
//| Mode          | varchar(50) | YES  |     | NULL    |       |
//| stat          | varchar(50) | YES  |     | NULL    |       |
//| cur_temp      | float       | YES  |     | NULL    |       |
//| tar_temp      | float       | YES  |     | NULL    |       |
//| cur_speed     | varchar(50) | YES  |     | NULL    |       |
//| fee           | float       | YES  |     | NULL    |       |
//| feerate       | int         | YES  |     | NULL    |       |
//| schedulecount | int         | YES  |     | NULL    |       |
//+---------------+-------------+------+-----+---------+-------+

//房间操作表roomopt:
//+--------+-------------+------+-----+---------+-------+
//| Field  | Type        | Null | Key | Default | Extra |
//+--------+-------------+------+-----+---------+-------+
//| RoomID | int         | NO   | PRI | NULL    |       |
//| OpID   | int         | YES  |     | NULL    |       |
//| opt    | varchar(50) | YES  |     | NULL    |       |
//| time   | varchar(50) | YES  |     | NULL    |       |
//+--------+-------------+------+-----+---------+-------+

//房间服务记录表 roomservice:
//+--------------+-------------+------+-----+---------+-------+
//| Field        | Type        | Null | Key | Default | Extra |
//+--------------+-------------+------+-----+---------+-------+
//| RoomID       | int         | NO   | PRI | NULL    |       |
//| Start_time   | varchar(50) | YES  |     | NULL    |       |
//| End_time     | varchar(50) | YES  |     | NULL    |       |
//| Service_time | int         | YES  |     | NULL    |       |
//| fee          | float       | YES  |     | NULL    |       |
//| cur_speed    | int         | YES  |     | NULL    |       |
//+--------------+-------------+------+-----+---------+-------+

class DBFacade : public QObject
{
	Q_OBJECT

public:
	DBFacade(QObject *parent);
	~DBFacade();
	void connectDB();
	//创建表
	void createStatTable();
	void createServiceTable();
	void createOptTable();
	char* tmtostring(tm* time);
	//获取系统时间，返回tm结构
	tm* gettime();
	//定时更新ui
	void updateUI();
	void insertRoom(int RoomID, int Mode, int stat, float CurrentTemp,float targetTemp,int speed,float fee,int feerate);
	void insertOpt(int RoomID, int optID, int opt, tm* time);
	void insertService(int RoomID, tm* time1, tm* time2, time_t time3, float fee,int speed);
	void updatecurtemp(int RoomID, float temp);
	void updatetartemp(int RoomID, float temp);
	void updateStat(int RoomID, int stat);
	void updatespeed(int RoomID, int speed);
	void updateFee(int RoomID, float fee);
	void updateRoom(int RoomId, int state, float cur_temp, float dist_temp, int speed, float fee);
	void addscheduleCount(int RoomID);
	//获取费用
	float queryFee(int RoomID);
	//获取某个房间的总服务时间
	time_t queryServiceTime(int RoomID);
	//获取账单
	QVector<QString> queryInvoice(int RoomID);
	//获取详单
	QVector<QString> queryRecption(int RoomID);
	//获取生成报表所需的信息
	QVector<RoomData> queryRoomData(RoomInfo roomlist);

private:
	QSqlDatabase db;
	QString sqlsentence;
	QSqlQuery query;
	QTimer* timer;
};
