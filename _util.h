
#ifndef	_UTIL
#define _UTIL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>

using namespace std;

//#define		umap	unordered_map		//  unordered_map 이 너무 길어서..
#define		umap	map		//  unordered_map 이 너무 길어서..

#include	"_DPoint.h"			// DPoint	struct ..    위경도를 CPoint처럼 사용한다..

// ===================================================================>  공유메모리관련...
// ===================================================================>  공유메모리관련...
// ===================================================================>  공유메모리관련...
// ===================================================================>  공유메모리관련...
#define		SHM_KEY    9999
#define		SHM_SIZE   100000

#define		SMS			'S'
#define		LMS			'L'
#define		ARS			'A'
#define		ARSHP		'B'

						//  공유메모리 서버의 INDEX..
#define		WEK_SERVER			0
#define		TTS_SERVER			1
#define		ADDRESS_SERVER		2
#define		COUPON_SERVER		3
#define		WEFTPD_SERVER		4
#define		WEUFTPD_SERVER		5
#define		OBSERVER_SERVER		6


#define		SHMIDX_ALIVE		    0		//      0 ~   999
#define		SHMIDX_TTS			 1000		//   1000 ~ 10000
#define		SHMIDX_MYSQL		10000		//  10000 ~ 30000
#define		SHMIDX_SMSARS		30000		//  30000 ~ 40000
#define		SHMIDX_LATLNG		40000		//  40000 ~ 50000		//  고객의 위경도요청-응답 route,,  coupon->weK 
#define		SHMIDX_ADDRESS		90000		//  90000 ~100000

#define		FREE   1

#define		SHM_CNT_LATLNG	   20			// 위경도요청시 shm 갯수,, 20개에서 늘어날수 있다..

struct  SHM_STRUCT    {
    char    shmAlive    [1000];     		//     0 ~   1000개
    char    shmTts      [9000];				//  1000 ~ 	 9000개
    char    shmMysql1  [20000]; 			// 10000 ~   2만개
    char    shmSmsArs  [30000];				// 30000 ~   1만개
    char    shmSmsLatLng [40000];			// 40000 ~   1만개
    char    shmAddress [90000];				// 90000 ~   1만개
};


struct  SHM_ALIVE {
    pid_t   pid;
    time_t  ptm;
};


struct SHM_LATLNG	{
	int		no;
	int		sock;
	int		size;
	int		status;			// 0 : none..  1: in,  2: read, 3:write  4: dong,,
	char	php[16];
	DPoint	dp;
	time_t	systime;		// 0 : none..  1: in,  2: read, 3:write  4: dong,,
	int		conn_flag;		// 현재 접속여부..

	int		ret;


	void Reset() { memset(this,0,sizeof(SHM_LATLNG)); }
};

// ===================================================================>  공유메모리관련...
// ===================================================================>  공유메모리관련...
// ===================================================================>  공유메모리관련...
// ===================================================================>  공유메모리관련...


// ===================================================================>  일반 유용한 define..
// ===================================================================>  일반 유용한 define..
// ===================================================================>  일반 유용한 define..

#define	 BUFF_SIZE		2048
#define	 MAX_DATA_SIZE	40960
#define	 MAX_SEND_LEN	(MAX_DATA_SIZE-BUFF_SIZE)

typedef unsigned char	BYTE;
typedef unsigned int	UINT;
typedef int  			BOOL;
typedef long long  		BIGINT;

#define	 SEPARATOR		'|'

#define	 Random(n)		  (rand()%(n))
#define	 RandomRange(a,b) ((rand()%((b)-(a)+1))+(a))

#define	 IS_DIGIT(x)  ((x>='0') && (x<='9'))
#define	 IS_ALPHA(x)  (((x>='A') && (x<='Z')) || ((x>='a') && (x<='z')))
#define	 IS_ALNUM(x)  (((x>='A') && (x<='Z')) || ((x>='a') && (x<='z')) || ((x>='0') && (x<='9')))
#define	 IS_UPPER(x)  ((x>='A') && (x<='Z'))
#define	 IS_LOWER(x)  ((x>='a') && (x<='z'))

#define	 TO_LOWER(x)  (IS_UPPER(x)==1 ? ((x)+32):(x))
#define	 TO_UPPER(x)  (IS_LOWER(x)==1 ? ((x)-32):(x))

#define  ABS(x,y)	 	 ((x)>=(y) ? ((x)-(y)) : ((y)-(x)))
#define  DIFF(x,y)	 	 ((x)-(y)>0?(x)-(y):(y)-(x))

#define	 MINUTE_SEC(x)	((x)*60)
#define	 HOUR_SEC(x)	((x)*3600)
#define	 DAY_SEC(x)		((x)*86400)
#define	 MONTH_SEC(x)	((x)*86400*30)
#define	 YEAR_SEC(x)	((x)*86400*365)

#define	 EUK_14			1400000000			//  14억..

// ===================================================================>  일반 유용한 define..
// ===================================================================>  일반 유용한 define..
// ===================================================================>  일반 유용한 define..


// ===================================================================>  _util.cpp  관련 함수..
extern	char	NULLBUF[1];
extern	char	*pDummy;				// 무의미하게 포인터 써야할때,,
extern	char	ProgName[32];
extern	int		g_DirectExecFlag;		// 직접실행여부..
extern	string	myHost;				// Host가 wemoney인지 pcall인지..

extern  time_t	g_Now;
extern	char	LogDir[80];



extern	int		HAN1JA_LEN;
extern	int		HAN2JA_LEN;
extern	int		HAN3JA_LEN;
extern	int		HAN4JA_LEN;
extern	int		HAN5JA_LEN;
extern	int		HAN7JA_LEN;

	
										// util.cpp
extern	void	nulcpy(char* p, char* q, int n);
extern	char*	itoa(int n);
extern	void	GetHostName();
extern	int		GetFileSize(char* fpath);
extern	char*	GotoString(char* buf, const char* tag);
extern	char*	GetToken(char* p, char dil, char* &pn=pDummy);
extern	char*	GetReverseToken(char* p, char dil);
extern	char*	GetToken_NotNull(char* p, char dil, char* &pn);
extern	char*	StrToken(char* src, const char* dils, char* &pn=pDummy);
extern	void	strPrintf(string& buf, const char *format, ...);

extern	char	*Trim(char *str);
extern	char	*leftTrim(char *str);
extern	char	*rightTrim(char *str);
extern	void	ReduceSpace(char *str);
extern	int		IsLastWord(char* buf, const char* word);	// 마지막단어가 word인지..  동작구,"구"
extern	char*	EraseChar (char* buf, const char del);
extern	void	EraseChars(char* buf, const char* del);
extern	char*	EraseChar (string buf, const char del);
extern	void	EraseChars(string buf, const char* del);
extern	void	EraseDigit(char* buf);
extern	void	EraseWord(char* buf, char* delword);
extern	void	ReplaceChar(char* src, const char from, const char to);
extern	void	ReplaceString(string& source, const char* from, const char* to);

extern	void	EraseLastWord(char* buf, char* delword);
extern	void	TruncateLastDong(char* buf, char* delword);

extern  int		getPkey();				// unique한 숫자 뽑아낸다..
extern	char*	getPkey(int k);			//  변환할 숫자  ,,  자릿수는 10자리로  1,3,5,7,9에 들어간다...
extern	int		getNum_byPkey(char* pkey);		//  pKey의 역변환...
extern  char*	getext(char* src, char dot);
extern	void	Space_To_Per20(char* url);
extern	int		strlen_mb(string& s);
extern	string	get_utf8(string& s, int ncnt);

extern	void	ExtractBunji(char* addr2, char* bunji, char* addr3=NULL);  //  addr2를 번지와 나머지로 나눈다..
extern	char*	en_crypt(char* src, char* dst);
extern	char*	de_crypt(char* src, char* dst);

extern	char*	ChangeDateTime(char* tim);
extern  int		GetTimeDuration(char* fromtm, char* totm);
extern	time_t  GetSystemTime(string& strtim);
extern	time_t  GetSystemTime(char* strtim);
extern	time_t	GetSystemTime2(char* strtim);
extern	char*	getPayWay(string& payway);

extern	void	WGS2TM(double lat, double lng, int& tm_lat, int& tm_lng);
extern	void	TM2WGS(int tm_lat, int tm_lng, double& lat, double& lng);

extern	int		Get_Distance(double lat1,double lng1,double lat2,double lng2);
extern	int		GetDistDir(int lat1, int long1, int lat2, int long2, int &dir);

extern	char*	strLower(char* src);
extern	char*	strUpper(char* src);
extern	int		IsAllUpper(char* src);
extern	int		IsAllLower(char* src);

extern  char*	strTime(struct tm* ptm);
extern	char*	strTime(time_t tt);
extern	char*	strDate(time_t tt);
extern	char*	strDate_NoSeparator(time_t tt);
extern	char*	strDateTime(time_t tt);
extern	char*	strHHMMSS(time_t tt);
extern	int		between_nowtm(string& tmstr, time_t nowtm);		// 현재시간(nowtm)이 tmstr내인지 아닌지.. 검사..
extern	char*	tel_dash(string tel);
extern	int		IsDashNum(char* src);		// 전화번호처럼 '-'(대쉬)포함 숫자로 이루어졌는지 여부..

extern	int		GetYear(time_t tt);
extern	int		GetMonth(time_t tt);
extern	int		GetDay (time_t tt);
extern	int		GetHour(time_t tt);
extern	int		GetMinute(time_t tt);
extern	int		GetWeek(time_t tt);

extern	int		Get1AnyInDir(char *dirname, char* any);
extern	int		Get1File(char *dirname, char* fname);
extern	int		Get1Dir(char *dirname, char* dir);
extern	int		GetFileList_subdir(map<string,int> &vList, char* dir, char* ext);		//  하위폴더까지 검색..		ext는 ".png"처럼 '.'까지 포함해서 넘겨준다..

extern	void	AddrMapFileRoad();
extern	void	CoordiMapFileRoad();
extern	void	ReqRepFileRoad();


extern	int		GetInt_Jason   (char* buf, const char* tag, char end, char* &pn);							// tag와 end사이의 값.
extern	double	GetDouble_Jason(char* buf, const char* tag, char end, char* &pn);							// tag와 end사이의 값.
extern	char*	GetPointer_Jason(char* buf, const char* tag, char end, char* &pn);							// tag와 end사이의 값.
extern	string	GetString_Jason(char* buf, const char* tag, char end, char* &pn);							// tag와 end사이의 값.

extern	char*	GetBlock(char* data, const char* tag, const char stb, const char enb, char* &pn);
extern	void	block_write(char*data, char* dir, char* fname);
extern	string	GetType_PlaceId(char* buf, const char* place_id, const char* tag, char end, char* &pn);		// palce_id를 거쳐 tag와 end사이의 값.
		
extern	char*	Remove_Do(char* addr);
extern	int		IsLastDongRoGa(char* buf);			// 마지막단어가 1가, 2가,   전국의 동에 해당하는 '가'인가?  영등포동3가, 을지로2가...
extern	char	GetAddrType(char* addr);						// 신주소, 구주소 구별,, 'r', 'a', 잘못된 주소면 '0'
extern	int		Separate_Addr1(char* addr, char word[][40]);		//  번지주소에서 도,시군,구,읍면동,리,번지로 6단계로 나눈다.
extern	int		Is_JibunAddress(string address);

extern  void	_LOGFILE(char *format);
extern  void	_LOGFILE(const char *format, ...);
extern  void    _LOGFILE_NONUM(const char *format, ...);

#endif
