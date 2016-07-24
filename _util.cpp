#include "_util.h"
#include <sys/stat.h>
#include <stdarg.h>

#define M_IDX_PLUS(a)	(a=(a+88)<BUFF_SIZE?(a+88):88)

char	_buff_[BUFF_SIZE+500];
int		_buff_idx = 0;			//  일시버퍼를 사용할때 쫑나지 않게 88~BUFF_SIZE 사이의 인덱싱..  특히 strDateTime,,

char	NULLBUF[1] = { 0 };		// 값이 변하지 않게 조심..  length == 0..
char	*pDummy;				// 무의미하게 포인터 써야할때,,
string	myHost;					// Host가 wemoney인지 pcall인지..
time_t	g_Now = time(0);

char	cryptbuf[63] = "ABCDEFGHIJKLMNOPQRSTUVEXYZ123456789abcdefghijklmnopqrstuvwxyz0";		//  입력 숫자, 출력 10자리 문자스트링..

int		HAN1JA_LEN	= strlen("가");
int		HAN2JA_LEN	= HAN1JA_LEN*2;
int		HAN3JA_LEN	= HAN1JA_LEN*3;
int		HAN4JA_LEN	= HAN1JA_LEN*4;
int		HAN5JA_LEN	= HAN1JA_LEN*5;
int		HAN7JA_LEN	= HAN1JA_LEN*7;



//------------------------------------------------------------------------------------>  유틸리티 function...

void nulcpy(char* p, char* q, int n)
{
	memcpy(p,q,n);
	p[n] = 0;
}


void GetHostName()
{
	gethostname(_buff_,sizeof(myHost));
	if(!strcmp(_buff_,"nlc15401"))		myHost = "pcall";		// wemoney, 개발장비
	else								myHost = "wemoney";		// pcall  , 서비스장비
}


int	GetFileSize(char* fpath)
{
	FILE* fp;
	int	size = 0;
	if((fp=fopen(fpath,"rb")) != NULL)  {
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		fclose(fp);
	}

	return size;
}


char* itoa(int	n)
{
	char *result = &_buff_[M_IDX_PLUS(_buff_idx)];
	sprintf(result,"%d",n);
	return result;
}


char* GotoString(char* buf, const char* tag)			//  match가 없으면 buf 끝으로 간다..
{
	char* p;
    if((p=strstr(buf,tag)) != NULL)		return	(p + strlen(tag));
	return NULL;
}


char* GetToken(char* p, char dil, char* &pn)
{
	if(pn=strchr(p,dil)) {
		*pn++ = 0;
		if(dil==' ') while(*pn==' ') *pn++ = 0;
	}
	else				pn = p + strlen(p); 

	return p;
}


char* GetReverseToken(char* p, char dil)
{
	char* str = p+strlen(p);
	if(p=strrchr(p,dil)) {
		str = p+1;
		*p-- = 0;
		if(dil==' ') while(*p==' ') *p-- = 0;
	}

	return str;
}



char* GetToken_NotNull(char* p, char dil, char* &pn)
{
	char *result = &_buff_[M_IDX_PLUS(_buff_idx)];
	*result = 0;

	if(pn=strchr(p,dil)) {
		nulcpy(result,p,pn-p);
		pn++;
		if(dil==' ') while(*pn==' ') *pn++;
	}
	else				{
		strcpy(result,p);
		pn = p + strlen(p); 
	}

	return result;
}



char* StrToken(char* src, const char* dils, char* &pn)
{
	char *p = src;
	char *st = src;
	for(;*p; p++)		{
		if(strchr(dils,*p)) {
			*p = 0;
			if(p == st) {	st++;  continue;	}
			p++;
			break;
		}
	}

	pn = p;
	return st;
}


void strPrintf(string& buf, const char *format, ...)
{
	char* out = &_buff_[M_IDX_PLUS(_buff_idx)];

	va_list args;
	va_start(args, format);
		vsprintf(out, format, args);
	va_end(args);
	
	buf = out;
}


char *Trim(char *str)
{
	char* ptr;
	for(ptr=str; *ptr; ptr++)
	{
		if (*ptr != ' ' && *ptr != '\t') break;
	} 
	if(ptr != str)  memmove(str,ptr,strlen(ptr)+1);

	for(ptr=str+strlen(str); ptr != str; ptr--)
	{
		if (*ptr != ' ' && *ptr != '\t') break;
		else	*ptr = 0;
	} 

	return str;
} 

char *leftTrim(char *str)
{
	char* ptr;
	for(ptr=str; *ptr; ptr++)
	{
		if (*ptr != ' ' && *ptr != '\t') break;
	} 
	if(ptr != str)  memmove(str,ptr,strlen(ptr)+1);

	return str;
} 

char *rightTrim(char *str)
{
	for(char *ptr=str+strlen(str); ptr != str; ptr--)
	{
		if (*ptr != ' ' && *ptr != '\t') break;
		else	*ptr = 0;
	} 

	return str;
} 


// -------------------------------------------->  공백 여러개면 1개로 만든다..
void ReduceSpace(char *str)
{
	char* p;
	while((p=strstr(str,"  ")) != NULL)	memmove(&p[1],&p[2],strlen(&p[1]));
} 


char* tel_dash(string tel)
{
	char* in  = (char*) tel.c_str();

	if(strchr(in,'-')) return in;
	
	char* out = &_buff_[M_IDX_PLUS(_buff_idx)];
	char fst[6], sec[6], end[6];
	int  len=strlen(in);
	char *p=in;

	if(len < 9 || len > 11)		return in;
	if(len == 9)	{
		if(!memcmp(in,"02",2))		{ nulcpy(fst,&in[0],2);  nulcpy(sec,&in[2],3); nulcpy(end,&in[6],4);  }
		else return in;
	}
	else if(len == 10)	{			
		if(!memcmp(in,"02",2))		{ nulcpy(fst,&in[0],2);  nulcpy(sec,&in[2],4); nulcpy(end,&in[6],4);  }
		else						{ nulcpy(fst,&in[0],3);  nulcpy(sec,&in[3],3); nulcpy(end,&in[6],4);  }
	}
	else							{ nulcpy(fst,&in[0],3);  nulcpy(sec,&in[3],4); nulcpy(end,&in[7],4);  }			

	sprintf(out,"%s-%s-%s", fst, sec, end);

	return out;
}


int IsDashNum(char* src)		// 전화번호처럼 '-'(대쉬)포함 숫자로 이루어졌는지 여부..
{
	for(;*src;src++)   if(*src != '-' && !IS_DIGIT(*src)) return 0;
	return 1;
}






int GetBinarySearch(vector<int>vList, int val, int*find)
{
	size_t size = vList.size();
	if(size == 0)  return -1;

	int ret, mid;
	int fst=0, end=(int)size-1;

	if(val < vList[fst]) return -1;
	if(val > vList[end]) return -2;

	while(1)	{
		mid	= (fst+end)/2;
		ret	= vList[mid] - val;

		
		if(ret == 0) {
			int cnt = 0;
			find[cnt++] = mid;
			for(int x=mid-1; x >= 0; x--)	{
				if(vList[x] != val) break; 
				find[cnt++] = x;
			}
			for(int x=mid+1; x < size; x++)	{
				if(vList[x] != val) break; 
				find[cnt++] = x;
			}
			return cnt;
		}

		if(ret > 0)		{
			fst = mid + 1;
			if(fst > end)	return -2;
		}
		else		{
			end = mid - 1;
			if(end < fst)	return -2;
		}
	}
}


void ExtractBunji(char* addr2, char* bunji, char* addr3)  //  addr2를 번지와 나머지로 나눈다..
{
	char *p = addr2;
	char *q = bunji;
	char* psan	= (char*)"산";  //  또는 "ㅡ"
	int	sanlen = strlen(psan);
	char* pbar	= (char*)"ㅡ";
	int	barlen = strlen(pbar);
	char* pbun	= (char*)"번지";
	int	bunlen = strlen(pbun);
	
	leftTrim(addr2);
	int minus_cnt = 0;
	for(int cnt=0; *p && cnt<15; cnt++) {
		if(IS_DIGIT(*p))					{ *q++ = *p++; }
		else if(*p == '-')					{ if(minus_cnt==0) *q++ = *p; minus_cnt++; p++; }		//  '-'가 2개이상이면 skip..
		else if(!memcmp(p,pbar,barlen))		{ *q++ = '-';	p+=barlen; }
		else if(!memcmp(p,pbun,bunlen))		{ p+=bunlen;	break; }
		else if(!memcmp(p,psan,sanlen))		{ strcpy((char*)q,psan); p += sanlen; q += sanlen; }
		else  break;
	}
	*q = 0;

	strcpy(addr3,p);
	Trim(addr3);
}


char* strLower(char* src)
{
	for(char*p=src;*p;p++) if(IS_UPPER(*p)) *p += 0x20;
	return src;
}


char* strUpper(char* src)
{
	for(char*p=src;*p;p++) if(IS_LOWER(*p)) *p -= 0x20;
	return src;
}


int IsAllUpper(char* src)
{
	for(char*p=src;*p;p++) if(!IS_UPPER(*p)) return 0;
	return 1;
}


int IsAllLower(char* src)
{
	for(char*p=src;*p;p++) if(!IS_LOWER(*p)) return 0;
	return 1;
}


int getPkey()		//  unique integer generate..
{
	static int pkey = (int)(time(0)-EUK_14);		//  현재로부터 14억을 뺀 수치.. 부터 시작..
	return pkey++;
}


char*	getPkey(int k)			//  변환할 숫자  ,,  자릿수는 10자리로  1,3,5,7,9에 들어간다...
{
	char* pkey = &_buff_[M_IDX_PLUS(_buff_idx)];

	// ============================================>  61진수 변환..  역순..
	int a[5] = { -1, };
	for(int x=0; x<5; x++) a[x] = -1;
	for(int x=0; x<5; x++, k/=61)	{
		if(k < 61)	{ a[x] = k; a[x+1] = 61; break; }
		a[x] = k % 61;
	}
	// ============================================>  61진수 변환..  역순..

	int idx;

	srand(time(0));
	for(int x=0,y=0; x<10; x++)	{
		if(x == (y*2+1)) {
			idx = a[y++];
			if(idx >= 0)  {
				pkey[x] = cryptbuf[idx];
				continue;
			}
		}		

		idx = Random(61);
		pkey[x] = cryptbuf[idx];
	}
	pkey[10] = 0;

	return pkey;
}


int	 getNum_byPkey(char* pkey)		//  pKey의 역변환...
{
	int val = 0;
	for(int x=9,y=0; x>=0; x--)	{
		if(pkey[x] == '0')  {
			y = x-2;
			continue;
		}
		if(y == 0)	continue;
		if(x == y)	{
			y = x-2;
			for(int idx =0; idx<61; idx++)	{
				if(cryptbuf[idx] == pkey[x]) val = val*61 + idx;
			}
		}
	}

	return val;
}


char* getext(char* src, char dot)
{
	char* p = strrchr(src,dot);
	if(p == NULL) p = src;
	else		 p++;
	return p;
}


char* strTime(struct tm* ptm)
{
	char *timebuf = &_buff_[M_IDX_PLUS(_buff_idx)];
	strftime(timebuf, 20, "%Y-%m-%d %H:%M", ptm);
	return timebuf;
}


char* strTime(time_t tt)
{
//_LOGFILE("\t\t\t_buff_idx=%d => ", _buff_idx);
	char *timebuf = &_buff_[M_IDX_PLUS(_buff_idx)];
//_LOGFILE("%d\n", _buff_idx);
	struct tm *ptm = localtime(&tt);
	strftime(timebuf, 20, "%Y-%m-%d %H:%M", ptm);
	return timebuf;
}

char* strDate(time_t tt)
{
	char *timebuf = &_buff_[M_IDX_PLUS(_buff_idx)];
	struct tm *ptm = localtime(&tt);
	strftime(timebuf, 20, "%Y-%m-%d", ptm);
	return timebuf;
}

char* strDateTime(time_t tt)
{
	char *timebuf = &_buff_[M_IDX_PLUS(_buff_idx)];
	struct tm *ptm = localtime(&tt);
	strftime(timebuf, 20, "%Y-%m-%d %H:%M:%S", ptm);
	return timebuf;
}

char* strHHMMSS(time_t tt)
{
	char *timebuf = &_buff_[M_IDX_PLUS(_buff_idx)];
	struct tm *ptm = localtime(&tt);
	strftime(timebuf, 20, "%H:%M:%S", ptm);
	return timebuf;
}


// =====================================> 현재시간이 tm_str(hh:mm~hh:mm)사이인지 여부....
int	between_nowtm(string& tmstr, time_t tmtm)
{
	if(tmstr.length() == 0)	return 0;

	int ret;
	int st_hour, st_min, en_hour, en_min;
	sscanf(tmstr.c_str(),"%d:%d~%d:%d", &st_hour, &st_min, &en_hour, &en_min);
	struct tm ttm = *localtime(&tmtm);

	int	nowtm = ttm.tm_hour*100 + ttm.tm_min;
	int start = st_hour*100 + st_min;
	int end   = en_hour*100 + en_min;
	
// _LOGFILE("\n\n========>  between_nowtm(%s,  %04d) %04d~%04d => ", tmstr.c_str(), nowtm, start, end);

		 if(start == end)				ret = 0;
	else if(start  < end)		{
		if(start<=nowtm && nowtm<end)	ret = 1;
		else							ret = 0;
	}
	else if(start  > end)		{
		if(start<=nowtm || nowtm<=end)	ret = 1;
		else							ret = 0;
	}

//	_LOGFILE_NONUM("%d\n\n", ret);

	return ret;
}





int IsLastWord(char* buf, const char* word)		// 마지막단어가 word인지..  동작구,"구"
{
	int len	= strlen(word);
	int idx	= strlen(buf) - len;

	if(idx < 0) return 0;

	if(memcmp(&buf[idx],word,len) == 0) return 1;
	return 0;
}


char* EraseChar(char* buf, char del)
{
	char *p,*q;

	for(p=q=buf; *p; p++) {
		if(*p != del)	{
			if(p!=q)	*q = *p;
			q++;
		}
	}
	*q = 0;
	return buf;
}


void EraseChars(char* buf, const char* del)
{
	char *p,*q,*r=buf;

	for(p=buf; *p; p++) {
		for(q=(char*)del; *q; q++) {
			if(*p == *q) goto NEXT;
		}
		*r++ = *p;
NEXT:;
	}
	*r = 0;
}

char* EraseChar(string buf, const char del)		{ return EraseChar ((char*)buf.c_str(), (char)del); }
void  EraseChars(string buf, const char* del)	{ return EraseChars((char*)buf.c_str(), del); }

void EraseDigit(char* buf)
{
	char *p,*q;

	for(p=q=buf; *p; p++) {
		if(! IS_DIGIT(*p))	{
			if(p != q)	*q = *p;
			q++;
		}
	}
	*q = 0;
}


void Space_To_Per20(char* url)
{
	char *p = &_buff_[M_IDX_PLUS(_buff_idx)];
	char *q = url;
	strcpy(p,q);

	for(; *p; p++) {
		if(*p == ' ')	q   += sprintf(q,"%%20");
		else			*q++ = *p;
	}
	*q = 0;
}


void EraseWord(char* buf, char* delword)
{
	char *p=buf,*q;
	int	len = strlen(delword);

	for(;q=strstr(p,delword);) {
		p = q;
		q += len;
		memmove(p,q,strlen(q)+1);;
	}
	*q = 0;
}


void EraseLastWord(char* buf, char* delword)
{
	char *tmp = &_buff_[M_IDX_PLUS(_buff_idx)];
	strcpy(tmp,buf);
	char *p = tmp, *q;
	int	len = strlen(delword);
	char  ch = *delword;

	for(;p=strrchr(tmp,ch);)	{
		if(!memcmp(p,delword,len)) {
			q = buf+(p-tmp);
			memmove(q,q+len,strlen(q+len)+1);		// NULL까지 복사.
			return;
		}
		*p = 0;
	}
}


void ReplaceChar(char* src, const char from, const char to )
{
	for(;*src;src++)  if(*src == from) *src = to;
}


void ReplaceString( string& src, const char* from, const char* to )
{
	size_t fromLen = strlen(from);
	size_t toLen = strlen(to);
	size_t pos = 0;

	while ((pos = src.find(from, pos)) != std::string::npos)
	{
		src.replace( pos, fromLen, to );
		pos += toLen; 
	}
}


void TruncateLastDong(char* buf, char* delword)
{
	EraseDigit(buf);

	char *tmp = &_buff_[M_IDX_PLUS(_buff_idx)];
	strcpy(tmp,buf);
	char *p = tmp;
	int	len = strlen(delword);
	char  ch = *delword;

	for(;p=strrchr(tmp,ch);)	{
		if(!memcmp(p,delword,len)) {
			len += p - tmp;
			buf[len] = 0;
			return;
		}
	}
}



//============================================================> 시간관련함수..
//============================================================> 시간관련함수..
//============================================================> 시간관련함수..
//============================================================> 시간관련함수..
//============================================================> 시간관련함수..
char* ChangeDateTime(char* tim)
{
	char  tbuf[32];
	strcpy(tbuf,tim);
	
	memcpy(&tim [0],&tbuf [0],4); tim [4] = '-';
	memcpy(&tim [5],&tbuf [4],2); tim [7] = '-';
	memcpy(&tim [8],&tbuf [6],2); tim[10] = ' ';
	memcpy(&tim[11],&tbuf [8],2); tim[13] = ':';
	memcpy(&tim[14],&tbuf[10],2); tim[16] = ':';
	memcpy(&tim[17],&tbuf[12],2); tim[19] = '\0';

	return tim;
}


int GetTimeDuration(char* fromtm, char* totm)
{
	char  tbuf[5];
	int	 ft,tt;
	
	struct tm tm;
	nulcpy(tbuf,&fromtm[0],4);	tm.tm_year = atoi(tbuf) - 1900;
	nulcpy(tbuf,&fromtm[4],2);	tm.tm_mon  = atoi(tbuf) - 1;
	nulcpy(tbuf,&fromtm[6],2);	tm.tm_mday  = atoi(tbuf);
	nulcpy(tbuf,&fromtm[8],2);	tm.tm_hour = atoi(tbuf);
	nulcpy(tbuf,&fromtm[10],2);	tm.tm_min  = atoi(tbuf);
	nulcpy(tbuf,&fromtm[12],2);	tm.tm_sec  = atoi(tbuf);
	ft = mktime(&tm);

	nulcpy(tbuf,&totm[10],2);	tm.tm_min  = atoi(tbuf);
	nulcpy(tbuf,&totm[12],2);	tm.tm_sec  = atoi(tbuf);
	tt = mktime(&tm);

	return tt-ft;
}


/*
int GetTimeDuration(char* fromtm, char* totm)
{
	char 	 tbuf[5];
	time_t	 ft,tt;
	
	return GetSystemTime(totm) - GetSystemTime(fromtm);
}
*/


time_t GetSystemTime(string& strtim) { return GetSystemTime((char*)strtim.c_str()); }
time_t GetSystemTime(char* strtim)
{
    struct tm tm;
    char  tbuf[5], *pn = strtim;

	int tmtm[6]={0,}, blen=4;

	for(int x=0; *pn && x<6; x++)	{
		if(x > 0)	blen = 2;
		if(strlen(pn) >= blen) {
			nulcpy(tbuf,pn,blen);
			tmtm[x] = atoi(tbuf);
			pn += blen;
			if(*pn) pn++;
		}
	}

	tm.tm_year = tmtm[0] - 1900;
	tm.tm_mon  = tmtm[1] - 1;
	tm.tm_mday = tmtm[2];
	tm.tm_hour = tmtm[3];
	tm.tm_min  = tmtm[4];
	tm.tm_sec  = tmtm[5];

	return mktime(&tm);
}

time_t GetSystemTime2(char* strtim)
{
	struct tm tm; memset((char*)&tm,0,sizeof(tm));

	sscanf(strtim,"%d-%d-%d %d:%d:%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
	tm.tm_year -= 1900; tm.tm_mon--;

	return mktime(&tm);
}



char* getPayWay(string& payway)
{
	char* pw = (char*)payway.c_str();
	if(!memcmp(pw,"11",2)) return (char*)"현금";
	if(!memcmp(pw,"12",2)) return (char*)"카드";
	if(!memcmp(pw,"21",2)) return (char*)"현금";
	if(!memcmp(pw,"22",2)) return (char*)"카드";
}


//============================================================> 압축..
//============================================================> 압축..
//============================================================> 압축..
char*	en_crypt(char* src, char* dst)
{
	char *p = dst;
	char ch;
	for(; *src;) {
		ch = *src++;
//		if(IS_DIGIT(ch) || ch=='.' || ch == ',') ch += 0x20;
		*p++ = (char)ch;
	}
	*p = 0;
	return dst;
}


char*	de_crypt(char* src, char* dst)
{
	char *p = dst;
	char ch;
	for(; *src;) {
		ch = *src++;
		if(IS_UPPER(ch)) ch -= 0x20;
		*p++ = (char)ch;
	}
	*p = 0;
	return dst;
}



//============================================================> 위경도 거리계산..
//============================================================> 위경도 거리계산..
//============================================================> 위경도 거리계산..
//============================================================> 위경도 거리계산..
//============================================================> 위경도 거리계산..
#include <math.h>
#define pi 3.14159265358979323846

double deg2rad(double deg) { return (deg * pi / 180); }
double rad2deg(double rad) { return (rad * 180 / pi); }



//int GetDistance(int tm_lat1, int tm_lng1, int tm_lat2, int tm_lng2)
//{
//	double da = tm_lat2 - tm_lat1;
//	double dn = tm_lng2 - tm_lng1;
//	return (int)sqrt(da*da + dn*dn);
//}


int Get_Distance(double lat1,double lng1,double lat2,double lng2)
{
	double theta = lng2-lng1;
	double dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
	dist = acos(dist);
	dist = rad2deg(dist);

//	double miles = dist * 60 * 1.1515;
//	return (int)(miles * 1.609344 * 1000);		//meter 
	return int(dist*111189.57696);
}


int GetDirection(double lat1, double lng1, double lat2, double lng2)
{
	double dlat = (lat2 - lat1);
	double dlng = (lng2 - lng1);

								// 몇시방향인지 ..
	int dir = rad2deg(atan(dlat/dlng)) + 285;
	dir = 12-dir/30;
	if(dir>12) dir -= 12;

	return dir;
}



int GetDistDir(double lat1,double lng1,double lat2,double lng2,int &dir)
{
	double dx = lat2 - lat1;
	double dy = lng2 - lng1;
	
	double rad= atan2(dx, dy);
	int Degree = rad2deg(rad);
 
	int clock[13] = {-1, 60, 30, 0, 330, 300, 270, 240, 210, 180, 150, 120, 90};

	Degree = ((Degree%360)+360)%360;
	
	int i;
	for(i=1; i<=12; i++) {
		if(clock[i] == 0) {
			if(Degree <= 15 || Degree >= 345) break;
		}
		else {
			if(Degree <= clock[i]+15 && Degree >= clock[i]-15) break;
		}
	}
	dir = i; // 각도

		
	double theta = lng2-lng1;
	double dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
	dist = acos(dist);
	dist = rad2deg(dist);
	double miles = dist * 60 * 1.1515;

	return (int)(miles * 1.609344 * 1000);//meter 
}


#include <dirent.h>

DIR *p_DIR;
struct dirent *dentry;

int Get1File(char *dirname, char* fname)
{
	if(dirname != NULL) {
		if(p_DIR) closedir(p_DIR);		// opendir로 얻은 핸들을 닫는다.
		p_DIR = opendir(dirname);		//  dir이 NULL이면 기존 dir에서 1 file get,
	} else {
		if(p_DIR == NULL ) return 0;
	}

	while((dentry=readdir(p_DIR)) != NULL) {
		if(dentry->d_type == DT_DIR)	continue;					// dir

		strcpy(fname,dentry->d_name);
		return 1;
	}
	closedir(p_DIR); // opendir로 얻은 핸들을 닫는다.
	p_DIR = NULL;
	return 0;
}



int Get1Dir(char *dirname, char* dir)
{
	if(dirname != NULL) {
		if(p_DIR) closedir(p_DIR); // opendir로 얻은 핸들을 닫는다.
		p_DIR = opendir(dirname);		//  dir이 NULL이면 기존 dir에서 1 file get,
	} else {
		if(p_DIR == NULL ) return 0;
	}

	while((dentry=readdir(p_DIR)) != NULL) {
		if(dentry->d_type == DT_DIR)	continue;					// dir

		strcpy(dir,dentry->d_name);
		return 1;
	}
	closedir(p_DIR); // opendir로 얻은 핸들을 닫는다.
	p_DIR = NULL;
	return 0;
}


int Get1AnyInDir(char *dirname, char* any)
{
	if(dirname != NULL) {
		if(p_DIR) closedir(p_DIR);		// opendir로 얻은 핸들을 닫는다.
		p_DIR = opendir(dirname);		//  dir이 NULL이면 기존 dir에서 1 file get,
	} else {
		if(p_DIR == NULL ) return 0;
	}

	while((dentry=readdir(p_DIR)) != NULL) {
		if(!strcmp(dentry->d_name,".")) continue;
		if(!strcmp(dentry->d_name,"..")) continue;

		strcpy(any,dentry->d_name);

		if(dentry->d_type == DT_DIR)	return 2;	// dir
		else							return 1;	// file
	}
	closedir(p_DIR); // opendir로 얻은 핸들을 닫는다.
	p_DIR = NULL;
	return 0;
}



// ----------------------------------------------------->  하위폴더까지 검색해서 map에 넣는다..		<fullpath,1>
// ----------------------------------------------------->  하위폴더까지 검색해서 map에 넣는다..		<fullpath,1>

int GetFileList_subdir(map<string,int> &vList, char* dir, char* ext)		//  하위폴더까지 검색..		ext는 ".png"처럼 '.'까지 포함해서 넘겨준다..
{
	DIR*	p_dir;
	struct	dirent *dentry;
	char*	pext;
	char	fpath[256];

	int		cnt = 0;

	if((p_dir=opendir(dir)) == NULL ) return 0;

	while((dentry=readdir(p_dir)) != NULL)
	{
		if(dentry->d_name[0] == '.') continue;

		if(dentry->d_type == DT_DIR)	{					// dir
			sprintf(fpath,"%s/%s", dir, dentry->d_name);
			cnt += GetFileList_subdir(vList, fpath, ext);
		}
		else		{									// file
			if((pext=strrchr(dentry->d_name,'.')) == NULL)	continue;	//  확장자가 없으면..  건너뛴다.

			if(!strcmp(pext,ext))	{
				sprintf(fpath,"%s/%s", dir, dentry->d_name);
				vList[string(fpath)] = 1;
				cnt++;
			}
		}
	}
	closedir(p_dir); // opendir로 얻은 핸들을 닫는다.

	return cnt;
}


// ==================================================>  tag를 찾아 값을 int로 변환.
int GetInt_Jason(char* buf, const char* tag, char end, char* &pn)
{
	char* p;
    if((p=strstr(buf,tag)) == NULL) {
		pn = buf + strlen(buf);
		return 0;
	}
	return atoi(GetToken_NotNull(&p[strlen(tag)],end, pn));
}

// ==================================================>  tag를 찾아 값을 int로 변환.
double GetDouble_Jason(char* buf, const char* tag, char end, char* &pn)
{
	char* p;
    if((p=strstr(buf,tag)) == NULL) {
		pn = buf + strlen(buf);
		return 0;
	}
	return atof(GetToken_NotNull(&p[strlen(tag)],end, pn));
}

// ==================================================>  tag와 end사이의 값..	원본손상됨..  
// ==================================================>  tag와 end사이의 값..	원본손상됨..  
// ==================================================>  tag와 end사이의 값..	원본손상됨..  
char* GetPointer_Jason(char* buf, const char* tag, char end, char* &pn)
{
	char* p;
    if((p=strstr(buf,tag)) == NULL) {
		pn = buf + strlen(buf);
		return NULLBUF;
	}
	return GetToken(&p[strlen(tag)],end, pn);
}

// ==================================================>  tag와 end사이의 값..	원본손상 없다..
// ==================================================>  tag와 end사이의 값..	원본손상 없다..
// ==================================================>  tag와 end사이의 값..	원본손상 없다..
string GetString_Jason(char* buf, const char* tag, char end, char* &pn)
{
	char* p;
    if((p=strstr(buf,tag)) == NULL) {
		pn = buf + strlen(buf);
		return "";
	}
	return (string)GetToken_NotNull(&p[strlen(tag)],end, pn);
}


// =================================================================================================>  palce_id를 거쳐 tag와 end사이의 값.	원본손상 없다..
// =================================================================================================>  palce_id를 거쳐 tag와 end사이의 값.	원본손상 없다..
// =================================================================================================>  palce_id를 거쳐 tag와 end사이의 값.	원본손상 없다..
string GetType_PlaceId(char* buf, const char* place_id, const char* tag, char end, char* &pn)		// palce_id를 거쳐 tag와 end사이의 값.	원본손상 없다..
{
_LOGFILE("GetType_PlaceId()..place_id='%s',tag='%s'->'%c'\n", place_id,tag,end);
	char *p;
    if((p=strstr(buf,place_id))  == NULL)  return (pn=buf+strlen(buf));

	return GetString_Jason(p+strlen(place_id), tag, end, pn);
}


// =================================================================================================>  stb(start)와 enb(end)사이의 값,, stb의 위치를 번지로 return
// =================================================================================================>  stb(start)와 enb(end)사이의 값,, stb의 위치를 번지로 return
// =================================================================================================>  stb(start)와 enb(end)사이의 값,, stb의 위치를 번지로 return
char*	GetBlock(char* data, const char* tag, const char stb, const char enb, char* &pn)
{
_LOGFILE("GetBlock()..tag='%s',  '%c' -> '%c'\n", tag, stb, enb);

	if((pn=strstr(data,tag)) == NULL)	pn = data;
	if((pn=strchr(pn+strlen(tag),stb)) == NULL)	return NULL;

	data = pn;

	int  cnt=0;
	char ch;
	while(ch=*pn++)								//  위에서 'stb'를 한번 만났기 때문에 cnt=1부터 시작한다..
	{
		     if(ch == stb)	cnt++;
		else if(ch == enb)	cnt--;

		if(cnt == 0)	{
			if(*pn) *pn++ = 0;
			return  data;
		}
	}

	return NULL;
}
// =================================================================================================>  stb(start)와 enb(end)사이의 값,, stb의 위치를 번지로 return




// =================================================================================================>  data write..
// =================================================================================================>  data write..
// =================================================================================================>  data write..
void   block_write(char*data, char* dir, char* fname)
{
	FILE* fp;
	char  fpath[512];
	sprintf(fpath,"%s/%s", dir, fname);

_LOGFILE("block_write() .. %s write..\n", fpath);
	if((fp=fopen(fpath,"w")) != NULL)	{

		fwrite(data,1,strlen(data),fp);
		fclose(fp);
	}
}
// =================================================================================================>  data write..

	


char* Remove_Do(char* addr)			//  각 시도중 도를 제거한다..  대한민국도 제거..
{									//  충청남도 천안시 서북구 .. => 천안시 서북구...  
	Trim(addr);
    char *pn = addr;

    if(!memcmp(pn,"대한민국",HAN4JA_LEN))   GetToken_NotNull(pn,' ',pn);

    if(!memcmp(pn,"충청남도",HAN4JA_LEN) || !memcmp(pn,"충남",HAN2JA_LEN)   || !memcmp(pn,"충청북도",HAN4JA_LEN) || !memcmp(pn,"충북",HAN2JA_LEN) ||
       !memcmp(pn,"전라남도",HAN4JA_LEN) || !memcmp(pn,"전남",HAN2JA_LEN)   || !memcmp(pn,"전라북도",HAN4JA_LEN) || !memcmp(pn,"전북",HAN2JA_LEN) ||
       !memcmp(pn,"경상남도",HAN4JA_LEN) || !memcmp(pn,"경남",HAN2JA_LEN)   || !memcmp(pn,"경상북도",HAN4JA_LEN) || !memcmp(pn,"경북",HAN2JA_LEN) ||
       !memcmp(pn,"경기도",HAN3JA_LEN)   || !memcmp(pn,"강원도",HAN3JA_LEN) || !memcmp(pn,"제주도",HAN3JA_LEN)   || !memcmp(pn,"제주특별자치도",HAN7JA_LEN) )
    {
        GetToken_NotNull(pn,' ',pn);
    }

    if(addr != pn) memmove(addr,pn,strlen(pn)+1);
	return addr;
}




int IsLastDongRoGa(char* buf)			// 마지막단어가 1가, 2가,   전국의 동에 해당하는 '가'인가?  영등포동3가, 을지로2가...
{
	int len_hangle = strlen("가");
	int idx	   = strlen(buf) - len_hangle;

	if(idx <= len_hangle*2) return 0;						// 최소 '가'글자이전  2글자이상 되어야 한다.  "본동1가"

	if(memcmp(&buf[idx],"가",len_hangle) != 0) return 0;	// '가' 가 아니면 return 0
	if(!IS_DIGIT(buf[--idx]))	return 0;						// '가'바로 이전이 숫자가 아니면 잘못..  1가 2가등

	idx -= len_hangle;
	if(memcmp(&buf[idx],"동",len_hangle) != 0 || memcmp(&buf[idx],"로",len_hangle) != 0) return 0;	// '로' 나 '동'이 아니면 return 0,  동1가 or 로3가..

	return 1;
}





// ==================================>  로그관련..
char	LogDir[80] = "/home/log/weK";
char	LogPath[80];

int		cnt = 0;
time_t	savet = 0;

char* strDate_NoSeparator(time_t tt)
{
	char	*timebuf = &_buff_[M_IDX_PLUS(_buff_idx)];
	struct	tm *ptm = localtime(&tt);
	strftime(timebuf, 20, "%Y%m%d", ptm);
	return timebuf;
}

int GetYear  (time_t tt) { struct tm *ptm = localtime(&tt); return ptm->tm_year+1900;	}
int GetMonth (time_t tt) { struct tm *ptm = localtime(&tt); return ptm->tm_mon+1;		}
int GetDay	 (time_t tt) { struct tm *ptm = localtime(&tt); return ptm->tm_mday;		}
int GetHour	 (time_t tt) { struct tm *ptm = localtime(&tt); return ptm->tm_hour;		}
int GetMinute(time_t tt) { struct tm *ptm = localtime(&tt); return ptm->tm_min;			}
int GetWeek  (time_t tt) { struct tm *ptm = localtime(&tt); return ptm->tm_wday;		}

void _LOGFILE(char *format)
{
	cnt++;

	sprintf(LogPath,"%s/%s_%02d.w", LogDir,strDate_NoSeparator(g_Now),GetHour(g_Now));

	FILE* fp = fopen(LogPath,"a");

	if(fp) {
		if(cnt == 1)	fprintf(fp,"\n\n\n\n%d: ", cnt);
		else			fprintf(fp,"%d: ", cnt);
		fputs(format, fp);
		fclose(fp);
	}

	if(g_DirectExecFlag == 0) return;	//  직접 실행한게 아니면..

	fprintf(stdout,"%d: ", cnt);
	fputs(format, stdout);
}


void _LOGFILE(const char *format, ...)
{
	cnt++;
va_list args;

va_start(args, format);
	sprintf(LogPath,"%s/%s_%02d.w", LogDir,strDate_NoSeparator(g_Now),GetHour(g_Now));

	FILE* fp = fopen(LogPath,"a");
	if(fp) {
		if(cnt == 1)	fprintf(fp,"\n\n\n\n%d: ", cnt);
		else			fprintf(fp,"%d: ", cnt);
		vfprintf(fp, format, args);
		fclose(fp);
	}
va_end(args);

	if(g_DirectExecFlag == 0) return;	//  직접 실행한게 아니면..

va_start(args, format);
	fprintf(stderr,"%d: ", cnt);
	vfprintf(stderr, format, args);
va_end(args);

}



					//  일련번호가 없는것,, cnt 가 필요없다..
void _LOGFILE_NONUM(const char *format, ...)
{
va_list args;

va_start(args, format);
	sprintf(LogPath,"%s/%s_%02d.w", LogDir,strDate_NoSeparator(g_Now),GetHour(g_Now));

	FILE* fp = fopen(LogPath,"a");
	if(fp) {
		vfprintf(fp, format, args);
		fclose(fp);
	}
va_end(args);

	if(g_DirectExecFlag == 0) return;	//  직접 실행한게 아니면..

va_start(args, format);
	vfprintf(stderr, format, args);
va_end(args);

}
