#pragma once
class Block
{
public:
	Block();
	~Block();

private:
	int iData[4];
	int iType;
	int iRotate;
	int iAxisRow;
	int iAxisCol;
};

