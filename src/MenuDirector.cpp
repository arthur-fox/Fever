//
//  MenuDirector.cpp
//  Fever
//
//  Created by Arthur Fox on 22/02/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#include "MenuDirector.h"
#include "LevelDirector.h"
#include "DialogBox.h"
#include <iostream>
#include <fstream>
#include <thread>

const int LOAD_LEVEL  = 0;
const int PLAY_LEVEL  = 1;
const int GEN_LEVEL   = 2;
const int HIGH_SCORES = 3;
const int NUM_MENU_OPTIONS = 4;
const int NUM_GEN_OPTIONS = 4;

Global* MenuDirector::ms_pGlobal(0);
std::string MenuDirector::ms_spath;
std::string MenuDirector::ms_sname;
std::string MenuDirector::ms_lpath;
int MenuDirector::ms_genOption;

#pragma mark -
#pragma mark Constructor

MenuDirector::MenuDirector( SDL_Surface* pScreen ) 
{
    ms_pGlobal = Global::SharedGlobal();
    
    m_pScreen = pScreen;
    
    m_pSceneManager = new SceneManager();
    
    m_menuOption = LOAD_LEVEL;
    m_displayScores = false;
    
    m_currScoreScreen = 0;
    m_totalScoreScreens = CalcTotalScoreScreens();

    m_loadedLevel.assign( LEVEL_NOT_LOADED );
    ms_spath.assign( LEVEL_NOT_LOADED );
    ms_sname.assign( LEVEL_NOT_LOADED );
    ms_lpath.assign( LEVEL_NOT_LOADED );
}

MenuDirector::~MenuDirector() 
{
    delete m_pSceneManager;
}

#pragma mark -
#pragma mark MenuFunctions

//Main menu screen, returns false if user has X'd out otherwise returns true
bool MenuDirector::Run( std::string* pLvlname )
{
	bool game = true;
	
    m_pSceneManager->RenderInMainMenu( m_pScreen, m_menuOption );
	
	SDL_SetEventFilter( MainMenuEventFilter );
	
    bool loop = true;
	while ( loop )
	{
        SDL_WaitEvent( &m_event );
        
		if( m_event.type == SDL_QUIT )
        {
			loop = game = false;
            break;
        }
        
        if (m_displayScores)
        {
            if ( m_event.type == SDL_KEYDOWN && m_event.key.keysym.sym == SDLK_ESCAPE )
                m_displayScores = false;
            
            if( m_event.key.keysym.sym == SDLK_LEFT)
            {
                m_currScoreScreen = ((m_currScoreScreen-1 % m_totalScoreScreens) + m_totalScoreScreens) % m_totalScoreScreens;
                ms_pGlobal->PlaySound(MUSIC_UI_SCROLL);
            }
            
            if( m_event.key.keysym.sym == SDLK_RIGHT)
            {
                m_currScoreScreen = ((m_currScoreScreen+1 % m_totalScoreScreens) + m_totalScoreScreens) % m_totalScoreScreens;
                ms_pGlobal->PlaySound(MUSIC_UI_SCROLL);
            }
            
        }
        else
        {
            if ( m_event.type == SDL_KEYDOWN && m_event.key.keysym.sym == SDLK_ESCAPE )
            {
                loop = game = false;
                break;
            }
        
            if( m_event.key.keysym.sym == SDLK_UP)
            {
                m_menuOption = ((m_menuOption-1 % NUM_MENU_OPTIONS) + NUM_MENU_OPTIONS) % NUM_MENU_OPTIONS;
                ms_pGlobal->PlaySound(MUSIC_UI_SCROLL);
            }
            
            if( m_event.key.keysym.sym == SDLK_DOWN)
            {
                m_menuOption = ((m_menuOption+1 % NUM_MENU_OPTIONS) + NUM_MENU_OPTIONS) % NUM_MENU_OPTIONS;
                ms_pGlobal->PlaySound(MUSIC_UI_SCROLL);
            }
            
            if( m_event.key.keysym.sym == SDLK_RETURN)
            {
                if (m_menuOption != GEN_LEVEL && m_menuOption != PLAY_LEVEL)
                    ms_pGlobal->PlaySound(MUSIC_UI_ACCEPT);
                loop = HandleOption();
            }
        }
        
        if( m_event.type == SDL_KEYDOWN && m_event.key.keysym.sym == SDLK_m)
        {
            ms_pGlobal->MuteUnMute();      
        }
        
        if (m_displayScores)
            m_pSceneManager->RenderInScores( m_pScreen, m_currScoreScreen, m_totalScoreScreens );
        else
            m_pSceneManager->RenderInMainMenu( m_pScreen, m_menuOption );
	}
	
	SDL_SetEventFilter( NULL );
    
    pLvlname->assign( m_loadedLevel );
	
	return game;
}

int MenuDirector::MainMenuEventFilter( const SDL_Event *pEvent )
{
	if( (pEvent->type == SDL_KEYDOWN  && 
         (pEvent->key.keysym.sym == SDLK_RETURN || pEvent->key.keysym.sym == SDLK_m
         || pEvent->key.keysym.sym == SDLK_UP   || pEvent->key.keysym.sym == SDLK_DOWN
         || pEvent->key.keysym.sym == SDLK_LEFT || pEvent->key.keysym.sym == SDLK_RIGHT))
         || pEvent->type == SDL_QUIT || pEvent->key.keysym.sym == SDLK_ESCAPE )
		return 1;
    
	return 0;
}

#pragma mark -
#pragma mark Options

// Returns TRUE if we wish to CARRY ON LOOPING in the main menu
bool MenuDirector::HandleOption()
{
    bool ret = true;
    
    if( m_menuOption == LOAD_LEVEL )
        ret = LoadLevel();
    
    else if( m_menuOption == PLAY_LEVEL )
        ret = PlayLevel();
    
    else if( m_menuOption == GEN_LEVEL )
        ret = GenLevel();
    
    else if( m_menuOption == HIGH_SCORES )
        ret = HighScores();
    
    return ret;
}

bool MenuDirector::LoadLevel()
{
    std::string lvlname = DialogBox::OpenFileBrowser( FORMATS_LEVEL );
    if ( lvlname.compare( LEVEL_NOT_LOADED ) != 0 )
    {
        m_loadedLevel.assign( lvlname );
        std::cout << "Loaded: " << Path::NameFromPath( m_loadedLevel ) + LEVEL_EXTENSION << std::endl;
    }
    
    return true;
}


bool MenuDirector::PlayLevel()
{
    
    if ( m_loadedLevel.compare( LEVEL_NOT_LOADED ) == 0 )
    {
        ms_pGlobal->PlaySound(MUSIC_FAIL);
        printf( "No level has been loaded!\n" );
        return true;
    }

    ms_pGlobal->PlaySound(MUSIC_UI_ACCEPT);
    std::cout << "Playing: " << Path::NameFromPath( m_loadedLevel ) + LEVEL_EXTENSION << std::endl;
    return false;
}

bool MenuDirector::GenLevel()
{
    if ( !MATLAB_OFF && !ms_pGlobal->IsThreadRunning() )
    {
        ms_pGlobal->PlaySound(MUSIC_UI_ACCEPT);
        std::string songpath = DialogBox::OpenFileBrowser(FORMATS_AUDIO);
        if( songpath.compare( LEVEL_NOT_LOADED ) != 0 )
        {
            //If user X's or ESC's out then we leave this menu
            if ( HandleGenOption() )
            {
                //Appropriatley name the level based on songname and option
                std::string songname = Path::NameFromPath( songpath );
                std::string lvlpath = LEVEL_PATH + songname + "_";
                if      (ms_genOption == OPTION_RANDOM   ) lvlpath.append(LEVEL_RANDOM   );
                else if (ms_genOption == OPTION_AMPLITUDE) lvlpath.append(LEVEL_AMPLITUDE);
                else if (ms_genOption == OPTION_FREQUENCY) lvlpath.append(LEVEL_FREQUENCY);
                else if (ms_genOption == OPTION_ENERGY   ) lvlpath.append(LEVEL_ENERGY   );
                lvlpath.append(LEVEL_EXTENSION);
                
                //HACK - because std::thread initialisation would not accept argument passing
                ms_spath.assign( songpath );
                ms_sname.assign( songname );
                ms_lpath.assign( lvlpath );
                std::thread genThread( Generate );
                genThread.detach();
            }
        }
    }
    else if ( MATLAB_OFF )
    {
        printf("Cannot Generate levels without MATLAB_ON\n");
        ms_pGlobal->PlaySound(MUSIC_FAIL);
    }
    else
    {
        printf("Wait for Level Generation to end!\n");
        ms_pGlobal->PlaySound(MUSIC_FAIL);
    }
    
    return true;
}


bool MenuDirector::HandleGenOption()
{	
    bool ret = true;
    
    ms_genOption = OPTION_RANDOM;
    m_pSceneManager->RenderInGenOptions( m_pScreen, ms_genOption );
	
	while ( true )
	{
        SDL_WaitEvent( &m_event );
        
		if( m_event.type == SDL_QUIT ||  (m_event.type == SDL_KEYDOWN && m_event.key.keysym.sym == SDLK_ESCAPE ) )
        {
            ret = false;
            break;
        }
            
        if( m_event.key.keysym.sym == SDLK_UP)
        {
            ms_genOption = ((ms_genOption-1 % NUM_GEN_OPTIONS) + NUM_GEN_OPTIONS) % NUM_GEN_OPTIONS;
            ms_pGlobal->PlaySound(MUSIC_UI_SCROLL);
        }
        
        if( m_event.key.keysym.sym == SDLK_DOWN)
        {
            ms_genOption = ((ms_genOption+1 % NUM_GEN_OPTIONS) + NUM_GEN_OPTIONS) % NUM_GEN_OPTIONS;
            ms_pGlobal->PlaySound(MUSIC_UI_SCROLL);
        }
        
        if( m_event.type == SDL_KEYDOWN && m_event.key.keysym.sym == SDLK_m)
        {
            ms_pGlobal->MuteUnMute();
        }
        
        if( m_event.key.keysym.sym == SDLK_RETURN)
        {
            ms_pGlobal->PlaySound(MUSIC_UI_ACCEPT);
            break;
        }
        
        m_pSceneManager->RenderInGenOptions( m_pScreen, ms_genOption );
	}
    
    return ret;
}

bool MenuDirector::HighScores()
{
    m_displayScores = true;
    m_totalScoreScreens = CalcTotalScoreScreens();
    printf("Selected HighScores!\n");
    
    return true;
}

#pragma mark -
#pragma mark Helper

int MenuDirector::CalcTotalScoreScreens()
{
    std::ifstream scoresFile;
    scoresFile.open( LEVEL_SCORES );
    
    scoresFile.seekg(0, scoresFile.end);
    int length = (int) scoresFile.tellg();
    scoresFile.seekg(0, scoresFile.beg);
    
    int numEntries = 0;
    while (scoresFile.tellg() != length)
    {
        std::string levelAndScore;
        std::getline(scoresFile, levelAndScore);
        numEntries++;
    }
    
    int ret = numEntries/SCORES_PER_SCREEN;
    if ( numEntries%SCORES_PER_SCREEN != 0 ) ret++;
    
    return ret;
}

void MenuDirector::Generate()
{
    if ( ms_spath.compare( LEVEL_NOT_LOADED ) == 0 || 
         ms_sname.compare( LEVEL_NOT_LOADED ) == 0 || 
         ms_lpath.compare( LEVEL_NOT_LOADED ) == 0)
    {
        printf( "Error generating level: paths not set correctly" );
        return;
    }
    
    Timer timeGeningLevel;
    timeGeningLevel.Start();
    ms_pGlobal = Global::SharedGlobal();
    ms_pGlobal->SetThreadRunning( true );
    
    if ( !LevelDirector::GenLevel( ms_lpath, ms_spath, ms_genOption ) )
    {
        printf( "Error generating level: %s \npath: %s\n", ms_lpath.c_str(), ms_spath.c_str() );
        ms_pGlobal->PlaySound(MUSIC_FAIL);
    }
    else
    {
        std::cout << "Successfuly generated: " << Path::NameFromPath(ms_lpath) + LEVEL_EXTENSION << std::endl;
        ms_pGlobal->PlaySound(MUSIC_SUCCESS);
    }
    
    printf("Time taken genning level: %dms, Gen option: %d\n", timeGeningLevel.GetTicks(), ms_genOption);
    ms_pGlobal->SetThreadRunning( false );
    timeGeningLevel.Stop();
}