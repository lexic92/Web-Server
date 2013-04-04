#include "Utils.h"

bool TestUtils(ostream & os)
{
    bool success = true;
    os << " ========== TESTING Utils.cc ===============" << endl;
    if (!TestHasSubstring(os)) success = false;
    if (!Testremove_newline(os)) success = false;
    if (!TestGetDate(os)) success = false;

    return success;
}

/*DONE*/
bool TestHasSubstring(ostream & os)
{
    bool success = true;

    os << "Testing hasSubstring..." << endl;
    //1. Basic test
    string hello = "hello";
    string lo = "lo";
    TEST(HasSubstring(hello, lo));

    //2. Testing that the function even works on empty strings.
    //An empty string has no substrings at all.
    string emptyString = "";
    TEST(!HasSubstring(emptyString, lo));

    //3. Testing a case when it fails with a completely different character
    string a = "a";
    TEST(!HasSubstring(hello, a));

    //4. Fails because it checks for the WHOLE substring, not just a part.
    string elloo = "elloo";
    TEST(!HasSubstring(hello, elloo));

    return success;
}

bool Testremove_newline(ostream & os)
{
    bool success = true;
    os << "Testing remove_newline..." << endl;
    //1. Test - only takes first occurrence of sentinel
    string hello = "hello";
    string result = remove_newline(hello, "l");
    TEST(equalStrings(hello, "lo"));
    TEST(equalStrings(result, "hel"));

    //2. Other test - if sentinel is not there, then assertions go off
    string i = "i";
    //remove_newline(hello, i);
    //should crash - and it did.

    //3. Test really long thing
    string longString = "aaaaaaaaaaabbbbbbcccccccddddddeeeeeeeeefffffffffggggggg";
    result = remove_newline(longString, "dddeeeeeeeee");
    TEST(equalStrings(longString, "fffffffffggggggg"));
    TEST(equalStrings(result, "aaaaaaaaaaabbbbbbcccccccddddddeeeeeeeee"));

    //4. Test one where the result is the empty 
    hello = "hello";
    result = remove_newline(hello, "o");
    TEST(equalStrings(result, "hello"));
    TEST(equalStrings(hello, ""));

    return success;
}

bool TestGetDate(ostream & os)
{
    bool success = true;

    os << "Testing GetDate..." << endl;
    //Pass in time(NULL) for current time
    string now = GetDate(time(NULL));
    os << "Curent time\nexpected: check your system's clock\nactual: "
	    << now << endl << endl;

    return success;
}

/*DONE*/
bool equalStrings(string s1, string s2)
{
    if (s1.compare(s2) == 0)
    {
	return true;
    }
    else
    {
	return false;
    }
}

/*DONE*/
string firstWord(const string & str)
{
    assert(!isspace(str.at(0)));

    string whitespaceChars = " \t\n\v\f\r";
    size_t whitespaceIndex = str.find_first_of(whitespaceChars);
    assert(whitespaceIndex != string::npos);

    return str.substr(0, whitespaceIndex);
}

/*DONE*/
string secondWord(const string & str)
{
    assert(!isspace(str.at(0)));

    string whitespaceChars = " \t\n\v\f\r";
    //abc def\n
    //012 456 7
    //firstWSIndex = 3
    size_t firstWSIndex = str.find_first_of(whitespaceChars, 0);
    assert(firstWSIndex != string::npos);

    //firstNonWSIndex = 4
    size_t firstNonWSIndex = str.find_first_not_of(whitespaceChars, firstWSIndex);
    assert(firstNonWSIndex != string::npos);

    //secondWSIndex = 7
    size_t secondWSIndex = str.find_first_of(whitespaceChars, firstNonWSIndex);
    assert(secondWSIndex != string::npos);

    //starts on and includes index 4, and the length is 7-4 = 3.
    return str.substr(firstNonWSIndex, secondWSIndex - firstNonWSIndex);
}

/*DONE*/
string thirdWord(const string & str)
{
    assert(!isspace(str.at(0)));

    string whitespaceChars = " \t\n\v\f\r";
    //abc def hij\n
    //012 456 8910 11

    //firstWSIndex = 3
    size_t firstWSIndex = str.find_first_of(whitespaceChars, 0);
    assert(firstWSIndex != string::npos);

    //firstNonWSIndex = 4
    size_t firstNonWSIndex = str.find_first_not_of(whitespaceChars, firstWSIndex);
    assert(firstNonWSIndex != string::npos);

    //secondWSIndex = 7
    size_t secondWSIndex = str.find_first_of(whitespaceChars, firstNonWSIndex);
    assert(secondWSIndex != string::npos);

    //secondNonWSIndex = 8
    size_t secondNonWSIndex = str.find_first_not_of(whitespaceChars, secondWSIndex);
    assert(secondNonWSIndex != string::npos);

    //thirdWSIndex = 11
    size_t thirdWSIndex = str.find_first_of(whitespaceChars, secondNonWSIndex);
    assert(thirdWSIndex != string::npos);

    //starts on and includes index 8, and the length is 11-8 = 3.
    return str.substr(secondNonWSIndex, thirdWSIndex - secondNonWSIndex);
}

/*DONE*/
string fourthWord(const string & str)
{
    assert(!isspace(str.at(0)));

    string whitespaceChars = " \t\n\v\f\r";
    //abc def hij     k  l  \n
    //012 456 8910 11 12-13 14
    //    * first nonWS
    //        * second nonWS
    //                *third nonWS
    //firstWSIndex = 3
    size_t firstWSIndex = str.find_first_of(whitespaceChars, 0);
    assert(firstWSIndex != string::npos);

    //firstNonWSIndex = 4
    size_t firstNonWSIndex = str.find_first_not_of(whitespaceChars, firstWSIndex);
    assert(firstNonWSIndex != string::npos);

    //secondWSIndex = 7
    size_t secondWSIndex = str.find_first_of(whitespaceChars, firstNonWSIndex);
    assert(secondWSIndex != string::npos);

    //secondNonWSIndex = 8
    size_t secondNonWSIndex = str.find_first_not_of(whitespaceChars, secondWSIndex);
    assert(secondNonWSIndex != string::npos);

    //thirdWSIndex = 11
    size_t thirdWSIndex = str.find_first_of(whitespaceChars, secondNonWSIndex);
    assert(thirdWSIndex != string::npos);

    //thirdNonWSIndex = 12
    size_t thirdNonWSIndex = str.find_first_not_of(whitespaceChars, thirdWSIndex);
    assert(thirdNonWSIndex != string::npos);

    //fourthWSIndex = 14
    size_t fourthWSIndex = str.find_first_of(whitespaceChars, thirdNonWSIndex);
    assert(fourthWSIndex != string::npos);

    //starts on and includes index 12, and the length is 14-12 = 2.
    return str.substr(thirdNonWSIndex, fourthWSIndex - thirdNonWSIndex);
}

/*DONE*/
void Send(string message, int socket)
{
    //Leave early if it's trying to send an empty string.
    if (message.compare("") == 0)
    {
	DEBUG("Tried to send empty string." << endl);
	return;
    }

    const char * ptr = message.c_str();
    int nleft = message.size();
    int nwritten = -1;

    //While you have more of the message to send... keep sending.
    while (nleft)
    {
	//"send":  On success, it returns  the  number  of  characters  sent. On
	//error, -1 is returned, and errno is set appropriately.
	if ((nwritten = send(socket, ptr, nleft, 0)) < 0)
	{
	    if (errno == EINTR)
	    {
		// the socket call was interrupted -- try again
		continue;
	    }
	    else
	    {
		// an error occurred, so break out
		perror("write");
		break;
	    }
	}
	else if (nwritten == 0)
	{
	    // the socket is closed
	    break;
	}
	//Update the number of characters left so the while loop knows when to stop
	nleft -= nwritten;

	//Update the pointer to point at the next part of the string that you need to send.
	ptr += nwritten;
    }
}

/*DONE*/
string read_newline(int socket, string sentinel, string & cache)
{
    //I make the assumption that a char is one byte.
    assert(sizeof (char) == 1);
    //Sentinel cannot be empty or it's useless.
    assert(sentinel.compare("") != 0);


    //------------- VARIABLES -------------------------
    int numberBytesRead = 0;
    size_t sentinelPosition = 0;


    //Allocate a buffer
    char * buf = (char *) calloc(1024, sizeof (char));
    //------------------------------------------------

    //Loop forever until you reach the sentinel (then return it) or there
    //was an error or the socket closed (which returns "").
    while (1)
    {
	//If we've reached the sentinel, then return the the message up to and
	//including the sentinel, and leave the remaining characters (if 
	//applicable) in the static "message" string for next time.

	//NOTE: "find" returns the position of the first occurrence in the string of the searched content.
	//If the content is not found, the member value npos is returned.
	//(If the sentinel is more than one character, it returns the position [index starting at 0] of the
	//first character of the sentinel.)
	sentinelPosition = cache.find(sentinel);

	//If the sentinel has been reached...
	if (sentinelPosition != string::npos)
	{
	    //substr(pos, n): character sequence that starts at character position 
	    //pos and has a length of n characters.
	    int totalLengthOfMessage = sentinel.size() + sentinelPosition;
	    string returnString = cache.substr(0, totalLengthOfMessage);

	    cache = cache.substr(totalLengthOfMessage, cache.size() - totalLengthOfMessage);

	    //Free up the allocated space and null the invalid pointer for good habit.
	    free(buf);
	    buf = NULL;
	    return returnString;
	}

	//About "recv": Returns the # of bytes received, or -1 if an error
	//occurred. Returns 0 when the peer has performed an orderly shutdown. 
	//If  no messages are available at the socket, the receive calls wait for
	//a message to arrive, unless the socket is non-blocking.

	//PUT CHARACTERS FROM SOCKET INTO THE BUFFER...
	//1024 is an arbitrary decision.
	numberBytesRead = recv(socket, buf, 1024, 0);

	if (numberBytesRead < 0)
	{
	    if (errno == EINTR)
	    {
		//the socket call was interrupted -- try again
		//Free up the allocated space and null the invalid pointer for good habit.
		free(buf);
		buf = NULL;
		DEBUG("continue" << endl);
		continue;
	    }
	    else
	    {
		//an error occurred, so break out
		//Free up the allocated space and null the invalid pointer for good habit.
		free(buf);
		buf = NULL;
		DEBUG("Error: exit the program" << endl);
		perror("recv");
		exit(-1);
	    }
	}
	else if (numberBytesRead == 0)
	{
	    //the socket is closed
	    //Free up the allocated space and null the invalid pointer for good habit.
	    free(buf);
	    buf = NULL;
	    DEBUG("Socket closed: return \"\"" << endl);
	    return "";
	}

	//APPEND THE BUFFER CHARS (THAT WERE JUST READ) TO THE MESSAGE
	//Appends a copy of the string formed by the first n characters in the array
	//of characters pointed by s. (const char * s, size_t n)
	cache.append(buf, numberBytesRead);
    }
}

/*DONE*/
string read_length(int socket, unsigned int length, string & cache)
{
    //I make the assumption that a char is one byte.
    assert(sizeof (char) == 1);
    assert(length > 0);

    //------------- VARIABLES -------------------------
    int numberBytesRead = 0;

    //Allocate a buffer
    char * buf = (char *) calloc(1024, sizeof (char));
    //------------------------------------------------



    //Loop forever until you've read enough characters (then return it) or there
    //was an error or the socket closed (which returns "").
    while (1)
    {

	//If we've read enough characters to return a substring with the intended
	//characters, then return the intended length of characters, and leave
	//the remaining characters (if applicable) in the static "message" string
	//for next time.
	if (cache.size() >= length)
	{
	    //substr(pos, n): character sequence that starts at character position 
	    //pos and has a length of n characters.
	    string returnString = cache.substr(0, length);
	    cache = cache.substr(length, cache.size() - length);

	    //Free up the allocated space and null the invalid pointer for good habit.
	    free(buf);
	    buf = NULL;

	    return returnString;
	}

	//About "recv": Returns the # of bytes received, or -1 if an error
	//occurred. Returns 0 when the peer has performed an orderly shutdown. 
	//If  no messages are available at the socket, the receive calls wait for
	//a message to arrive, unless the socket is non-blocking.

	//PUT CHARACTERS FROM SOCKET INTO THE BUFFER...
	//1024 is an arbitrary decision.
	numberBytesRead = recv(socket, buf, 1024, 0);

	if (numberBytesRead < 0)
	{
	    if (errno == EINTR)
	    {
		//the socket call was interrupted -- try again
		//Free up the allocated space and null the invalid pointer for good habit.
		free(buf);
		buf = NULL;
		DEBUG("continue" << endl);
		continue;
	    }
	    else
	    {
		//an error occurred, so break out
		//Free up the allocated space and null the invalid pointer for good habit.
		free(buf);
		buf = NULL;
		DEBUG("Error: exit the program" << endl);
		perror("recv");
		exit(-1);
	    }
	}
	else if (numberBytesRead == 0)
	{
	    //the socket is closed
	    //Free up the allocated space and null the invalid pointer for good habit.
	    free(buf);
	    buf = NULL;
	    DEBUG("Socket closed: return \"\"" << endl);
	    return "";
	}

	//APPEND THE BUFFER CHARS (THAT WERE JUST READ) TO THE MESSAGE...
	//Appends a copy of the string formed by the first n characters in the array
	//of characters pointed by s. (const char * s, size_t n)
	cache.append(buf, numberBytesRead);
    }
}

/*DONE*/
string itoa(int value, int base)
{

    std::string buf;

    // check that the base if valid
    if (base < 2 || base > 16) return buf;

    enum
    {
	kMaxDigits = 35
    };
    buf.reserve(kMaxDigits); // Pre-allocate enough space.

    int quotient = value;

    // Translating number to string with base:
    do
    {
	buf += "0123456789abcdef"[ std::abs(quotient % base) ];
	quotient /= base;
    }
    while (quotient);

    // Append the negative sign
    if (value < 0) buf += '-';

    std::reverse(buf.begin(), buf.end());
    return buf;
}

/*DONE*/
string hostNameToIPAddress(string host)
{
    DEBUG("host: " << host << endl);
    // use DNS to get host name
    struct hostent *hostEntry;
    hostEntry = gethostbyname(host.c_str());
    if (!hostEntry)
    {
	cout << "No such host name: " << host << endl;
	perror("DNS");
	exit(-1);
    }
    //Choose the first IP Address.
    string IPAddress = inet_ntoa(*(struct in_addr *) hostEntry->h_addr_list[0]);
    DEBUG("IPAddress: " << IPAddress << endl);
    return IPAddress;
}

/*DONE*/
void myGetLine(istream & is, string & str)
{
    getline(is, str);
    str += "\n";
}

/*DONE*/
bool hasOneWord(const string & str)
{
    if ((!isspace(str.at(0))) == false)
    {
	return false;
    }

    string whitespaceChars = " \t\n\v\f\r";
    size_t whitespaceIndex = str.find_first_of(whitespaceChars);
    if ((whitespaceIndex != string::npos) == false)
    {
	return false;
    }

    return true;
}

/*DONE*/
bool hasTwoWords(const string & str)
{
    if ((!isspace(str.at(0))) == false)
    {
	return false;
    }

    string whitespaceChars = " \t\n\v\f\r";
    //abc def\n
    //012 456 7
    //firstWSIndex = 3
    size_t firstWSIndex = str.find_first_of(whitespaceChars, 0);
    if ((firstWSIndex != string::npos) == false)
    {
	return false;
    }

    //firstNonWSIndex = 4
    size_t firstNonWSIndex = str.find_first_not_of(whitespaceChars, firstWSIndex);
    if ((firstNonWSIndex != string::npos) == false)
    {
	return false;
    }

    //secondWSIndex = 7
    size_t secondWSIndex = str.find_first_of(whitespaceChars, firstNonWSIndex);
    if ((secondWSIndex != string::npos) == false)
    {
	return false;
    }

    //starts on and includes index 4, and the length is 7-4 = 3.
    return true;
}

/*DONE*/
bool hasThreeWords(const string & str)
{
    if ((!isspace(str.at(0))) == false)
    {
	return false;
    }

    string whitespaceChars = " \t\n\v\f\r";
    //abc def hij\n
    //012 456 8910 11

    //firstWSIndex = 3
    size_t firstWSIndex = str.find_first_of(whitespaceChars, 0);
    if ((firstWSIndex != string::npos) == false)
    {
	return false;
    }

    //firstNonWSIndex = 4
    size_t firstNonWSIndex = str.find_first_not_of(whitespaceChars, firstWSIndex);
    if ((firstNonWSIndex != string::npos) == false)
    {
	return false;
    }

    //secondWSIndex = 7
    size_t secondWSIndex = str.find_first_of(whitespaceChars, firstNonWSIndex);
    if ((secondWSIndex != string::npos) == false)
    {
	return false;
    }

    //secondNonWSIndex = 8
    size_t secondNonWSIndex = str.find_first_not_of(whitespaceChars, secondWSIndex);
    if ((secondNonWSIndex != string::npos) == false)
    {
	return false;
    }

    //thirdWSIndex = 11
    size_t thirdWSIndex = str.find_first_of(whitespaceChars, secondNonWSIndex);
    if ((thirdWSIndex != string::npos) == false)
    {
	return false;
    }

    //starts on and includes index 8, and the length is 11-8 = 3.
    return true;
}

/*DONE*/
bool hasFourWords(const string & str)
{
    if (!isspace(str.at(0)) == false)
    {
	return false;
    }

    string whitespaceChars = " \t\n\v\f\r";
    //abc def hij     k  l  \n
    //012 456 8910 11 12-13 14
    //    * first nonWS
    //        * second nonWS
    //                *third nonWS
    //firstWSIndex = 3
    size_t firstWSIndex = str.find_first_of(whitespaceChars, 0);
    if ((firstWSIndex != string::npos) == false)
    {
	return false;
    }

    //firstNonWSIndex = 4
    size_t firstNonWSIndex = str.find_first_not_of(whitespaceChars, firstWSIndex);
    if ((firstNonWSIndex != string::npos) == false)
    {
	return false;
    }

    //secondWSIndex = 7
    size_t secondWSIndex = str.find_first_of(whitespaceChars, firstNonWSIndex);
    if ((secondWSIndex != string::npos) == false)
    {
	return false;
    }

    //secondNonWSIndex = 8
    size_t secondNonWSIndex = str.find_first_not_of(whitespaceChars, secondWSIndex);
    if ((secondNonWSIndex != string::npos) == false)
    {
	return false;
    }

    //thirdWSIndex = 11
    size_t thirdWSIndex = str.find_first_of(whitespaceChars, secondNonWSIndex);
    if ((thirdWSIndex != string::npos) == false)
    {
	return false;
    }

    //thirdNonWSIndex = 12
    size_t thirdNonWSIndex = str.find_first_not_of(whitespaceChars, thirdWSIndex);
    if ((thirdNonWSIndex != string::npos) == false)
    {
	return false;
    }

    //fourthWSIndex = 14
    size_t fourthWSIndex = str.find_first_of(whitespaceChars, thirdNonWSIndex);
    if ((fourthWSIndex != string::npos) == false)
    {
	return false;
    }

    return true;
}

/*DONE*/
double GetCurrentTime()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    double time = ts.tv_sec + ts.tv_nsec / 1000000000.0;
    return time;
}

/*DONE*/
bool read_once(int socket, string & cache)
{
    //I make the assumption that a char is one byte.
    assert(sizeof (char) == 1);

    //------------- VARIABLES -------------------------
    int numberBytesRead = 0;

    //Allocate a buffer
    char * buf = (char *) calloc(1024, sizeof (char));
    //------------------------------------------------


    //About "recv": Returns the # of bytes received, or -1 if an error
    //occurred. Returns 0 when the peer has performed an orderly shutdown. 
    //If  no messages are available at the socket, the receive calls wait for
    //a message to arrive, unless the socket is non-blocking.

    //PUT CHARACTERS FROM SOCKET INTO THE BUFFER...
    //1024 is an arbitrary decision.
    numberBytesRead = recv(socket, buf, 1024, 0);

    if (numberBytesRead < 0)
    {
	if (errno == EINTR)
	{
	    //Free up the allocated space and null the invalid pointer for good habit.
	    free(buf);
	    buf = NULL;
	    //Socket is still open, although it was interrupted.
	    return true;
	}
	else
	{
	    //an error occurred, so break out
	    //Free up the allocated space and null the invalid pointer for good habit.
	    free(buf);
	    buf = NULL;
	    DEBUG("Error: exit the program" << endl);
	    perror("recv");
	    exit(-1);
	}
    }
    else if (numberBytesRead == 0)
    {
	//the socket is closed
	//Free up the allocated space and null the invalid pointer for good habit.
	free(buf);
	buf = NULL;
	DEBUG("Socket closed" << endl);
	return false;
    }

    //APPEND THE BUFFER CHARS TO THE MESSAGE
    //Appends a copy of the string formed by the first n characters in the array
    //of characters pointed by s. (const char * s, size_t n)
    cache.append(buf, numberBytesRead);

    //Free up the allocated space and null the invalid pointer for good habit.
    free(buf);
    buf = NULL;
    return true;
}

/*INCOMPLETE*/
bool HasSubstring(string str, string substring)
{
    if (str.find(substring) == string::npos)
    {
	return false;
    }
    else
    {
	return true;
    }
}

/*DONE*/
string remove_newline(string & str, const string & sentinel)
{
    assert(HasSubstring(str, sentinel));
    assert(!equalStrings(str, ""));

    //----------Store the first part in a string--------
    int sentinelPosition = str.find(sentinel);

    //substr(pos, n): character sequence that starts at character position 
    //pos and has a length of n characters.
    int totalLengthOfMessage = sentinel.size() + sentinelPosition;
    string returnString = str.substr(0, totalLengthOfMessage);

    //---------Remove the first part from the original string--------
    str = str.substr(totalLengthOfMessage, str.size() - totalLengthOfMessage);

    return returnString;
}

/*DONE*/
string read_all_available(int socket, bool & closed)
{
    //I make the assumption that a char is one byte.
    assert(sizeof (char) == 1);

    //------------- VARIABLES -------------------------
    int numberBytesRead = 0;
    closed = false; //Default
    string cache;

    //Allocate a buffer
    char * buf = (char *) calloc(1024, sizeof (char));
    //------------------------------------------------

    //Loop forever until it would have blocked, 
    //if there's an error, or if the socket is closed (which returns "").
    while (1)
    {
	//About "recv": Returns the # of bytes received, or -1 if an error
	//occurred. Returns 0 when the peer has performed an orderly shutdown. 
	//If  no messages are available at the socket, the receive calls wait for
	//a message to arrive, unless the socket is non-blocking.

	//PUT CHARACTERS FROM SOCKET INTO THE BUFFER...
	//1024 is an arbitrary decision.
	numberBytesRead = recv(socket, buf, 1024, MSG_DONTWAIT);

	if (numberBytesRead < 0)
	{
	    if (errno == EINTR)
	    {
		//the socket call was interrupted -- try again
		//Free up the allocated space and null the invalid pointer for good habit.
		free(buf);
		buf = NULL;
		continue;
	    }
	    else if (errno == EAGAIN || errno == EWOULDBLOCK)
	    {
		return cache;
	    }
	    else
	    {
		//an error occurred, so break out
		//Free up the allocated space and null the invalid pointer for good habit.
		free(buf);
		buf = NULL;
		DEBUG("Error: exit the program" << endl);
		perror("recv");
		exit(-1);
	    }
	}
	else if (numberBytesRead == 0)
	{
	    //the socket is closed
	    //Free up the allocated space and null the invalid pointer for good habit.
	    free(buf);
	    buf = NULL;
	    closed = true;
	    return cache;
	}

	//APPEND THE BUFFER CHARS (THAT WERE JUST READ) TO THE MESSAGE
	//Appends a copy of the string formed by the first n characters in the array
	//of characters pointed by s. (const char * s, size_t n)
	cache.append(buf, numberBytesRead);
    }
}

/*DONE*/
string GetDate(time_t t)
{
    struct tm *gmt;
    char buf [200];
    memset(buf, 0, 200);
    gmt = gmtime(&t);
    if (gmt == NULL)
    {
	return "";
    }
    if (strftime(buf, sizeof (buf), "%a, %d %b %Y %H:%M:%S GMT", gmt) == 0)
    {
	return "";
    }
    return string(buf);
}

void PrintToFile(char * fileName, string WhatToPrint)
{
    ofstream outputFile;
    outputFile.open(fileName, ios_base::trunc);
    if (outputFile.is_open())
    {
	outputFile << WhatToPrint << endl << endl;
    }
    else
    {
	DEBUG("Failed to open output file." << endl);
    }
    outputFile.close();
    return;
}