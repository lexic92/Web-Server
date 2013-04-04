#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <iostream>
#include "Utils.h"
#include "HTTPRequest.h"
#include "URL.h"
#include "Config.h"
#include "Utils.h"
#include "HTTPResponse.h"
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <fcntl.h>



class Handler
{
public:
    
    /**
     * Tests this class
     */
    static bool Test(ostream & os);
    
    //Testing sub-methods
    static bool TestGetFilePath(ostream & os);
    static bool TestGetFileSize(ostream & os);
    static bool TestGetLastModified(ostream & os);
    static bool TestGetContentLength(ostream & os);
    static bool TestGetContentType(ostream & os);
    
    /**
     * Required for stl map.
     */
    Handler();
    
    /**
     * @param config pointer to the parsed config file.
     * @param c the client file descriptor
     * @post time is initialized to the current time
     * @post cache is initialized to ""
     * @post config is initialized to config
     * @post c is intialized to c
     */
     Handler(Config * config, int c);

    /**
     * Dtor: Doesn't need to do anything.
     */
    ~Handler();

    /**
     * Copy Ctor
     * @post this->c = other.c
     * @post this->cache = other.cache
     * @post this->time = other.time
     * @post this->config = other.config
     */
    Handler(const Handler & other);

    /**
     * Handle a client request.
     *
     * @return true if the socket is closed, else false.
     */
    bool Handle();

    /**
     * Gets the stored time.
     */
    double GetTime();
    
   /**
    * @pre fd is a file descriptor of a file.
    * @return the size of the given file, in integer form, or -1 if an error
    * occurred.
    */
   static size_t GetFileSize(int fd);
   
   /**
    * @pre fd is a file descriptor of a file.
    * @return the date the file was last modified, according to the date
    * format of HTTP 1.1, or "" if an error occurred.
    */
    static string GetLastModified(int fd);
    
    /**
     * Gets the content type for a given file by looking at the file's
     * extension. The fileName can have any amount of characters before
     * the extension.
     * 
     * @pre fileName has an extension (.txt, etc.)
     * @pre there is at least one character after the last period in the string.
     * @param fileName the name of the file
     * @return the content type of the file, according to the config file.
     */
    string GetContentType(string fileName);
    
    /**
     * @pre fd is a file descriptor of a file.
     * @return the size of the given file, in bytes, in string form, or "" if
     * an error occurred.
     */
    static string GetContentLength(int fd);
    
private:
    /**
     * The flag if the client socket is closed or not.
     */
    bool closed;
    
    /**
     * The file descriptor of the client it will handle.
     */
    int c;
    
    /**
     * The cache for this handler's associated socket.
     */
    string cache;
    
    /**
     * The time at which the "handle" method was most recently called.
     * If it has never been called before, it will return the time its 
     * constructor was made.
     */
    double storedTime;
    
    /**
     * A pointer to the object storing the parsed configuration settings.
     */
    Config * config;
    
    /**
     * Handles a HTTP request.
     * 
     * @param r the request to handle.
     */
    void HandleRequest(HTTPRequest r);
    
     /**
     * Handles a GET request.
      * 
      * @pre r's method is GET.
      * @param r the request to handle.
     */
    void HandleGETRequest(HTTPRequest r);
    
    /**
     * Sends a HTTPResponse with a code-500
     * Internal Server Error, and signals that the program is true.
     * @post closed = true
     */
    void SendCode500();
    
     /**
     * Converts the request r into an absolute pathname of the requested file
     * on the server's machine.
      * 
      * @return the absolute pathname of the requested file, or the empty string
      * if the config file does not have a directory associated with the 
      * specified host.
     */
   string GetFilePath(HTTPRequest r);
};

