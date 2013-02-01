#include "SBSystem.h"

#include <assert.h>
#include <vector>
#include <algorithm>

using namespace std;

HWND hWnd;
SBSystem *sbs;

enum {COMMAND_NONE, COMMAND_START, COMMAND_TOGUI, COMMAND_PAUSE, COMMAND_EXIT};

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd,
                         UINT message,
                         WPARAM wParam,
                         LPARAM lParam);

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
	SetCurrentDirectory(L"I:\\Development\\SpaceBastard\\SpaceBastard\\Debug");
	sbs = new SBSystem;
	sbs->loadConfig();

    // this struct holds information for the window class
    WNDCLASSEX wc;

    // clear out the window class for use
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    // fill in the struct with the needed information
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"WindowClass1";

    // register the window class
    RegisterClassEx(&wc);

    // create the window and use the result as the handle
	DWORD winStyle;
	winStyle = (sbs->isFullScreen()) ? WS_EX_TOPMOST | WS_POPUP : WS_OVERLAPPEDWINDOW;

    hWnd = CreateWindowEx(NULL,
                          L"WindowClass1",    // name of the window class
						  L"Space Bastard: \"Shoot'em all!\"",    // title of the window
                          winStyle,    // window style
						  //WS_EX_TOPMOST | WS_POPUP,
                          200,    // x-position of the window
                          100,    // y-position of the window
						  sbs->getWidth(),    // width of the window
						  sbs->getHeight(),    // height of the window
                          NULL,    // we have no parent window, NULL
                          NULL,    // we aren't using menus, NULL
                          hInstance,    // application handle
                          NULL);    // used with multiple windows, NULL

    // display the window on the screen
    ShowWindow(hWnd, nCmdShow);

	sbs->setWindowHandle(hWnd);
	sbs->setInstance(hInstance);
	assert(sbs->init());

	MSG msg;
	bool done = false;
	int command = COMMAND_NONE;

	while(true) {

		if(command == COMMAND_START) {
			sbs->startLoading();
			command = COMMAND_NONE;
		}

		if(command == COMMAND_TOGUI) {
			command = COMMAND_NONE;
			sbs->returnToGui();
		}

		if(command == COMMAND_EXIT)
			break;

		if(command == COMMAND_PAUSE)
			continue;

		// RENDER CIKLAS
		while(!command) {
			DWORD starting_point = GetTickCount();

			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				if (msg.message == WM_QUIT)
					break;

					TranslateMessage(&msg);
					DispatchMessage(&msg);
			}

			sbs->listenForInput();
			if(!sbs->inGame() && !sbs->inLoadingProgress() && sbs->inGui()) {
				// rodomas GUI
				sbs->updateCursor();

				// meniu mygtuku paspaudimai
				switch(sbs->buttonClicked()) {
					case SBSystem::CLICKED_START:
						sbs->playButtonSound("start");
						command = COMMAND_START;
						break;

					case SBSystem::CLICKED_FULLSCREEN:
						sbs->playButtonSound("fullscreen");
						break;

					case SBSystem::CLICKED_EXIT:
						command = COMMAND_EXIT;
						break;
				}
			}

			if(sbs->inGame() && !sbs->inLoadingProgress() && sbs->inGui()) {
				// rodomas GUI zaidimo metu
				sbs->updateCursor();

				// mygtuku paspaudimai
				switch(sbs->buttonClicked()) {
					case SBSystem::CLICKED_RESUME:						
						sbs->returnToGame();
						sbs->playButtonSound("resume");
						break;

					case SBSystem::CLICKED_STOP:						
						sbs->stopGame();
						sbs->returnToGui();
						sbs->playButtonSound("stop");
						break;

					case SBSystem::CLICKED_FULLSCREEN:
						sbs->playButtonSound("fullscreen");
						break;

					case SBSystem::CLICKED_EXIT:
						command = COMMAND_EXIT;
						break;
				}
			}

			else if (sbs->inGame()) {
				if(sbs->keyPressed(DIK_ESCAPE)) {
					sbs->returnToGui();
				}
				else {
					sbs->updateHero();
					sbs->updateEntities();
				}
			}


////////////////////////////////////////////////////////////////////////////
/// STARTING RENDERING
////////////////////////////////////////////////////////////////////////////
			sbs->startRendering();
	
			if(sbs->inLoadingProgress()) {
				// loadinimo ekranas
				sbs->showLoadingProgress();
			} else if(sbs->inGui()) {
				// rodomas gui
				sbs->renderGui();
				sbs->gui->showCursor();
			}
			else if(sbs->inGame()) {
				// veiksmas
				if(sbs->level->levelFinished()) {
					sbs->stopGame();
					command = COMMAND_TOGUI;
					break;
				}
				sbs->level->run();
				sbs->renderEntities();
				sbs->renderElements();
			}

			sbs->stopRendering();
////////////////////////////////////////////////////////////////////////////
/// FINISHING RENDERING
////////////////////////////////////////////////////////////////////////////

			while ((GetTickCount() - starting_point) < 50);
		} 

	}

	sbs->release();
    return msg.wParam;
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // sort through and find what code to run for the message given
    switch(message)
    {
        // this message is read when the window is closed
        case WM_DESTROY:
            {
                // close the application entirely
                PostQuitMessage(0);
                return 0;
            } break;
    }

    // Handle any messages the switch statement didn't
    return DefWindowProc (hWnd, message, wParam, lParam);
}