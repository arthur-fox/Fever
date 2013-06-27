//
//  Global.h
//  Fever
//
//  Created by Arthur Fox on 14/02/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#ifndef Fever_Global_h
#define Fever_Global_h

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "engine.h"
#include "Path.h"

#define GAME_NAME               "MOZART"//"FEVER"

#define RESOURCE_PATH           "Fever.app/Contents/Resources/"
#define RESOURCE_TITLE_FONT     "ldmusic.ttf"
#define RESOURCE_FONT           "Insignia.otf"
#define RESOURCE_PLAYER         "Mozart5.png"//"Blob2.png"
#define RESOURCE_COIN           "SimpleCoin2.png"
#define RESOURCE_NOTE1          "Note1_2.png"
#define RESOURCE_NOTE2          "Note2_2.png"
#define RESOURCE_NOTE3          "Note3_2.png"
#define RESOURCE_CLEF           "TrebleClef_2.png"
#define RESOURCE_LEFT_ARROW     "LeftArrow.png"
#define RESOURCE_RIGHT_ARROW    "RightArrow.png"
#define RESOURCE_BACKGROUND     "TransparentBackground.png"//"CompletelyTransparentBack.png"

#define LEVEL_PATH              "/Users/arthurfox/Desktop/Programming/Xcode/Project_MusicToGame/Levels/"
#define LEVEL_EXTENSION         ".lvl"
#define LEVEL_NOT_LOADED        "NULL"
#define LEVEL_TEMP              "/Users/arthurfox/Desktop/Programming/Xcode/Project_MusicToGame/Levels/TempOutput.plvl"
#define LEVEL_SCORES            "/Users/arthurfox/Desktop/Programming/Xcode/Project_MusicToGame/Levels/Scores.sc"
#define LEVEL_RANDOM            "Random"
#define LEVEL_AMPLITUDE         "Amplitude"
#define LEVEL_FREQUENCY         "Frequency"
#define LEVEL_ENERGY            "Energy"

#define MUSIC_SUCCESS           "/Users/arthurfox/Desktop/Programming/Xcode/Project_MusicToGame/Songs/Secret.wav"
#define MUSIC_FAIL              "/Users/arthurfox/Desktop/Programming/Xcode/Project_MusicToGame/Songs/Error.wav"
#define MUSIC_UI_SCROLL         "/Users/arthurfox/Desktop/Programming/Xcode/Project_MusicToGame/Songs/FFUIScroll.wav"
#define MUSIC_UI_ACCEPT         "/Users/arthurfox/Desktop/Programming/Xcode/Project_MusicToGame/Songs/FFUIAccept.wav"
#define MUSIC_NEW_RECORD        "/Users/arthurfox/Desktop/Programming/Xcode/Project_MusicToGame/Songs/NewRecord.wav"

#define FORMATS_AUDIO           "wav,mp3,WAV,MP3,AU"
#define FORMATS_LEVEL           "lvl"

//Debugging variables -- when all are false the game has its complete functionality
const bool MATLAB_OFF = false;
const bool COINS_ON = false;
const bool PAUSE_INVISIBLE_ON = false;
//const bool GEN_JAGGED_HILLS_ON = false;

// ************************************************************

//Update attributes
const bool VARIABLE_TIME_STEP = true;
const int FRAMES_PER_SECOND = 60;
const int UPDATES_PER_SECOND = 100;
const int LIMIT_UPDATES = 3; 
const float FIXED_TIME_STEP = 1000.0f/UPDATES_PER_SECOND;

//Screen attributes
const int SCREEN_WIDTH = 1024;//*1.8;
const int SCREEN_HEIGHT = 576;//*1.45;
const int SCREEN_BPP = 0;//32;

//The dimensions of the level 
const int LEVEL_WIDTH = SCREEN_WIDTH; //This is just for background scrolling
const int LEVEL_HEIGHT = SCREEN_HEIGHT*1.3;

//File attributes
const int MAX_PATH_SIZE = 200;

//Music attributes
const int DEFAULT_VOLUME = 128;
const int PAUSE_VOLUME = 30; // IS THIS NEEDED?

//Font attributes
const int TINY_FONT = 0;
const int SMALL_FONT = 1;
const int MEDIUM_FONT = 2;
const int LARGE_FONT = 3;
const int TITLE_FONT = 4;

//Dimensions of player
const int PLAYER_WIDTH = 100;
const int PLAYER_HEIGHT = 70;
const int PLAYER_JUMP = 80; // Rough estimate needed for Note generation

//Player Constants
const int INIT_JUMP = 1000; //766;
const int INIT_GRAVITY = 3090;
const int INIT_WALK = 736;

//Note Constants
//const int NOTE_WIDTH = 45; // NOTE: NOT CONSTANT!
const int NOTE_HEIGHT = 45; // NOTE: Constant for all notes except Treble Clef

//Score constants
const int INIT_SCORE = 0;
const int INIT_MULT = 1;

//Level constants
const float WALL_WIDTH = SCREEN_WIDTH/34; //30;
const float LEVEL_SPEED_FACTOR = 4; //3 - Maybe go back to 3? Tonight seems a bit too fast, or maybe its not?
const float LEVEL_PADDING = PLAYER_HEIGHT*1.5;
const int DEFAULT_LEVEL_SPEED = 350;

//Generation constants
const int OPTION_RANDOM    = 0;
const int OPTION_AMPLITUDE = 1;
const int OPTION_FREQUENCY = 2;
const int OPTION_ENERGY    = 3;

//High score constants
const int SCORES_PER_SCREEN = 15;

// ************************************************************

//Color attributes
const int WHITE_COLOUR = 1;

// Global gives access to global variables, functions and includes.
class Global 
{
public:	
	//Singleton pattern
	static Global* SharedGlobal();	
    
    //Static functions
    static bool IsThreadRunning()                   {return ms_genThreadRunning;}
    static void SetThreadRunning( bool running )    {ms_genThreadRunning = running;}
    
    //Inline functions
    inline bool IsMuted() const                     {return m_muted;}
    inline void MuteUnMute()                        {m_muted = !m_muted;}
    inline bool IsFPS() const                       {return m_dispFPS;}
    inline void DisplayFPS(bool fps)                {m_dispFPS = fps;}
    
    void Initialisation();
    void CleanUp();
    void PlaySound(const char* sound);
    
    TTF_Font* GetFont( int font ) const;
    SDL_Color GetColor( int color ) const;
    
	SDL_Surface* LoadImage( const std::string& filename ) const;
	void ApplySurface( int x, int y, SDL_Surface* pSource, SDL_Surface* pDestination, SDL_Rect* clip = NULL ) const;
    
    void EvalMatlabString( const char* str );
    
private:
	//Singleton instance
	static Global* ms_pGlobal;
    static Path* ms_pPath;
    
    //Static functions
    static bool ms_genThreadRunning;
    
    //Private Variables
    Engine *m_pMateng;
    TTF_Font  *m_pTinyFont, *m_pSmallFont, *m_pMediumFont, *m_pLargeFont, *m_pTitleFont;
    SDL_Color m_whiteColour;
    bool m_muted, m_dispFPS;
    Mix_Chunk *sucChunk, *failChunk, *scrollChunk, *acceptChunk, *recordChunk;
    
    //Private Functions
	Global();    
    void InitSDL() const;
    void InitMatlab();
    void InitFontsColors();
    void InitMusic();
};

#endif
