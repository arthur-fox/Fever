//
//  GameDirector.cpp
//  Fever
//
//  Created by Arthur Fox on 14/02/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#include "GameDirector.h"
#include "MenuDirector.h"
#include "LevelDirector.h"

#pragma mark -
#pragma mark Construct

GameDirector::GameDirector()
{
    //Initialise the screen 
	SDL_WM_SetCaption(GAME_NAME, NULL );
	m_pScreen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
	if ( m_pScreen == NULL ) {
		printf( "Could not set %dx%dx%d video mode: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_GetError() );
		SDL_Quit();
		exit(1);
	}
}

GameDirector::~GameDirector()
{
	SDL_FreeSurface( m_pScreen );
}

#pragma mark -
#pragma mark GameFunctions

// This is the main game loop!
void GameDirector::Run()
{	
	bool game = true; //while true the game should still run
	
    MenuDirector menu = MenuDirector( m_pScreen );
    
    std::string lvlpath;
    lvlpath.reserve( MAX_PATH_SIZE );
    
	while ( game ){
        
		game = menu.Run( &lvlpath );
		
        if ( game )
        {   
            LevelDirector level = LevelDirector( lvlpath, m_pScreen );
            
            game = level.Run();
        }
	}
}