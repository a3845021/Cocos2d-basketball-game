#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <vector>
using std::string;
using std::vector;
class Global
{
public:
	Global();
	~Global();
	static int score1;
	static int score2;
	static int score3;
	static string gameSessionId;
	static string toString(vector<char> *buffer);
	static string getSessionIdFromHeader(string header);
};

#endif