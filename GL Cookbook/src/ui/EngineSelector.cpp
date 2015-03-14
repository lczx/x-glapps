#include "EngineSelector.h"

#include <string>
#include <iostream>

#include <tchar.h>
#include <Windows.h>

#include "../engines/ch1.h"

using namespace std;

// TODO: Consider adding a new class to hold chapters, so we can
//       iterate it instead of modifying this code here and there.

const EngineSpec &EngineSelector::selectSpec()
{
	clearConsole();

	cout << "My OpenGL (3.3+) playground\n\n";

	cout << "  Chapter 1 (A):\n";
	for (unsigned i = 0; i < c1Engines_.size(); ++i)
		cout << string(4, ' ') << (i + 1) << ". " << c1Engines_[i].name << '\n';
	cout << '\n';

	/* Add additional chapters here, consider an algorithm if more than 3... */

	string sel;
	cout << "Select a test to run (ex. A3): ";
	cin >> sel;

	const EngineSpec &spec = selectSpec(sel);
	clearConsole();
	return spec;
}

const EngineSpec& EngineSelector::selectSpec(const std::string& id)
{
	if (id.length() != 2) exitApplication();

	vector<EngineSpec> *pVec = nullptr;
	switch (tolower(id[0])) {
	case 'a':
		pVec = &c1Engines_; break;
	default:
		// ^^^ ADD MORE CASES UP HERE FOR NEW CHAPTERS
		exitApplication();
	}

	unsigned int sn = id[1] - '0' - 1;
	if (sn < 0 || sn >= pVec->size())
		exitApplication();

	return (*pVec)[sn];
}

void EngineSelector::registerEngines()
{
	c1Engines_.push_back({ "Getting Started", [] { return new GettingStartedEngine(); } });
}

bool EngineSelector::clearConsole()
{
	// http://www.cplusplus.com/articles/4z18T05o/

	COORD homeCoords{ 0, 0 };

	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return true;

	// Get the number of cells in the current buffer
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return true;

	SHORT cellCount = csbi.dwSize.X * csbi.dwSize.Y;
	DWORD writtenCount;

	// Fill the entire buffer with spaces
	if (!FillConsoleOutputCharacter(hStdOut, _T(' '),
		cellCount, homeCoords, &writtenCount)) return true;

	// Fill the entire buffer with the current colors and attributes
	if (!FillConsoleOutputAttribute(hStdOut, csbi.wAttributes,
		cellCount, homeCoords, &writtenCount)) return true;

	// Move the cursor home
	SetConsoleCursorPosition(hStdOut, homeCoords);
	return false;
}

void EngineSelector::exitApplication()
{
	cerr << "Wrong, I will exit now...\n";
	exit(EXIT_FAILURE);
}
