#include "wLib.h"

// ========================================================>  STL 
// ========================================================>  STL 
// ========================================================>  STL 
// ========================================================>  STL 
// ========================================================>  STL 
// ========================================================>  STL 
// ========================================================>  STL 
// ========================================================>  STL 
// ========================================================>  STL 



Z_POS*  GetMember(umap<string,Z_POS*>& mList, string pos_hp)
{
	auto it = mList.find(pos_hp);
	if(it != mList.end())	return it->second;
	return NULL;
}


int  GetMember(umap<string,int>& mList, string idx)
{
	auto it = mList.find(idx);
	if(it != mList.end())	return it->second;
	return 0;
}



int  GetMember(vector<int>& mList, int idx)
{
	auto it=find(mList.begin(),mList.end(), idx);
	if(it != mList.end())	return 1;
	return 0;
}


int  GetMember(vector<string>& mList, string idx)
{
	auto it=find(mList.begin(),mList.end(), idx);
	if(it != mList.end())	return 1;
	return 0;
}


// ========================================================>  STL 
// ========================================================>  STL 
// ========================================================>  STL 
int  EraseMember(umap<int,int>& mList, int idx)
{
	auto it = mList.find(idx);
	if(it != mList.end())	{	mList.erase(it); return 1;	}
	return 0;
}





void ReadUser(string user_id, struct  sockaddr_in& clnt_addr)
{
	char fpath[256];
	sprintf(fpath,"/home/log/quiz/%s", user_id.c_str());
	FILE* fp = fopen(fpath,"rb");
	if(fp == NULL)	return;

	fread((char*)&clnt_addr,1,sizeof(struct sockaddr_in),fp);
	fclose(fp);
}


void WriteUser(string user_id, struct  sockaddr_in clnt_addr)
{
	char fpath[256];
	sprintf(fpath,"/home/log/quiz/%s", user_id.c_str());
	FILE* fp = fopen(fpath,"wb");
	if(fp == NULL)	return;

	fwrite((char*)&clnt_addr,1,sizeof(struct sockaddr_in),fp);
	fclose(fp);
}
