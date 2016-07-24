#include "wLib.h"

#include "mysql_connection.h"
#include "mysql_driver.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#define  MYSQLHOST (char*)"localhost"
#define  MYSQLDB   (char*)"wemoney"

char	 MYSQLID[20];
char	 MYSQLPW[20];

int		GetMysqlIdPw();

int		query_display = 1;

struct _dbpool_	{
	sql::Driver		*driver = NULL;
	map<sql::Connection*, pair<time_t,time_t>> conMap;		// pair<time_t,time_t>의 first는 GetConnectTime이고 second는 NewConnectionTime(생성시간)이다..

	int		seq_num, con_cnt;

	_dbpool_()	{
		seq_num = con_cnt = 0;
		GetMysqlIdPw();								
		driver = sql::mysql::get_mysql_driver_instance();
	}

	sql::Connection* NewConnection()
	{
		sql::Connection* con = driver->connect(MYSQLHOST,MYSQLID,MYSQLPW);
		con->setSchema(MYSQLDB);

		return con;
	}


	sql::Connection* RefineConnection()
	{
		sql::Connection* con = driver->connect(MYSQLHOST,MYSQLID,MYSQLPW);
		con->setSchema(MYSQLDB);

		return con;
	}


	sql::Connection* GetConnection()
	{
		seq_num++; con_cnt++;

		time_t conn_time, create_time;
		for( auto it=conMap.begin(); it!=conMap.end(); )		{
			conn_time   = it->second.first;
			create_time = it->second.second;

			if(DIFF(g_Now,it->second.second) > MINUTE_SEC(20))	{		//  생성된지 20분이 지나면 없앤다..
				delete(it->first);
				conMap.erase(it++);
				continue;
			}

			if(conn_time == 0)	{								// 사용중인지 아닌지, 사용중이면 time stamp 찍혀있다..
				it->second.first = g_Now;
//				_LOGFILE("\t\t\tGetConnection(0x%02x) .. seq_num=%d, con_cnt=%d/%d\n\n", (BIGINT)it.first, seq_num, con_cnt, conMap.size());
				return it->first;
			}

			it++;
		}

		sql::Connection* con = NewConnection();
		conMap[con] = make_pair(g_Now,g_Now);

		_LOGFILE("\t\t\tNewConnection(0x%0x) .. seq_num=%d, con_cnt=%d/%d\n\n", (BIGINT)con, seq_num, con_cnt, conMap.size());

		return con;
	}

	void ReturnConnection(sql::Connection *con)
	{
		auto it = conMap.find(con);
		if(it != conMap.end())	{
			time_t* conn_time   = &(it->second.first);

			*conn_time = 0;						//  first: db연결시간..  second: con 생성시간..
			if(DIFF(g_Now,it->second.second) > MINUTE_SEC(20))	{		//  생성된지 20분이 지나면 없앤다..
				delete(con);
//				_LOGFILE("\t\t\tConnect(0x%02x) 제거..(10분경과.. %d개) .. ", (BIGINT)con, conMap.size()-1);
				conMap.erase(it);
			}
		}
		con_cnt--;
//		_LOGFILE("\t\t\tReturnConnection() .. con_cnt=%d\n\n", con_cnt);
	}
} DbPool;




int GetMysqlIdPw()
{
	char    buf[50];
	FILE* fp;
	if((fp=fopen("/home/www/pcall/cfg/mysql.info","r")) == NULL)    {
	    _LOGFILE("mysql.info open.. error, 아주심각.... !!!");
	    exit(-1);
	}

	for(;;) {
	    if(fgets(buf,sizeof(buf),fp)==NULL) break;
	    strtok(buf,"\r\n");
	    EraseChar(buf,' ');
	    if(!memcmp(buf,"id=",3))        strcpy(MYSQLID,&buf[3]);
	    if(!memcmp(buf,"pw=",3))        strcpy(MYSQLPW,&buf[3]);
	}
	if(strlen(MYSQLID)==0 || strlen(MYSQLPW)==0)  {
	    _LOGFILE("mysql.info id,pw.. error, 아주심각.... !!!");
	    exit(-1);
	}
}



sql::Connection* dbGetStatement(sql::Statement* &stmt, char* qry)
{

	sql::Connection* con = DbPool.GetConnection();

	if(query_display == 1)	_LOGFILE( "Query('%s')..", qry);
	stmt = con->createStatement();

	return con;
}


int dbErrorProcess(int errcode, const char* err, int display, char* buf=NULL)
{
	if(query_display == 0 && buf != NULL)	_LOGFILE( "Query('%s')..", buf);

	if(display)	_LOGFILE_NONUM( "\t\t오류(%d):%s\n", errcode, err);
	if(buf)		strcpy(buf,err);

	if(strstr(err,"Lost connection") || strstr(err,"gone away"))  {
		exit(0);
//		db_driver_eeconnect();
	}
	return -errcode;
}


int dbReturn(int ret, sql::Connection *con, sql::Statement* &stmt, sql::ResultSet* &res)
{
	if(res)	 { delete res;  res  = NULL;	}
	if(stmt) { delete stmt; stmt = NULL;	}

	DbPool.ReturnConnection(con);
	
	if(query_display == 1)	_LOGFILE_NONUM("\t\tret = %d\n", ret);
	query_display = 1;

	return ret;
}


int dbQuery(char* Query)
{
	sql::Connection *con;
	sql::Statement *stmt = NULL;
	sql::ResultSet  *res = NULL;

	int	ret=0;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);
		ret = 1;
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
		else {
			_LOGFILE_NONUM("DB Query OK(성공) QueryCount=%d !!!\n", DbPool.seq_num);
			ret = 1;
        }
	}

	return dbReturn(ret, con, stmt, res);
}




int dbInsertQuery(char* Query)
{
	sql::Connection *con;
	sql::Statement *stmt = NULL;
	sql::ResultSet  *res = NULL;

	int	ret=0;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);
		ret = 1;
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
		else {
			_LOGFILE_NONUM("DB Insert OK(성공) QueryCount=%d !!!\n", DbPool.seq_num);
			ret = 1;
        }
	}

	return dbReturn(ret, con, stmt, res);
}



int dbInsertQuery_Index(char* Query)
{
	sql::Connection *con;
	sql::Statement *stmt = NULL;
	sql::ResultSet  *res = NULL;

	int	ret  = 0;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() == 0) {
			res = stmt->executeQuery("select last_insert_id();");
			if(res->next())	 ret = res->getInt(1);	
		}
		else  {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

_LOGFILE( "last_insert_id() = %d\t\t", ret);
	return dbReturn(ret, con, stmt, res);
}


int dbInsert_Select(char* Query, char* fields)
{
	sql::Connection *con;
	sql::Statement *stmt = NULL;
	sql::ResultSet  *res = NULL;

	int	ret  = 0;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() == 0) {
			sprintf(Query,"select %s=(select last_insert_id())", fields); 
			res = stmt->executeQuery(Query);
			if(res->next())	 ret = res->getInt(1);	
		}
		else  {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

_LOGFILE( "last_insert_id() = %d\t\t", ret);
	return dbReturn(ret, con, stmt, res);
}


int dbUpdateQuery(char* Query)
{
	sql::Connection *con;
	sql::Statement *stmt = NULL;
	sql::ResultSet  *res = NULL;

	int	ret=0;
	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
		else {
			sprintf(Query,"DB Update OK(성공)!!!");
			_LOGFILE_NONUM("DB Update OK(성공) g_QueryCount=%d !!!\n", DbPool.seq_num);
        }
	}

	return dbReturn(ret, con, stmt, res);
}


int dbDeleteQuery(char* Query)
{
	int ret=0;

	sql::Connection *con;
	sql::Statement *stmt = NULL;
	sql::ResultSet  *res = NULL;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
		else {
			res = stmt->executeQuery("select row_count()");
			if(res->next())	 ret = res->getInt(1);	

			sprintf(Query,"DB Delete OK(성공)!!!");
			_LOGFILE_NONUM("DB Delete OK(성공) g_QueryCount=%d !!!\n", DbPool.seq_num);
		}
	}

	return dbReturn(ret, con, stmt, res);
}



// =================================================>  테이블 index만 취한다..
int dbSelect_Index(char* Query)
{
	sql::Connection *con;
	sql::Statement *stmt = NULL;
	sql::ResultSet  *res = NULL;

	int	ret=0;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		if(res->next())	ret = res->getInt(1);

		if(ret > 0) _LOGFILE_NONUM("DB Select OK(성공)!!!\n", DbPool.seq_num);
		else		_LOGFILE_NONUM("데이타가 없습니다.");
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

	return dbReturn(ret, con, stmt, res);
}



// =================================================>  std::string로 return 한다..
string dbSelect_String(char* Query)
{
	sql::Connection *con;
	sql::Statement *stmt = NULL;
	sql::ResultSet  *res = NULL;

	int		ret = 0;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		ret = res->rowsCount();

		if(res->next())	strcpy(Query,res->getString(1).c_str());

		_LOGFILE_NONUM("DB Select OK(Rows=%d) !!!\n", ret);
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

	dbReturn(ret, con, stmt, res);

	if(ret <= 0) return string("");
	_LOGFILE_NONUM("\t => retstr = %s\n\n", Query);
	return string(Query);
}



// =================================================>  Confirm..  있는지 확인용..
int dbSelect_Confirm(char* Query)
{
	sql::Connection *con;
	sql::Statement *stmt = NULL;
	sql::ResultSet  *res = NULL;

	int	ret=0;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		if(res->rowsCount() > 0)	ret = 1;
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

	return dbReturn(ret, con, stmt, res);
}


// -----------------------------------------------> SELECT * FROM table

int dbSelectTable(char* Query, vector<string>& vCols)
{
	sql::Connection *con;
	sql::Statement *stmt = NULL;
	sql::ResultSet  *res = NULL;
	sql::ResultSetMetaData *res_meta;

	int ret=0,cols;
	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		res_meta = res->getMetaData();
		cols = res_meta->getColumnCount();

		for(ret=0; res->next(); ret++)		{
			for (int j=1;j<=cols;j++)  vCols.push_back(res->getString(j));
		}
		if(ret == 0)  sprintf(Query,"데이타가 없습니다.");
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

	return dbReturn(ret, con, stmt, res);
}


// -----------------------------------------------> SELECT * FROM table
int dbSelect_TableRows(char* Query, short* pcol)
{
	sql::Connection *con;
	sql::Statement *stmt = NULL;
	sql::ResultSet  *res = NULL;
	sql::ResultSetMetaData *res_meta;

	int ret=0;
	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		res_meta = res->getMetaData();
		int rows = res->rowsCount();
		int cols = res_meta->getColumnCount();
		if(pcol != NULL)	*pcol = cols;

		char* pt = Query;
		pt += sprintf(pt,"%d|",rows);

		for(ret=0; res->next(); ret++)		{
			for (int j=1;j<=cols;j++)  pt += sprintf(pt,"%s|",res->getString(j).c_str());
		}
		if(ret == 0)  sprintf(Query,"데이타가 없습니다.");

	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

	return dbReturn(ret, con, stmt, res);
}


// -----------------------------------------------> SELECT * FROM table
// select 데이타가 적을때 쓴다.. 총량이 40960 이하..
// ----------------------------------------------->

int dbSelect_Table(char* Query, short* pcol)
{
	sql::Connection *con;
	sql::Statement *stmt = NULL;
	sql::ResultSet  *res = NULL;
	sql::ResultSetMetaData *res_meta;

	int ret=0,cols;
	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		res_meta = res->getMetaData();
		cols = res_meta->getColumnCount();
		if(pcol != NULL)	*pcol = cols;

		char* pt = Query;
		for(ret=0; res->next(); ret++)		{
			for (int j=1;j<=cols;j++)  pt += sprintf(pt,"%s|",res->getString(j).c_str());
		}
		if(ret == 0)  sprintf(Query,"데이타가 없습니다.");
	}

	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

	return dbReturn(ret, con, stmt, res);
}


// -----------------------------------------------> 
// select 데이타가 많을때 쓴다.. 총량이 40960 이상.. 또는 많을거라고 추정될때,,
// ----------------------------------------------->
int dbSelect_MultiLine(char* Query, map<int,string>& vList)
{
	sql::Connection *con;
	sql::Statement *stmt = NULL;
	sql::ResultSet  *res = NULL;
	sql::ResultSetMetaData *res_meta;

	int ret;
	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		res_meta = res->getMetaData();
		int cols = res_meta->getColumnCount();

		for(ret=0; res->next(); ret++)		{
			char* pt = Query;
			for (int j=1;j<=cols;j++)  pt += sprintf(pt,"%s|",res->getString(j).c_str());
			vList[ret] = string(Query);
		}
		if(ret == 0)  sprintf(Query,"데이타가 없습니다.");

	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

	return dbReturn(ret, con, stmt, res);
}


// -----------------------------------------------> SELECT * FROM table
int dbSelect_Table_Delimiter(char* Query, char delimiter, short* pcol)
{
	sql::Connection *con;
	sql::Statement *stmt = NULL;
	sql::ResultSet  *res = NULL;
	sql::ResultSetMetaData *res_meta;

	int ret=0,cols;
	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		res_meta = res->getMetaData();
		cols = res_meta->getColumnCount();
		if(pcol != NULL)	*pcol = cols;

		char* pt = Query;
		for(ret=0; res->next(); ret++)		{
			for (int j=1;j<=cols;j++)  pt += sprintf(pt,"%s%c",res->getString(j).c_str(),delimiter);
		}
		if(ret == 0)  strcpy(Query,"데이타가 없습니다.");
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

	return dbReturn(ret, con, stmt, res);
}


// ---------------------------------------------------------> Select 간단한 테이블인덱스와 별도 int를 쌍으로하는 map.. 가져온다..
int	 dbSelect_TablePair(pair<int,string> &vpair, char* Query)		//  필드는 2개이상..	하나일때는 second=1로세팅..
{
	sql::Connection *con;
	sql::Statement	*stmt = NULL;
	sql::ResultSet  *res = NULL;
	sql::ResultSetMetaData *res_meta;

	int ret=0;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		res_meta = res->getMetaData();
		int cols = res_meta->getColumnCount();

		if(res->next()) {
			vpair.first  = res->getInt(1);
			vpair.second = res->getString(2);
			ret = 1;
		}
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

	return dbReturn(ret, con, stmt, res);
}




// ---------------------------------------------------------> Select 간단한 테이블인덱스와 별도 int를 쌍으로하는 map.. 가져온다..
int	 dbSelect_TableVec(vector<int> &vList, char* Query)		//  필드는 2개이상..	하나일때는 second=1로세팅..
{
	sql::Connection *con;
	sql::Statement	*stmt = NULL;
	sql::ResultSet  *res = NULL;
	sql::ResultSetMetaData *res_meta;

	int ret=0;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		res_meta = res->getMetaData();
		int cols = res_meta->getColumnCount();

		for (ret=0; res->next(); ret++) vList.push_back(res->getInt(1));
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

	return dbReturn(ret, con, stmt, res);
}




int	 dbSelect_TableMap(map<int,int> &vList, char* Query)		//  필드는 2개이상..	하나일때는 second=1로세팅..
{
	sql::Connection *con;
	sql::Statement	*stmt = NULL;
	sql::ResultSet  *res = NULL;
	sql::ResultSetMetaData *res_meta;

	int ret=0;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		res_meta = res->getMetaData();
		int cols = res_meta->getColumnCount();

		for (ret=0; res->next(); ret++) {

			int idx  = res->getInt(1);

			if(cols == 1) vList[idx] = 1;
			else		  vList[idx] = res->getInt(2);
		}
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

	return dbReturn(ret, con, stmt, res);
}




// ---------------------------------------------------------> Select 간단한 테이블인덱스와 별도 int를 쌍으로하는 map.. 가져온다..
int	 dbSelect_TableMap(multimap<int,int> &vList, char* Query)		//  필드는 2개이상..	하나일때는 second=1로세팅..
{
	sql::Connection *con;
	sql::Statement	*stmt = NULL;
	sql::ResultSet  *res = NULL;
	sql::ResultSetMetaData *res_meta;

	int ret=0;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		res_meta = res->getMetaData();
		int cols = res_meta->getColumnCount();

		for (ret=0; res->next(); ret++)
		{
			int idx  = res->getInt(1);
			int idx2 = res->getInt(2);
			vList.insert(pair<int,int>(idx,idx2));
		}
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

	return dbReturn(ret, con, stmt, res);
}




// ---------------------------------------------------------> Select 간단한 테이블인덱스와 별도 int를 쌍으로하는 map.. 가져온다..
int	 dbSelect_TableMap(map<int,string> &vList, char* Query)		//  필드는 2개이상..	하나일때는 second=1로세팅..
{
	sql::Connection *con;
	sql::Statement	*stmt = NULL;
	sql::ResultSet  *res = NULL;
	sql::ResultSetMetaData *res_meta;

	int ret=0;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		res_meta = res->getMetaData();
		int cols = res_meta->getColumnCount();

		for (ret=0; res->next(); ret++) {

			int idx  = res->getInt(1);
			if(vList.find(idx) != vList.end()) {
				if(vList[idx].length() > 0) continue;
			}

			if(cols == 1) vList[idx] = "";
			else		  vList[idx] = res->getString(2);
		}
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

	return dbReturn(ret, con, stmt, res);
}






// ---------------------------------------------------------> Select 간단한 테이블인덱스와 별도 int를 쌍으로하는 map.. 가져온다..
int	 dbSelect_TableMap(map<string,int> &vList, char* Query)		//  필드는 2개이상..	하나일때는 second=1로세팅..
{
	sql::Connection *con;
	sql::Statement	*stmt = NULL;
	sql::ResultSet  *res = NULL;
	sql::ResultSetMetaData *res_meta;

	int ret=0;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		res_meta = res->getMetaData();
		int cols = res_meta->getColumnCount();

		for (ret=0; res->next(); ret++) {

			string idx  = res->getString(1);
			if(vList.find(idx) != vList.end()) {
				if(vList[idx] > 0) continue;
			}

			if(cols == 1) vList[idx] = 0;
			else		  vList[idx] = res->getInt(2);
		}
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

	return dbReturn(ret, con, stmt, res);
}





// ---------------------------------------------------------> Select 간단한 테이블인덱스와 별도 int를 쌍으로하는 map.. 가져온다..
int	 dbSelect_TableMap(map<int,map<int,int>> &vList, char* Query)		//  필드는 2개이상..	하나일때는 second=1로세팅..
{
	sql::Connection *con;
	sql::Statement	*stmt = NULL;
	sql::ResultSet  *res = NULL;
	sql::ResultSetMetaData *res_meta;

	int ret=0;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		res_meta = res->getMetaData();
		int cols = res_meta->getColumnCount();

		for (ret=0; res->next(); ret++) {

			int idx   = res->getInt(1);
			int val2  = res->getInt(2);
			int val3  = res->getInt(3);

			auto it = vList.find(idx);
			if(it == vList.end()) {
				map<int,int> m;
				m[val2] = val3;
				vList[idx] = m;
			}
			else	{
				(it->second)[val2] = val3;;
			}
		}
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

	return dbReturn(ret, con, stmt, res);
}



// ---------------------------------------------------------> Select 간단한 테이블인덱스와 별도 int를 쌍으로하는 map.. 가져온다..
int	 dbSelect_TableMap(map<int,pair<int,int>> &vList, char* Query)		//  필드는 2개이상..	하나일때는 second=1로세팅..
{
	sql::Connection *con;
	sql::Statement	*stmt = NULL;
	sql::ResultSet  *res = NULL;
	sql::ResultSetMetaData *res_meta;

	int ret=0;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		res_meta = res->getMetaData();
		int cols = res_meta->getColumnCount();

		for (ret=0; res->next(); ret++) {

			int idx  = res->getInt(1);
			if(vList.find(idx) != vList.end()) continue;

			int ndat  = res->getInt(2);
			vList[idx] = make_pair(ndat,res->getInt(3));
		}
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

	return dbReturn(ret, con, stmt, res);
}



// ---------------------------------------------------------> Select 간단한 테이블인덱스와 별도 int를 쌍으로하는 map.. 가져온다..
int	 dbSelect_TableMap(map<int,pair<int,string>> &vList, char* Query)		//  필드는 2개이상..	하나일때는 second=1로세팅..
{
	sql::Connection *con;
	sql::Statement	*stmt = NULL;
	sql::ResultSet  *res = NULL;
	sql::ResultSetMetaData *res_meta;

	int ret=0;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		res_meta = res->getMetaData();
		int cols = res_meta->getColumnCount();

		for (ret=0; res->next(); ret++) {

			int idx  = res->getInt(1);
			if(vList.find(idx) != vList.end()) continue;

			int ndat  = res->getInt(2);
			vList[idx] = make_pair(ndat,res->getString(3));
		}
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

	return dbReturn(ret, con, stmt, res);
}



// ---------------------------------------------------------> Select 간단한 테이블인덱스와 별도 int를 쌍으로하는 map.. 가져온다..
int	 dbSelect_TableMap(map<int,pair<string,string>> &vList, char* Query)		//  필드는 2개이상..	하나일때는 second=1로세팅..
{
	sql::Connection *con;
	sql::Statement	*stmt = NULL;
	sql::ResultSet  *res = NULL;
	sql::ResultSetMetaData *res_meta;

	int ret=0;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		res_meta = res->getMetaData();
		int cols = res_meta->getColumnCount();

		for (ret=0; res->next(); ret++) {

			int idx  = res->getInt(1);
			if(vList.find(idx) != vList.end()) continue;

			vList[idx] = make_pair(res->getString(2),res->getString(3));
		}
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

	return dbReturn(ret, con, stmt, res);
}


// ---------------------------------------------------------> 특히 쿠폰테이블일 경우 유용..  키는 string, 정보는 int,,
int	 dbSelect_TableMap(map<string,pair<int,int>> &vList, char* Query)	
{
	sql::Connection *con;
	sql::Statement	*stmt = NULL;
	sql::ResultSet  *res = NULL;
	sql::ResultSetMetaData *res_meta;

	int ret=0;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		res_meta = res->getMetaData();
		int cols = res_meta->getColumnCount();

		for (ret=0; res->next(); ret++) {

			string str  = res->getString(1);
			if(vList.find(str) != vList.end()) continue;

			vList[str] = make_pair(res->getInt(2),res->getInt(3));
		}
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

	return dbReturn(ret, con, stmt, res);
}



// -----------------------------------------------> SELECT * FROM table

int dbSelect_Position(Z_POS* pPos)
{
	char Query[2048];
	sprintf(Query,"select * from t_user where user_id = '%s';", pPos->user_id.c_str());
	
	sql::Connection *con;
	sql::Statement *stmt = NULL;
	sql::ResultSet  *res = NULL;

	int	ret=0;
	string str;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		if(res->next()) {
			pPos->user_idx   = res->getInt("user_id");
			pPos->win_count  = res->getInt("win_count");
			pPos->lose_count = res->getInt("lose_count");

			pPos->user_id	= res->getString("user_id");
			pPos->user_name	= res->getString("user_name");
			pPos->user_img	= res->getString("user_img");

			pPos->create_date	= res->getString("create_date");
			pPos->app_start_time	= res->getString("app_start_time");
			pPos->app_end_time	= res->getString("app_end_time");
			pPos->last_play_time	= res->getString("last_play_time");

			ret = 1;
		}
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

	return dbReturn(ret, con, stmt, res);
}


		
