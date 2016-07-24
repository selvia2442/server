/*
 * Server.cpp
 * Create By	: Lee Byung No
 * Create Date  : 2014-08-10
 * Update By	:
 * Udate Date   :
 * Update Reason :
 */


#include <dirent.h>
#include <arpa/inet.h>
#include <mqueue.h> 


#include "wLib.h"
#include "_Mysql.h"







void udp_recv_process(int udpsock);
void udp_send_process(Z_POS* pPos, char* sbuf);





char	 	ProgName[32];
int		g_DirectExecFlag;		//  직접실행여부..
time_t		g_ProgStartTime;
					// ARS,SMS,LMS 과금정보...
int	BindUdpPort = 19797;

map<string,Z_POS*> g_PosMap;

int udp_serv_sock;

fd_set  reads;

int		fd_MaxSock;

int main(int argc, char **argv)
{
	strcpy(LogDir,(char*)"/home/log/quiz");

	g_ProgStartTime = time(0);
	if(!memcmp(argv[0],"./",2)) g_DirectExecFlag = 1;		//  직접실행여부..
	strcpy(ProgName,strrchr(argv[0],'/')+1);

	g_Now = time(0);


	struct sockaddr_in serv_addr;
	int nSockOpt;

	udp_serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(udp_serv_sock == -1) {  _LOGFILE("socket() error"); exit(-1); }

	bzero((char*)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(BindUdpPort);

	nSockOpt = 1;
	setsockopt(udp_serv_sock,SOL_SOCKET,SO_REUSEADDR,&nSockOpt,sizeof(nSockOpt));

	if(bind(udp_serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1) { _LOGFILE("udp bind() error"); exit(-1); }

	_LOGFILE("bind port =%d\n\n", BindUdpPort);


	FD_ZERO(&reads);
	FD_SET(udp_serv_sock, &reads);		// waiting = 2;
	fd_MaxSock=udp_serv_sock;

	int ret;
	int fd, recv_len;
	int clntsock, cli_len;
	struct sockaddr_in cli_addr;
	struct timeval timeout;
	fd_set temps;

	
	for(int x=0; ; x++)
	{
		g_Now = time(0);

		temps=reads;
		timeout.tv_sec  = 2;
		timeout.tv_usec = 0;

		if((ret=select(fd_MaxSock+1,&temps,0,0,&timeout))==-1)  {
			_LOGFILE("select() error");
			exit(-1);
		}

		if(ret == 0)  {
			_LOGFILE("select(%d)\tudpsock=%d\tfd_MaxSock=%d  고객접속:%d \n", ret, udp_serv_sock, fd_MaxSock, g_PosMap.size());
			continue;
		}

		for(fd=1; fd<=fd_MaxSock; fd++)
		{
			if(!FD_ISSET(fd, &temps)) continue;

			_LOGFILE("fdset(%d).. \n", fd);

			udp_recv_process(fd);
		}
	}
}


//==================================================================================================================>  UDP	수신...
//==================================================================================================================>  UDP	수신...
//==================================================================================================================>  UDP	수신...
//==================================================================================================================>  UDP	수신...
//==================================================================================================================>  UDP	수신...
//==================================================================================================================>  UDP	수신...
//==================================================================================================================>  UDP	수신...
//==================================================================================================================>  UDP	수신...
//==================================================================================================================>  UDP	수신...
//==================================================================================================================>  UDP	수신...
//==================================================================================================================>  UDP	수신...
//==================================================================================================================>  UDP	수신...

void udp_recv_process(int udpsock)
{
	char	ubuf[2500], *pn = ubuf;
	char	sbuf[2500];
	struct	sockaddr_in clnt_addr;
	UINT	clilen = sizeof(clnt_addr);
	
	int	rlen = recvfrom(udpsock,(void *)ubuf,2048,0,(struct sockaddr *)&clnt_addr, &clilen);
	if(rlen <= 0)  return;
	ubuf[rlen] = 0;

_LOGFILE("\n <++++++%s: udp_recv_process(%dbyte): \"%s\"\n",strHHMMSS(g_Now), rlen, ubuf);

	string srcId   = GetToken(pn,SEPARATOR,pn);
	string dstId   = GetToken(pn,SEPARATOR,pn);

strcpy(sbuf,pn);				//  보내기위해 저장..

	string command = GetToken(pn,SEPARATOR,pn);
	string key     = GetToken(pn,SEPARATOR,pn);
	string data    = GetToken(pn,SEPARATOR,pn);


	_LOGFILE_NONUM("고객수: %d\t====> %s\n",g_PosMap.size(),ubuf);


	Z_POS *pPos, *pDstPos;

	

	if((pPos=GetMember(g_PosMap,srcId)) == NULL) {
		pPos = new Z_POS;

		pPos->udpsock = udpsock;

		char	peer_ip[20];
		inet_ntop(AF_INET,&clnt_addr.sin_addr,peer_ip,sizeof(peer_ip));
		pPos->peer_ip = peer_ip;

		pPos->peerport   = ntohs(clnt_addr.sin_port);
		memcpy((char*)&pPos->clnt_addr,(char*)&clnt_addr, clilen);

		pPos->user_id = srcId;
	}

	g_PosMap[pPos->user_id] = pPos;
	WriteUser(pPos->user_id, pPos->clnt_addr);



_LOGFILE("\n srcId='%s'\n",srcId.c_str());
_LOGFILE("\n dstId='%s'\n",dstId.c_str());
_LOGFILE("\n command='%s'\n",command.c_str());

	if(dstId.length() == 0 || dstId == "0")	{
		udp_send_process(pPos, sbuf);	sleep(1);
		udp_send_process(pPos, sbuf);
		return;
	}	
		
	if((pDstPos=GetMember(g_PosMap,dstId)) == NULL) { pDstPos = new Z_POS; }

//	dbSelect_Position(pPos);
//	dbSelect_Position(pDstPos);
	
	pDstPos = GetMember(g_PosMap,dstId);



//	pPos->LOGGING();



	if(command == "ROOM")	{ }
	if(command == "READY")	{ }
	if(command == "GAME")	{ }


	udp_send_process(pDstPos, sbuf);
}




//======================================================>  UDP	전송...
//======================================================>  UDP	전송...
//======================================================>  UDP	전송...
//======================================================>  UDP	전송...
void udp_send_process(Z_POS* pPos, char* sbuf)
{
	int slen = strlen(sbuf)+1;

_LOGFILE("++++++> udp_send_process(%dbyte): \"%s\"\n\n", slen, sbuf);

	UINT clilen = sizeof(pPos->clnt_addr);
	sendto(pPos->udpsock,sbuf,slen,0,(struct sockaddr *)&pPos->clnt_addr, clilen);		// 끝NULL(+1)..
//	pPos->LOGGING();
}



