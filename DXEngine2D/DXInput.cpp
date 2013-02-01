#include "DXInput.h"

DXInput::DXInput(HINSTANCE hInst)
{
	DirectInput8Create(hInst, DIRECTINPUT_VERSION, 
						IID_IDirectInput8, (LPVOID *) &din, NULL);
}

DXInput::~DXInput()
{
	devMouse->Unacquire();
	devMouse->Release();
	din->Release();
}

bool DXInput::initMouse(HWND hWnd)
{
	if(FAILED(din->CreateDevice(GUID_SysMouse, &devMouse, NULL)) )
		return false;

	if(FAILED(devMouse->SetDataFormat(&c_dfDIMouse)))
		return false;

	if(FAILED(devMouse->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
		return false;

	DIPROPDWORD dipdw;
    
	// the header
    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;
    // the data
    dipdw.dwData            = DIPROPAXISMODE_REL;

	if(FAILED(devMouse->SetProperty(DIPROP_AXISMODE, &dipdw.diph) ))
		return false;

	devMouse->Acquire();

	return true;
}

/***
 *
 */
bool DXInput::initKeyboard(HWND hWnd)
{
	if(FAILED(din->CreateDevice(GUID_SysKeyboard, &devKeyboard, NULL)) )
		return false;

	if(FAILED(devKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		return false;

	if(FAILED(devKeyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
		return false;

	devKeyboard->Acquire();
	return true;
}

void DXInput::acquireMouse()
{
	//devMouse->Acquire();
}

void DXInput::unacquireMouse()
{
	//devMouse->Unacquire();
}

int DXInput::getMouseInput(long &x, long &y, long &z)
{
	DIMOUSESTATE dims;
	HRESULT hr;
/*
	hr = devMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID) &dims);
	if(hr == DIERR_NOTACQUIRED)
		devMouse->Acquire();
*/	
	bool done = false;
	while (!done) { 
		// query DirectInput for newest mouse data 
		hr = devMouse->GetDeviceState(sizeof(DIMOUSESTATE), &dims); 
		if (FAILED(hr)) 
		{ 
			if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED) 
			{ 
				// device lost... reacquire 
				hr = devMouse->Acquire(); 
				if (FAILED(hr)) 
				{ 
					// error handling goes here 
					done=1; 
				} 
			} 
			else
			{ 
				// it’s some other error – handle it 
				done = 1; 
			} 
		} 
		else // read mouse successfully! 
		{ 
			done = 1; 
		} 
  } // end while loop – we’ve read DI correctly 

	x = dims.lX;
	y = dims.lY;
	z = dims.lZ;

	if (dims.rgbButtons[0] & 0x80)
		return PRESSED_LEFT;
	if (dims.rgbButtons[1] & 0x80)
		return PRESSED_RIGHT;

	return PRESSED_NONE;
}

/***
 *
 */
bool DXInput::keyPressed(long dikCode)
{
    static BYTE keystate[256];   
    devKeyboard->GetDeviceState(256, (LPVOID)keystate);    // fill keystate with values

	if(keystate[dikCode] & 0x80)
	   return true;

	return false;
}