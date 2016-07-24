#ifndef	PACKET_DEFINE
#define PACKET_DEFINE

										// ★prog_code는 서비스마다 고유 중요
#define PROG_CODE		"chunancode"		// t_abc_config에서 서비스 지역을 설정
#define PROG_VERSION	"chunanVersion"		// t_abc_config에서 서비스 지역을 설정

#include "../_util.h"
#include "../_protocol.h"

#define		SMS		'S'
#define		LMS		'L'
#define		ARS		'A'
#define		ARSHP	'B'


#define	 CHATCONTROL	 1
#define  STATUSCONTROL	 2
#define	 BOTHCONTROL	 3


struct ABC_CONFIG  {
	int		BindTcpPort;
	int		BindUdpPort;

	int		dbHandKey;

	int		LiveCheckPiriod;
	int		JoinPrice;

	string	FromTel;		//  dbOrderSet  program path..  "prog_dbOrderSetint		dbOrderProgKey;
	string	Master;
};





#define CONNECTING_USER  	2
#define USER_OR_PSWD_ERROR	1
#define	LOGIN_OK		 	0
#define USER_NOT_EXIST		-1
#define PSWD_ERROR			-2



#define CX_TMCELL		 60000		//  TM128좌표에서  TM_LNG 폭,, 경도 셀폭. 
#define GET_CELL_NO(y,x) ((y)*6000 + ((x)-100000)/10)	// TM 좌표에서 10분의 1 축적

#define WATCH_CONNECT     100

#define CONNECT_YET_STATUS   -1
#define ONLY_CONNECT_STATUS   0
#define LOGIN_SUCCESS_STATUS  1


#define OK   1
#define FAIL 0

#define	ON	 'o'
#define OFF  'f'

#define ALL_IDX   0 	//  idx가 0인경우는 전체


#endif // PDU_DEFINE end


