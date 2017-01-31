#include "Sprite.h"
#include <stdio.h>
#include <iostream>
#include <algorithm>

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
	delete[] data_;
}

bool Sprite::initialise(unsigned int w, unsigned int h, std::string path)
{
	spriteHeight_ = h;
	spriteWidth_ = w;

	spriteBox_.Initialise(0, w, 0, h);

	//Initialise HAPI
	if (!HAPI.LoadTexture(path, &data_, spriteWidth_, spriteHeight_))
		return false;

	return true;
}

void Sprite::blit(BYTE *destination, unsigned int destWidth, int posX, int posY)
{
	BYTE *destinationPointer = destination + (posX + (posY*destWidth)) * 4;
	BYTE *sourcePointer = data_;

	for (unsigned int y = 0; y < spriteHeight_; y++)
	{
		memcpy(destinationPointer, sourcePointer, spriteWidth_ * 4);

		//Move source pointer to the next line
		sourcePointer += spriteWidth_ * 4;

		//Move destination pointer to the next line
		destinationPointer += destWidth * 4;
	}

	return;

}

void Sprite::clipBlit(BYTE *destination, unsigned int destWidth, int posX, int posY, const Rectangle &destBox)
{
	//Create and minipulate screenspace hitbox
	Rectangle clippedRectangle(spriteBox_);
	
	//Translate hitbox to screen space
	clippedRectangle.Translate(posX, posY);

	if (clippedRectangle.Outside(destBox))
		return;

	if (destBox.Contains(clippedRectangle))
		blit(destination, destWidth, posX, posY);

	//Clip rectangle
	clippedRectangle.ClipTo(destBox);

	//Translate back to texture space
	clippedRectangle.Translate(-posX, -posY);

	//Clamp possitions to screen space
	if (posX < 0)
		posX = 0;
	if (posY < 0)
		posY = 0;

	BYTE *destinationPointer = destination + (posX + (posY*destWidth)) * 4;
	BYTE *sourcePointer = data_ + (clippedRectangle.Get(0) + (clippedRectangle.Get(2)*spriteWidth_)) * 4;

	for (unsigned int y = 0; y < clippedRectangle.Height(); y++)
	{
		memcpy(destinationPointer, sourcePointer, clippedRectangle.Width() * 4);

		//Move source pointer to the next line
		sourcePointer += clippedRectangle.Width() * 4;

		//Move destination pointer to the next line
		destinationPointer += destWidth * 4;
	}

	return;

}


void Sprite::alphaBlit(BYTE *destination, unsigned int destWidth, unsigned int destHeight, int posX, int posY)
{
	BYTE *destinationPointer = destination + (posX + (posY*destWidth)) * 4;
	BYTE *sourcePointer = data_;

	for (unsigned int y = 0; y < spriteHeight_; y++)
	{
		for (unsigned int x = 0; x < spriteWidth_; x++)
		{
			if(sourcePointer[3] == 0)
			{
			
			}
			else 
			{
				if (sourcePointer[3] == 255)
				{
					memcpy(destinationPointer, sourcePointer, 4);
				}
				else
				{
					BYTE red = sourcePointer[0];
					BYTE green = sourcePointer[1];
					BYTE blue = sourcePointer[2];
					BYTE alpha = sourcePointer[3];

					destinationPointer[0] = destinationPointer[0] + ((alpha * (red - destinationPointer[0])) >> 8);
					destinationPointer[1] = destinationPointer[1] + ((alpha * (green - destinationPointer[1])) >> 8);
					destinationPointer[2] = destinationPointer[2] + ((alpha * (blue - destinationPointer[2])) >> 8);
				}
			}

			//Advance source pointer
			sourcePointer += 4;

			//Advance destination pointer
			destinationPointer += 4;
		}

		destinationPointer += (destWidth - spriteWidth_) * 4;

	}


}

void Sprite::clipAlphaBlit(BYTE *destination, unsigned int destWidth, unsigned int destHeight, int posX, int posY, const Rectangle &destBox)
{
	//Create and manipulate screenspace hitbox
	Rectangle clippedRectangle(this->spriteBox_);

	//Translate hitbox to screen space
	clippedRectangle.Translate(posX, posY);

	if (clippedRectangle.Outside(destBox))
		return;

	Rectangle centreRect;
	centreRect.Initialise(600, 640, 350, 370);
	if (clippedRectangle.Contains(centreRect))
	{
		HAPI.SetControllerRumble(0,10000,10000);
	}
	else
	{
		HAPI.SetControllerRumble(0, 0, 0);
	}

	if (destBox.Contains(clippedRectangle))
		alphaBlit(destination, destWidth, destHeight, posX, posY);

	//Clip rectangle
	clippedRectangle.ClipTo(destBox);

	//Translate back to texture space
	clippedRectangle.Translate(-posX, -posY);

	//Clamp possitions to screen space
	if (posX < 0)
		posX = 0;
	if (posY < 0)
		posY = 0;

	//Const data
	const int crleft = clippedRectangle.Get(0);
	const int crtop = clippedRectangle.Get(2);

	BYTE *destinationPointer = destination + (posX + (posY*destWidth)) * 4;
	BYTE *sourcePointer = data_ + (crleft + (crtop * spriteWidth_ )) * 4;

	for (unsigned int y = 0; y < clippedRectangle.Height(); y++)
	{
		for (unsigned int x = 0; x < clippedRectangle.Width(); x++)
		{
			if (sourcePointer[3] == 0)
			{

			}
			else
			{
				if (sourcePointer[3] == 255)
				{
					memcpy(destinationPointer, sourcePointer, 4);
				}
				else
				{
					BYTE red = sourcePointer[0];
					BYTE green = sourcePointer[1];
					BYTE blue = sourcePointer[2];
					BYTE alpha = sourcePointer[3];

					destinationPointer[0] = destinationPointer[0] + ((alpha * (red - destinationPointer[0])) >> 8);
					destinationPointer[1] = destinationPointer[1] + ((alpha * (green - destinationPointer[1])) >> 8);
					destinationPointer[2] = destinationPointer[2] + ((alpha * (blue - destinationPointer[2])) >> 8);
				}
			}

			//Advance source pointer
			sourcePointer += 4;

			//Advance destination pointer
			destinationPointer += 4;
		}

		sourcePointer += (spriteWidth_ - clippedRectangle.Width()) * 4;
		destinationPointer += (destWidth - clippedRectangle.Width()) * 4;

	}


}
