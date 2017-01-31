#pragma once
#include <HAPI_lib.h>

class Input
{
public:
	Input();
	~Input();

	void Update(int &posX, int& posY, unsigned int speed);
	int Firing();
};

