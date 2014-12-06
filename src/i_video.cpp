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
// $Log:$
//
// DESCRIPTION:
//	DOOM graphics stuff for X11, UNIX.
//
//-----------------------------------------------------------------------------

static const char
rcsid[] = "$Id: i_x.c,v 1.6 1997/02/03 22:45:10 b1 Exp $";

#include <stdlib.h>
#include <unistd.h>

#include <stdarg.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <errno.h>
#include <signal.h>

#include "doomstat.h"
#include "i_system.h"
#include "v_video.h"
#include "m_argv.h"
#include "d_main.h"

VideoHandler *vidHandler = NULL;

int VideoHandler::TranslateKey(const SDL_Keycode &key) {

    	switch(key) {
      		case SDLK_LEFT:		return KEY_LEFTARROW;
      		case SDLK_RIGHT:	return KEY_RIGHTARROW;
		case SDLK_DOWN:		return KEY_DOWNARROW;
		case SDLK_UP:		return KEY_UPARROW;
      		case SDLK_ESCAPE:	return KEY_ESCAPE;
      		case SDLK_RETURN:	return KEY_ENTER;
		case SDLK_TAB:		return KEY_TAB;
		case SDLK_F1:		return KEY_F1;		
		case SDLK_F2:		return KEY_F2;		
		case SDLK_F3:		return KEY_F3;		
		case SDLK_F4:		return KEY_F4;		
		case SDLK_F5:		return KEY_F5;		
		case SDLK_F6:		return KEY_F6;		
		case SDLK_F7:		return KEY_F7;		
		case SDLK_F8:		return KEY_F8;		
		case SDLK_F9:		return KEY_F9;		
		case SDLK_F10:		return KEY_F10;		
		case SDLK_F11:		return KEY_F11;		
		case SDLK_F12:		return KEY_F12;		
		case SDLK_BACKSPACE:
		case SDLK_DELETE:	return KEY_BACKSPACE;
		case SDLK_PAUSE:	return KEY_PAUSE;
		case SDLK_EQUALS:
		case SDLK_KP_EQUALS:	return KEY_EQUALS;
      		case SDLK_MINUS:
		case SDLK_KP_MINUS:	return KEY_MINUS:
      		case SDLK_LSHIFT:
		case SDLK_RSHIFT:	return KEY_RSHIFT;
		case SDLK_LCTRL:
		case SDLK_RCTRL:	return KEY_RCTRL;
		case SDLK_LALT:
		case SDLK_LMETA:
		case SDLK_RALT;
		case SDLK_RMETA:	return KEY_RALT;	
		default: {
			if(key >= SDLK_SPACE && key <= SDLK_BACKQUOTE)
				return (key - SDLK_SPACE) + ' ';

			if(key >= SDLK_A && key <= SDLK_Z)
				return (key - SDLK_A) + 'a';
		}
	}

	return 0;
}

int VideoHandler::TranslateMouseButtons(const char &btn) {
	return ((btn & SDL_BUTTON_LEFT) |
		((btn & SDL_BUTTON_RIGHT) ? 2 : 0) |
		((btn & SDL_BUTTON_MIDDLE) ? 4 : 0)); 
}

VideoHandler::VideoHandler(int width, int height)
		: width(width), height(height),
		windowHdl(NULL), renderHdl(NULL), blitSurface(NULL) {
	int windowFlags = (SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_GRABBED);
	int renderFlags = (SDL_RENDERER_SOFTWARE | SDL_RENDER_TARGETTEXTURE);
	signal(SIGINT, (void (*)(int)) I_Quit);

	SDL_Init(SDL_INIT_EVERYTHING);

    
	windowHdl = SDL_CreateWindow("DOOM",
						SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
						width, height,
						windowFlags);						

	if(windowHdl == NULL)
		I_Error("VideoHandler: Failed to initialize SD");

	renderHdl = SDL_CreateRenderer(windowHdl, -1, renderFlags);

	Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif
	
	blitSurface = SDL_CreateRGBSurface(0,
						width, height,
						8,
						rmask, gmask, bmask, amask);
}

VideoHandler::~VideoHandler() {
	if(blitSurface != NULL)
		SDL_FreeSurface(blitSurface);

	if(renderHdl != NULL)
		SDL_DestroyRenderer(renderHdl);

	if(windowHdl != NULL)
		SDL_DestroyWindow(windowHdl);

	SDL_Quit();
}

void VideoHandler::ProcessEvents() {
	SDL_Event sdlEvt;
	event_t	  dEvt;
	while(SDL_PollEvent(&sdlEvt)) {
		memset(&dEvt, 0, sizeof(event_t));

		switch(sdlEvt.type) {
			case SDL_KEYDOWN:
			case SDL_KEYUP: {
				const SDL_KeyboardEvent *kbEvt = dynamic_cast<const SDL_KeyboardEvent *>(&sdlEvt);
	
				dEvt.type = (sdlEvt.type == SDL_KEYDOWN ? ev_keydown : ev_keyup);	
				
				dEvt.data1 = TranslateKey(kbEvt->key.keysym.sym,
							  kbEvt->key.);
				D_PostEvent(&dEvt);

				break;
			}

			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP: {
				const SDL_MouseButtonEvent *mbEvt = dynamic_Cast<const SDL_MouseButtonEvent *>(&sdlEvt);

				MouseData md;
				md.btnClick = true;
				md.btnMask = TranslateMouseButtons(mbEvt->button);
				md.x = mbEvt->x;
				md.y = mbEvt->y;
				lastMouseData = md;
				
				dEvt.type = ev_mouse;
				dEvt.data1 = md.btnMask;
				
				D_PostEvent(&dEvt);
				break;
			}

			case SDL_MOUSEMOTION: {	
				const SDL_MouseButtonEvent *mbEvt = dynamic_Cast<const SDL_MouseButtonEvent *>(&sdlEvt);
				
				MouseData md;
				md.btnMask = TranslateMouseButtons(mbEvt->button);
				md.x = mbEvt->x;
				md.y = mbEvt->y;
				lastMouseData = md;

				dEvt.type = ev_mouse;
				dEvt.data1 = md.btnMask;
				dEvt.data2 = (md.x - lastMouseData.x);
				dEvt.data3 = (lastMouseData.y - md.y);

				if(dEvt.data2 || dEvt.data3)
					D_PostEvent(&dEvt);
				
				break;
			}
		}
	}
}

void VideoHandler::UpdateNoBlit() {

}

void VideoHandler::StartFrame() {

}

void VideoHandler::StartTic() {
	ProcessEvents();
}

void VideoHandler::FinishUpdate() {
	SDL_LockSurface(blitSurface);
	memcpy(blitSurface->pixels, screens[0], (width * height));	
	SDL_UnlockSurface(blitSurface);

	SDL_Texture * textBuf = SDL_CreateTextureFromSurface(renderHdl, blitSurface);
	SDL_RenderClear(renderHdl);
	SDL_RenderCopy(renderHdl, textBuf,
			NULL, NULL);
	SDL_RenderPresent(renderHdl);
	SDL_DestroyTexture(textBuf);
}

void VideoHandler::ReadScreen(byte *pixDest) {
	memcpy(pixDest, screens[0], width * height);
}



