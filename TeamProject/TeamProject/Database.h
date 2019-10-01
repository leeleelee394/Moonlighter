#pragma once

class Database
{
	SingletonHeader(Database)
private:
	map<string, int> intData;
	map<string, Vector2> vector2Data; 
	map<string, string> stringData;
public:
	int GetIntData(string key);
	Vector2 GetVector2Data(string key);
	string GetStringData(string key);

	void AddIntData(const string key, const int data);
	void AddVector2Data(const string key, const Vector2 data);
	void AddStringData(const string key,const string data);
};

#define _Database Database::Get()