#include "Input.h"
using namespace HAPISPACE;

Input::Input()
{
}

Input::~Input()
{
}

void Input::Update(int & posX, int& posY, unsigned int speed)
{
	//Set Keyboard data (inificient to call every update but couldn't figure out how to set it as a member variable)
	const HAPI_TKeyboardData &keyData_ = HAPI.GetKeyboardData();

	//Controller set
	const HAPI_TControllerData &cData = HAPI.GetControllerData(0);


	//Handle controller input
	if (cData.isAttached)
	{
		if (cData.digitalButtons[HK_DIGITAL_DPAD_DOWN] || cData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] < -HK_GAMEPAD_LEFT_THUMB_DEADZONE)
			posY += speed;

		if (cData.digitalButtons[HK_DIGITAL_DPAD_RIGHT] || cData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] > HK_GAMEPAD_LEFT_THUMB_DEADZONE)
			posX += speed;

		if (cData.digitalButtons[HK_DIGITAL_DPAD_UP] || cData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] > HK_GAMEPAD_LEFT_THUMB_DEADZONE)
			posY -= speed;

		if (cData.digitalButtons[HK_DIGITAL_DPAD_LEFT] || cData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] < -HK_GAMEPAD_LEFT_THUMB_DEADZONE)
			posX -= speed;
	}

	//Handle keyboard character input
	if (keyData_.scanCode[HK_DOWN] || keyData_.scanCode['S'])
		posY += speed;

	if (keyData_.scanCode[HK_RIGHT] || keyData_.scanCode['D'])
		posX += speed;

	if (keyData_.scanCode[HK_UP] || keyData_.scanCode['W'])
		posY -= speed;

	if (keyData_.scanCode[HK_LEFT] || keyData_.scanCode['A'])
		posX -= speed;
}

int Input::Firing()
{
	//Set Keyboard data (inificient to call every update but couldn't figure out how to set it as a member variable)
	const HAPI_TKeyboardData &keyData_ = HAPI.GetKeyboardData();

	//Controller set
	const HAPI_TControllerData &cData = HAPI.GetControllerData(0);

	//Check for Fire!
	if (cData.digitalButtons[HK_DIGITAL_A] || keyData_.scanCode[HK_SPACE])
		return 1;
	else if (cData.digitalButtons[HK_DIGITAL_B] || keyData_.scanCode[HK_SHIFT])
		return 2;
	else
		return 0;
}

