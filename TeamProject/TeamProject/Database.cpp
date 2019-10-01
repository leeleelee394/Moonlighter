#include "stdafx.h"
#include "Database.h"

SingletonCpp(Database)

Database::Database()
{
}


Database::~Database()
{
	intData.clear();
	vector2Data.clear();
	stringData.clear();
}

int Database::GetIntData(string key)
{
	map<string, int>::iterator iter = intData.find(key);
	if (iter != intData.end())
		return iter->second;
	return 0;
}

Vector2 Database::GetVector2Data(string key)
{
	map<string, Vector2>::iterator iter = vector2Data.find(key);
	if (iter != vector2Data.end())
		return iter->second;
	return Vector2();
}

string Database::GetStringData(string key)
{
	map<string, string>::iterator iter = stringData.find(key);
	if (iter != stringData.end())
		return iter->second;
	return string();
}

void Database::AddIntData(const string key, const int data)
{
	intData[key] = data;
}

void Database::AddVector2Data(const string key, const Vector2 data)
{
	vector2Data[key] = data;
}

void Database::AddStringData(const string key, const string data)
{
	stringData[key] = data;
}
