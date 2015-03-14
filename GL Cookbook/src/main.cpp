#include "ui/GLApplication.h"
#include "ui/EngineSelector.h"

int main(int argc, char *argv[])
{
	EngineSelector es;
	es.registerEngines();

	// Version 3.3 or 4.3, INTEL HD 3000 SUPPORTS MAX 3.1 <---
	GLApplication::initializeGLContext(&argc, argv, 3, 1);

	GLApplication app =
		GLApplication(argc > 1 ? es.selectSpec(argv[1]) : es.selectSpec());

	app.createWindow(1024, 640);

	app.glewShowInfo();
	app.loop();

	return 0;
}
