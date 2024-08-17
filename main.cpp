#include "App.h"

int main() {
	//running the window app
	{
		try
		{
			App app;
			//CoInitialize(NULL);
			while (app.isRun());
		}
		catch (...)
		{
			return -1;
		}
	}
	return 0;
}