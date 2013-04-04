# Makefile for CS 360 example code

CXX=	g++ -g -Wall -lrt
OBJS =	server.o Utils.o handler.o HTTPRequest.o HTTPResponse.o URL.o Config.o Tokenizer.o client.o

LIBS=

CCFLAGS= -g

all:	server client

server: server.o Utils.o handler.o HTTPRequest.o HTTPResponse.o URL.o Config.o Tokenizer.o
	$(CXX) -o web server.o Utils.o handler.o HTTPRequest.o HTTPResponse.o URL.o Config.o Tokenizer.o $(LIBS)

client: client.o Utils.o handler.o HTTPRequest.o HTTPResponse.o URL.o Config.o Tokenizer.o
	  $(CXX) -o client client.o Utils.o handler.o HTTPRequest.o HTTPResponse.o URL.o Config.o Tokenizer.o $(LIBS)

clean:
	rm -f $(OBJS) $(OBJS:.o=.d)

realclean:
	rm -f $(OBJS) $(OBJS:.o=.d) msgd test


# These lines ensure that dependencies are handled automatically.
%.d:	%.cc
	$(SHELL) -ec '$(CC) -M $(CPPFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'

include	$(OBJS:.o=.d)
