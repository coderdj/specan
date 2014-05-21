# Makefile for spectral analyzer

CC		= g++
CFLAGS		= -I/usr/local/include/pbf $(shell root-config --cflags) -Wall -g -DLINUX -fPIC
LDFLAGS         = -L/usr/local/lib $(shell root-config --libs) $(shell root-config --glibs) -lpbf -lsnappy -lprotobuf -lpthread -lssl -lcrypto
SOURCES         = $(shell echo ./*cc)
OBJECTS         = $(SOURCES: .cc=.o)
CPP             = specan

all: $(SOURCES) $(CPP)

$(CPP) : $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) $(LDFLAGS) -o $(CPP)

clean:	
	rm $(CPP)


