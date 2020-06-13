#include "DBFacade.h"
#include"global.h"

DBFacade::DBFacade(QObject *parent)
	: QObject(parent)
{
	connectDB();
	createStatTable();
	createOptTable();
	createServiceTable();
}

DBFacade::~DBFacade()
{
}

void DBFacade::connectDB()
{
	db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("localhost");
	db.setDatabaseName("buptacsystem");
	db.setUserName("root");
	db.setPassword("c3n14z26boy");
	if (!db.open())
	{

		qDebug() << "DatabaseError";
		qDebug() << db.lastError();

	}
	else {
		qDebug() << "Connect MYSQL SUCCESS";
	}
}

void DBFacade::createStatTable()
{
	QSqlQuery query(db);
	sqlsentence = "create table RoomStat(RoomID int PRIMARY KEY,Mode varchar(50),stat varchar(50),cur_temp float,tar_temp float,cur_speed varchar(50),fee float,feerate int,schedulecount int)";
	query.exec(sqlsentence);
	qDebug() << db.lastError();
}

void DBFacade::createServiceTable()
{
	QSqlQuery query(db);
	QString sql2 = "create table RoomService(RoomID int PRIMARY KEY,Start_time varchar(50),End_time varchar(50),Service_time int ,fee float,cur_speed int)";
	query.exec(sql2);
	qDebug() << db.lastError();
	
}


void DBFacade::createOptTable()
{
	QSqlQuery query(db);
	QString sql1 = "create table RoomOpt(RoomID int PRIMARY KEY,OpID int,opt varchar(50),time varchar(50))";
	query.exec(sql1);
	qDebug() << db.lastError();
}

char* DBFacade::tmtostring(tm* time)
{
	char Time[255];
	char myFormat[] = "%Y-%m-%d:%H:%M:%S";
	strftime(Time, sizeof(Time), myFormat, time);
	return Time;
}

tm* DBFacade::gettime()
{
	time_t now = time(0);
	tm* nowtime = localtime(&now);
	return nowtime;
}

void DBFacade::insertRoom(int RoomID, int Mode, int stat, float CurrentTemp, float targetTemp, int speed, float fee, int feerate)
{
	QSqlQuery query1(db);
	QString sql1 = QString("insert into RoomStat values(%1,%2,%3,%4,%5,%6,%7,%8,0)").arg(RoomID).arg(Mode).arg(stat).arg(CurrentTemp).arg(targetTemp).arg(speed).arg(fee).arg(feerate);
	query1.exec(sql1);
}

void DBFacade::insertOpt(int RoomID, int optID, int opt, tm* time)
{
	QSqlQuery query;
	QString sql1 = QString("insert into RoomOpt values(%1,%2,%3,%4)").arg(RoomID).arg(optID).arg(opt).arg(tmtostring(time));
	query.exec(sql1);
}

void DBFacade::insertService(int RoomID, tm* time1, tm* time2, time_t time3, float fee,int speed)
{
	QSqlQuery query(db);
	QString sql1 = QString("issert into RoomService values(%1,%2,%3,%4,%5,&6)").arg(RoomID).arg(tmtostring(time1)).arg(tmtostring(time2)).arg(time3).arg(fee).arg(speed);
	query.exec(sql1);
}

void DBFacade::updatecurtemp(int RoomID, float temp)
{
	QSqlQuery query(db);
	QString sql1 = QString("update RoomStat set cur_temp=%1 where RoomID=%2").arg(temp).arg(RoomID);
	query.exec(sql1);
}

void DBFacade::updatetartemp(int RoomID, float temp)
{
	QSqlQuery query(db);
	QString sql1 = QString("update RoomStat set tar_temp=%1 where RoomID=%2").arg(temp).arg(RoomID);
	query.exec(sql1);
}

void DBFacade::updatespeed(int RoomID, int speed)
{
	QSqlQuery query(db);
	QString sql1 = QString("update RoomStat set cur_speed=%1 where RoomID=%2").arg(speed).arg(RoomID);
	query.exec(sql1);
}

void DBFacade::updateFee(int RoomID, float fee)
{
	QSqlQuery query(db);
	QString sql1 = QString("update RoomStat set fee=%1 where RoomID=%2").arg(fee).arg(RoomID);
	query.exec(sql1);
}

void DBFacade::updateRoom(int RoomId, int state, float cur_temp, float dist_temp, int speed, float fee)
{
	QSqlQuery query(db);
	QString sql1 = QString("update RoomStat set stat =%1 , set cur_temp = % 2,set tar_temp = %3, set cur_speed = speed,set	fee = %5 where RoomID = % 6").arg(state)
		.arg(cur_temp).arg(dist_temp).arg(speed).arg(fee);
	query.exec(sql1);
}

void DBFacade::addscheduleCount(int RoomID)
{
	QSqlQuery query(db);
	QString sql1 = QString("update RoomStat set schedulecount= schedulecount+1 where RoomID=%2").arg(RoomID);
	query.exec(sql1);
}

float DBFacade::queryFee(int RoomID)
{
	QString queryfee = QString("select fee from RoomStat where RoomID=%1").arg(RoomID);
	float fee;
	QSqlQuery fee_query(db);
	fee_query.exec(queryfee);
	while (fee_query.next())
	{
		fee = fee_query.value(0).toFloat();
	}
	return fee;
}

time_t DBFacade::queryServiceTime(int RoomID)
{
	QSqlQuery query;
	QString sql1 = QString("select Service_time from RoomService where RoomID=%1 ").arg(RoomID);
	time_t totaltime=0;
	query.exec(sql1);
	while (query.next())
	{
		totaltime = totaltime + query.value(0).toInt();
	}
	return totaltime;
}

QVector<QString> DBFacade::queryInvoice(int RoomID)
{
	QSqlQuery query;
	float fee = queryFee(RoomID);
	tm* Tm= gettime();
	QString maketime = QString::fromLocal8Bit("打印时间");
	QString time= QString::fromLocal8Bit(tmtostring(Tm));
	QString makeRoom = QString::fromLocal8Bit("房间号");
	QString roomID = QString::fromLocal8Bit("%1").arg(RoomID);
	QString makeFee = QString::fromLocal8Bit("总费用");
	QString Fee = QString::fromLocal8Bit("%1").arg(fee);
	QVector<QString> invoice;
	invoice.push_back(maketime);
	invoice.push_back(time);
	invoice.push_back(makeRoom);
	invoice.push_back(roomID);
	invoice.push_back(makeFee);
	invoice.push_back(Fee);
	return invoice;
}

QVector<QString> DBFacade::queryRecption(int RoomID)
{
	QSqlQuery query;
	tm* Tm = gettime();
	QString timetitle = QString::fromLocal8Bit("打印时间");
	QString maketime = QString::fromLocal8Bit(tmtostring(Tm));
	QString Roomtitle = QString::fromLocal8Bit("房间号");
	QString id = QString::fromLocal8Bit("%1").arg(RoomID);
	QString FeeTitle = QString::fromLocal8Bit("总费用");
	QString totoalfee = QString::fromLocal8Bit("%1").arg(queryFee(RoomID));
	QString starttitle = QString::fromLocal8Bit("开始时间");
	QString endtitle = QString::fromLocal8Bit("结束时间");
	QString speedtitle = QString::fromLocal8Bit("风速");
	QString feetitle = QString::fromLocal8Bit("花费");
	QString highspeed = QString::fromLocal8Bit("高");
	QString midspeed = QString::fromLocal8Bit("中");
	QString lowspeed = QString::fromLocal8Bit("低");
	QVector<QString> reception;
	QString sql = QString( "select RoomID,Start_time, End_time,cur_speed,fee from RoomService where RoomID=%1").arg(RoomID);
	reception.push_back(timetitle);
	reception.push_back(maketime);
	reception.push_back("");
	reception.push_back(Roomtitle);
	reception.push_back(id);
	reception.push_back("");
	reception.push_back("");
	reception.push_back(FeeTitle);
	reception.push_back(totoalfee);
	reception.push_back("");
	reception.push_back("");
	reception.push_back("");
	reception.push_back("");
	reception.push_back("");
	reception.push_back("");
	reception.push_back(starttitle);
	reception.push_back(endtitle);
	reception.push_back(speedtitle);
	reception.push_back(feetitle);
	query.exec(sql);
	while (query.next())
	{
		reception.push_back(query.value(0).toString());
		reception.push_back(query.value(1).toString());
		if (query.value(2).toInt() == LOW_FANSPEED)
		{
			reception.push_back(lowspeed);
		}
		else if (query.value(2).toInt() == MID_FANSPEED)
		{
			reception.push_back(midspeed);
		}
		else {
			reception.push_back(highspeed);
		}
		reception.push_back(query.value(3).toString());
	}

	return reception;
}

QVector<RoomData> DBFacade::queryRoomData(RoomInfo roomlist)
{
	QSqlQuery query;
	QVector<RoomData> roomData;
	QVector<int>::iterator it;
	QString sql;
	for (it = roomlist.rooms.begin(); it != roomlist.rooms.end(); it++)
	{
		RoomData room;
		sql = QString("select count(opt) from RoomOpt where RoomID=%1 and opt=%2 or opt=%3").arg(*it).arg(OPEN).arg(CLOSE);
		query.exec(sql);
		while (query.next())
		{
			room.switch_n = query.value(0).toInt();
		}
		sql = QString("select count(opt) from RoomOpt where RoomID=%1 and opt=%2 ").arg(*it).arg(CHANGETEMP);
		query.exec(sql);
		while (query.next()) 
		{
			room.temp_n = query.value(0).toInt();
		}
		sql = QString("select count(opt) from RoomOpt where RoomID=%1 and opt=%2 ").arg(*it).arg(CHANGESPEED);
		query.exec(sql);
		while (query.next())
		{
			room.wind_n = query.value(0).toInt();
		}
		room.fee = queryFee(*it);
		room.data_key = *it;
		sql = QString("select schedulecount from RoomStat where RoomID = %1").arg(*it);
		query.exec(sql);
		while (query.next())
		{
			room.dispatch_n = query.value(0).toInt();
		}
		room.total_time = queryServiceTime(*it);
		roomData.push_back(room);
	}
	return roomData;
}


