Web-Server
==========

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
