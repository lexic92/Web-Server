#include "client.h"

int main(int argc, char **argv)
{
    //VARIABLES
    string IPAddressStr; //The IP Address of the machine name given on the command line.
    int s; //The client socket file descriptor
    int port; //The port to connect to on the server
    string cache = ""; //The cache for the read function calls

    //Get arguments
    ProcessCommandLineArgs(argc, argv, port, IPAddressStr);
    DEBUG("Port: " << port << endl);
    DEBUG("IPAddressStr: " << IPAddressStr << endl);

    // create socket
    s = socket(PF_INET, SOCK_STREAM, 0);
    if (s < 0)
    {
	perror("socket");
	exit(-1);
    }

    // setup socket address structure and connect to server
    struct sockaddr_in server;
    memset(&server, 0, sizeof (server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    if (!inet_aton(IPAddressStr.c_str(), &server.sin_addr))
    {
	perror("inet_addr() conversion error");
	exit(-1);
    }
    if (connect(s, (const struct sockaddr *) &server, sizeof (server)) < 0)
    {
	perror("connect");
	exit(-1);
    }

    //PARSE USER INPUT AND DO SOMETHING ABOUT IT
    cout << "Press enter to send a pretedermined message." << endl;
    while (1)
    {
	bool shouldBreak = false;
	HandleUserInput(shouldBreak, s, cache);
	if(shouldBreak)
	{
	    break;
	}
	
    }
    
    DEBUG("Socket closed");
    // Close socket
    close(s);
    return 0;
}

/*DONE*/
void ProcessCommandLineArgs(int argc, char ** argv, int & port, string & IPAddressStr)
{
    int option;
    port = 8080; //Default.
    debug = false; //Default.
    string machineName = ""; //Default to signal to use the loopback address.

    // process command line options using getopt()
    // see "man 3 getopt"
    while ((option = getopt(argc, argv, "p:s:d")) != -1)
    {
	switch (option)
	{
	case 'p':
	    port = atoi(optarg);
	    break;
	case 'd':
	    debug = true;
	    break;
	case 's':
	    machineName = optarg;
	    break;
	default:
	    //They had an option that wasn't specified
	    cout << "client [-p port] [-s server] [-d]" << endl;
	    perror("commandlineargs");
	    exit(EXIT_FAILURE);
	}
    }
    //If there are no more option characters, getopt() returns -1.
    if (equalStrings(machineName, ""))
    {
	IPAddressStr = "127.0.0.1";
    }
    else
    {
	IPAddressStr = hostNameToIPAddress(machineName);
    }
    return;
}

/*DONE*/
void HandleUserInput(bool & shouldBreak, int s, string & cache)
{
	//Get input
	shouldBreak = false;
	string userInput;
	getline(cin, userInput);
	
	//Send request
	DEBUG("Sending message" << endl);
	string request = "A /  HTTP/1.1\r\nHost:localhost\r\nUser-Agent:client\r\nFrom: Lexi\r\n\r\n";
	Send(request, s);
	PrintToFile("ClientOUT.txt", request);
	
	//Read Response
	DEBUG("Reading response" << endl);
	string response = read_newline(s, "\r\n\r\n", cache);
	HTTPResponse r;
	r.parse(response);
	string document = read_length(s, atoi(r.header("Content-Length").c_str()), cache);
	PrintToFile("ClientIN.txt", response.append(document));
	
	//Break if the socket is closed
	if(equalStrings(response, ""))
	{
	    shouldBreak = true;
	}
	return;
}