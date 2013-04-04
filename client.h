#ifndef CLIENT_H
#define CLIENT_H

#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include "Utils.h"
#include "HTTPResponse.h"

using namespace std;

/**
 * Debug flag. If it is set to true, then the DEBUG-macro-statements will print out.
 */
bool debug;

/**
 * Sets up a client that connects to the machine name arg specified by -s and port arg specified by -p
 * with an optional debug option (-d).  Then it reads input from the user and handles it somehow, specified
 * by the function HandleUserInput.
 */
int main(int argc, char **argv);

/**
 * 
 * 
 * @param argc the argc argument passed to "main" from the terminal
 * @param argv the argv argument passed to "main" from the terminal
 * @param port reference to the int which will be set with the port to which the client will connect.
 * @param IPAddressStr where to store the IP Address of the machine name
 * @post if the -d flag exists, debug will be set to true. Otherwise, it will be set to false.
 * @post the port number from "-p [port]" will be stored in port.  If not specified, port will
 * be set to a default value of 3000.
 */
void ProcessCommandLineArgs(int argc, char ** argv, int & port, string & IPAddressStr);

/**
 * Reads from the socket and handles it.
 * Currently: Sends a GET request to the server whenever the user presses enter.
 * Reads response
 * 
 * @param shouldBreak will be set to true if it should break, false otherwise.
 * @param s the socket of the client
 * @param cache the cache of the client
 */
void HandleUserInput(bool & shouldBreak, int s, string & cache);

#endif	/* CLIENT_H */