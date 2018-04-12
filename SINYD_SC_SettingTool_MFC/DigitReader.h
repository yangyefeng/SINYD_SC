#pragma once
#include <string>
#include <list>

using namespace std;
struct Digit
{
	int id;
	string value; 
	int height;
	int weight;
	string model;
};

class DigitsRW
{
public:
	DigitsRW();
	virtual ~DigitsRW();

	list<Digit> ReadFromXML(const string& xml_name);
	bool WriteOneDigitToXML(const string& xml_name, Digit digit);
	bool DeleteOneDigitInXML(const string& xml_name, Digit digit);
};

