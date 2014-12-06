// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// DESCRIPTION:
//	System specific interface stuff.
//
//-----------------------------------------------------------------------------


#ifndef __I_VIDEO__
#define __I_VIDEO__

#include <SDL2/SDL.h>

#include "doomtype.h"
#include "doomdef.h"

struct MouseData {
	int btnMask;

	int x;
	int y;

	MouseData() : btnMask(0), x(0), y(0) {}	

	inline MouseData& operator =(const MouseData& rhs) {
		btnMask = rhs.btnMask;
		x = rhs.x;
		y = rhs.y;

		return (*this);
	}
};

class VideoHandler {
protected:
private:
	int height;
	int width;

	MouseData lastMouseData;

	SDL_Window *windowHdl;
	SDL_Renderer *renderHdl;
	SDL_Surface *blitSurface;

	int TranslateKey(const SDL_Keycode &key);	
	int TranslateMouseButtons(const char &btn);
public:
	VideoHandler(int width = SCREENWIDTH, int height = SCREENHEIGHT);
	~VideoHandler();

	void ProcessEvents();
	void UpdateNoBlit();
	void StartFrame();
	void StartTic();
	void FinishUpdate();

	void ReadScreen(byte *pixDest);
	void SetPalette(byte *rgbPal);

	int Height() { return height; }
	void Height(int h) { height = h; }

	int Width() { return width; }
	void Width(int w) { width = w; }
};

extern VideoHandler *vidHandler;
#endif
