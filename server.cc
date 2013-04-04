#include "server.h"

/*DONE*/
int main(int argc, char **argv)
{
    //My variables:
    int port;
    map<int, Handler> handlerMap;
    
    //Set up Config-related variables:
    Config config;
    Config * p_config = &config;
    config.parse("web.conf");
    int timeout = atoi(config.parameter("timeout").c_str());
    
    //Process Command line args
    ProcessCommandLineArgs(argc, argv, port);

    //Run test driver instead of actual program if it is indicated.
    if (test)
    {
	TestEverything();
	return 0;
    }


    //Set up listening socket
    int s = SetUpListeningSocket(port);
    DEBUG("listening socket: " << s << endl);

    //Make epoller
    int epfd = epoll_create(1);

    //Add listening socket to poller
    AddSocketToPoller(epfd, s);

    //LOOP FOREVER.
    while (1)
    {
	// Do poll.
	struct epoll_event events[MAX_EVENTS];
	int numberOfReadySockets = DoPoll(epfd, events, MAX_EVENTS);

	//For each index of ready sockets...
	for (int i = 0; i < numberOfReadySockets; i++)
	{
	    //Get the ready socket's fd and handle the listening socket one way,
	    //and the client sockets a different way.
	    int readySocket = events[i].data.fd;
	    if (readySocket == s)
	    {
		//Accept new client
		int c = accept(s, NULL, NULL);
		if (c < 0)
		{
		    perror("accept");
		    //Poll again.
		    break;
		}
		DEBUG("Accepted new client: " << c << endl);

		//Add new client to the handler map and to the epoller.
		AddSocketToPoller(epfd, c);
		Handler h(p_config, c); //<----- gets the current time set here.
		handlerMap[c] = h;
		DEBUG("Handler map size: " << handlerMap.size() << endl);
	    }
	    else
	    {
		HandleReadySocket(readySocket, handlerMap, epfd, config);
		//^--- sets current time here
	    }
	}
	//Time out idle sockets
	TimeOutSockets(epfd, handlerMap, timeout);
    }
}

/*DONE*/
void ProcessCommandLineArgs(int argc, char ** argv, int & port)
{
    int option;
    port = 8080; //Default.

    // process command line options using getopt()
    // see "man 3 getopt"
    while ((option = getopt(argc, argv, "p:dt")) != -1)
    {
	switch (option)
	{
	case 'p':
	    port = atoi(optarg);
	    if (port == 0)
	    {
		cout << "server -p port [-d]" << endl;
		perror("commandLineArgs");
		exit(EXIT_FAILURE);
	    }
	    break;
	case 'd':
	    debug = true; //global variable.
	    break;
	case 't':
	    test = true; //global variable
	    break;
	default:
	    //They had an option that wasn't specified
	    cout << "server -p port [-d]" << endl;
	    perror("commandLineArgs");
	    exit(EXIT_FAILURE);
	}
    }
    //If there are no more option characters, getopt() returns -1.
}

/*DONE*/
int SetUpListeningSocket(int port)
{
    //Variables:
    struct sockaddr_in server;
    int s;

    // setup socket address structure
    memset(&server, 0, sizeof (server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    // create socket
    s = socket(PF_INET, SOCK_STREAM, 0);
    if (!s)
    {
	perror("socket");
	exit(-1);
    }

    // set socket to immediately reuse port when the application closes
    int opt = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof (opt)) < 0)
    {
	perror("setsockopt");
	exit(-1);
    }

    // call bind to associate the socket with our local address and
    // port
    if (bind(s, (const struct sockaddr *) &server, sizeof (server)) < 0)
    {
	perror("bind");
	exit(-1);
    }

    // convert the socket to listen for incoming connections
    if (listen(s, SOMAXCONN) < 0)
    {
	perror("listen");
	exit(-1);
    }
    return s;
}


/*DONE*/
int DoPoll(int epfd, struct epoll_event * events, int maxEvents)
{
    int nfds = epoll_wait(epfd, events, maxEvents, 1);
    if (nfds < 0)
    {
	perror("epoll");
	exit(EXIT_FAILURE);
    }
    return nfds;
}

/*DONE*/
void HandleReadySocket(int readySocket, map<int, Handler> & handlerMap, int epfd, Config config)
{
    //Assert that the readySocket does exist in the handler map.
    assert(handlerMap.find(readySocket) != handlerMap.end());

    //Handle the socket.
    bool closed = handlerMap[readySocket].Handle();

    //If the socket is closed, remove it from the epoller, close the socket,
    //and erase it & its handler from the handler map.
    if (closed)
    {
	DEBUG("Boot off client: " << readySocket << endl);
	BootOffClient(epfd, readySocket, handlerMap);
    }
}

/*DONE*/
void TimeOutSockets(int epfd, map<int, Handler> & handlerMap, int timeout)
{
    double currentTime = GetCurrentTime();
    
    //Make a list of all the sockets that are timed out.
    list<int> timedOutFds;
    for (map<int, Handler>::iterator it = handlerMap.begin(); it != handlerMap.end(); it++)
    {
	//If it's been idle too long, add it to the list
	double mostRecentHandledTime = (*it).second.GetTime();
	if ((currentTime - mostRecentHandledTime) > timeout)
	{
	    DEBUG("Delay too long: " << (currentTime - mostRecentHandledTime) << endl);
	    timedOutFds.push_back((*it).first);   
	}
    }
    
    //Go through the list of timed-out sockets and boot off each one
    for(list<int>::iterator it = timedOutFds.begin(); it != timedOutFds.end(); it++)
    {
	BootOffClient(epfd, (*it), handlerMap);
    }
}

/*DONE*/
void BootOffClient(int epfd, int c, map<int, Handler> & handlerMap)
{
    RemoveSocketFromPoller(epfd, c);
    close(c);
    handlerMap.erase(c);
}

/*DONE*/
void RemoveSocketFromPoller(int epfd, int removeThisSocket)
{
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = removeThisSocket;
    epoll_ctl(epfd, EPOLL_CTL_DEL, removeThisSocket, &ev);
}

/*DONE*/
void AddSocketToPoller(int epfd, int addThisSocket)
{
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = addThisSocket;
    epoll_ctl(epfd, EPOLL_CTL_ADD, addThisSocket, &ev);
}

/*INCOMPLETE*/
void TestEverything()
{
    bool success = true;

    if (!Config::Test(cout)) success = false;
    if (!TestUtils(cout)) success = false;
    if (!Handler::Test(cout)) success = false;  
    
    cout << "\n\n";
    if (success)
    {
	cout << "Tests Succeeded!" << endl;
    }
    else
    {
	cout << "Tests Failed!" << endl;
    }
}


