#include "App.h"

int main() {
	//running the window app
	{
		try
		{
			App app;
			while (app.isRun());
		}
		catch (...)
		{
			return -1;
		}
	}
	return 0;
}