#include "UISpecSelector.h"

#include <string>
#include <iostream>

#include <tchar.h>
#include <Windows.h>

#include "EngineRegistry.h"

using namespace std;

// TODO: Consider adding a new class to hold chapters, so we can
//       iterate it instead of modifying this code here and there.

const EngineSpec &UISpecSelector::selectSpec()
{
	clearConsole();

	cout << "My OpenGL (3.3+) playground\n\n";

	// Assuming we won't go ever beyond 'Z'...
	char ux = 'A';

	for (auto &unit : EngineRegistry::getInstanceData()) {
		cout << "  " << unit.first << " (" << ux++ << "):\n";
		for (unsigned i = 0; i < unit.second.size(); ++i)
			cout << string(4, ' ') << (i + 1) << ". " << unit.second[i].name << '\n';
		cout << '\n';
	}

	string sel;
	cout << "Select a test to run (ex. A3): ";
	cin >> sel;

	const EngineSpec &spec = selectSpec(sel);
	clearConsole();
	return spec;
}

const EngineSpec &UISpecSelector::selectSpec(const std::string& id)
{
	if (id.length() != 2) exitApplication();

	int iid = tolower(id[0]) - 'a'; // Unit index
	int jid = id[1] - '0' - 1; // Engine index in unit

	EngineSpec *pSpec = nullptr;

	// Bad way to access a map...
	if (iid >= EngineRegistry::getInstanceData().size()) exitApplication();
	
	int i = 0;
	for (auto &unit : EngineRegistry::getInstanceData()) {
		if (iid == i++) { // Found our engines group
			if (jid >= unit.second.size()) exitApplication();
			pSpec = &unit.second[jid];
		}
	}

	return *pSpec;
}

bool UISpecSelector::clearConsole()
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

void UISpecSelector::exitApplication()
{
	cerr << "Wrong, I will exit now...\n";
	exit(EXIT_FAILURE);
}
