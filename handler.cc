#include "handler.h"

/*static*/
bool Handler::Test(ostream & os)
{
    os << "=============Testing Handler ============" << endl;
    bool success = true;
    if (!TestGetFilePath(os)) success = false;
    if (!TestGetFileSize(os)) success = false;
    if (!TestGetLastModified(os)) success = false;
    if (!TestGetContentType(os)) success = false;

    return success;
}

/*static*/
bool Handler::TestGetFilePath(ostream & os)
{
    bool success = true;
    os << "--------------Testing GetFilePath...-------------------" << endl;

    //I just use this to experiment to see how the HTTPRequest parser works.
    string s = string("GET / HTTP/1.1\r\nHost:carmelo.cs.byu.edu\r\nUser-Agent:downloader\r\nFrom: Lexi\r\n\r\n");
    HTTPRequest request;
    request.parse(s);
    cout << "Host: " << request.header(string("Host")) << " URI: " << request.uri() << endl;

    //Set up test object
    Config config;
    config.parse("web.conf");
    Config * p_config = &config;
    Handler h(p_config, 1);

    //Basic test
    TEST(equalStrings(h.GetFilePath(request), "/tmp/web1/index.html"));

    os << endl;
    return success;
}

/*static*/

/*DONE*/
bool Handler::TestGetFileSize(ostream & os)
{
    bool success = true;
    os << "------------- Testing GetFileSize ---------------" << endl;
    int index = open("/home/lexi/Documents/index.html", O_RDONLY);
    int article = open("/home/lexi/Documents/article.pdf", O_RDONLY);
    int image = open("/home/lexi/Documents/image.jpg", O_RDONLY);

    TEST(GetFileSize(index) == 30);
    TEST(GetFileSize(article) == 17);
    TEST(GetFileSize(image) == 5);

    os << endl;
    return success;
}

/*static*/

/*DONE*/
bool Handler::TestGetLastModified(ostream & os)
{
    bool success = true;
    os << "-------------------- Testing GetLastModified ---------------" << endl;

    int index = open("/home/lexi/Documents/index.html", O_RDONLY);
    int article = open("/home/lexi/Documents/article.pdf", O_RDONLY);
    int image = open("/home/lexi/Documents/image.jpg", O_RDONLY);

    os << "index.html\nExpected: Fri 11 Jan 2013 02:31:55 PM MST\nActual: "
	    << GetLastModified(index) << "\n" << endl;
    os << "article.pdf\nExpected: Fri 11 Jan 2013 02:36:23 PM MST\nActual: "
	    << GetLastModified(article) << "\n" << endl;
    os << "image.jpg\nExpected: Fri 11 Jan 2013 02:35:42 PM MST\nActual: "
	    << GetLastModified(image) << "\n" << endl;

    os << endl;
    return success;
}

/*static*/

/*DONE*/
bool Handler::TestGetContentType(ostream & os)
{
    bool success = true;
    os << "------------- Testing GetContentType -----------" << endl;
    //Set up test object
    Config config;
    config.parse("web.conf");
    Config * p_config = &config;
    Handler h(p_config, 1);

    TEST(equalStrings(h.GetContentType("/home/lexi/Documents/index.html"), "text/html"));
    TEST(equalStrings(h.GetContentType("/home/lexi/Documents/article.pdf"), "application/pdf"));
    TEST(equalStrings(h.GetContentType("/home/lexi/Documents/image.jpg"), "image/jpeg"));

    //I assume that if it isn't there, it will simply return an empty string.
    TEST(equalStrings(h.GetContentType(".aoksdjfpaos"), ""));
    os << endl;
    return success;
}

/*DONE*/
Handler::Handler()
{
    //Do nothing
}

/*DONE*/
Handler::Handler(Config * config, int c) : config(config), cache(""),
storedTime(GetCurrentTime()), c(c)
{
    //Do nothing
}

/*DONE*/
Handler::~Handler()
{
    //Do nothing
}

/*DONE*/
Handler::Handler(const Handler & other)
{
    this->cache = other.cache;
    this->storedTime = other.storedTime;
    this->config = other.config;
    this->c = other.c;
}

/*DONE*/
bool Handler::Handle()
{
    //Update time
    storedTime = GetCurrentTime();

    //Read until it would have blocked...
    string str = read_all_available(c, closed);

    //Add it to the current leftovers from last time, if there were any...
    cache.append(str);

    //Parse all the HTTP requests out of it, and handle each one.
    while (HasSubstring(str, "\r\n\r\n"))
    {
	string requestStr = remove_newline(str, "\r\n\r\n");
	HTTPRequest httpRequest;
	httpRequest.parse(requestStr);
	HandleRequest(httpRequest);
    }

    //Let the caller know if the socket is closed or not.
    return closed;
}

/*DONE*/
double Handler::GetTime()
{
    return storedTime;
}

/*DONE* - unless you want extra credit*/
void Handler::HandleRequest(HTTPRequest r)
{
    string method = r.method();
    DEBUG("method: " << method << endl);

    //NOTE: THIS TEST MUST COME FIRST! (Before testing the method)
    //If the method is empty, or the URI is empty, or the host is unknown...
    if (equalStrings(method, "") || equalStrings(r.uri(), "") ||
	    equalStrings(config->host(r.header(string("Host"))), ""))
    {
	//...then it is a Bad request
	HTTPResponse response;
	response.version("HTTP/1.1");
	response.header("Server", "Lexi's Server");
	response.header("Date", GetDate(time(NULL)));
	response.header("Content-Length", "37");
	response.header("Content-Type", "text/html");
	response.code("400");
	response.phrase("Bad Request");
	Send(response.str(), c);
	Send("<html><h1>400 Bad Request</h1></html>", c);
    }

    else if (equalStrings(method, "GET"))
    {
	HandleGETRequest(r);
    }
    else
    {
	//Requested method not implemented
	HTTPResponse response;
	response.version("HTTP/1.1");
	response.header("Server", "Lexi's Server");
	response.header("Date", GetDate(time(NULL)));
	response.header("Content-Length", "41");
	response.header("Content-Type", "text/html");
	response.code("501");
	response.phrase("Not Implemented");
	Send(response.str(), c);
	Send("<html><h1>501 Not Implemented</h1></html>", c);
    }
    return;
}

/*DONE*/
void Handler::HandleGETRequest(HTTPRequest r)
{
    DEBUG("Handling GET request" << endl);
    string filePath = GetFilePath(r);

    DEBUG("Host: " << r.header(string("Host")) << " FilePath: " << filePath << endl);

    int fd = open(filePath.c_str(), O_RDONLY);
    if (fd == -1)
    {
	//Send responses based on different errors
	if (errno == EACCES)
	{
	    HTTPResponse response;
	    response.version("HTTP/1.1");
	    response.header("Server", "Lexi's Server");
	    response.header("Date", GetDate(time(NULL)));
	    response.code("403");
	    response.header("Content-Length", "35");
	    response.header("Content-Type", "text/html");
	    response.phrase("Forbidden");
	    DEBUG("403 Forbidden" << endl);
	    Send(response.str(), c);
	    Send("<html><h1>403 Forbidden</h1></html>", c);
	    close(fd);
	}
	else if (errno == ENOENT)
	{
	    HTTPResponse response;
	    response.version("HTTP/1.1");
	    response.header("Server", "Lexi's Server");
	    response.header("Date", GetDate(time(NULL)));
	    response.header("Content-Length", "35");
	    response.header("Content-Type", "text/html");
	    response.code("404");
	    response.phrase("Not found");
	    DEBUG("404 Not found" << endl);
	    Send(response.str(), c);
	    Send("<html><h1>404 Not Found</h1></html>", c);
	    close(fd);
	}
	else
	{
	    perror("open");
	    SendCode500();
	    close(fd);
	}
    }
    else
    {
	//Call all the calls that might have Code-500 errors.
	string lastModified = GetLastModified(fd);
	if (equalStrings(lastModified, ""))
	{
	    perror("fstat");
	    SendCode500();
	}
	string contentLength = GetContentLength(fd);
	if (equalStrings(contentLength, ""))
	{
	    perror("fstat");
	    SendCode500();
	}
	int fileSize = GetFileSize(fd);
	if (fileSize == -1)
	{
	    perror("fstat");
	    SendCode500();
	}

	//Since they didn't have errors, now use the data to send the OK response
	//and the requested file.
	HTTPResponse response;
	response.version("HTTP/1.1");
	response.header("Server", "Lexi's Server");
	response.header("Date", GetDate(time(NULL)));
	response.code("200");
	response.phrase("OK");
	response.header("Last-Modified", lastModified);
	response.header("Content-Length", contentLength);
	response.header("Content-Type", GetContentType(filePath));
	DEBUG("200 OK" << endl);
	Send(response.str(), c);
	sendfile(c, fd, NULL, fileSize);
	close(fd);
    }
    return;
}

/*DONE*/
string Handler::GetFilePath(HTTPRequest r)
{
    string host = r.header(string("Host"));
    string directory = config->host(host);
    if (equalStrings(directory, ""))
    {
	return "";
    }
    else
    {
	string uri = r.uri();
	if (equalStrings(uri, "/"))
	{
	    uri = "/index.html";
	}
	string filepath = directory.append(uri);
	return filepath;
    }
}

/*static*/
size_t Handler::GetFileSize(int fd)
{
    //Fill in this buffer with information about the file...
    struct stat buf;
    int result = fstat(fd, &buf);
    if (result == -1)
    {
	//Signals that an error occurred.
	return -1;
    }
    //st_size: total size in bytes.
    return buf.st_size;
}

/*static*/
string Handler::GetLastModified(int fd)
{
    //Fill in this buffer with information about the file...
    struct stat buf;
    int result = fstat(fd, &buf);
    if (result == -1)
    {
	//Signals that an error occurred.
	return "";
    }
    //st_mtime: time of last modification
    return GetDate(buf.st_mtime);
}

/*static*/
string Handler::GetContentType(string fileName)
{
    //Plus one so it is pointing after the period.
    unsigned int pos = fileName.find_last_of(".") + 1;

    //Make sure we didn't go off the edge
    assert(fileName.size() > pos);

    string extension = fileName.substr(pos, fileName.size() - pos);
    return config->media(extension);
}

/*static*/
string Handler::GetContentLength(int fd)
{
    //Fill in this buffer with information about the file...
    struct stat buf;
    int result = fstat(fd, &buf);
    if (result == -1)
    {
	//Signals that an error occurred.
	return "";
    }

    //st_size: total size in bytes.
    return itoa(buf.st_size, 10);
}

void Handler::SendCode500()
{
    HTTPResponse response;
    response.version("HTTP/1.1");
    response.header("Server", "Lexi's Server");
    response.header("Date", GetDate(time(NULL)));
    response.code("500");
    response.phrase("Internal Server Error");
    response.header("Content-Length", "47");
    response.header("Content-Type", "text/html");
    Send(response.str(), c);
    DEBUG("Internal Server Error" << endl);
    Send("<html><h1>500 Internal Server Error</h1></html>", c);
    // Exit the program
    exit(-1);
    return;
}