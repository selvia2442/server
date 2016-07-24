TARGET  = quiz20
OBJS    = wApp.o wLib.o _Mysql.o _util.o
CC      = g++
RM	= rm -f
LINK	= -lpthread -lmysqlcppconn
CPPFLAGS= -c -g  -std=c++11

all: $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LINK)

wApp.o:wApp.cpp
	$(CC) $(CPPFLAGS) wApp.cpp

wLib.o:wLib.cpp
	$(CC) $(CPPFLAGS) wLib.cpp

_Mysql.o:_Mysql.cpp
	$(CC) $(CPPFLAGS) _Mysql.cpp

_util.o:_util.cpp
	$(CC) $(CPPFLAGS) _util.cpp

clean: 
	@$(RM) $(OBJS) $(TARGET)
	@echo "화일을 삭제했습니다."
