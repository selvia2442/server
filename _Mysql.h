extern	int  query_display;


extern	void db_driver_reconnect();
extern	void dbIdleCheck();

extern	int	 dbQuery(char* Query);
extern	int  dbInsertQuery(char* Query);
extern	int	 dbInsertQuery_Index(char* Query);

extern	int  dbUpdateQuery(char* Query);
extern  int	 dbDeleteQuery(char* Query);
extern	int	   dbSelect_Index(char* Query);
extern	string  dbSelect_String(char* Query);

extern	int  dbSelect_Confirm(char* Query);
extern	int dbSelect_Position(Z_POS* pPos);


