#pragma once
/********************************************************************
## BinaryWriter ##
이진 파일 저장 모듈 클래스 
*********************************************************************/
class BinaryWriter
{
public:
	BinaryWriter();
	~BinaryWriter();

	void Open(wstring filePath, UINT openOption = CREATE_ALWAYS);
	void Close();

	void Bool(bool data);
	void Word(WORD data);
	void Int(int data);
	void UInt(UINT data);
	void Float(float data);
	void Double(double data);

	void String(const string& data);
	void Byte(void* data, UINT dataSize);

protected:
	HANDLE fileHandle;
	DWORD size;
};


/********************************************************************
## BinaryReader ##
이진 파일 읽어오는 모듈 클래스 
*********************************************************************/
class BinaryReader
{
public:
	BinaryReader();
	~BinaryReader();

	bool Open(wstring filePath);
	void Close();

	bool Bool();
	WORD Word();
	int Int();
	UINT UInt();
	float Float();
	double Double();

	string String();
	void Byte(void** data, UINT dataSize);

protected:
	HANDLE fileHandle;
	DWORD size;
};