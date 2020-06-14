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

// ������Ϣ��roomstat:
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

//���������roomopt:
//+--------+-------------+------+-----+---------+-------+
//| Field  | Type        | Null | Key | Default | Extra |
//+--------+-------------+------+-----+---------+-------+
//| RoomID | int         | NO   | PRI | NULL    |       |
//| OpID   | int         | YES  |     | NULL    |       |
//| opt    | varchar(50) | YES  |     | NULL    |       |
//| time   | varchar(50) | YES  |     | NULL    |       |
//+--------+-------------+------+-----+---------+-------+

//��������¼�� roomservice:
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
	//������
	void createStatTable();
	void createServiceTable();
	void createOptTable();
	char* tmtostring(tm* time);
	//��ȡϵͳʱ�䣬����tm�ṹ
	tm* gettime();
	//��ʱ����ui
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
	//��ȡ����
	float queryFee(int RoomID);
	//��ȡĳ��������ܷ���ʱ��
	time_t queryServiceTime(int RoomID);
	//��ȡ�˵�
	QVector<QString> queryInvoice(int RoomID);
	//��ȡ�굥
	QVector<QString> queryRecption(int RoomID);
	//��ȡ���ɱ����������Ϣ
	QVector<RoomData> queryRoomData(RoomInfo roomlist);

private:
	QSqlDatabase db;
	QString sqlsentence;
	QSqlQuery query;
	QTimer* timer;
};
