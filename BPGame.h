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

#ifndef __BPGAME_H__
#define __BPGAME_H__

#define SAFE_DELETE(a) { delete(a); (a) = 0; }
#define TIMERREDRAW 1000

#include <vector>
#include <map>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream> 

#include "SDL.h"

#include <SDL_gles.h>
#include <GLES/gl.h>

#include "SDL_mixer.h"

#include "Texture.h"
#include "Colour.h"
#include "BPPoint.h"
#include "MessageBox.h"
#include "BPList.h"
#include "Minigame.h"
#include "MiniGameContainer.h"
#include "TestResultContainer.h"
#include "SpriteFont.h"
#include "BGObject.h"

enum GameStates { FIRST_RUN, MAIN_MENU, ABOUT, PLAY_MENU, PRACTISE_MENU, PLAYING, TEST_STATUS, TEST_RESULTS, PROFESSOR, OPTIONS, HELP, HISTORY, CREDITS, STORE, BRAINBOOST, MARATHON, MAIN_MENU_PLAY, PLAY_MAIN_MENU, PLAY_PRACTISE, PRACTISE_PLAY, PLAY_HISTORY, HISTORY_CREDITS, CREDITS_HISTORY, HISTORY_PLAY, TEST_RESULTS_PLAY, PRACTISE_PROFESSOR, MAIN_MENU_OPTIONS, OPTIONS_MAIN_MENU, MAIN_MENU_HELP, HELP_MAIN_MENU, DO_QUIT };
enum MiniGameType { BALLOONBLASTER, BOMBHUNT, BPSAYS, BUBBLETROUBLE, CARDMATCH, CONNEX, CUPSNBALLS, DICEOFF, FLASHCOUNTING, FLASHLIGHT, IQTEST, JEWELFLIP, JEWELJAM, MARBLEDROP, MEMORYBLOX, MEMORYBOX, MEMORYMATHS, MINESWEEP, MOONJUMP, NEXTINLINE, NUMBERSNAKE, ODDONEOUT, PATCHMATCH, PERFECTPATHS, ROUTEFINDER, RPS, SCRAMBLED, SETFINDER, SHARPSHOOTER, SHORTCIRCUITSUDOKU, SHUFFLEPUZZLER, STRANGERDANGER, SYMBOLICLOGIC, UNDERTHEHAT, UNTANGLER, WORDSMASH };
enum FontSizes { TINY = 16, XSMALL = 19, SMALL = 22, NORMAL = 24, LARGE = 30, XLARGE = 40, XXLARGE = 50, MEGA = 72, BRAINWEIGHT = 82, ALMOSTEPIC = 90, EPIC = 96, LARGEST = 120 };
enum Colours { WHITE, BLACK, NOCOLOUR };

class BPMiniGame;
class SpriteFont;

struct cmp_str {
	bool operator()(char const *a, char const *b) {
		return std::strcmp(a, b) < 0;
	}
};

class BPGame {
public:
    	int GLViewRenderbuffer;
	int GLViewFramebuffer;
	
	int TickCount; // number of ticks ever
	int ElapsedTickCount; // number of ticks since last update
	float ElapsedSeconds;
	
	int CheatMainTaps;
	int CheatOptionsTaps;

	bool EnableSound;
	bool EnableMusic;
	
	int NumUnlockedGames;
	
	BPPoint EmptyPoint;
	
	Colour* Black;
	Colour* White;
	Colour* TransparentWhite;
	Colour* Blue;
	Colour* Cyan;
	Colour* Green;
	Colour* ConnexGreen;
	Colour* Orange;
	Colour* Red;
	Colour* LightRed;
	Colour* DarkRed;
	Colour* Yellow;
	Colour* DarkGrey;
	
	static Texture* sfcLogo;
	static bool ShowingMessageBox;
	static bool ShowingClearScores;
	static SpriteFont* sfcMessageBoxTitle;
	static SpriteFont* sfcMessageBoxText;
	
	Texture* sfcMainMenu;
	Texture* sfcPlayMenu;
	Texture* sfcPractiseMenu;
	Texture* sfcPractiseMenuHider;
	Texture* sfcProfessorTalk;
	Texture* sfcCredits;
	Texture* sfcOptions;
	Texture* sfcOptionsSoundsOff;
	Texture* sfcOptionsMusicOff;
	Texture* sfcHistoryBackground;
	Texture* sfcMarathonMode;
	Texture* sfcBrainBoost;
	Texture* sfcFirstRun;
	Texture* sfcHelp;
	
	Texture* sfcLoading;
	
	Texture* sfcSecretHi;
	Texture* sfcSecretLo;
	
	Texture* sfcResultsFail;
	Texture* sfcResultsBronze;
	Texture* sfcResultsSilver;
	Texture* sfcResultsGold;
	Texture* sfcResultsPlatinum;
	
	Texture* sfcBottomBar;
	Texture* sfcMinigameHelp;
	Texture* sfcMinigameBack;
	Texture* sfcQuitTest;
	
	Texture* sfcWhite;
	
	Texture* sfcMiniGameUnknown;
	
	Texture* sfcTestBackground;
	Texture* sfcResultsBackground;
	Texture* sfcResultsBackground2;
	map<int, Texture*> sfcResults90;
	
	BPList<Texture*> sfcBGObjectTypes;
	BPList<BGObject*> BackgroundObjects;
	int LastBGObject;
	
	BPList<Texture*> sfcStarTypes;
	
	Mix_Music* Music;
	map<const char*, Mix_Chunk*, cmp_str> Sounds;
	
	Texture* sfcCorrect;
	Texture* sfcWrong;
	
	BPPList<Texture*> sfcTestStatus;
	BPPList<TestResultContainer*> TestResultContainers;
	
	GameStates GameState;
	int LastStateChange;
	bool GameStateJustChanged; // used to skip one frame of anim updates to avoid jumping after long game changes such as starting a minigame
	
	float AnimCounter;
	float PractiseAnimCounter;
	
	BPMiniGame* ActiveMiniGame;
	string* CurrentMiniGame;
	
	int PractisePageNumber;
	bool PractisePageMoveLeft; // true if items should be moving right to left
	
	bool FirstRun;
	bool Secret1;
	bool Secret2;
	bool Secret3;
	bool Secret4;
	
	void Init(int width, int height);
	void Update(float elapsed);
	void Draw();
	
	BPPoint TouchEvent;
	
	BPPList<const char*> TestBrainJobs;
	BPPList<const char*> PeopleNames;
	
	map<MiniGameType, BPMiniGame_Container*> MiniGames;
	
	BPList<int> AllTestScores;
	BPList<int> ShowScores;
	BPList<MiniGameType> TestMiniGames;
	BPList<int> TestScores;
	
	SpriteFont* sfcTestWeight;
	SpriteFont* sfcTestJob;
	SpriteFont* sfcHighestBrainWeight;
	
	int TestPosition;
	bool InTestMode;
	
	int CurrentBrainWeight;
	int HighestBrainWeight;
	int TestBrainWeight;
	const char* TestBrainJob;
	
	float Acceleration[3];
		
        inline void ConvertCoordinates(float &x, float &y) {
            /**
             * This converts the physical (screen) coordinates to the
             * game world coordinates. We've rotated the screen by 90
             * degrees, and the resolution is also different. We then
             * center the scaled image on the screen (the "-26"). The
             * "1.666/1.6" is here, because the screen has a ratio of
             * 480/800 and the world 480/320.
             *
             *   world:      screen:
             *   a--b        b--d
             *   |  |        |  |
             *   c--d        a--c
             *   320x480     800x480
             *
             * The world is centered on the screen, so the world takes
             * up 720 pixels height on the screen - 40 pixels are black
             * border at the top (screen: left) and bottom (screen: right).
             *
             * These 40 pixels are ~ 26 pixels in world coordinates:
             *   offset_in_world = (40 * 480 / 800) * 1.666 / 1.5
             **/
            float tmp = (480.-y)*320./480.;
            y = ((x*480./800.)*1.666/1.5)-26;
            x = tmp;
        }
	void TouchStart(float x, float y);
	void TouchStop(float x, float y);
	void TouchDrag(float x, float y);
	void Accelerate(float x, float y, float z);
	
	Texture* LoadBitmap(const char* filename, int actualwidth = 100, int actualheight = 100);
	Mix_Chunk* LoadSound(const char* filename, const char* extension = "wav");
	void PlaySound(const char* sound, bool preload_only = false, bool force_play = false);
	void AddTestScore(int score);
	void AddPlatinumGame();
	
	void FillRectangle(Colour col, float x, float y, float width, float height);
	void DrawImage(Texture* tex, float x, float y);
	void DrawImage(Texture* tex, float x, float y, Colour &col);
	void DrawImage(Texture* tex, float x, float y, float rotation, float scale, Colour &col);
	static void AllocString(SpriteFont** tex, const char* str, FontSizes size, float width, float height, Alignments align, bool bold = false);	
	void DrawString(SpriteFont* tex, Colour &col, float x, float y);
	void DrawString(SpriteFont* tex, Colours col, float x, float y);
	void DrawString(SpriteFont* tex, float x, float y);
	void Clear(Colour* col);
	bool PointOverRect(int x1, int y1, int x2, int y2, int width, int height);
	bool RectOverRect(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);
	int RandomRange(int min, int max);
	int RandomRangeExcept(int min, int max, int except);
	string SeparateThousands(int number);
	string* TicksToTime(int ticks);
	int DivRem(int Num, int Div, int* Rem);
	void DrawLine(int fromx, int fromy, int tox, int toy, Colour* col, float width);
	float Hermite(float value1, float tangent1, float value2, float tangent2, float amount);
	float Lerp(float value1, float value2, float amount);
	float SmoothStep(float value1, float value2, float amount);
	float SmoothStep2(float value1, float value2, float amount);
	Colour ColourSmoothStep (Colour &from, Colour &to, float amount);
	float Clamp(float value, float minval, float maxval);
	bool StartsWithVowel(string* word);
	const char* GetName();
	Colour* ColorLerp (Colour* from, Colour* to, float amount);
		
	void LoadSettings();
	void SaveSettings();
	
	void RenderMainMenu(float xoff, float yoff);
	void RenderOptionsMenu();
	void RenderPlayMenu(float xoff, float yoff);
	void RenderPractiseMenu();
	void RenderProfessor();
	void RenderPlaying();
	void RenderTestStatus();
	void RenderTestResults(float xoff, float yoff);
	void RenderHistory(float xoff, float yoff);
	void RenderBrainBoost();
	void RenderMarathon();
	void RenderCredits(float xoff, float yoff);
	void RenderHelp(float xoff, float yoff);
	
	void CalculateTestResults();
	void CreateMiniGame(MiniGameType game, bool marathon, bool practise);
	void SetGameState(GameStates state);
	void PositionPractiseGames();
	
	void RunTest();
	void NextTest();
	void CancelTest();
	
	void PlayMusic(const char* name);
	void StopMusic();
	
	void PromptResetScores();
	void ExecuteResetScores();
	
	bool FileExists(const char* filename);
	
	static bool IsNull(void* somepointer) { return (somepointer == NULL); }
	
	vector<string> Split(const string& s);
};

#endif
