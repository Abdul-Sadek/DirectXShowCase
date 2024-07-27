#include "App.h"

int main() {
	//creating resources
	try
	{
		//InputSystem::create();
	}
	catch (...)
	{
		return -1;
	}
	//running the window app
	{
		try
		{
			App app;
			while (app.isRun());
		}
		catch (...)
		{
			//InputSystem::release();
			return -1;
		}
	}
	//releasing resources
	//InputSystem::release();
	return 0;
}