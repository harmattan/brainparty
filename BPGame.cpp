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

#include <ctime>
#include "BPGame.h"
#include "SDL.h"

#include <SDL_gles.h>
#include <GLES/gl.h>

#include "SDL_ttf.h" 
#include <string>


#include "bombhunt.h"
#include "balloonblaster.h"
#include "bpsays.h"
#include "bubbletrouble.h"
#include "cardmatch.h"
#include "connex.h"
#include "cupsnballs.h"
#include "diceoff.h"
#include "flashcounting.h"
#include "flashlight.h"
#include "iqtest.h"
#include "jewelflip.h"
#include "jeweljam.h"
#include "marbledrop.h"
#include "memoryblox.h"
#include "memorybox.h"
#include "memorymaths.h"
#include "minesweep.h"
#include "moonjump.h"
#include "nextinline.h"
#include "numbersnake.h"
#include "oddoneout.h"
#include "patchmatch.h"
#include "perfectpaths.h"
#include "routefinder.h"
#include "rps.h"
#include "scrambled.h"
#include "setfinder.h"
#include "sharpshooter.h"
#include "shortcircuitsudoku.h"
#include "shufflepuzzler.h"
#include "strangerdanger.h"
#include "symboliclogic.h"
#include "underthehat.h"
#include "untangler.h"
#include "wordsmash.h"

Texture* BPGame::sfcLogo;
bool BPGame::ShowingMessageBox;
bool BPGame::ShowingClearScores;
SpriteFont* BPGame::sfcMessageBoxText;
SpriteFont* BPGame::sfcMessageBoxTitle;

void BPGame::Init(int width, int height) {
	srand(time(0));
	
	EnableSound = true;
	EnableMusic = true;
	
	EmptyPoint = BPPoint(0,0);
	LastStateChange = 0;
	
	LastBGObject = 0;
	
	Black = new Colour(0.0f, 0.0f, 0.0f, 1.0f);
	White = new Colour(1.0f, 1.0f, 1.0f, 1.0f);
	TransparentWhite = new Colour(1.0f, 1.0f, 1.0f, 0.0f);
	
	Blue = new Colour(0.0f, 0.0f, 1.0f, 1.0f);
	Cyan = new Colour(0.0f, 1.0f, 1.0f, 1.0f);
	Green = new Colour(0.0f, 1.0f, 0.0f, 1.0f);
	Orange = new Colour(1.0f, 0.75f, 0.0f, 1.0f);
	Red = new Colour(1.0f, 0.0f, 0.0f, 1.0f);
	LightRed = new Colour(1.0f, 0.5f, 0.6f, 1.0f);
	DarkRed = new Colour(0.5f, 0.0f, 0.0f, 1.0f);
	Yellow = new Colour(1.0f, 1.0f, 0.0f, 1.0f);
	DarkGrey = new Colour(0.5f, 0.5f, 0.5f, 1.0f);
	ConnexGreen = new Colour(0.0f, 0.8f, 0.0f, 0.7f);
	
	CheatMainTaps = CheatOptionsTaps = 0;

	InTestMode = false;
	
	TickCount = 0;
	ElapsedSeconds = 0;
	AnimCounter = 0.0f;
	PractiseAnimCounter = 0.0f;
	
	PractisePageNumber = 0;
	PractisePageMoveLeft = true;
	
	sfcTestWeight = NULL;
	sfcTestJob = NULL;
	sfcHighestBrainWeight = NULL;
	
	HighestBrainWeight = -1;
	CurrentBrainWeight = -1;
	
	TestBrainJobs.Add("Fast Food Assistant");
	TestBrainJobs.Add("Fast Food Manager");
	TestBrainJobs.Add("Bingo Club Manager");
	TestBrainJobs.Add("Web Designer");
	TestBrainJobs.Add("Personal Shopper");
	TestBrainJobs.Add("Aromatherapist");
	TestBrainJobs.Add("Solicitor");
	TestBrainJobs.Add("Psychotherapist");
	TestBrainJobs.Add("Lifestyle Consultant");
	TestBrainJobs.Add("Librarian");
	TestBrainJobs.Add("Acupuncturist");
	TestBrainJobs.Add("Insurance Broker");
	TestBrainJobs.Add("Bank Manager");
	TestBrainJobs.Add("Fashion Stylist");
	TestBrainJobs.Add("Interior Designer");
	TestBrainJobs.Add("Fashion Consultant");
	TestBrainJobs.Add("Sales Assistant");
	TestBrainJobs.Add("Actor");
	TestBrainJobs.Add("Sales Executive");
	TestBrainJobs.Add("Movie Director");
	TestBrainJobs.Add("Hotel Manager");
	TestBrainJobs.Add("Botanist");
	TestBrainJobs.Add("Marketing Executive");
	TestBrainJobs.Add("Economist");
	TestBrainJobs.Add("Statistician");
	TestBrainJobs.Add("Theatre Director");
	TestBrainJobs.Add("Nurse");
	TestBrainJobs.Add("Police Officer");
	TestBrainJobs.Add("Meteorologist");
	TestBrainJobs.Add("Art Valuer");
	TestBrainJobs.Add("Museum Curator");
	TestBrainJobs.Add("Marine Biologist");
	TestBrainJobs.Add("School Teacher");
	TestBrainJobs.Add("Dentist");
	TestBrainJobs.Add("Software Engineer");
	TestBrainJobs.Add("Forensic Scientist");
	TestBrainJobs.Add("Psychiatrist");
	TestBrainJobs.Add("Archaeologist");
	TestBrainJobs.Add("Engineer");
	TestBrainJobs.Add("Investment Analyst");
	TestBrainJobs.Add("Physicist");
	TestBrainJobs.Add("Sculptor");
	TestBrainJobs.Add("Painter");
	TestBrainJobs.Add("Poet");
	TestBrainJobs.Add("Doctor");
	TestBrainJobs.Add("Composer");
	TestBrainJobs.Add("Fighter Pilot");
	TestBrainJobs.Add("Architect");
	TestBrainJobs.Add("Brain Surgeon");
	TestBrainJobs.Add("Professor");
	TestBrainJobs.Add("Philosopher");
	TestBrainJobs.Add("Astronaut");
	TestBrainJobs.Add("Nobel Prize Winner");
	TestBrainJobs.Add("Genius");
	TestBrainJobs.Add("Super Genius");
	TestBrainJobs.Add("Albert Einstein");
	
	PeopleNames.Add("Andrea");
	PeopleNames.Add("Andrew");
	PeopleNames.Add("Bob");
	PeopleNames.Add("Catherine");
	PeopleNames.Add("Dawn");
	PeopleNames.Add("Diana");
	PeopleNames.Add("Esther");
	PeopleNames.Add("Graham");
	PeopleNames.Add("Hannah");
	PeopleNames.Add("Jack");
	PeopleNames.Add("James");
	PeopleNames.Add("Jeff");
	PeopleNames.Add("Jeremy");
	PeopleNames.Add("John");
	PeopleNames.Add("Kathryn");
	PeopleNames.Add("Laura");
	PeopleNames.Add("Louise");
	PeopleNames.Add("Lyn");
	PeopleNames.Add("Mark");
	PeopleNames.Add("Matthew");
	PeopleNames.Add("Mike");
	PeopleNames.Add("Paul");
	PeopleNames.Add("Rebecca");
	PeopleNames.Add("Richard");
	PeopleNames.Add("Sandra");
	PeopleNames.Add("Scott");
	PeopleNames.Add("Tanya");
	PeopleNames.Add("Terry");
	PeopleNames.Add("Tim");
	PeopleNames.Add("William");
	
	
	sfcMiniGameUnknown = LoadBitmap("minigame_unknown", 90, 90);
	MiniGames[BALLOONBLASTER] = new BPMiniGame_Container(BALLOONBLASTER, "balloonblaster", "Balloon Blaster!", LoadBitmap("minigame_balloonblaster", 90, 90), false);
	MiniGames[BOMBHUNT] = new BPMiniGame_Container(BOMBHUNT, "bombhunt", "Bomb Hunt", LoadBitmap("minigame_bombhunt", 90, 90), false);
	MiniGames[BPSAYS] = new BPMiniGame_Container(BPSAYS, "bpsays", "Brain Party says...", LoadBitmap("minigame_bpsays", 90, 90), false);
	MiniGames[BUBBLETROUBLE] = new BPMiniGame_Container(BUBBLETROUBLE, "bubbletrouble", "Bubble Trouble", LoadBitmap("minigame_bubbletrouble", 90, 90), false);
	MiniGames[CARDMATCH] = new BPMiniGame_Container(CARDMATCH, "cardmatch", "Card Match", LoadBitmap("minigame_cardmatch", 90, 90), false);
	MiniGames[CUPSNBALLS] = new BPMiniGame_Container(CUPSNBALLS, "cupsnballs", "Cups 'n' Balls", LoadBitmap("minigame_cupsnballs", 90, 90), false);
	MiniGames[CONNEX] = new BPMiniGame_Container(CONNEX, "connex", "Connex", LoadBitmap("minigame_connex", 90, 90), false);
	MiniGames[DICEOFF] = new BPMiniGame_Container(DICEOFF, "diceoff", "Dice Off", LoadBitmap("minigame_unknown", 90, 90), true);
	MiniGames[FLASHCOUNTING] = new BPMiniGame_Container(FLASHCOUNTING, "flashcounting", "Flash Counting", LoadBitmap("minigame_flashcounting", 90, 90), false);
	MiniGames[FLASHLIGHT] = new BPMiniGame_Container(FLASHLIGHT, "flashlight", "Flashlight", LoadBitmap("minigame_flashlight", 90, 90), false);
	MiniGames[IQTEST] = new BPMiniGame_Container(IQTEST, "iqtest", "IQ Test", LoadBitmap("minigame_iqtest", 90, 90), false);
	MiniGames[JEWELFLIP] = new BPMiniGame_Container(JEWELFLIP, "jewelflip", "Jewel Flip", LoadBitmap("minigame_jewelflip", 90, 90), false);
	MiniGames[JEWELJAM] = new BPMiniGame_Container(JEWELJAM, "jeweljam", "Jewel Jam", LoadBitmap("minigame_jeweljam", 90, 90), false);
	MiniGames[MARBLEDROP] = new BPMiniGame_Container(MARBLEDROP, "marbledrop", "Marble Drop", LoadBitmap("minigame_marbledrop", 90, 90), false);
	MiniGames[MEMORYBLOX] = new BPMiniGame_Container(MEMORYBLOX, "memoryblox", "MemoryBlox", LoadBitmap("minigame_memoryblox", 90, 90), false);
	MiniGames[MEMORYBOX] = new BPMiniGame_Container(MEMORYBOX, "memorybox", "Memory Box", LoadBitmap("minigame_memorybox", 90, 90), false);
	MiniGames[MEMORYMATHS] = new BPMiniGame_Container(MEMORYMATHS, "memorymaths", "Memory Maths", LoadBitmap("minigame_memorymaths", 90, 90), false);
	MiniGames[MINESWEEP] = new BPMiniGame_Container(MINESWEEP, "minesweep", "Mine Sweep", LoadBitmap("minigame_minesweep", 90, 90), false);
	MiniGames[MOONJUMP] = new BPMiniGame_Container(MOONJUMP, "moonjump", "Moon Jump", LoadBitmap("minigame_moonjump", 90, 90), false);
	MiniGames[NEXTINLINE] = new BPMiniGame_Container(NEXTINLINE, "nextinline", "Next in Line", LoadBitmap("minigame_nextinline", 90, 90), false);
	MiniGames[NUMBERSNAKE] = new BPMiniGame_Container(NUMBERSNAKE, "numbersnake", "Number Snake", LoadBitmap("minigame_numbersnake", 90, 90), false);
	MiniGames[ODDONEOUT] = new BPMiniGame_Container(ODDONEOUT, "oddoneout", "Odd One Out", LoadBitmap("minigame_oddoneout", 90, 90), false);
	MiniGames[PATCHMATCH] = new BPMiniGame_Container(PATCHMATCH, "patchmatch", "Patch Match", LoadBitmap("minigame_patchmatch", 90, 90), false);
	MiniGames[PERFECTPATHS] = new BPMiniGame_Container(PERFECTPATHS, "perfectpaths", "Perfect Paths", LoadBitmap("minigame_perfectpaths", 90, 90), false);
	MiniGames[ROUTEFINDER] = new BPMiniGame_Container(ROUTEFINDER, "routefinder", "Route Finder", LoadBitmap("minigame_routefinder", 90, 90), false);
	MiniGames[RPS] = new BPMiniGame_Container(RPS, "rockpaperscissors", "Rock, Paper, Scissors", LoadBitmap("minigame_rps", 90, 90), false);
	MiniGames[SCRAMBLED] = new BPMiniGame_Container(SCRAMBLED, "scrambled", "Scrambled", LoadBitmap("minigame_scrambled", 90, 90), false);
	MiniGames[SETFINDER] = new BPMiniGame_Container(SETFINDER, "setfinder", "Set Finder", LoadBitmap("minigame_setfinder", 90, 90), false);
	MiniGames[SHARPSHOOTER] = new BPMiniGame_Container(SHARPSHOOTER, "sharpshooter", "Sharpshooter", LoadBitmap("minigame_sharpshooter", 90, 90), true);
	MiniGames[SHORTCIRCUITSUDOKU] = new BPMiniGame_Container(SHORTCIRCUITSUDOKU, "shortcircuitsudoku", "Short-circuit Sudoku", LoadBitmap("minigame_shortcircuitsudoku", 90, 90), false);
	MiniGames[SHUFFLEPUZZLER] = new BPMiniGame_Container(SHUFFLEPUZZLER, "shufflepuzzler", "Shuffle Puzzler", LoadBitmap("minigame_shufflepuzzler", 90, 90), true);
	MiniGames[STRANGERDANGER] = new BPMiniGame_Container(STRANGERDANGER, "strangerdanger", "Stranger Danger", LoadBitmap("minigame_unknown", 90, 90), true);
	MiniGames[SYMBOLICLOGIC] = new BPMiniGame_Container(SYMBOLICLOGIC, "symboliclogic", "Symbolic Logic", LoadBitmap("minigame_symboliclogic", 90, 90),  false);
	MiniGames[UNDERTHEHAT] = new BPMiniGame_Container(UNDERTHEHAT, "underthehat", "Under the Hat", LoadBitmap("minigame_underthehat", 90, 90), true);
	MiniGames[UNTANGLER] = new BPMiniGame_Container(UNTANGLER, "untangler", "Untangler", LoadBitmap("minigame_untangler", 90, 90), false);
	MiniGames[WORDSMASH] = new BPMiniGame_Container(WORDSMASH, "wordsmash", "Word Smash", LoadBitmap("minigame_unknown", 90, 90), true);

	sfcLogo = LoadBitmap("brainparty", 312, 227);
	ShowingMessageBox = false;
	ShowingClearScores = false;
	sfcMessageBoxTitle = NULL;
	sfcMessageBoxText = NULL;
	
	sfcTestStatus.Add(LoadBitmap("teststatus1", 320, 75));
	sfcTestStatus.Add(LoadBitmap("teststatus2", 320, 75));
	sfcTestStatus.Add(LoadBitmap("teststatus3", 320, 75));
	sfcTestStatus.Add(LoadBitmap("teststatus4", 320, 75));
	sfcTestStatus.Add(LoadBitmap("teststatus5", 320, 75));
	
	sfcResults90[FAIL] = LoadBitmap("results_fail_90", 90, 90);
	sfcResults90[BRONZE] = LoadBitmap("results_bronze_90", 90, 90);
	sfcResults90[SILVER] = LoadBitmap("results_silver_90", 90, 90);
	sfcResults90[GOLD] = LoadBitmap("results_gold_90", 90, 90);
	sfcResults90[PLATINUM] = LoadBitmap("results_platinum_90", 90, 90);

	sfcFirstRun = LoadBitmap("firstrun", 320, 480);
	sfcHelp = LoadBitmap("menu_help", 320, 480);
	sfcLoading = LoadBitmap("loading", 320, 480);
	
	sfcOptions = LoadBitmap("menu_options", 320, 480);
	sfcOptionsSoundsOff = LoadBitmap("sound_off", 320, 48);
	sfcOptionsMusicOff = LoadBitmap("music_off", 320, 48);
	
	sfcTestBackground = LoadBitmap("bg_test", 512, 512);
	sfcResultsBackground = LoadBitmap("bg_results", 512, 512);
	sfcResultsBackground2 = LoadBitmap("bg_results2", 320, 480);
	sfcHistoryBackground = LoadBitmap("history", 320, 480);
	
	sfcMarathonMode = LoadBitmap("menu_marathonmode", 320, 480);
	sfcBrainBoost = LoadBitmap("menu_brainboost", 320, 480);
	
	sfcBGObjectTypes.Add(LoadBitmap("emptycircle", 256, 256));
	sfcBGObjectTypes.Add(LoadBitmap("doubleemptycircle", 256, 256));
	sfcBGObjectTypes.Add(LoadBitmap("doublecircle", 256, 256));
	sfcBGObjectTypes.Add(LoadBitmap("circlering", 256, 256));
	sfcBGObjectTypes.Add(LoadBitmap("filledcircle", 256, 256));
	
	sfcSecretLo = LoadBitmap("secret_lo", 71, 66);
	sfcSecretHi = LoadBitmap("secret_hi", 71, 66);
	
	sfcStarTypes.Add(LoadBitmap("star_dark", 3, 3));
	sfcStarTypes.Add(LoadBitmap("star_medium", 3, 3));
	sfcStarTypes.Add(LoadBitmap("star_bright", 3, 3));

	Sounds["baa"] = LoadSound("baa");
	Sounds["baa2"] = LoadSound("baa2");
	Sounds["baa3"] = LoadSound("baa3");
	Sounds["balloon_pop"] = LoadSound("balloon_pop");
	Sounds["beep_hi"] = LoadSound("beep_hi");
	Sounds["bubble"] = LoadSound("bubble");
	Sounds["card_flip"] = LoadSound("card_flip");
	Sounds["card_flip2"] = LoadSound("card_flip2");
	Sounds["click"] = LoadSound("click");
	Sounds["correct"] = LoadSound("correct");
	Sounds["down"] = LoadSound("down");
	Sounds["explosion"] = LoadSound("explosion");
	Sounds["gem_select"] = LoadSound("gem_select");
	Sounds["glass_break"] = LoadSound("glass_break");
	Sounds["gun1"] = LoadSound("gun1");
	Sounds["gun2"] = LoadSound("gun2");
	Sounds["happy_ring"] = LoadSound("happy_ring");
	Sounds["meow"] = LoadSound("meow");
	Sounds["mouse_click"] = LoadSound("mouse_click");
	Sounds["result"] = LoadSound("result");
	Sounds["slide"] = LoadSound("slide");
	Sounds["soft_boom"] = LoadSound("soft_boom");
	Sounds["swoosh_long"] = LoadSound("swoosh_long");
	Sounds["whack"] = LoadSound("whack");
	Sounds["wrong"] = LoadSound("wrong");
	Sounds["wrong2"] = LoadSound("wrong2");
	Sounds["wrong3"] = LoadSound("wrong3");
	Sounds["zap"] = LoadSound("zap");
	
	PlaySound("mouse_click", true, false);
	
	Music = NULL;
		
	sfcMainMenu = LoadBitmap("menu_main", 320, 480);
	sfcPlayMenu = LoadBitmap("menu_play", 320, 480);
	sfcPractiseMenu = LoadBitmap("menu_practise", 320, 480);
	sfcPractiseMenuHider = LoadBitmap("menu_practise_hider", 134, 34);
	sfcProfessorTalk = LoadBitmap("professor_talk", 320, 480);
	sfcCredits = LoadBitmap("menu_credits", 320, 480);
	
	sfcBottomBar = LoadBitmap("bottombar", 320, 64);
	
	sfcQuitTest = LoadBitmap("quit_test", 320, 480);
	
	sfcResultsFail = LoadBitmap("results_fail", 320, 480);
	sfcResultsBronze = LoadBitmap("results_bronze", 320, 480);
	sfcResultsSilver = LoadBitmap("results_silver", 320, 480);
	sfcResultsGold = LoadBitmap("results_gold", 320, 480);
	sfcResultsPlatinum = LoadBitmap("results_platinum", 320, 480);
	
	sfcWhite = LoadBitmap("white", 64, 64);
	
	sfcCorrect = LoadBitmap("correct", 320, 92);
	sfcWrong = LoadBitmap("wrong", 320, 92);

	ActiveMiniGame = NULL;
	
	LoadSettings();
	
	if (FirstRun) {
		SetGameState(FIRST_RUN);
	} else {
		SetGameState(MAIN_MENU);		
	}
	
	PlayMusic("theme");
	
	
	// Hackage for stress testing!
	
//	for (map<MiniGameType, BPMiniGame_Container*>::const_iterator it = MiniGames.begin(); it != MiniGames.end(); ++it) {
//		for (int i = 0; i < 1000000; ++i) {
//			NSLog("Creating %", it->second->ShortName);
//			CreateMiniGame(UNTANGLER, false, true);
//			ActiveMiniGame->Start();
//		}
//	}
}

void BPGame::Update(float elapsed) {
	ElapsedSeconds = elapsed;
	ElapsedTickCount = floor(elapsed * 1000);
	TickCount += ElapsedTickCount;
	
	if (ShowingMessageBox) {
		return;
	}

	if (!GameStateJustChanged) {
		// some gamestates need to increment the animcounter - do that now!
		switch (GameState) {
			case MAIN_MENU_PLAY:
			case PLAY_MAIN_MENU:
			case PRACTISE_PROFESSOR:
			case MAIN_MENU_OPTIONS:
			case OPTIONS_MAIN_MENU:
			case PLAY_HISTORY:
			case HISTORY_CREDITS:
			case CREDITS_HISTORY:
			case HISTORY_PLAY:
			case TEST_RESULTS_PLAY:
			case MAIN_MENU_HELP:
			case HELP_MAIN_MENU:
				if (AnimCounter < 1.0f) {
					AnimCounter += 3.5f * elapsed;
					if (AnimCounter > 1.0f) {
						AnimCounter = 1.0f;
					}
				}
				
				break;
				
			case PLAY_PRACTISE:
			case PRACTISE_PLAY:
				if (AnimCounter < 1.0f) {
					AnimCounter += 3.5f * elapsed;
					if (AnimCounter > 1.0f) {
						AnimCounter = 1.0f;
					}
				}
				
				// don't break - we need to execute the following one too!
				
			case PRACTISE_MENU:
				if (PractiseAnimCounter < 1.0f) {
					PractiseAnimCounter += 3.5f * elapsed;
					if (PractiseAnimCounter > 1.0f) {
						PractiseAnimCounter = 1.0f;
					}
				}
				
				break;

			case TEST_STATUS:
				if (LastStateChange + 5000 < TickCount) {
					NextTest();
				}
				
				if (LastBGObject + 1000 < TickCount) {
					BGObject* obj = new BGObject();
					obj->Scale = 0.0f;
					obj->Pos = BPPoint(RandomRange(0, 320), RandomRange(0, 480));
					
					switch (RandomRange(0, 17)) {
					case 0:
					case 1:
					case 2:
					case 3:
					case 4:
					case 5:
					case 6:
					case 7:
						obj->Type = sfcBGObjectTypes[0];
						break;
						
					case 8:
					case 9:
					case 10:
					case 11:
						obj->Type = sfcBGObjectTypes[1];
						break;
						
					case 12:
					case 13:
					case 14:
						obj->Type = sfcBGObjectTypes[2];
						break;
						
					case 15:
					case 16:
						obj->Type = sfcBGObjectTypes[3];
						break;
						
					default:
						obj->Type = sfcBGObjectTypes[4];
						break;
					}
					
					BackgroundObjects.Add(obj);
					LastBGObject = TickCount;
				}
					

				for (int i = BackgroundObjects.Count - 1; i >= 0; --i) {
					BackgroundObjects[i]->Scale += 0.3f * ElapsedSeconds;
					
					if (BackgroundObjects[i]->Scale >= 1.0f) {
						BackgroundObjects.RemoveAt(i);
					}
				}
				
				
				break;
				
			case TEST_RESULTS:
				for (int i = 0; i < TestResultContainers.Count; ++i) {
					TestResultContainer* result = TestResultContainers[i];
					
					if (result->GoingUp) {
						result->FadeUpAnim += 0.2f * ElapsedSeconds;
						if (result->FadeUpAnim > 1.0f) {
							result->FadeUpAnim = 1.0f;
							result->GoingUp = false;
						}
					} else {
						result->FadeUpAnim -= 0.4f * ElapsedSeconds;
						if (result->FadeUpAnim < -1.0f) {
							result->FadeUpAnim = -1.0f;
							result->GoingUp = true;
						}
					}
				}
				
				break;
				
		}
	} else {
		GameStateJustChanged = false; // we skipped our animcounter update - now continue as normal
	}
	
	switch (GameState) {
		case PLAY_PRACTISE:
		case PRACTISE_PLAY:
			if (AnimCounter == 1.0f) {
				if (GameState == PLAY_PRACTISE) {
					SetGameState(PRACTISE_MENU);
				} else {
					SetGameState(PLAY_MENU);					
				}
			}
			// don't break - we need to update the practise screen behind the scenes too
			
		case PRACTISE_MENU:		
				for (map<MiniGameType, BPMiniGame_Container*>::const_iterator it = MiniGames.begin(); it != MiniGames.end(); ++it) {
					it->second->X = SmoothStep(it->second->StartX, it->second->DestX, PractiseAnimCounter);
					it->second->Y = SmoothStep(it->second->StartY, it->second->DestY, PractiseAnimCounter);
				}

				break;
			
		case PLAYING:
			ActiveMiniGame->TickMiniGame();
			break;
			
		case MAIN_MENU_PLAY:
			if (AnimCounter == 1.0f) SetGameState(PLAY_MENU);
			break;
			
		case PLAY_MAIN_MENU:
			if (AnimCounter == 1.0f) SetGameState(MAIN_MENU);
			break;

		case PLAY_HISTORY:
			if (AnimCounter == 1.0f) SetGameState(HISTORY);
			break;
			
		case HISTORY_CREDITS:
			if (AnimCounter == 1.0f) SetGameState(CREDITS);
			break;
			
		case CREDITS_HISTORY:
			if (AnimCounter == 1.0f) SetGameState(HISTORY);
			break;
			
		case HISTORY_PLAY:
			if (AnimCounter == 1.0f) SetGameState(PLAY_MENU);
			break;

		case PRACTISE_PROFESSOR:
			if (AnimCounter == 1.0f) SetGameState(PROFESSOR);
			break;
			
		case MAIN_MENU_OPTIONS:
			if (AnimCounter == 1.0f) SetGameState(OPTIONS);
			break;
			
		case OPTIONS_MAIN_MENU:
			if (AnimCounter == 1.0f) SetGameState(MAIN_MENU);
			break;
			
		case TEST_RESULTS_PLAY:
			if (AnimCounter == 1.0f) SetGameState(PLAY_MENU);
			break;
			
		case MAIN_MENU_HELP:
			if (AnimCounter == 1.0f) SetGameState(HELP);
			break;
			
		case HELP_MAIN_MENU:
			if (AnimCounter == 1.0f) SetGameState(MAIN_MENU);
			break;
	}
}

void BPGame::Draw() {
	Clear(Black);
		
	switch (GameState) {
		case FIRST_RUN:
			sfcFirstRun->Draw(0, 0);
			break;
		
		case MAIN_MENU:
			RenderMainMenu(0, 0);
			break;
			
		case MAIN_MENU_PLAY:
			{
				float offval = SmoothStep(0, -320, AnimCounter);
				float onval = SmoothStep(320, 0, AnimCounter);
				
				RenderMainMenu(offval, 0.0f);
				RenderPlayMenu(onval, 0.0f);			
			}
			
			break;
			
		case PLAY_MAIN_MENU:
			{				
				float offval = SmoothStep(-320, 0, AnimCounter);
				float onval = SmoothStep(0, 320, AnimCounter);
				
				RenderMainMenu(offval, 0.0f);
				RenderPlayMenu(onval, 0.0f);			
			}
			
			break;
			
		case PLAY_HISTORY:
			{
				float offval = SmoothStep(0, -480, AnimCounter);
				float onval = SmoothStep(480, 0, AnimCounter);
				
				RenderPlayMenu(0.0f, offval);
				RenderHistory(0.0f, onval);			
			}
			
			break;
			
		case HISTORY_CREDITS:
			{
				float offval = SmoothStep(0, -480, AnimCounter);
				float onval = SmoothStep(480, 0, AnimCounter);
				
				RenderHistory(0.0f, offval);
				RenderCredits(0.0f, onval);			
			}
			
			break;

		case CREDITS_HISTORY:
			{
				float offval = SmoothStep(0, 480, AnimCounter);
				float onval = SmoothStep(-480, 0, AnimCounter);
				
				RenderCredits(0.0f, offval);
				RenderHistory(0.0f, onval);			
			}
			
			break;
			
		case HISTORY_PLAY:
			{
				float offval = SmoothStep(0, 480, AnimCounter);
				float onval = SmoothStep(-480, 0, AnimCounter);
				
				RenderHistory(0.0f, offval);
				RenderPlayMenu(0.0f, onval);			
			}
			
			break;
			
		case MAIN_MENU_OPTIONS:
			RenderOptionsMenu();
			
			glColor4f(1.0f, 1.0f, 1.0f, 1 - AnimCounter);
			RenderMainMenu(0.0f, 0.0f);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			
			break;
			
		case OPTIONS_MAIN_MENU:
			RenderMainMenu(0.0f, 0.0f);
			
			glColor4f(1.0f, 1.0f, 1.0f, 1 - AnimCounter);
			RenderOptionsMenu();
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			
			break;
			
		case MAIN_MENU_HELP:
		{
			float onval = SmoothStep(480, 0, AnimCounter);
			
			RenderMainMenu(0.0f, 0.0f);
			RenderHelp(0.0f, onval);			
		}
			
			break;
			
		case HELP_MAIN_MENU:
		{
			float offval = SmoothStep(0, 480, AnimCounter);

			RenderMainMenu(0.0f, 0.0f);
			RenderHelp(0.0f, offval);
		}
			
			break;
			
		case TEST_RESULTS_PLAY:
			{				
				float offval = SmoothStep(0, 480, AnimCounter);
				float onval = SmoothStep(-480, 0, AnimCounter);
				
				RenderTestResults(0.0f, offval);
				RenderPlayMenu(0.0f, onval);			
			}
			
			break;
			
		case PLAY_MENU:
			RenderPlayMenu(0.0f, 0.0f);
			break;
			
		case PRACTISE_PLAY:
			RenderPractiseMenu();
			
			glColor4f(1.0f, 1.0f, 1.0f, AnimCounter);
			RenderPlayMenu(0.0f, 0.0f);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			
			break;
			
		case PLAY_PRACTISE:
			RenderPractiseMenu();
			
			glColor4f(1.0f, 1.0f, 1.0f, 1 - AnimCounter);
			RenderPlayMenu(0.0f, 0.0f);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			
			break;
			
		case PRACTISE_MENU:
			RenderPractiseMenu();
			break;
			
		case PROFESSOR:
			RenderProfessor();
			break;

		case PRACTISE_PROFESSOR:		
			RenderPractiseMenu();

			glColor4f(1.0f, 1.0f, 1.0f, AnimCounter);
			RenderProfessor();
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			break;
			
		case PLAYING:
			RenderPlaying();
			break;
			
		case TEST_STATUS:
			RenderTestStatus();
			break;
			
		case TEST_RESULTS:
			RenderTestResults(0.0f, 0.0f);
			break;
			
		case OPTIONS:
			RenderOptionsMenu();
			break;
			
		case HISTORY:
			RenderHistory(0.0f, 0.0f);
			break;

		case BRAINBOOST:
			RenderBrainBoost();
			break;
			
		case MARATHON:
			RenderMarathon();
			break;
			
		case CREDITS:
			RenderCredits(0.0f, 0.0f);
			break;
			
		case HELP:
			RenderHelp(0.0f, 0.0f);
			break;
	}
	
	if (ShowingMessageBox) {
		FillRectangle(Colour(0.0f, 0.0f, 0.0f, 0.85f), 0, 0, 320, 480);
		
		Colour col = Colour(1.0f, 1.0f, 1.0f, 1.0f);
		sfcLogo->Draw(160, 90, 0.0f, 0.6f, col);
		sfcMessageBoxTitle->drawAtPoint(0, 170);
		
		glColor4f(0.8f, 0.8f, 0.8f, 1.0f);
		sfcMessageBoxText->drawAtPoint(10, 205);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

Texture* BPGame::LoadBitmap(const char*	 filename, int actualwidth, int actualheight) {
	return new Texture(filename, actualwidth, actualheight);
}

void BPGame::AddTestScore(int score) {
	if (InTestMode) {
		TestScores.Add(score);
		
		if (TestScores.Count == TestMiniGames.Count) {
			// that was the last game in this test run!
			CalculateTestResults();
			SetGameState(TEST_RESULTS);
			InTestMode = false;
			PlayMusic("results");
		} else {
			SetGameState(TEST_STATUS);
		}
	}
}

void BPGame::FillRectangle(Colour col, float x, float y, float width, float height) {
	glColor4f(col.R, col.G, col.B, col.A);
	sfcWhite->Draw(x, y, width, height);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void BPGame::DrawImage(Texture* tex, float x, float y) {
	tex->Draw(x, y);
}

void BPGame::DrawImage(Texture* tex, float x, float y, Colour &col) {
	tex->Draw(x, y, col);
}


void BPGame::DrawImage(Texture* tex, float x, float y, float rotation, float scale, Colour &col) {
	tex->Draw(x, y, rotation, scale, col);	
}

void BPGame::Clear(Colour* col) {
	glClearColor(col->R, col->G, col->B, col->A);
	glClear(GL_COLOR_BUFFER_BIT);
}

bool BPGame::PointOverRect(int x1, int y1, int x2, int y2, int width, int height) {
	if (x1 >= x2 && x1 <= x2 + width) {
		if (y1 >= y2 && y1 <= y2 + height) {
			return true;
		}
	}
	
	return false;
}

bool BPGame::RectOverRect(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2) {
	if (x1 + width1 < x2) return false;
	if (x1 > x2 + width2) return false;
	if (y1 + height1 < y2) return false;
	if (y1 > y2 + height2) return false;
	
	return true;
}

void BPGame::TouchStart(float x, float y) {
	if (ShowingMessageBox) {
		return;
	}
	
        ConvertCoordinates(x, y);
	TouchEvent.X = x;
	TouchEvent.Y = y;

	switch (GameState) {
		case PLAYING:
			if (ActiveMiniGame != NULL) {
				ActiveMiniGame->HandleMouseDown(TouchEvent);
			}
			
			break;
	}
}

void BPGame::TouchStop(float x, float y) {
	if (ShowingMessageBox) {
		ShowingMessageBox = false;
		ShowingClearScores = false;
		return;
	}

        ConvertCoordinates(x, y);
	TouchEvent.X = x;
	TouchEvent.Y = y;
	
	switch (GameState) {
		case FIRST_RUN:
			SetGameState(MAIN_MENU);
			PlaySound("mouse_click");
			break;
			
		case MAIN_MENU:
			if (PointOverRect(TouchEvent.X, TouchEvent.Y, 37, 207, 248, 78)) {
				SetGameState(MAIN_MENU_PLAY);
				PlaySound("mouse_click");
			} else if (PointOverRect(TouchEvent.X, TouchEvent.Y, 22, 309, 272, 58)) {
				SetGameState(MAIN_MENU_OPTIONS);
				PlaySound("mouse_click");
			} else if (PointOverRect(TouchEvent.X, TouchEvent.Y, 73, 370, 170, 58)) {
				SetGameState(MAIN_MENU_HELP);
				PlaySound("mouse_click");
                        } else if (PointOverRect(TouchEvent.X, TouchEvent.Y, 260, 0, 60, 60)) {
                                SetGameState(DO_QUIT);
			} else if (TouchEvent.Y < 207) {
				++CheatMainTaps;
			}
			
			break;
			
		case PLAY_MENU:
			if (PointOverRect(TouchEvent.X, TouchEvent.Y, 0, 48, 320, 128)) {
				if (AllTestScores.Count == 0) {
					MessageBox::Show("Once you have completed your first test, this will show your brain weight.", "Brain weight");
				} else {
					MessageBox::Show("This is your current brain weight, as measured by the last test you did.", "Brain weight");
				}
			} else if (PointOverRect(TouchEvent.X, TouchEvent.Y, 16, 192, 288, 48)) {
				RunTest();
			} else if (PointOverRect(TouchEvent.X, TouchEvent.Y, 15, 252, 289, 48)) {
				if (NumUnlockedGames > 0) {
					SetGameState(PLAY_PRACTISE);
					PlaySound("mouse_click");
				} else {
					MessageBox::Show("You can't practise any games until you have completed at least one test. Go on, it won't take long!", "No games to practise");
				}
			} else if (PointOverRect(TouchEvent.X, TouchEvent.Y, 28, 312, 264, 48)) {
				SetGameState(PLAY_HISTORY);
				PlaySound("mouse_click");
			} else if (PointOverRect(TouchEvent.X, TouchEvent.Y, 74, 410, 172, 48)) {
				SetGameState(PLAY_MAIN_MENU);
				PlaySound("mouse_click");
			}
			break;
			
		case PRACTISE_MENU:
			if (PointOverRect(TouchEvent.X, TouchEvent.Y, 91, 437, 137, 27)) {
				SetGameState(PRACTISE_PLAY);
				PlaySound("mouse_click");
			} else if (PointOverRect(TouchEvent.X, TouchEvent.Y, 15, 384, 134, 34)) {
				// back
				if (PractiseAnimCounter == 1.0f) {
					if (PractisePageNumber != 0) {
						PractisePageNumber -= 9;
						if (PractisePageNumber < 0) PractisePageNumber = 0;
						PractisePageMoveLeft = false;
						PositionPractiseGames();
						PlaySound("mouse_click");
					}
				}
			} else if (PointOverRect(TouchEvent.X, TouchEvent.Y, 173, 383, 134, 34)) {
				// more
				if (PractiseAnimCounter == 1.0f) {
					if (PractisePageNumber + 9 < NumUnlockedGames) {
						PractisePageNumber += 9;
						PractisePageMoveLeft = true;
						PositionPractiseGames();
						PlaySound("mouse_click");
					}
				}
			} else {
				// did the player ask to play a minigame?
				int i = 0;
				int j = 0;
				int xpos = 0;
				
				for (map<MiniGameType, BPMiniGame_Container*>::const_iterator it = MiniGames.begin(); it != MiniGames.end(); ++it) {
					if (it->second->IsSecret) continue;
					if (!it->second->Unlocked) continue;
					
					if (i < PractisePageNumber) {
						++i;
						continue;
					}
					
					xpos = i - PractisePageNumber;
					
					y = 75 + (xpos * 100);
					x = 15 + (j * 100);
					
					if (PointOverRect(TouchEvent.X, TouchEvent.Y, x, y, 90, 90)) {
						if (it->second->Unlocked) {
							PlaySound("mouse_click");
							CreateMiniGame((MiniGameType)it->second->GameCode, false, true);
						}
						
						break;
					}
					
					++j;
					
					if (j == 3) {
						j = 0;
						++i;
					}
					
					if (i > PractisePageNumber + 2) break;	
				}
			}
			break;
			
		case PROFESSOR:
			ActiveMiniGame->PlayMusic();
			ActiveMiniGame->Start();
			SetGameState(PLAYING);
			break;
			
		case PLAYING:
			ActiveMiniGame->HandleMouseUp(TouchEvent);
			break;
			
		case TEST_STATUS:
			NextTest();
			break;
			
		case TEST_RESULTS:
			if (LastStateChange + 1600 < TickCount) {
				SetGameState(TEST_RESULTS_PLAY);
				PlayMusic("theme");
			}
			
			break;
			
		case OPTIONS:
			if (PointOverRect(x, y, 21, 193, 277, 39)) {
				EnableSound = !EnableSound;
				PlaySound("click", false, true);
				SaveSettings();
			} else if (PointOverRect(x, y, 29, 259, 262, 37)) {
				EnableMusic = !EnableMusic;
				PlaySound("click");
				
				if (EnableMusic == false) {
					StopMusic();
				} else {
					PlayMusic("theme");
				}
				
				SaveSettings();	
			} else if (PointOverRect(x, y, 15, 353, 290, 31)) {
				PromptResetScores();
				PlaySound("click");
				
			} else if (PointOverRect(x, y, 0, 416, 320, 64)) {
				SetGameState(OPTIONS_MAIN_MENU);
				PlaySound("mouse_click");
			} else if (TouchEvent.Y < 193) {
				++CheatOptionsTaps;
			}
			
			break;
			
		case HISTORY:
			if (PointOverRect(x, y, 17, 45, 285, 161)) {
				MessageBox::Show("This graph shows your most recent test results.", "Your test history");
			} else if (PointOverRect(x, y, 21, 251, 285, 57)) {
				MessageBox::Show("This is the highest brain weight you have scored in any test. Each minigame is worth up to 500g, and you get five in a test, so the maximum brain weight is 2500g.", "Highest Brain Weight");
			} else if (PointOverRect(x, y, 0, 349, 74, 66)) {
				if (Secret1) {
					CreateMiniGame(DICEOFF, false, false);
				} else {
					MessageBox::Show("This secret game is unlocked when you get a Platinum score in Balloon Blaster.", "Secret locked!");
				}
			} else if (PointOverRect(x, y, 82, 349, 74, 66)) {
				if (Secret2) {
					CreateMiniGame(WORDSMASH, false, false);
				} else {
					MessageBox::Show("This secret game is unlocked when you get a Platinum score in Jewel Jam.", "Secret locked!");
				}				
			} else if (PointOverRect(x, y, 162, 349, 74, 66)) { 
				if (Secret3) {
					SetGameState(MARATHON);
					PlaySound("mouse_click");
				} else {
					MessageBox::Show("This secret area is unlocked when you get a Platinum score in Odd One Out.", "Secret locked!");
				}
			} else if (PointOverRect(x, y, 245, 349, 74, 66)) {
				if (Secret4) {
					SetGameState(BRAINBOOST);
					PlaySound("mouse_click");
				} else {
					MessageBox::Show("This secret area is unlocked when you get a Platinum score in Untangler.", "Secret locked!");
				}				
			} else if (PointOverRect(x, y, 2, 418, 110, 54)) {
				SetGameState(HISTORY_PLAY);
				PlaySound("mouse_click");
			} else if (PointOverRect(x, y, 182, 418, 136, 54)) {
				SetGameState(HISTORY_CREDITS);
				PlaySound("mouse_click");
			}
			
			break;
				
		case CREDITS:
			if (PointOverRect(x, y, 193, 203, 126, 148)) {
				// clicked on the professor!
				if (CheatMainTaps >= 5 && CheatOptionsTaps >= 5) {
					MessageBox::Show("Hey, it looks like you activated the cheat code - all the minigames and secrets have been unlocked. We hope you're proud of yourself!", "Cheat activated!");	
					
					for (map<MiniGameType, BPMiniGame_Container*>::const_iterator it = MiniGames.begin(); it != MiniGames.end(); ++it) {
						it->second->Unlocked = true;
					}
					
					Secret1 = Secret2 = Secret3 = Secret4 = true;
					
					SaveSettings();
					
					PlaySound("meow");
				}
			}
			
			SetGameState(CREDITS_HISTORY);
			PlaySound("mouse_click");
			break;
			
		case MARATHON:
			if (PointOverRect(x, y, 12, 201, 90, 90)) {
				CreateMiniGame(BALLOONBLASTER, true, false);
			} else if (PointOverRect(x, y, 116, 201, 90, 90)) {
				CreateMiniGame(BPSAYS, true, false);
			} else if (PointOverRect(x, y, 220, 201, 90, 90)) {
				CreateMiniGame(JEWELJAM, true, false);
			} else if (PointOverRect(x, y, 12, 305, 90, 90)) {
				CreateMiniGame(SHORTCIRCUITSUDOKU, true, false);
			} else if (PointOverRect(x, y, 116, 305, 90, 90)) {
				CreateMiniGame(SYMBOLICLOGIC, true, false);
			} else if (PointOverRect(x, y, 220, 305, 90, 90)) {
				CreateMiniGame(UNTANGLER, true, false);
			} else if (PointOverRect(x, y, 0, 423, 320, 64)) {
				SetGameState(HISTORY);
				PlaySound("mouse_click");
			}
			
			break;
			
		case BRAINBOOST:
			if (PointOverRect(x, y, 53, 184, 90, 90)) {
				CreateMiniGame(SHARPSHOOTER, true, false);
			} else if (PointOverRect(x, y, 179, 184, 90, 90)) {
				CreateMiniGame(SHUFFLEPUZZLER, true, false);
			} else if (PointOverRect(x, y, 53, 305, 90, 90)) {
				CreateMiniGame(STRANGERDANGER, true, false);
			} else if (PointOverRect(x, y, 179, 305, 90, 90)) {
				CreateMiniGame(UNDERTHEHAT, true, false);
			} else if (PointOverRect(x, y, 0, 423, 320, 64)) {
				SetGameState(HISTORY);
				PlaySound("mouse_click");
			}
			
			break;
			
		case HELP:
			SetGameState(HELP_MAIN_MENU);
			PlaySound("mouse_click");
			break;
	}
}

void BPGame::TouchDrag(float x, float y) {
	if (ShowingMessageBox) {
		return;
	}
	
        ConvertCoordinates(x, y);
	TouchEvent.X = x;
	TouchEvent.Y = y;

	switch (GameState) {
		case PLAYING:
			if (ActiveMiniGame != NULL) {
				ActiveMiniGame->HandleMouseMove(TouchEvent);
			}
			
			break;
	}
}

const string TrimString(const std::string& str) {
	string::size_type First = str.find_first_not_of( ' ' );

	if (First == string::npos) {
		First = 0;
	}

	string::size_type Last = str.find_last_not_of( ' ' );
	if (Last != string::npos) {
		Last = (Last + 1) - First;
	}

  	return str.substr(First, Last);
}


void BPGame::AllocString(SpriteFont** tex, const char* str, FontSizes size, float width, float height, Alignments align, bool bold) {
	SAFE_DELETE((*tex));
	
	// it's incredibly lazy to open and close the font each time - hurray!
	TTF_Font* fnt = TTF_OpenFont("/opt/brainparty/Content/freesans.ttf", size - 3); // NB: the -3 is here because the freesans.ttf font we're using is a big chunkier than the iPhone font
	
	static SDL_Color white = { 255, 255, 255, 255 };
	
	vector<string> lines; // we'll store the split lines here
	string temp(str); // the current text we're working on
	temp += " ";  // always add a space to the end of the text to enable wrapping - this gets removed later
	int n = 0; // current index
	int p = 0; // previous space

	while (n != -1) {		
		std::string strSub;
		n = temp.find(" ", p + 1 );		// -- Find the next " "
		
		std::string line_so_far = temp.substr(0, n);
		int w,h;
		
		const char* c_str = line_so_far.c_str();
		TTF_SizeText(fnt, c_str, &w, &h);
		
		if (w >= width)	{
			strSub = temp.substr(0, p);
			lines.push_back(strSub);
			
			if (n != -1) temp = temp.substr( p+1, string::npos );			
			p = 0;
		} else {
			p = n;
		}
	}

	// do the last line too
	string strSub = temp.substr(0, p);
	lines.push_back(strSub);
	
		
	// create a SpriteFont to hold all the information - this is used for drawing later
	(*tex) = new SpriteFont();
	(*tex)->Width = width;
	(*tex)->Height = height;
	(*tex)->Align = align;
	
	// render all the text
	for (int i = 0; i < lines.size(); ++i) {
		SDL_Surface* sfc = TTF_RenderText_Blended(fnt, TrimString(lines[i]).c_str(), white);
		
		if (sfc != NULL) {
			(*tex)->Surfaces.Add(new Texture(sfc));
			SDL_FreeSurface(sfc);
		}
	}
	
	TTF_CloseFont(fnt);
}

void BPGame::DrawString(SpriteFont* tex, Colour &col, float x, float y) {
	if (tex == NULL) return;
	
	glColor4f(col.R, col.G, col.B, col.A);	
	tex->drawAtPoint(x,y);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void BPGame::DrawString(SpriteFont* tex, Colours col, float x, float y) {
	if (tex == NULL) return;	
	
	if (col == WHITE) {
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	} else if (col == BLACK) {
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);		
	}
	
	tex->drawAtPoint(x,y);
	
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void BPGame::DrawString(SpriteFont* tex, float x, float y) {
	if (tex == NULL) return;
			
	tex->drawAtPoint(x,y);
}

int BPGame::RandomRange(int min, int max) {
	if (max < min) return min;
	
	int value = rand();
	return (value % ((max - min) + 1) + min);
}

int BPGame::RandomRangeExcept(int min, int max, int except) {
	int val = RandomRange(min, max);
	
	while (val == except) {
		val = RandomRange(min, max);
	}
	
	return val;
}

void BPGame::LoadSettings() {
	Secret1 = Secret2 = Secret3 = Secret4 = false;
	NumUnlockedGames = 0;

	ifstream ifs;
	ifs.open("/home/user/.brainparty");
	
	FirstRun = false;
	
	if (ifs.is_open()) {
		string temp;
	
		int loading_stage = 0;
		int line_number = 0;
		int secret_num = 1;

		while (getline(ifs, temp)) {
			++line_number;
		
			if (temp.size() == 0) {
				++loading_stage;
			} else {
				switch (loading_stage) {
					case 0:
						// loading whether we have music and sound
						if (line_number == 1) {
							if (temp.compare("1") == 0) {
								EnableSound = true;
							} else {
								EnableSound = false;							
							}
						} else {
							if (temp.compare("1") == 0) {
								EnableMusic = true;
							} else {
								EnableMusic = false;							
							}
						}
						break;
				
					case 1:
						// loading secrets
						switch (secret_num) {
							case 1:
								if (temp.compare("1") == 0) {
									Secret1 = true;
								} else {
									Secret1 = false;							
								}
						
								break;
						
							case 2:
								if (temp.compare("1") == 0) {
									Secret2 = true;
								} else {
									Secret2 = false;							
								}
								break;
						
							case 3:
								if (temp.compare("1") == 0) {
									Secret3 = true;
								} else {
									Secret3 = false;							
								}
								break;
						
							case 4:
								if (temp.compare("1") == 0) {;
							
									Secret4 = true;
								} else {
									Secret4 = false;							
								}
								break;
						}

						++secret_num;
					
						break;
					
					case 2:
					{
						// loading test scores
						CurrentBrainWeight = atoi(temp.c_str());
						AllTestScores.Add(CurrentBrainWeight);
					
						if (CurrentBrainWeight > HighestBrainWeight) {
							HighestBrainWeight = CurrentBrainWeight;
						}
					
						break;
					}	

					
					case 3:
						const char* gamename = temp.c_str();
				
						for (map<MiniGameType, BPMiniGame_Container*>::const_iterator it = MiniGames.begin(); it != MiniGames.end(); ++it) {
							if (it->second->IsSecret) continue;
					
							if (strcmp(gamename, it->second->ShortName) == 0) {
								it->second->Unlocked = true;
								++NumUnlockedGames;
								continue;
							}
						}
					
						break;
				}
			}
		}
	} else {
		FirstRun = true;
	}
	
	if (CurrentBrainWeight != -1) {
		ostringstream output;
		output << CurrentBrainWeight << "g";
		AllocString(&sfcTestWeight, output.str().c_str(), BRAINWEIGHT, 320, 100, CENTRED, true);
	} else {
		AllocString(&sfcTestWeight, "???", BRAINWEIGHT, 320, 100, CENTRED, true);		
	}
	
	if (HighestBrainWeight != -1) {
		ostringstream output;
		output << HighestBrainWeight << "g";
		AllocString(&sfcHighestBrainWeight, output.str().c_str(), XXLARGE, 320, 100, CENTRED, true);
	} else {
		AllocString(&sfcHighestBrainWeight, "N/A", XXLARGE, 320, 100, CENTRED, true);		
	}
}

void BPGame::SaveSettings() {
 	ofstream savefile;
 	savefile.open("/home/user/.brainparty");
	savefile << EnableSound << endl;
	savefile << EnableMusic << endl;
	savefile << endl;
	
	savefile << Secret1 << endl;
	savefile << Secret2 << endl;
	savefile << Secret3 << endl;
	savefile << Secret4 << endl;
	savefile << endl;
	
	for (int i = 0; i < AllTestScores.Count; ++i) {
		savefile << AllTestScores[i] << endl;
	}
	
	savefile << endl;
	
	for (map<MiniGameType, BPMiniGame_Container*>::const_iterator it = MiniGames.begin(); it != MiniGames.end(); ++it) {
		if (it->second->Unlocked) {
			savefile << it->second->ShortName << endl;
		}
	}
	
 	savefile.close();

	LoadSettings();
}

string BPGame::SeparateThousands(int number) {
	char buffer[20];
	
	int i, digit, out_index = 0;

	for (i = number; i != 0; i /= 10) {
		digit = i % 10;

		if ((out_index + 1) % 4 == 0) {
			buffer[out_index++] = ',';
		}
		
		buffer[out_index++] = digit + '0';
	}
	
	buffer[out_index] = '\0';
	
	string retval = string(buffer);
	reverse(retval.begin(), retval.end());
	return retval;
}

string* BPGame::TicksToTime(int ticks) {
	int seconds_elapsed = floor(ticks / 1000);
	int minutes = floor(seconds_elapsed / 60.0);
	int seconds = seconds_elapsed % 60;
	
	ostringstream output;
	
	if (seconds < 10) {
		output << minutes << ":0" << seconds;
	} else {
		output << minutes << ":" << seconds;
	}
	
	string* retval = new string(output.str());
	return retval;
}

int BPGame::DivRem(int Num, int Div, int* Rem) {
	(*Rem) = Num % Div;
	return floor(Num / Div);
}

void BPGame::DrawLine(int fromx, int fromy, int tox, int toy, Colour* col, float width) {	
	GLfloat vertices[] = { fromx, fromy, tox, toy }; 

	glDisable(GL_TEXTURE_2D);
	glLineWidth(width);
	glColor4f(col->R, col->G, col->B, col->A);
	glLoadIdentity();

	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_LINES, 0, 2);
	
	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

float BPGame::Clamp(float value, float minval, float maxval) {
	if (value > maxval) {
		value = maxval;
	} else if (value < minval) {
		value = minval;
	}
		
	return value;
}

float BPGame::Hermite(float value1, float tangent1, float value2, float tangent2, float amount) {
	float AmountSquared = amount * amount;
	float AmountCubed = AmountSquared * amount;

	float result = 0.0f;
	
	if (amount == 0.0f) {
		result = value1;
	} else if (amount == 1.0f) {
		result = value2;
	} else {
		result = (2 * value1 - 2 * value2 + tangent2 + tangent1) * AmountCubed + (3 * value2 - 3 * value1 - 2 * tangent1 - tangent2) * AmountSquared + tangent1 * amount + value1;
	}

	return result;
}

float BPGame::Lerp(float value1, float value2, float amount) {
	float result = Clamp(amount, 0.0f, 1.0f);
	return value1 + (value2 - value1) * result;
}

float BPGame::SmoothStep(float value1, float value2, float amount) {
	float result = Clamp(amount, 0.0f, 1.0f);
	result = Hermite(value1, 0.0, value2, 0.0, result);
	return result;
}

float BPGame::SmoothStep2(float value1, float value2, float amount) {
	float result = Clamp(amount, 0.0f, 1.0f);
	
	if (result > 0.5f) {
		result = Hermite(value2, 0.0f, value1, 0.0f, (result - 0.5f) * 2.0f);
	} else {
		result = Hermite(value1, 0.0f, value2, 0.0f, result * 2.0f);
	}
	
	return result;
}

Colour BPGame::ColourSmoothStep(Colour &from, Colour &to, float amount) {
	return Colour(SmoothStep(from.R, to.R, amount), SmoothStep(from.G, to.G, amount), SmoothStep(from.B, to.B, amount), SmoothStep(from.A, to.A, amount));
}


bool BPGame::StartsWithVowel(string* word) {
	char chr = (*word)[0];
	if (chr == 'a' || chr == 'e' || chr == 'i' || chr == 'o' || chr == 'u'){
		return true;
	}
	
	return false;
}

const char* BPGame::GetName() {
	return PeopleNames[RandomRange(0, PeopleNames.Count - 1)];
}

void BPGame::RenderMainMenu(float xoff, float yoff) {
	DrawImage(sfcMainMenu, xoff, yoff);
}

void BPGame::RenderOptionsMenu() {
	DrawImage(sfcOptions, 0, 0);
	
	if (!EnableSound) {
		DrawImage(sfcOptionsSoundsOff, 0, 188);
	}
	
	if (!EnableMusic) {
		DrawImage(sfcOptionsMusicOff, 0, 254);		
	}
}

void BPGame::RenderPlayMenu(float xoff, float yoff) {
	DrawImage(sfcPlayMenu, xoff, yoff);
	DrawString(sfcTestWeight, xoff, yoff + 58.0f);	
}

void BPGame::RenderPractiseMenu() {
	DrawImage(sfcPractiseMenu, 0, 0);
	
	for (map<MiniGameType, BPMiniGame_Container*>::const_iterator it = MiniGames.begin(); it != MiniGames.end(); ++it) {
		if (!it->second->IsShowing) {
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f - PractiseAnimCounter);
		}
		
		if (it->second->Pic == NULL) {
			DrawImage(sfcMiniGameUnknown, it->second->X, it->second->Y);
		} else {
			DrawImage(it->second->Pic, it->second->X, it->second->Y);
		}

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
	
	if (PractisePageNumber == 0) {
		// hide Back
		glColor4f(1.0f, 1.0f, 1.0f, 0.85f);
		DrawImage(sfcPractiseMenuHider, 15, 384);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
	
	if (PractisePageNumber + 9 >= NumUnlockedGames) {
		glColor4f(1.0f, 1.0f, 1.0f, 0.85f);
		DrawImage(sfcPractiseMenuHider, 173, 383);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

void BPGame::RenderProfessor() {
	DrawImage(sfcProfessorTalk, 0, 0);

	ActiveMiniGame->DrawProfessorText();
}

void BPGame::RenderPlaying() {
	ActiveMiniGame->RenderMiniGame();
}

void BPGame::RenderTestStatus() {
	static float rotation = 0.0f;
	rotation += 15 * ElapsedSeconds;
	
	Clear(Black);
	DrawImage(sfcTestBackground, 160.0f, 240.0f, rotation, 1.2f, (*White));
	Colour col = Colour(1.0f, 1.0f, 1.0f, 0.2f);
	DrawImage(sfcTestBackground, 160.0f, 240.0f, -rotation, 1.2f, col);
	
	for (int i = 0; i < BackgroundObjects.Count; ++i) {
		BGObject* obj = BackgroundObjects[i];
		
		float alpha;
		
		if (obj->Scale < 0.5f) {
			// fading in
			alpha = (obj->Scale / 2.5f);
		} else {
			// fading out
			alpha = 0.4f - (obj->Scale / 2.5f);
		}
		
		alpha = Clamp(alpha, 0.0f, 1.0f);
		Colour col = Colour(1.0f, 1.0f, 1.0f, alpha);
		
		DrawImage(obj->Type, obj->Pos.X, obj->Pos.Y, 0.0f, obj->Scale, col);
	}
	
	DrawImage(sfcTestStatus[TestPosition], 0.0f, 208.0f);
}

void BPGame::RenderTestResults(float xoff, float yoff) {
	if (LastStateChange + 200 > TickCount) {
		// plain background
		DrawImage(sfcResultsBackground, xoff + 160.0f, yoff + 240.0f, 0.0f, 1.0f, (*White));
	} else if (LastStateChange + 1000 > TickCount) {
		// fade in Your Brain Weighs text
		DrawImage(sfcResultsBackground, xoff + 160.0f, yoff + 240.0f, 0.0f, 1.0f, (*White));
		Colour col = Colour(1.0f, 1.0, 1.0f, 1 - (((LastStateChange + 1000) - TickCount) / 800.0f));
		DrawImage(sfcResultsBackground2, xoff + 160.0f, yoff + 240.0f, 0.0f, 1.0f, col);
	} else if (LastStateChange + 1600 > TickCount) {
		// fade in brain weight
		DrawImage(sfcResultsBackground2, xoff + 160.0f, yoff + 240.0f, 0.0f, 1.0f, (*White));
		float alpha = 1 - (((LastStateChange + 1600) - TickCount) / 600.0f);

		glColor4f(1.0f, 1.0f, 1.0f, alpha);
		DrawString(sfcTestWeight, xoff + 0.0f, yoff + 93.0f);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	} else {
		// show everything
		DrawImage(sfcResultsBackground2, xoff + 160.0f, yoff + 240.0f, 0.0f, 1.0f, (*White));
		DrawString(sfcTestWeight, xoff + 0.0f, yoff + 93.0f);
		DrawString(sfcTestJob, xoff + 0.0f, yoff + 200.0f);
	}
	
	float diff = 1 - (((LastStateChange + 500) - TickCount) / 500.0f);
	
	for (int i = 0; i < TestResultContainers.Count; ++i) {
		TestResultContainer* result = TestResultContainers[i];
		
		int x, y, rot;
		
		if (LastStateChange + 500 > TickCount) {
			// fading in
			x = SmoothStep(160, result->Pos.X, diff);
			y = SmoothStep(525, result->Pos.Y, diff);
			rot = SmoothStep(60 + (i * 60), 0, diff);
			Colour col = Colour(1.0f, 1.0f, 1.0f, SmoothStep(0.0f, 1.0f, diff));
			DrawImage(MiniGames[(MiniGameType)result->MiniGame]->Pic, xoff + x, yoff + y, rot, 1.0f, col);
		} else {
			x = result->Pos.X;
			y = result->Pos.Y;
			rot = 0;
			DrawImage(MiniGames[(MiniGameType)result->MiniGame]->Pic, xoff + x, yoff + y, rot, 1.0f, (*White));
			Colour col = Colour(1.0f, 1.0f, 1.0f, SmoothStep(0.0f, 1.0f, result->FadeUpAnim));
			DrawImage(sfcResults90[result->Rank], xoff + x, yoff + y, 0.0f, 1.0f, col);
		}
	}
}

void BPGame::RenderHistory(float xoff, float yoff) {
	DrawImage(sfcHistoryBackground, xoff, yoff);
	
	DrawString(sfcHighestBrainWeight, xoff, 246.0f + yoff);
	
	if (Secret1) {
		DrawImage(sfcSecretHi, 2 + xoff, 349 + yoff);
	} else {
		DrawImage(sfcSecretLo, 2 + xoff, 349 + yoff);
	}
	
	if (Secret2) {
		DrawImage(sfcSecretHi, 83 + xoff, 349 + yoff);
	} else {
		DrawImage(sfcSecretLo, 83 + xoff, 349 + yoff);
	}
	
	if (Secret3) {
		DrawImage(sfcSecretHi, 164 + xoff, 349 + yoff);
	} else {
		DrawImage(sfcSecretLo, 164 + xoff, 349 + yoff);
	}
	
	if (Secret4) {
		DrawImage(sfcSecretHi, 246 + xoff, 349 + yoff);
	} else {
		DrawImage(sfcSecretLo, 246 + xoff, 349 + yoff);
	}
	
	int graph_height = 148;
	int last_score = -1;
	
	if (ShowScores.Count == 1) {
		// just one score - draw a dot!
		int this_score = round(ShowScores[0] / 2500.0f * graph_height);
		DrawLine(52 + xoff, 201 - this_score + yoff, 56 + xoff, 201 - this_score + yoff, Red, 4.0f);
	} else {
		for (int i = 0; i < ShowScores.Count; ++i) {
			int this_score = round(ShowScores[i] / 2500.0f * graph_height);
		
			if (last_score != -1) {
				// connect the last dot to this dot
				DrawLine(54 + ((i - 1) * 30) + xoff, 201 - last_score + yoff, 54 + (i * 30) + xoff, 201 - this_score + yoff, Red, 3.0f);
			}
		
			last_score = this_score;
		}
	}
}

void BPGame::RenderBrainBoost() {
	DrawImage(sfcBrainBoost, 0, 0);
}

void BPGame::RenderMarathon() {
	DrawImage(sfcMarathonMode, 0, 0);	
}

void BPGame::RenderHelp(float xoff, float yoff) {
	DrawImage(sfcHelp, xoff, yoff);
}

void BPGame::RenderCredits(float xoff, float yoff) {
	DrawImage(sfcCredits, xoff, yoff);
}

void BPGame::CalculateTestResults() {
	// add up all our scores and see how well we did...
	TestBrainWeight = 0;
	
	for (int i = 0; i < TestScores.Count; ++i) {
		TestBrainWeight += TestScores[i];
	}
	
	if (TestBrainWeight < 500) {
		TestBrainJob = TestBrainJobs[0];
	} else if (TestBrainWeight < 600) {
		TestBrainJob = TestBrainJobs[1];
	} else if (TestBrainWeight < 650) {
		TestBrainJob = TestBrainJobs[2];
	} else if (TestBrainWeight < 700) {
		TestBrainJob = TestBrainJobs[3];
	} else if (TestBrainWeight < 750) {
		TestBrainJob = TestBrainJobs[4];
	} else if (TestBrainWeight < 800) {
		TestBrainJob = TestBrainJobs[5];
	} else if (TestBrainWeight < 850) {
		TestBrainJob = TestBrainJobs[6];
	} else if (TestBrainWeight < 900) {
		TestBrainJob = TestBrainJobs[7];
	} else if (TestBrainWeight < 950) {
		TestBrainJob = TestBrainJobs[8];
	} else if (TestBrainWeight < 1000) {
		TestBrainJob = TestBrainJobs[9];
	} else if (TestBrainWeight < 1050) {
		TestBrainJob = TestBrainJobs[10];
	} else if (TestBrainWeight < 1100) {
		TestBrainJob = TestBrainJobs[11];
	} else if (TestBrainWeight < 1150) {
		TestBrainJob = TestBrainJobs[12];
	} else if (TestBrainWeight < 1175) {
		TestBrainJob = TestBrainJobs[13];
	} else if (TestBrainWeight < 1200) {
		TestBrainJob = TestBrainJobs[14];
	} else if (TestBrainWeight < 1225) {
		TestBrainJob = TestBrainJobs[15];
	} else if (TestBrainWeight < 1250) {
		TestBrainJob = TestBrainJobs[16];
	} else if (TestBrainWeight < 1275) {
		TestBrainJob = TestBrainJobs[17];
	} else if (TestBrainWeight < 1300) {
		TestBrainJob = TestBrainJobs[18];
	} else if (TestBrainWeight < 1325) {
		TestBrainJob = TestBrainJobs[19];
	} else if (TestBrainWeight < 1350) {
		TestBrainJob = TestBrainJobs[20];
	} else if (TestBrainWeight < 1375) {
		TestBrainJob = TestBrainJobs[21];
	} else if (TestBrainWeight < 1400) {
		TestBrainJob = TestBrainJobs[22];
	} else if (TestBrainWeight < 1425) {
		TestBrainJob = TestBrainJobs[23];
	} else if (TestBrainWeight < 1450) {
		TestBrainJob = TestBrainJobs[24];
	} else if (TestBrainWeight < 1475) {
		TestBrainJob = TestBrainJobs[25];
	} else if (TestBrainWeight < 1500) {
		TestBrainJob = TestBrainJobs[26];
	} else if (TestBrainWeight < 1525) {
		TestBrainJob = TestBrainJobs[27];
	} else if (TestBrainWeight < 1550) {
		TestBrainJob = TestBrainJobs[28];
	} else if (TestBrainWeight < 1575) {
		TestBrainJob = TestBrainJobs[29];
	} else if (TestBrainWeight < 1600) {
		TestBrainJob = TestBrainJobs[30];
	} else if (TestBrainWeight < 1625) {
		TestBrainJob = TestBrainJobs[31];
	} else if (TestBrainWeight < 1650) {
		TestBrainJob = TestBrainJobs[32];
	} else if (TestBrainWeight < 1675) {
		TestBrainJob = TestBrainJobs[33];
	} else if (TestBrainWeight < 1700) {
		TestBrainJob = TestBrainJobs[34];
	} else if (TestBrainWeight < 1725) {
		TestBrainJob = TestBrainJobs[35];
	} else if (TestBrainWeight < 1750) {
		TestBrainJob = TestBrainJobs[36];
	} else if (TestBrainWeight < 1775) {
		TestBrainJob = TestBrainJobs[37];
	} else if (TestBrainWeight < 1800) {
		TestBrainJob = TestBrainJobs[38];
	} else if (TestBrainWeight < 1825) {
		TestBrainJob = TestBrainJobs[39];
	} else if (TestBrainWeight < 1850) {
		TestBrainJob = TestBrainJobs[40];
	} else if (TestBrainWeight < 1875) {
		TestBrainJob = TestBrainJobs[41];
	} else if (TestBrainWeight < 1900) {
		TestBrainJob = TestBrainJobs[42];
	} else if (TestBrainWeight < 1925) {
		TestBrainJob = TestBrainJobs[43];
	} else if (TestBrainWeight < 1950) {
		TestBrainJob = TestBrainJobs[44];
	} else if (TestBrainWeight < 1975) {
		TestBrainJob = TestBrainJobs[45];
	} else if (TestBrainWeight < 2000) {
		TestBrainJob = TestBrainJobs[46];
	} else if (TestBrainWeight < 2025) {
		TestBrainJob = TestBrainJobs[47];
	} else if (TestBrainWeight < 2050) {
		TestBrainJob = TestBrainJobs[48];
	} else if (TestBrainWeight < 2075) {
		TestBrainJob = TestBrainJobs[49];
	} else if (TestBrainWeight < 2100) {
		TestBrainJob = TestBrainJobs[50];
	} else if (TestBrainWeight < 2125) {
		TestBrainJob = TestBrainJobs[51];
	} else if (TestBrainWeight < 2150) {
		TestBrainJob = TestBrainJobs[52];
	} else if (TestBrainWeight < 2175) {
		TestBrainJob = TestBrainJobs[53];
	} else if (TestBrainWeight < 2200) {
		TestBrainJob = TestBrainJobs[54];
	} else {
		TestBrainJob = TestBrainJobs[55];
	}
	
	AllocString(&sfcTestJob, TestBrainJob, NORMAL, 320.0f, 80.0f, CENTRED, false);
	
	// add the new score to the array
	AllTestScores.Add(TestBrainWeight);
	CurrentBrainWeight = TestBrainWeight;
	
	// unlock the games we just played
	for (int i = 0; i < TestMiniGames.Count; ++i) {
		MiniGames[TestMiniGames[i]]->Unlocked = true;
	}
	
	// now prepare the results screen
	TestResultContainers.Clear();
	
	for (int i = 0; i < TestMiniGames.Count; ++i) {
		TestResultContainer* result = new TestResultContainer();
		
		ostringstream brainweight;
		brainweight << TestScores[i] << "g";
		AllocString(&result->sfcBrainWeight, brainweight.str().c_str(), NORMAL, 90.0f, 30.0f, CENTRED, true);
		
		result->MiniGame = TestMiniGames[i];
		result->Rank = BPMiniGame::GetRank(TestScores[i]);
		result->GoingUp = false;
		
		if (i < 2) {
			// first row
			result->Pos = BPPoint(110 + (i * 100), 325);
		} else {
			// second row
			result->Pos = BPPoint(60 + ((i - 2) * 100), 425);
		}
		
		result->FadeUpAnim = Lerp(-1.0f, 0.0f, i / (float)(TestMiniGames.Count - 1));
		TestResultContainers.Add(result);
	}
	
	SaveSettings();
}

void BPGame::CreateMiniGame(MiniGameType game, bool marathon, bool practise) {	
	if (ActiveMiniGame != NULL) {		
		SAFE_DELETE(ActiveMiniGame);
	}
		
	switch (game) {
		case BALLOONBLASTER:
			ActiveMiniGame = new BPMiniGame_BalloonBlaster(this);
			break;
			
		case BOMBHUNT:
			ActiveMiniGame = new BPMiniGame_BombHunt(this);
			break;
			
		case BPSAYS:
			ActiveMiniGame = new BPMiniGame_BPSays(this);
			break;
			
		case BUBBLETROUBLE:
			ActiveMiniGame = new BPMiniGame_BubbleTrouble(this);
			break;
			
		case CARDMATCH:
			ActiveMiniGame = new BPMiniGame_CardMatch(this);
			break;
			
		case CONNEX:
			ActiveMiniGame = new BPMiniGame_Connex(this);
			break;

		case CUPSNBALLS:
			ActiveMiniGame = new BPMiniGame_CupsNBalls(this);
			break;
			
		case DICEOFF:
			ActiveMiniGame = new BPMiniGame_DiceOff(this);
			break;
			
		case FLASHCOUNTING:
			ActiveMiniGame = new BPMiniGame_FlashCounting(this);
			break;
			
		case FLASHLIGHT:
			ActiveMiniGame = new BPMiniGame_Flashlight(this);
			break;
			
		case IQTEST:
			ActiveMiniGame = new BPMiniGame_IQTest(this);
			break;
			
		case JEWELFLIP:
			ActiveMiniGame = new BPMiniGame_JewelFlip(this);
			break;
			
		case JEWELJAM:
			ActiveMiniGame = new BPMiniGame_JewelJam(this);
			break;
			
		case MARBLEDROP:
			ActiveMiniGame = new BPMiniGame_MarbleDrop(this);
			break;
			
		case MEMORYBLOX:
			ActiveMiniGame = new BPMiniGame_MemoryBlox(this);
			break;
			
		case MEMORYBOX:
			ActiveMiniGame = new BPMiniGame_MemoryBox(this);
			break;
			
		case MEMORYMATHS:
			ActiveMiniGame = new BPMiniGame_MemoryMaths(this);
			break;
			
		case MINESWEEP:
			ActiveMiniGame = new BPMiniGame_MineSweep(this);
			break;
			
		case MOONJUMP:
			ActiveMiniGame = new BPMiniGame_MoonJump(this);
			break;

		case NEXTINLINE:
			ActiveMiniGame = new BPMiniGame_NextInLine(this);
			break;
			
		case NUMBERSNAKE:
			ActiveMiniGame = new BPMiniGame_NumberSnake(this);
			break;
			
		case ODDONEOUT:
			ActiveMiniGame = new BPMiniGame_OddOneOut(this);
			break;

		case PATCHMATCH:
			ActiveMiniGame = new BPMiniGame_PatchMatch(this);
			break;
			
		case PERFECTPATHS:
			ActiveMiniGame = new BPMiniGame_PerfectPaths(this);
			break;
			
		case ROUTEFINDER:
			ActiveMiniGame = new BPMiniGame_RouteFinder(this);
			break;
			
		case RPS:
			ActiveMiniGame = new BPMiniGame_RPS(this);
			break;
			
		case SCRAMBLED:
			ActiveMiniGame = new BPMiniGame_Scrambled(this);
			break;
			
		case SETFINDER:
			ActiveMiniGame = new BPMiniGame_SetFinder(this);
			break;
			
		case SHARPSHOOTER:
			ActiveMiniGame = new BPMiniGame_Sharpshooter(this);
			break;

		case SHORTCIRCUITSUDOKU:
			ActiveMiniGame = new BPMiniGame_ShortCircuitSudoku(this);
			break;
			
		case SHUFFLEPUZZLER:
			ActiveMiniGame = new BPMiniGame_ShufflePuzzler(this);
			break;
			
		case STRANGERDANGER:
			ActiveMiniGame = new BPMiniGame_StrangerDanger(this);
			break;
			
		case SYMBOLICLOGIC:
			ActiveMiniGame = new BPMiniGame_SymbolicLogic(this);
			break;
			
		case UNDERTHEHAT:
			ActiveMiniGame = new BPMiniGame_UnderTheHat(this);
			break;

		case UNTANGLER:
			ActiveMiniGame = new BPMiniGame_Untangler(this);
			break;
			
		case WORDSMASH:
			ActiveMiniGame = new BPMiniGame_WordSmash(this);
			break;
	}
	
	ActiveMiniGame->Init();
	
	if (marathon) ActiveMiniGame->SetMarathon();
	
	if (GameState == HISTORY) {
		ActiveMiniGame->ReturnType = RT_SECRET;
	} else if (GameState == BRAINBOOST) {
		ActiveMiniGame->ReturnType = RT_BRAINBOOST;
	}
	
	if (practise) {
		SetGameState(PRACTISE_PROFESSOR);
	} else {
		SetGameState(PROFESSOR);		
	}
}

void BPGame::SetGameState(GameStates state) {
	GameState = state;
	AnimCounter = 0.0f;
	GameStateJustChanged = true;
	
	if (state != TEST_RESULTS_PLAY) {
		// don't reset LastStateChange when going from test results back to the play menu, as that resets the
		// animation on the results screen
		LastStateChange = TickCount;
	}
	
	switch (state) {
		case PLAY_PRACTISE:
			PractisePageNumber = 0;
			PractisePageMoveLeft = true;
			
			for (map<MiniGameType, BPMiniGame_Container*>::const_iterator it = MiniGames.begin(); it != MiniGames.end(); ++it) {
				it->second->IsShowing = false;
			}
			
			PositionPractiseGames();
			
			break;
			
		case HISTORY:
			ShowScores.Clear();
			
			if (AllTestScores.Count > 8) {
				for (int i = AllTestScores.Count - 8; i < AllTestScores.Count; ++i) {
					ShowScores.Add(AllTestScores[i]);
				}				
			} else {
				for (int i = 0; i < AllTestScores.Count; ++i) {
					ShowScores.Add(AllTestScores[i]);
				}
			}
			
			break;
			
		case PROFESSOR:
			PlayMusic("professor");
			break;
			
		case TEST_STATUS:
			PlaySound("happy_ring");
			break;
	}
}

void BPGame::PositionPractiseGames() {
	PractiseAnimCounter = 0.0f;
	
	// get all minigames off the screen
	for (map<MiniGameType, BPMiniGame_Container*>::const_iterator it = MiniGames.begin(); it != MiniGames.end(); ++it) {
		if (it->second->IsShowing) {
			// these are visible - make them scroll offscreen neatly
			it->second->StartX = it->second->X;
			it->second->StartY = it->second->Y;
			
			if (PractisePageMoveLeft) {
				it->second->DestX = -100;
			} else {
				it->second->DestX = 320;
			}
			
			it->second->DestY = 175;
		} else {
			it->second->Y = it->second->StartY = it->second->DestY = 175;			

			if (PractisePageMoveLeft) {
				it->second->X = it->second->StartX = it->second->DestX = -100;
			} else {
				it->second->X = it->second->StartX = it->second->DestX = 320;				
			}
		}
		
		it->second->IsShowing = false;
	}
	
	BPPList<BPPoint*> PractisePositions;
	
	for (int row = 0; row < 3; ++row) {
		for (int col = 0; col < 3; ++col) {
			PractisePositions.Add(new BPPoint(15 + (col * 100), 75 + (row * 100)));
		}
	}
		
	int i = 0;
	int j = 0;
	
	for (map<MiniGameType, BPMiniGame_Container*>::const_iterator it = MiniGames.begin(); it != MiniGames.end(); ++it) {
		if (it->second->IsSecret) continue;
		if (!it->second->Unlocked) continue;
		
		if (i < PractisePageNumber) {
			++i;
			it->second->IsShowing = false;
			continue;
		}
		
		it->second->DestX = PractisePositions[j]->X;
		it->second->DestY = PractisePositions[j]->Y;
		
		it->second->StartY = PractisePositions[j]->Y;
		
		if (PractisePageMoveLeft) {
			it->second->StartX = 320;
		} else {
			it->second->StartX = -100;
		}
		
		it->second->IsShowing = true;
		
		++i;
		++j;
		
		if (i >= PractisePageNumber + 9) break;
	}
	
	PractisePositions.Clear();
}

void BPGame::RunTest() {
	StopMusic();
	
	TestMiniGames.Clear();
	TestScores.Clear();
	TestPosition = 0;
	
	BPList<MiniGameType> SeenMiniGames;
	BPList<MiniGameType> UnseenMiniGames;
	
	// add them all to an array, then shuffle the array
	for (map<MiniGameType, BPMiniGame_Container*>::const_iterator it = MiniGames.begin(); it != MiniGames.end(); ++it) {
		if (it->second->IsSecret) continue; // don't allow hidden minigames into the test
		
		if (it->second->Unlocked) {
			SeenMiniGames.Add(it->first);
		} else {
			UnseenMiniGames.Add(it->first);			
		}
	}
	
	SeenMiniGames.Shuffle();
	UnseenMiniGames.Shuffle();
	
	int to_add;
	
	if (SeenMiniGames.Count == 0) {
		// we've seen no games yet; add five unseen games
		to_add = 5;
	} else {
		to_add = min(UnseenMiniGames.Count, 4);
	}
	
	for (int i = 0; i < to_add; ++i) {
		TestMiniGames.Add(UnseenMiniGames[i]);
	}
	
	// figure out how many seen games we need to add, and add them
	to_add = 5 - to_add;
	for (int i = 0; i < to_add; ++i) {
		TestMiniGames.Add(SeenMiniGames[i]);
	}
	
	// shuffle the list so that the seen/unseen games come in any order
	TestMiniGames.Shuffle();
	
	InTestMode = true;
	SetGameState(TEST_STATUS);
}

void BPGame::NextTest() {
	if (TestPosition == TestMiniGames.Count) {
		// use this for quick testing
//	if (true) {
//		TestScores.Add(150);
//		TestScores.Add(250);
//		TestScores.Add(350);
//		TestScores.Add(450);
//		TestScores.Add(500);
		CalculateTestResults();
		SetGameState(TEST_RESULTS);
		
		InTestMode = false;
		
		PlayMusic("results");

		return;
	}
	
	CreateMiniGame(TestMiniGames[TestPosition], false, false);
	++TestPosition;
}

void BPGame::PlaySound(const char* sound, bool preload_only, bool force_play) {

	if (!EnableSound && !force_play) return;
	
	if (Sounds.find(sound) != Sounds.end()) {
		if (preload_only) {
			// do nothing - this is only for iPhone
		} else {
			Mix_PlayChannel(-1, Sounds[sound], 0);
		}
	} else {
		printf("Error: sound %s doesn't exist.\n", sound);
	}
}

void BPGame::PlayMusic(const char* name) {
	if (!EnableMusic) return;
	
	if (Music != NULL) {
		Mix_HaltMusic();
		Mix_FreeMusic(Music);
		Music = NULL;
	}
	
	string* file = new string(name);
	file->insert(0, "/opt/brainparty/Content/");
	file->append(".ogg");
	
	Music = Mix_LoadMUS(file->c_str());
	Mix_PlayMusic(Music, -1);

	delete(file);
}

void BPGame::StopMusic() {
	if (Music != NULL) {
		Mix_HaltMusic();
		Mix_FreeMusic(Music);
		Music = NULL;
	}
}

bool BPGame::FileExists(const char * filename) {
    if (FILE * file = fopen(filename, "r")) {
        fclose(file);
        return true;
    }
    return false;
}

Mix_Chunk* BPGame::LoadSound(const char* filename, const char* extension) {
	string* file = new string(filename);
	file->insert(0, "/opt/brainparty/Content/");
	file->append(".wav");
	
	Mix_Chunk* retval = Mix_LoadWAV(file->c_str());
	if (retval == NULL) printf("Warning: couldn't load file %s.\n", filename);
	delete file;
	
	return retval;
}

void BPGame::CancelTest() {
	GameState = PLAY_MENU;
	TestMiniGames.Clear();
	TestScores.Clear();
	TestPosition = 0;
	
	InTestMode = false;
	
	PlayMusic("theme");
}

void BPGame::Accelerate(float x, float y, float z) {
	Acceleration[0] = x;
	Acceleration[1] = y;
	Acceleration[2] = z;
}

void BPGame::PromptResetScores() {
	ShowingClearScores = true;
	MessageBox::Show("Are you sure you want to wipe all your test scores so far? Press Y on your keyboard now to wipe your scores. Click the mouse or press any other key cancel.", "Clear scores?");
}

void BPGame::ExecuteResetScores() {
	AllTestScores.Clear();
	SaveSettings();
	MessageBox::Show("Your test scores have been wiped and you have a clean slate - good luck!", "Scores wiped!");
}

Colour* BPGame::ColorLerp(Colour* from, Colour* to, float amount) {
	Colour* col = new Colour(Lerp(from->R, to->R, amount), Lerp(from->G, to->G, amount), Lerp(from->B, to->B, amount), Lerp(from->A, to->A, amount));
	return col;
}
