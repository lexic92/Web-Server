/**
    LAB 4 - WEB SERVER
    A web server in C++ that implements some of the HTTP 1.1 specification given in RFC 2616.
    Uses a scalable event-driven server architecture using the epoll interface to manage a
    set of sockets in a single thread.

    Syntax:
	web -p [port] -d -t
	(default port is 8080.  -d turns debug on. -t runs the test driver INSTEAD of the server.)

    Features:
	1. GET method
	2. multiple requests on one TCP connection (In accordance with HTTP 1.1)
	3. multiple hosts on one web server (In accordance with HTTP 1.1)
	4. serves "/index.html" if the URI given is "/"
	5. These headers: (ignores other headers)
		Date (using RFC 822 / RFC 1123 format dates)
		Server
		Content-Type
		Content-Length
		Last-Modified (using RFC 822 / RFC 1123 format dates)
	6. One listening socket that handles all clients' requests
	7. Closes connection when client closes it
	8. Closes connection when it times out
	9. Timeout second can be set in .config file
	10. Determines content-type of a file using its extension. (extensions must be defined in the .config file.)
	11. These response codes:
		200 OK
		400 Bad Request : if the web server parses the request and the method or URI is empty, or if the request contains a host that this server doesn't handle
		403 Forbidden: if the web server does not have permission to open the requested file
		404 Not Found: if the web server cannot find the requested file
		500 Internal Server Error: if the web server encounters any other error when trying to open a file
		501 Not Implemented: if the web server does not implement the requested method
	12. Reads in a config file with any number of lines of the following syntaxes:
		host [name] [path]          //where the document root is located for a given host. This path can be either relative "web1" or absolute "/tmp/web1". A relative path starts in the current directory, whereas an absolute path starts in the root directory.
		media [ext] [type]          //The MIME type to use in the Content-Type header for a document that ends in the given extension. If a document has no extension, or if its extension is not listed in the configuration file, then your server should treat it as a text/plain file.
		parameter [name] [value]    //time, in seconds, that a socket is allowed to be idle before it will be closed.
	13. Separate cache for each socket
	14. Configures each client to use non-blocking socket I/O, and continues to call recv() on the client until it returns an error code indicating that no data is available.


    Extra credit: (NOT YET IMPLEMENTED)
	1. HEAD and range requests (extra credit)

 */

#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <iostream>
#include "Utils.h"
#include "handler.h"
#include <list>
#include <map>
#include <string>
#include "Config.h"
#include <iterator>

#define MAX_EVENTS 1000


//----------------------------- VARIABLES --------------------------
/**
 * Has to be global because other files use debugging too. Default: false.
 */
bool debug = false;

/**
 * For testing
 */
bool test = false;


//------------------------------ FUNCTIONS --------------------------
/**
    The whole server.
    - Processes command line arguments
    - If the -t flag is on, runs a test driver and exits the program
    - Parses the configuration file
 * - Makes an epoller with a listening socket.
 *  Polls the epoller continuously, accepting and handling clients
 *  and timing them out.
 */
int main(int argc, char **argv);

/**
 * Process command line args. Port cannot be 0.
 * Default values: port = 8080, debug = off.
 * Syntax: server -p port -d -t
 * -d: debug on
 * -t: run the test driver instead of running the program.
 * @param argc same as main's parameter.
 * @param argv same as main's parameter.
 * @param port the variable that will store the port argument.
 */
void ProcessCommandLineArgs(int argc, char ** argv, int & port);

/**
 * Creates a listening socket, sets it up with setsockopt, bind, and listen.
 * @param port the port of the listening socket.
 * @return the file descriptor of the listening socket.
 */
int SetUpListeningSocket(int port);

/**
    Calls epoll_wait with the given parameters.
    However, the call will always wait for a maximum of 1 millisecond.

    ----------- FROM EPOLL_WAIT MANUAL PAGE: -----------

    int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);

    The epoll_wait() system call waits for events on the epoll
    instance  referred  to  by  the file descriptor epfd.  The
    memory area pointed to by events will contain  the  events
    that  will  be  available for the caller.  Up to maxevents
    are returned by epoll_wait().  The maxevents argument must
    be greater than zero.

    The call waits for a maximum time of timeout milliseconds.
    Specifying a timeout of -1 makes epoll_wait() wait indefi‐
    nitely,  while  specifying  a  timeout equal to zero makes
    epoll_wait() to return immediately even if no events  are
    available (return code equal to zero).

    @param epfd the file descriptor of the epoll instance you want to wait for events on.
    @param events will contain the events that will be available for the caller
    @param maxEvents the maximum number of events returned by epoll_wait().
    @return the number of file descriptors ready for the requested I/O
 */
int DoPoll(int epfd, struct epoll_event * events, int maxEvents);

/**
    Handles the request of the socket. If it's closed, it closes the socket and removes it
    from the handlerMap and the epoller.
    @pre readySocket is one of the sockets that the epoller listed as being ready.
    @param readySocket the socket to handle.
    @param handlerMap the map to look up the readySocket's handler in.
    @param epfd the epoller's file descriptor.
 */
void HandleReadySocket(int readySocket, map<int, Handler> & handlerMap, int epfd, Config config);

/**
 * Compares each socket in the handler map's most recent time it was handled
 * with the current time. If the difference is greater than the timeout
 * (aka it was idle too long), then it boots off the client.
 */
void TimeOutSockets(int epfd, map<int, Handler> & handlerMap, int timeout);

/**
 * Does 3 things necessary when you boot off a client:
 * 1. removes client socket from epoller
 * 2. Closes client socket
 * 3. removes client's handler from handlermap.
 * 
 * @param epfd the epoller
 * @param c the client socket fd
 * @param handlerMap the handler map
 */
void BootOffClient( int epfd, int c, map<int, Handler> & handlerMap);

/**
 * Removes the given socket from the given epoll file descriptor.
 * @param epfd the epoll file descriptor
 * @param removeThisSocket socket to remove
 */
void RemoveSocketFromPoller(int epfd, int removeThisSocket);

/**
 * Adds the given socket to the given epoll file descriptor.
 * 
 * 
    (epollFileDescriptor, operationToBePerformedOnTargetFileDescriptor,
    targetFileDescriptor, epollEventData)

    ----------- FROM EPOLL_CTL MANUAL PAGE: -----------

    int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
    This system call performs control operations on the  epoll
    instance  referred  to  by  the  file descriptor epfd.  It
    requests that the operation op be performed for the target
    file descriptor, fd.
    The event argument describes the object linked to the file
    descriptor fd.  The struct epoll_event is defined as :

	   typedef union epoll_data {
	       void        *ptr;
	       int          fd;
	       uint32_t     u32;
	       uint64_t     u64;
	   } epoll_data_t;

	   struct epoll_event {
	       uint32_t     events;       Epoll events
	       epoll_data_t data;         User data variable
	   };

    The  events member is a bit set composed using the available event types.

    EPOLL_CTL_ADD
    Register the target file descriptor fd on the epoll
    instance referred to by the  file  descriptor  epfd
    and  associate  the  event  event with the internal
    file linked to fd.

 * @param epfd the epoll file descriptor
 * @param addThisSocket targetFileDescriptor, which will be added to the epoller.
 */
void AddSocketToPoller(int epfd, int addThisSocket);


/**
    Test driver for the other cpp files in this project.
 */
void TestEverything();


#endif // SERVER_H
