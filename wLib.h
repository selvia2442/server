#ifndef	 QAPP
#define	 QAPP

#define	 FREE	1


#include "_util.h"

#include <netinet/in.h>

struct Z_POS
{
	int		user_idx;
	string		user_id;
	string		user_name;
	string		user_img;

	int		win_count;
	int		lose_count;

	string		create_date;
	string		app_start_time;
	string		app_end_time;
	string		last_play_time;



	int		udpsock;
	string		peer_ip;
	UINT		peerport;
	struct 		sockaddr_in clnt_addr;


	void LOGGING()
	{
		_LOGFILE_NONUM("user_idx	= %d\n",  user_idx);
		_LOGFILE_NONUM("user_id		= '%s'\n",  user_id.c_str());
		_LOGFILE_NONUM("user_name	= '%s'\n",  user_name.c_str());
		_LOGFILE_NONUM("user_img	= '%s'\n",  user_img.c_str());
		_LOGFILE_NONUM("win_count	= %d\n",  win_count);
		_LOGFILE_NONUM("lose_count	= %d\n",  lose_count);
	}
};





extern  Z_POS*  	GetMember(map<string,Z_POS*>& mList, string pos_hp);
extern	int		GetMember(map<string,int>& mList, string pos_hp);

extern	int		GetMember(vector<string>& mList, string idx);
extern	int		GetMember(vector<int>& mList, int idx);

extern	int		EraseMember(map<int,int>& mList, int idx);
extern  void 		EraseMember(map<string,Z_POS*>& mList, string pos_hp,int free=0);


void ReadUser(string user_id, struct  sockaddr_in& clnt_addr);
void WriteUser(string user_id, struct  sockaddr_in clnt_addr);

#endif
