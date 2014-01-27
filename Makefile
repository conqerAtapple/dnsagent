CC=g++
CFLAGS= -I/usr/include/mysql  -g -c -Wall
LDFLAGS= -L/usr/local/lib
LDLIBS= -lmysqlpp -lmysqlclient -lldns

SOURCES=main.cpp \
		Constants.cpp \
		Database.cpp \
		Domain.cpp \
		DNSPing.cpp\
		DNSPinger.cpp \
		DNSResolver.cpp \
		Environment.cpp

OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=dnsagent

all :  $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) :  $(OBJECTS)
		$(CC) $(LDFLAGS) $(LDLIBS) $(OBJECTS) -o $@

.cpp.o :
		$(CC) $(CFLAGS) $< -o $@

clean :
	rm -f *.o; rm -f $(EXECUTABLE)

debug: CFLAGS+= -DDEBUG

debug: $(SOURCES) $(EXECUTABLE)
