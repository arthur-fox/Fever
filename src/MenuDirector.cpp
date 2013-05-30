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

const int LOAD_LEVEL = 0;
const int PLAY_LEVEL = 1;
const int GEN_LEVEL  = 2;
const int NUM_OPTIONS = 3;

Global* MenuDirector::ms_pGlobal(0);
std::string MenuDirector::ms_spath;
std::string MenuDirector::ms_sname;
std::string MenuDirector::ms_lpath;

#pragma mark -
#pragma mark Constructor

MenuDirector::MenuDirector( SDL_Surface* pScreen ) 
{
    ms_pGlobal = Global::SharedGlobal();
    
    m_pScreen = pScreen;
    
    m_pSceneManager = new SceneManager();
    
    m_option = LOAD_LEVEL;

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
	
    m_pSceneManager->RenderInMainMenu( m_pScreen, m_option );
	
	SDL_SetEventFilter( MainMenuEventFilter );
	
    bool loop = true;
	while ( loop )
	{
        SDL_WaitEvent( &m_event );
        
		if( m_event.type == SDL_QUIT || (m_event.type == SDL_KEYDOWN && m_event.key.keysym.sym == SDLK_ESCAPE) )
        {
			loop = game = false;
            break;
        }
        
        if( m_event.key.keysym.sym == SDLK_UP)
            m_option = ((m_option-1 % NUM_OPTIONS) + NUM_OPTIONS) % NUM_OPTIONS;
        
        if( m_event.key.keysym.sym == SDLK_DOWN)
            m_option = ((m_option+1 % NUM_OPTIONS) + NUM_OPTIONS) % NUM_OPTIONS;
        
        if( m_event.key.keysym.sym == SDLK_RETURN)
        {
            loop = HandleOption();
        }
        
        m_pSceneManager->RenderInMainMenu( m_pScreen, m_option );     
	}
	
	SDL_SetEventFilter( NULL );
    
    pLvlname->assign( m_loadedLevel );
	
	return game;
}

int MenuDirector::MainMenuEventFilter( const SDL_Event *pEvent )
{
	if( (pEvent->type == SDL_KEYDOWN  && 
         (pEvent->key.keysym.sym == SDLK_RETURN || pEvent->key.keysym.sym == SDLK_UP || pEvent->key.keysym.sym == SDLK_DOWN))
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
    
    if( m_option == LOAD_LEVEL)
        ret = LoadLevel();
    
    else if( m_option == PLAY_LEVEL)
        ret = PlayLevel();
    
    else if( m_option == GEN_LEVEL)
        ret = GenLevel();
    
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
    
    if ( m_loadedLevel.compare(LEVEL_NOT_LOADED) == 0 )
    {
        printf( "No level has been loaded!\n" );
        return true;
    }
    
    std::cout << "Playing: " << Path::NameFromPath( m_loadedLevel ) + LEVEL_EXTENSION << std::endl;
    return false;
}

bool MenuDirector::GenLevel()
{
    if ( MATLAB_ON && !ms_pGlobal->IsThreadRunning() )
    {
        std::string songpath = DialogBox::OpenFileBrowser(FORMATS_AUDIO);
        if( songpath.compare( LEVEL_NOT_LOADED ) != 0 )
        {
            std::string songname = Path::NameFromPath( songpath );
            std::string lvlpath = LEVEL_PATH + songname + LEVEL_EXTENSION;
            
            //HACK - because std::thread initialisation would not accept argument passing
            ms_spath.assign( songpath );
            ms_sname.assign( songname );
            ms_lpath.assign( lvlpath );
            std::thread genThread( Generate );
            genThread.detach();
        }
    }
    else if ( !MATLAB_ON )
    {
        printf("Cannot Generate levels without MATLAB_ON\n");
    }
    else
    {
        printf("Wait for Level Generation to end!\n");
    }
    
    return true;
}

#pragma mark -
#pragma mark Helper

void MenuDirector::Generate()
{
    if ( ms_lpath.compare( LEVEL_NOT_LOADED ) == 0 || 
         ms_lpath.compare( LEVEL_NOT_LOADED ) == 0 || 
         ms_lpath.compare( LEVEL_NOT_LOADED ) == 0 )
    {
        printf( "Error generating level: paths not set correctly" );
        return;
    }
    
    ms_pGlobal = Global::SharedGlobal();
    ms_pGlobal->SetThreadRunning( true );
    
    if ( !LevelDirector::GenLevel( ms_lpath, ms_spath ) )
    {
        Mix_Chunk *chunk;
        chunk = Mix_LoadWAV(MUSIC_FAIL);
        Mix_PlayChannel(1, chunk, 0);
        printf( "Error generating level: %s \npath: %s\n", ms_lpath.c_str(), ms_spath.c_str() );
    }
    else
    {
        Mix_Chunk *chunk;
        chunk = Mix_LoadWAV(MUSIC_SUCCESS);
        Mix_PlayChannel(1, chunk, 0);
        std::cout << "Successfuly generated: " << ms_sname + LEVEL_EXTENSION << std::endl;
    }
    
    ms_pGlobal->SetThreadRunning( false );
}