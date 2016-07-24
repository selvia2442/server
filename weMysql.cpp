#include "weLib.h"

#include "weMysql.h"
#include "../_Mysql.cpp"



// =======================================================>  t_ownerpoint 가져오기..
int dbSelectOwnerPointTable(vector<T_OWNERPOINT*>& vList, char* Query)
{
	sql::Connection *con;
	sql::Statement *stmt = NULL;
	sql::ResultSet  *res = NULL;

	int ret = 0;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		T_OWNERPOINT *pOwnPoint;
		for (ret=0; res->next(); ret++) {

			pOwnPoint = new T_OWNERPOINT;
			vList.push_back(pOwnPoint);

			pOwnPoint->deal_idx  = res->getInt("deal_idx");
			pOwnPoint->owner_idx = res->getInt("owner_idx");

			pOwnPoint->in_out      = res->getString("in_out");
			pOwnPoint->deal_kind   = res->getString("deal_kind");

			pOwnPoint->table_name  = res->getString("table_name");
			pOwnPoint->table_idx   = res->getInt("table_idx");
			pOwnPoint->transaction = res->getString("transaction");
			
			pOwnPoint->deal_point  = res->getInt("deal_point");
			pOwnPoint->use_point   = res->getInt("use_point");
			pOwnPoint->tot_point   = res->getInt("tot_point");
			pOwnPoint->create_date = res->getString("create_date");
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


// =======================================================>  t_ownerpoint 정산하기.. 매우중요.. 
int dbSelectOwnerPointTable(map<int,pair<int,int>>& vList, char* Query)
{
	sql::Connection *con;
	sql::Statement *stmt = NULL;
	sql::ResultSet  *res = NULL;

	int ret = 0;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		T_OWNERPOINT *pOwnPoint;
		int dealidx,usepoint;
		for (ret=0; res->next(); ret++) {
			dealidx  = res->getInt("deal_idx");
			usepoint = res->getInt("use_point");
			vList[ret] = pair<int,int>(dealidx,usepoint);
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
// <=======================================================  t_ownerpoint 정산하기.. 매우중요.. 
// <=======================================================  t_ownerpoint 정산하기.. 매우중요.. 
// <=======================================================  t_ownerpoint 정산하기.. 매우중요.. 




int dbDelete_1Record(const char* table, const char* keyname, int idx)
{
	char Query[256];
	sprintf(Query, "DELETE from %s where %s=%d;", table, keyname, idx);

	return dbDeleteQuery(Query);
}



// ============================================================>  t_menu select
int	 dbSelectMenuTable(vector<T_MENU*> &vList, int shop_idx)
{
	char Query[256];
	sprintf(Query,"select * from t_menu where shop_idx=%d;", shop_idx);

	int ret=0;

	sql::Connection *con;
	sql::Statement *stmt = NULL;
	sql::ResultSet  *res = NULL;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		T_MENU* pMenu;
		for (ret=0; res->next(); ret++) {

			int	idx  = res->getInt("menu_idx");
			if(idx <= 0) continue;

			pMenu = new T_MENU;
			vList.push_back(pMenu);

			pMenu->menu_idx  = idx;

			pMenu->shop_idx  = res->getInt("shop_idx");
//			pMenu->shop_code = res->getString("shop_code");
			pMenu->parent	 = res->getInt("parent");

			pMenu->gubun     = res->getInt("gubun");
			pMenu->menu_name = res->getString("menu_name");
			pMenu->menu_photo= res->getString("menu_photo");

			pMenu->price	 = res->getInt("price");
			pMenu->discount	 = res->getInt("discount");

			pMenu->menu_text = res->getString("menu_text");
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

int dbGetMyKind(int shopIdx, int* myIdxs)
{
	char Query[256];
	sprintf(Query,"select kind_idx from t_sk_link where shop_idx=%d;", shopIdx);

	sql::Connection *con;
	sql::Statement *stmt = NULL;
	sql::ResultSet  *res = NULL;

	int ret=0;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		for (ret=0; res->next(); ret++) {
			myIdxs[ret] = res->getInt("kind_idx");
			if(myIdxs[ret] <= 0) continue;
		}
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

	myIdxs[ret] = 0;

	return dbReturn(ret, con, stmt, res);
}



// -----------------------------------------------> SELECT * FROM table

int dbGetMyRegion(int ownerIdx, int* myIdxs)
{
	char Query[256];
	sprintf(Query,"select region_idx from t_sor_link where owner_idx=%d;", ownerIdx);

	sql::Connection *con;
	sql::Statement *stmt = NULL;
	sql::ResultSet  *res = NULL;

	int ret = 0;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		for (ret=0; res->next(); ret++) {
			myIdxs[ret] = res->getInt("region_idx");
			if(myIdxs[ret] <= 0) continue;
		}
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

	myIdxs[ret] = 0;

	return dbReturn(ret, con, stmt, res);
}




// ============================================================> t_rgn  테이블 read..
// ============================================================> t_rgn  테이블 read..
int dbSelect_RegionMap(umap<int,T_RGN*> &vList, char* Query)
{
	sql::Connection *con;
	sql::Statement	*stmt = NULL;
	sql::ResultSet  *res  = NULL;

	int ret = 0, idx;
	try	{
		dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		T_RGN *pRgn;
		for (ret=0; res->next(); ret++)
		{
			if((idx=res->getInt("region_idx")) <= 0) continue;

			pRgn = GetMember(vList, idx);
			if(pRgn == NULL)	{
				pRgn = new T_RGN;
				vList[idx] = pRgn;
				pRgn->region_idx = idx;
			}

			pRgn->dp.lat	= res->getDouble("lat");
			pRgn->dp.lng	= res->getDouble("lng");

			pRgn->DO		= res->getString("DO");
			pRgn->sigun		= res->getString("sigun");
			pRgn->gu		= res->getString("gu");
			pRgn->emdong	= res->getString("emdong");
			pRgn->ri		= res->getString("ri");

			pRgn->address	= res->getString("address");
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




// ============================================================> t_shopowner  테이블 read..
// ============================================================> t_shopowner  테이블 read..

void Set_OwnerData(T_OWNER* pOwner, sql::ResultSet  *res)
{
	pOwner->owner_id		= res->getString("owner_id");
	pOwner->owner_code		= res->getString("owner_code");

	pOwner->inform_yn   	= res->getString("inform_yn");

	pOwner->shop_cnt		= res->getInt("shop_cnt");
	pOwner->shop_idx		= res->getInt("shop_idx");
	pOwner->shop_idxs		= res->getString("shop_idxs");

	pOwner->login_yn		= res->getString("login_yn");
	pOwner->hp				= res->getString("hp");

	pOwner->system_info		= res->getString("system_info");
	pOwner->name			= res->getString("name");
	pOwner->group			= res->getInt("group");
	pOwner->confirm			= res->getInt("confirm");

	pOwner->total_point		= res->getInt("total_point");
	pOwner->small_point		= res->getInt("small_point");

	pOwner->addr1			= res->getString("addr1");
	pOwner->addr2			= res->getString("addr2");

	pOwner->dp.lat			= res->getDouble("lat");
	pOwner->dp.lng			= res->getDouble("lng");
	pOwner->radius			= res->getInt("radius");

	pOwner->modify_yn		= res->getString("modify_yn");
	pOwner->order_addr_type	= res->getString("order_addr_type");

	pOwner->license_no		= res->getString("license_no");
	pOwner->bank_name		= res->getString("bank_name");
	pOwner->account_number	= res->getString("account_number");
	pOwner->depositor		= res->getString("depositor");

	pOwner->del_flag		= res->getInt("del_flag");

	pOwner->valid_systime	= g_Now + MINUTE_SEC(60);		//  20분이상 지나면 MENU_V를 통해 다시 읽는다..
}



// -----------------------------------------------> SELECT * FROM table

T_OWNER* dbSelect_OwnerTable(umap<int,T_OWNER*> &vList, umap<string,T_OWNER*> &vIdList, umap<string,T_OWNER*> &vHpList, char* Query)
{
	sql::Connection *con;
	sql::Statement *stmt = NULL;
	sql::ResultSet  *res = NULL;

	int ret=0, idx;

	T_OWNER *pOwner = NULL;
	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		ret = res->rowsCount();

		for (int x=0; res->next(); x++)
		{
			if((idx=res->getInt("owner_idx")) <= 0) continue;

			pOwner = GetMember(vList, idx);
			if(pOwner == NULL)	{
				pOwner = new T_OWNER;
				vList[idx] = pOwner;
			}

			pOwner->owner_idx = idx;
			Set_OwnerData(pOwner,res);

			vIdList[pOwner->owner_id] = pOwner;		//  hp테이블에도 넣는다..
			vHpList[pOwner->hp] = pOwner;		//  hp테이블에도 넣는다..
		}
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

	dbReturn(ret, con, stmt, res);

	return pOwner;
}



// -----------------------------------------------> SELECT * FROM table

void Set_ShopData(T_SHOP* pShop, sql::ResultSet  *res)
{
	pShop->owner_idx	= res->getInt("owner_idx");
	pShop->shop_code	= res->getString("shop_code");
	pShop->shop_name	= res->getString("shop_name");
	pShop->shop_title	= res->getString("shop_title");

	pShop->tel_main		= res->getString("tel_main");
	pShop->call050		= res->getString("call050");
	pShop->tel   		= res->getString("tel");

	pShop->open_time		= res->getString("open_time");
	pShop->msg_order_no_time= res->getString("msg_order_no_time");
	pShop->discount_time	= res->getString("discount_time");

	pShop->open_yn		= res->getString("open_yn");
	pShop->msg_order_yn	= res->getString("msg_order_yn");
	pShop->motjip_yn	= res->getString("motjip_yn");

	pShop->min_price	= res->getInt("min_price");
	pShop->total_call	= res->getInt("total_call");
	pShop->order_no		= res->getInt("order_no");

	pShop->del_flag		= res->getInt("del_flag");

	pShop->valid_systime = g_Now + MINUTE_SEC(20);		//  20분이상 지나면 MENU_V를 통해 다시 읽는다..
}



T_SHOP* dbSelect_1Shop(umap<int,T_SHOP*> &vList, int idx)
{
	char Query[256];
	sprintf(Query,"select * from t_shop where shop_idx=%d;", idx);
	
	sql::Connection *con;
	sql::Statement  *stmt = NULL;
	sql::ResultSet  *res  = NULL;

	int ret=0;
	T_SHOP *pShop = NULL;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		ret = res->rowsCount();

		if(res->next())
		{
			idx = res->getInt("shop_idx");

			pShop = GetMember(vList, idx);
			if(pShop == NULL)	{
				pShop = new T_SHOP;
				vList[idx] = pShop;
			}

			pShop->shop_idx = idx;
			Set_ShopData(pShop,res);
		}
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

	dbReturn(ret, con, stmt, res);
	return pShop;
}




// -----------------------------------------------> SELECT * FROM table

int dbSelect_ShopTable(umap<int,T_SHOP*> &vList)
{
	char Query[256];
	sprintf(Query,"select * from t_shop where del_flag != 'y';");
	
	sql::Connection *con;
	sql::Statement *stmt = NULL;
	sql::ResultSet  *res = NULL;

	int ret=0, idx;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		ret = res->rowsCount();

		T_SHOP *pShop;
		for (int x=0; res->next(); x++)
		{
			if((idx=res->getInt("shop_idx")) <= 0) continue;

			pShop = GetMember(vList, idx);
			if(pShop == NULL)	{
				pShop = new T_SHOP;
				vList[idx] = pShop;
			}
			
			pShop->shop_idx = idx;
			Set_ShopData(pShop,res);
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


void Set_OrderData(T_ORDER* pOrder, sql::ResultSet  *res)
{
	pOrder->order_hp  = res->getString("order_hp");
	pOrder->user_idx  = res->getInt("user_idx");
	pOrder->owner_idx = res->getInt("owner_idx");
	pOrder->shop_idx  = res->getInt("shop_idx");
	pOrder->order_name= res->getString("order_name");
	pOrder->total_price   = res->getInt("total_price");
	pOrder->addr_idx	= res->getInt("addr_idx");

	pOrder->addr1	   = res->getString("addr1");
	pOrder->addr2	   = res->getString("addr2");
	pOrder->secret_no  = res->getString("secret_no");

	pOrder->dp.lat		= res->getDouble("lat");
	pOrder->dp.lng		= res->getDouble("lng");

	pOrder->modify_yn		= res->getString("modify_yn");

	pOrder->order_text = res->getString("order_text");

	pOrder->shop_name = res->getString("shop_name");

	pOrder->pay_way   = res->getString("pay_way");
	pOrder->use_point   = res->getInt("use_point");

	pOrder->acpt_yn		= res->getString("acpt_yn");

	pOrder->create_date	   = res->getString("create_date");
	pOrder->create_systime = GetSystemTime((char*)pOrder->create_date.c_str());
}
// -----------------------------------------------> SELECT * FROM t_order

int dbSelectOrderTable_1Order(T_ORDER* pOrder, int order_idx)
{
	char Query[256];

	sprintf(Query,"select * from t_order where order_idx=%d;", order_idx);

	sql::Connection *con;
	sql::Statement *stmt = NULL;
	sql::ResultSet  *res = NULL;

	int	ret = 0;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		if(res->next())
		{
			pOrder->order_idx = order_idx; 
			Set_OrderData(pOrder,res);
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


// -----------------------------------------------> SELECT * FROM t_order

int dbSelect_OrderTable(umap<int,T_ORDER*> &vList, char* Query)
{
	sql::Connection *con;
	sql::Statement *stmt = NULL;
	sql::ResultSet  *res = NULL;

	int	ret;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		T_ORDER *pOrder;

		for(ret=0; res->next(); ret++)
		{
			int idx = res->getInt("order_idx");
			if((pOrder=GetMember(vList,idx)) == NULL)	{
				pOrder = new T_ORDER;
				vList[idx] = pOrder;
			}

			pOrder->order_idx = idx; 
			Set_OrderData(pOrder,res);
		}

		ret = 1;
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

	return dbReturn(ret, con, stmt, res);
}



// -----------------------------------------------> 사설...
int dbGetOrderDetail(T_ORDER* pOdn)
{
	char Query[256];
	sprintf(Query,"select * from t_order_detail where order_idx = %d;", pOdn->order_idx);

	sql::Connection *con;
	sql::Statement *stmt = NULL;
	sql::ResultSet  *res = NULL;

	int ret = 0;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		pOdn->menu_cnt = res->rowsCount();

		T_ORDER_DETAIL* pOD;
		for (ret=0; res->next(); ret++)
		{
			pOD = new T_ORDER_DETAIL;	// 소멸: CweOrder::MakeOrderSheet()
			pOdn->voDetail.push_back(pOD);

			pOD->odetail_idx = res->getInt("odetail_idx");
			pOD->order_idx   = res->getInt("order_idx");
			pOD->order_cnt   = res->getInt("order_cnt");
			pOD->menu_idx    = res->getInt("menu_idx");
			pOD->menu_name   = res->getString("menu_name");
			pOD->price       = res->getInt("price");
			pOD->cnt         = res->getInt("cnt");
			pOD->sub_total_price      = res->getInt("sub_total_price");
			pOD->create_date = res->getString("create_date");
		}
		_LOGFILE((char*)"세부주문 read 완료...\n");
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

	return dbReturn(ret, con, stmt, res);
}



/*		나중에 고친다..  속도향상을 위해..
// -----------------------------------------------> 사설...
int dbGetOrderDetail(umap<int,T_ORDER*> &vList)
{
	char Query[2048], *pt=Query;
	sprintf(Query,"select * from t_order_detail where order_idx in (
	= %d;", pOdn->order_idx);

	sql::Connection *con;
	sql::Statement *stmt = NULL;
	sql::ResultSet  *res = NULL;

	int ret = 0;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		pOdn->menu_cnt = res->rowsCount();

		T_ORDER_DETAIL* pOD;
		for (ret=0; res->next(); ret++)
		{
			pOD = new T_ORDER_DETAIL;	// 소멸: CweOrder::MakeOrderSheet()
			pOdn->voDetail.push_back(pOD);

			pOD->odetail_idx = res->getInt("odetail_idx");
			pOD->order_idx   = res->getInt("order_idx");
			pOD->order_cnt   = res->getInt("order_cnt");
			pOD->menu_idx    = res->getInt("menu_idx");
			pOD->menu_name   = res->getString("menu_name");
			pOD->price       = res->getInt("price");
			pOD->cnt         = res->getInt("cnt");
			pOD->sub_total_price      = res->getInt("sub_total_price");
			pOD->create_date = res->getString("create_date");
		}
		_LOGFILE((char*)"세부주문 read 완료...\n");
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

	return dbReturn(ret, con, stmt, res);
}
*/



// -----------------------------------------------> SELECT * FROM table

int dbSelect_Position(Z_POSITION* pPos)
{
	char Query[2048];
	sprintf(Query,"select * from z_position where pos_hp = '%s';", pPos->pos_hp.c_str());
	
	sql::Connection *con;
	sql::Statement *stmt = NULL;
	sql::ResultSet  *res = NULL;

	int	ret=0;
	string str;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		if(res->next()) {
			pPos->pos_idx   = res->getInt("pos_idx");
			pPos->user_key  = res->getString("user_key");
			pPos->dp.lat	= res->getDouble("lat");
			pPos->dp.lng	= res->getDouble("lng");
			pPos->last_time = res->getInt("last_time");
			pPos->visit_cnt = res->getInt("visit_cnt");
			pPos->owner_idx = res->getInt("owner_idx");
			pPos->region_idx= res->getInt("region_idx");
			pPos->user_idx	= res->getInt("user_idx");
			pPos->align     = res->getInt("align");
			pPos->radius    = res->getInt("radius");

			pPos->version_up	= res->getString("version_up");
			pPos->serial_info	= res->getString("serial_info");
			pPos->mobile_info	= res->getString("mobile_info");

			{
				char* pw = (char*)pPos->mobile_info.c_str();
				pPos->mobile_width  = atoi(GetToken_NotNull(pw,',',pw));
				pPos->mobile_height = atoi(GetToken_NotNull(pw,',',pw));
			}

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


		

// =================================================> ARS로부터 응답받은다음 결과코드 알기위해 select.. 
// =================================================> ARS로부터 응답받은다음 결과코드 알기위해 select.. 
// =================================================> ARS로부터 응답받은다음 결과코드 알기위해 select.. 
// =================================================> ARS로부터 응답받은다음 결과코드 알기위해 select.. 
int dbSelect_ReturnArs(T_SMS* pArs, char* Query)
{
	sql::Connection *con;
	sql::Statement *stmt = NULL;
	sql::ResultSet  *res = NULL;

	int	ret = 0;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		if(res->next())
		{
			pArs->s_idx	= res->getInt("ars_idx");

			pArs->owner_idx = res->getInt("owner_idx");
			pArs->shop_idx	= res->getInt("shop_idx");
			pArs->from_tel	= res->getString("from_tel");
			pArs->to_tel	= res->getString("to_tel");
			pArs->order_idx = res->getInt("order_idx");

			pArs->ok_code	= res->getString("ok_code");
			pArs->retval	= res->getString("retval");

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



// =================================================>  임시이고,, 바로 지운다..
#include	"bnlee.h"
int dbSelectReview(vector<T_REVIEW*>& vList, char* Query)
{
	sql::Connection *con;
	sql::Statement *stmt = NULL;
	sql::ResultSet  *res = NULL;

	int	ret;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		for(ret=0; res->next(); ret++)
		{
			T_REVIEW*	pRv = new T_REVIEW;
			vList.push_back(pRv);
			
			pRv->shop_idx			= res->getInt("shop_idx");
			pRv->order_idx			= res->getInt("order_idx");
			pRv->hp   				= res->getString("hp");
			pRv->nick     			= res->getString("nick");
			pRv->content     		= res->getString("content");
			pRv->review_photo1     	= res->getString("review_photo1");
			pRv->review_photo2     	= res->getString("review_photo2");
			pRv->review_photo3     	= res->getString("review_photo3");

			pRv->like  = res->getInt("like");
			pRv->point  = res->getInt("point");

			pRv->create_date     = res->getString("create_date");
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







sql::Driver		*driver1 = NULL;

// ==========================================================================>
int dbInsertPcallDB(char* Query)
{
	char* pData = Query;

	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *res;

	int	ret = 0;

	try	{
		if(driver1 == NULL)	driver1 = sql::mysql::get_mysql_driver_instance();
		con     = driver1->connect("nlc15401",MYSQLID,"wnscjfrla");
		con->setSchema(MYSQLDB);

		stmt = con->createStatement();
		_LOGFILE( "Query('%s')\n", pData);
		res = stmt->executeQuery(pData);

		ret = 1;
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			strcpy(Query,err.c_str());
			_LOGFILE( "오류:%s\n", Query);
			ret = 0;
		}
		else {
			res = stmt->executeQuery("select last_insert_id();");
			if(res->next())	 ret = res->getInt(1);	
			_LOGFILE((char*)"Query OK(성공) !!!\n");
		}
	}

	delete stmt;
	con->close();
	delete con;
	return ret;
}


// -----------------------------------------------> Select t_user table..

T_USER*	 dbSelect_UserTable(string hp)
{
	sql::Connection *con;
	sql::Statement	*stmt = NULL;
	sql::ResultSet  *res = NULL;

	int	ret=0;
	T_USER*	pUser = NULL;

	char Query[256];
	sprintf(Query,"select * from t_user where user_id='%s';", hp.c_str());
	
	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		ret = res->rowsCount();

		if(res->next())
		{
			pUser = new T_USER;
			pUser->user_id		= hp;

			pUser->nick			= res->getString("nick");
			pUser->user_idx		= res->getInt("user_idx");

			pUser->addr1		= res->getString("addr1");
			pUser->addr2		= res->getString("addr2");
			pUser->dp.lat		= res->getDouble("lat");
			pUser->dp.lng		= res->getDouble("lng");
		}
	}
	catch (sql::SQLException &e) {
		string err = e.what();
		if(err.length() > 0) {
			ret = dbErrorProcess(e.getErrorCode(),err.c_str(),1,Query);
		}
	}

	dbReturn(ret, con, stmt, res);
	return pUser;
}



// -----------------------------------------------> Select t_user table..

int	dbSelect_CouponTable(T_COUPON* pCoupon, char* Query)
{
	sql::Connection *con;
	sql::Statement	*stmt = NULL;
	sql::ResultSet  *res = NULL;

	int	ret=0;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		ret = res->rowsCount();

		if(res->next())
		{
			pCoupon->coupon_idx		= res->getInt("user_idx");

			pCoupon->coupon_code	= res->getString("coupon_code");
			pCoupon->coupon_name	= res->getString("coupon_name");
			pCoupon->yyyymm			= res->getInt("yyyymm");
			pCoupon->dong_idx		= res->getInt("dong_idx");

//			pCoupon->user_idx		= res->getInt("user_idx");
			pCoupon->order_idx		= res->getInt("order_idx");

			pCoupon->made_shop		= res->getInt("made_shop");
			pCoupon->coupon_type	= res->getString("coupon_type");
			pCoupon->coupon_kind	= res->getString("coupon_kind");

			pCoupon->cash_enable	= res->getString("cash_enable");
			pCoupon->start_date		= res->getString("start_date");
			pCoupon->reg_date		= res->getString("reg_date");
			pCoupon->expire_date	= res->getString("expire_date");
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




// -----------------------------------------------> Select t_user table..

int	dbSelect_BannerTable(map<int,T_BANNER*>& vList, char* Query)
{
	sql::Connection *con;
	sql::Statement	*stmt = NULL;
	sql::ResultSet  *res = NULL;

	int	ret=0;

	try	{
		con = dbGetStatement(stmt,Query);
		res = stmt->executeQuery(Query);

		T_BANNER* pBanner;
		for(ret=0; res->next(); ret++)
		{
			int idx = res->getInt("banner_idx");
			auto it = vList.find(idx);
			if(it == vList.end())	{
				pBanner = new T_BANNER;
				vList[idx] = pBanner;
			}
			else	{
				pBanner = it->second;
			}

			pBanner->banner_idx		= idx; 
			pBanner->divide			= res->getInt("divide");
			pBanner->weight			= res->getInt("weight");

			pBanner->shop_idx		= res->getInt("shop_idx");
			pBanner->radius			= res->getInt("radius");
			pBanner->dp.lat			= res->getDouble("lat");
			pBanner->dp.lng			= res->getDouble("lng");
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



