#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <errno.h>
#include <cassert>
#include <stdio.h>
#include <algorithm>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

/**
 * For use in the DEBUG macro. It is defined wherever the program parses the debug flag.
 */
extern bool debug;

/**
 * Syntax: DEBUG(stringToPrint); 
 * Output: Prints the file name and line number, followed by the stringToPrint, to standard output
 * if the debug flag is set to true.
 */
#define DEBUG(str) if(debug) do {std::cout << "[" << __FILE__ << ", " << __LINE__ << "] " << str << flush;}while(false)

/**
 * Syntax: TEST(boolean)
 * The boolean is something that you want to be true.  If it is not, then it will write
 * "Test Failed [Filename, Line#]" to standard output.
 */
#define TEST(cond) \
do {\
	if (!(cond)) {\
		success = false; cout << "Test Failed [" << __FILE__ << ", " << __LINE__ << "]" << endl;\
	}\
}while(false)

/**
 * Tests the Utils.cc file.
 */
bool TestUtils(ostream & os);

/**
 * TEST FUNCTIONS
 * Each of these tests the function indicated.
 */
bool TestHasSubstring(ostream & os);
bool Testremove_newline(ostream & os);
bool TestGetDate(ostream & os);

/**
 * @return true if s1.compare(s1) == 0.  I.e., they are the exact same string.
 */
bool equalStrings(string s1, string s2);

/**
 * @pre str does not start with a whitespace character.
 * @pre str ends with a whitespace.
 * @pre str has at least 1 "word" where a word is a grouping of consecutive
 *  non-whitespace characters.
 * @return first non-whitespace part of the string.
 */
string firstWord(const string & str);

/**
 * @pre str does not start with a whitespace character.
 * @pre str ends with a whitespace.
 * @pre str has at least 2 "words" where a word is a grouping of consecutive
 *  non-whitespace characters.
 * @return second non-whitespace part of the string.
 */
string secondWord(const string & str);

/**
 * @pre str does not start with a whitespace character.
 * @pre str ends with a whitespace.
 * @pre str has at least 3 "words" where a word is a grouping of consecutive
 *  non-whitespace characters.
 * @return third non-whitespace part of the string.
 */
string thirdWord(const string & str);

/**
 * @pre str does not start with a whitespace character.
 * @pre str ends with a whitespace.
 * @pre str has at least 4 "words" where a word is a grouping of consecutive
 *  non-whitespace characters.
 * @return third non-whitespace part of the string.
 */
string fourthWord(const string & str);

/**
 * Completely sends a message over a socket, unless there was an
 * error.  It does not handle the errors. You have no way of knowing if
 * this function call had an error or not.
 *
 * @pre "socket" is in a connected state.
 * @param socket the socket which you want to send the message with.
 * @param message what you want to send over the socket.
 */
void Send(string message, int socket);

/**
 * Read from a socket until a sentinel is reached.
 * It will return all the characters, up to AND INCLUDING the sentinel.
 * If an error occurred or the socket is closed, then this function will return
 * the empty string.
 * Note: The sentinel cannot be empty, so the string will be at least one
 * character.
 *
 * @param s the socket file descriptor
 * @param sentinel the sentinel
 * @param cache it will append read chars to this and search for the sentinel, and once it
 * finds the sentinel, the cache will consist solely of the remaining characters after the sentinel.
 * @return a string of chars from the socket, up to and including the sentinel. Returns empty string
 * if the socket is closed.
 * @pre the sentinel cannot be the empty string.
 * @pre the socket must be ready to call "recv."
 */
string read_newline(int socket, string sentinel, string & cache);

/**
 * Read from a socket for a specified length.
 * If an error occurred or the socket is closed, then this function will return
 * the empty string.
 *
 * @pre the socket must be ready to call "recv."
 * @pre length must be > 0 in order to differentiate an error from a correct thing.
 * @param s the socket file descriptor
 * @param length the number of chars you want to read
 * @param cache it will append read chars to this until it reaches the appropriate length,
 * and then it will return what was in the cache before and the chars needed to reach the
 * appropriate length, and THEN the cache will consist of the excess characters.
 * @return a string of "length" chars from the socket. Returns an empty string if the socket is closed.
 */
string read_length(int socket, unsigned int length, string & cache);


/**
 * C++ version 0.4 std::string style "itoa":
 * Contributions from Stuart Lowe, Ray-Yuan Sheu,
 * Rodrigo de Salvo Braz, Luc Gallant, John Maloney
 * and Brian Hunt
 */
string itoa(int value, int base);

/**
 * Converts a host name into an IP Address string in the 4-dot formation.
 * Chooses the first IPAddress on the list (doesn't randomize).
 *
 * @param host the host name
 * @return an IP Address of that host in 4-dot formation. If there is no such
 * host name, it returns an empty string, "".
 */
string hostNameToIPAddress(string host);

/**
 * The exact same as "getline(istream & is, string & str",
 * but it does not discard the delimiter.
 */
void myGetLine(istream & is, string & str);

bool hasOneWord(const string & str);
bool hasTwoWords(const string & str);
bool hasThreeWords(const string & str);
bool hasFourWords(const string & str);

/**
 * Gets the current time. The unit is SECONDS. But it's a double
 * so it has a fraction with it.
 */
double GetCurrentTime();

/**
 * Determines if the string has a given substring.
 * 
 * @param str the string in question
 * @param substr the substring in question
 * @return true if str contains the given substring within it.
 */
bool HasSubstring(string str, string substring);

/**
 * Takes off the front of a string, up to and including the sentinel,
 * and returns it.
 * 
 * @pre sentinel is a substring of str.
 * @pre sentinel is not the empty string
 * @post str has all of the characters removed from the beginning, up to and
 * including the sentinel.
 * @return a substring of str, starting at position 0, up to and including the sentinel.
 */
string remove_newline(string & str, const string & sentinel);


/**
 * Removes all the chars up to and including the sentinel, 
 * off of the given string.
 * 
 * @pre sentinel is a substring of string
 * @pre sentinel is not the empty string
 * @post str has all the chars up to and including the sentinel removed.
 * @param str the string
 * @param sentinel the sentinel
 * @param pos the position of the first character of the sentinel in str.
 * (AKA The result of str.find(sentinel).)
 */
void ChopOffFrontOfString(string & str, string & sentinel, int pos);

/**
 * Read from a socket until it would have blocked.
 * It will return all the characters read.
 * If an error occurred it will exit the program.
 * If the socket is closed, the closed flag will be true.
 *
 * @pre the socket must be ready to call "recv."
 * @param socket the socket file descriptor
 * @param closed a status flag, true if the socket is closed.
 * @return All the characters read that were available without having to block.
 */
string read_all_available(int socket, bool & closed);

/**
 * Converts the given time into a date format compliant with HTTP/1.1
 * Date header syntax and last-modified header syntax.
 * Taken from the CS360 slides.
 * 
 * @param t the time you want to convert
 * @return the newly-formated date
 */
string GetDate(time_t t);

/**
 * Prints the given string to the given file. If it doesn't open, then
 * it prints a debugging message.
 * 
 * @param fileName the file to print to
 * @param WhatToPrint the
 */
void PrintToFile(char * fileName, string WhatToPrint);

#endif	/* UTILS_H */

