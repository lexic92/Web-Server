#include "Config.h"

using namespace std;

/*DONE*/
Config::Config()
{
}

/*DONE*/
Config::~Config()
{
}

/*DONE*/
void Config::parse(const char* filename)
{
    host_.clear();
    media_.clear();
    parameter_.clear();

    string line;

    // open file
    ifstream file(filename);
    if (!file.is_open())
        return;

    // parse file
    while (!file.eof())
    {
        getline(file,line);
        parseLine(line);
    }

    // close file
    file.close();
}

/*DONE*/
string& Config::host(string name)
{
    return host_[name];
}

/*DONE*/
string& Config::media(string name)
{
    return media_[name];
}

/*DONE*/ string& Config::parameter(string name)
{
    return parameter_[name];
}

/*DONE*/
void Config::parseLine(std::string &line)
{
    vector<string> tokens = tokenizer_.parse(line);
    if (tokens.size() < 3)
        return;
    if (tokens.at(0) == "host")
        host_[tokens.at(1)] = tokens.at(2);
    else if (tokens.at(0) == "media")
        media_[tokens.at(1)] = tokens.at(2);
    else if (tokens.at(0) == "parameter")
        parameter_[tokens.at(1)] = tokens.at(2);
}

/*DONE*/
/*static*/
bool Config::Test(ostream & os)
{
    bool success = true;
    Config c;
    c.parse("test.conf");
    TEST(equalStrings(c.host("carmelo.cs.byu.edu"),"/tmp/web1"));
    TEST(equalStrings(c.host("localhost"),"/tmp/web2"));
    TEST(equalStrings(c.media("txt"), "text/plain"));
    TEST(equalStrings(c.media("html"), "text/html"));
    TEST(equalStrings(c.media("jpg"), "image/jpeg"));
    TEST(equalStrings(c.media("gif"), "image/gif"));
    TEST(equalStrings(c.media("png"), "image/png"));
    TEST(equalStrings(c.media("pdf"), "application/pdf"));
    TEST(equalStrings(c.media("py"), "text/x-script.python"));
    TEST(equalStrings(c.parameter("timeout"), "1"));

    return success;
}
