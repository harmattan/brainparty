// Brain Party
// Copyright (C) 2010 Paul Hudson (http://www.tuxradar.com/brainparty)

// Brain Party is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include <unistd.h>
#include "SDL.h"

#include <SDL_gles.h>
#include <GLES/gl.h>

#include "SDL_mixer.h" 
#include "SDL_ttf.h" 

#include "BPGame.h"

BPGame* Game;

int main(int argc, char *argv[]) {
	if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 ) {
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}
	
	// init all the SDL stuff
        /*putenv((char*)"SDL_VIDEO_WINDOW_POS");
	putenv((char*)"SDL_VIDEO_CENTERED=1");

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);*/
		
	SDL_WM_SetCaption("Brain Party", "Brain Party");
	SDL_WM_SetIcon(SDL_LoadBMP("/opt/brainparty/Content/icon.bmp"), NULL);
	
	Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048);
	TTF_Init();

        SDL_GLES_Init(SDL_GLES_VERSION_1_1);
	
 	SDL_Surface* screen = SDL_SetVideoMode(0, 0, 16, SDL_SWSURFACE | SDL_FULLSCREEN);
        SDL_ShowCursor(0);

        SDL_GLES_Context *context;

        context = SDL_GLES_CreateContext();
        SDL_GLES_MakeCurrent(context);

	// clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// load the splash screen texture
	Texture* tex_splash = new Texture("hudzillagames", 320, 480);

	// set up all the OpenGL stuff
	glViewport(0, 0, 800, 480);
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

        /**
         * Rotate by 90 degrees, scale to fullscreen, keep
         * aspect ratio and center on screen (see also the
         * ConvertCoordinates function in BPGame.h).
         **/
        glRotatef(90, 0, 0, 1);
	glOrthof(0, 320, 506, -26, -100.0f, 100.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// now render the splash screen...
	tex_splash->Draw(0, 0);

	// ...update the screen
	SDL_GLES_SwapBuffers();
	
	// and flush out any SDL events that are waiting - this makes the window activate
	SDL_Event event;
	while (SDL_PollEvent(&event)) { }
	
	// load all the game data
	Game = new BPGame();
	Game->Init(320, 480);
	
	// finally sleep for a second so the splash screen is visible
	SDL_Delay(1000);
	
	// free up the memory; the splash screen isn't used again
	delete tex_splash;


	float seconds_elapsed;
	int previous_time = SDL_GetTicks();
	
	bool mouse_down = false;
	
	while (true) {	
		int new_time = SDL_GetTicks();
		int ticks_elapsed = new_time - previous_time;
		previous_time = new_time;
		
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_MOUSEBUTTONDOWN:
					mouse_down = true;
					Game->TouchStart(event.button.x, event.button.y);
					break;
				case SDL_MOUSEBUTTONUP:
					mouse_down = false;
					Game->TouchStop(event.button.x, event.button.y);
					break;
				case SDL_MOUSEMOTION:
					if (mouse_down) Game->TouchDrag(event.motion.x, event.motion.y);
					break;
				case SDL_KEYUP:
					if (Game->ShowingClearScores) {
						Game->ShowingMessageBox = false;
						Game->ShowingClearScores = false;
						
						if (event.key.keysym.sym == (Uint16)'y') {
							Game->ExecuteResetScores();
						}
					}
					
					break;
				case SDL_QUIT:
					exit(0);
			}
		}

                if (Game->GameState == DO_QUIT) {
                    break;
                }

		float target_time = 16;

		if (ticks_elapsed < target_time) {
			SDL_Delay(target_time - ticks_elapsed);
		}

		seconds_elapsed = ticks_elapsed / 1000.0f;
		Game->Update(seconds_elapsed);
		Game->Draw();
		SDL_GLES_SwapBuffers();
	}
	
	delete Game;

        SDL_GLES_DeleteContext(context);
        SDL_GLES_Quit();

	TTF_Quit();
	Mix_CloseAudio();

	SDL_Quit();

        return EXIT_SUCCESS;
}
