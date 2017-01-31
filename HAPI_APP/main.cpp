#include "World.h"

// Every HAPI program has a HAPI_Main as an entry point
// When this function exits the program will close down
void HAPI_Main()
{
	//Create and initialise game world
	World gameWorld;
	gameWorld.Initialise();

	//Initialise ticker
	DWORD lastUpdate = 0;
	const DWORD tick = 1000 / 60;

	//Run game loop
	while (HAPI.Update())
	{
		DWORD timePassed = HAPI.GetTime() - lastUpdate;
		//Check update timer
		if(timePassed >= tick)
		{
			//Update that game world
			gameWorld.Run();
			lastUpdate = HAPI.GetTime();

			//Detect collisions
			gameWorld.CheckCollisions();
		}
		//Draw world
		gameWorld.Draw();

	}
}
