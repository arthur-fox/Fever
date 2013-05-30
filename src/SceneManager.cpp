//
//  SceneManager.cpp
//  Fever
//
//  Created by Arthur Fox on 14/02/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#include "SceneManager.h"
#include <iostream>
#include <sstream>
#include <iomanip>

//initialise platform array and speed
SceneManager::SceneManager()
{
    m_pGlobal = Global::SharedGlobal();
    
    //Render fps text
	std::stringstream stream;
	stream << FRAMES_PER_SECOND;
	std::string mystr = stream.str();
	m_pFrames = TTF_RenderText_Blended( m_pGlobal->GetFont( SMALL_FONT ), mystr.c_str(), m_pGlobal->GetColor( WHITE_COLOUR ) );
	if( m_pFrames == NULL ){
		printf("Could not render fps: %s\n", TTF_GetError());
		exit(1);
	}
    
	m_pFeverText = TTF_RenderText_Blended( m_pGlobal->GetFont( TITLE_FONT ), "FEVER", m_pGlobal->GetColor( WHITE_COLOUR ) );
	if( m_pFeverText == NULL ){
		printf("Could not render climber text: %s\n", TTF_GetError());
		exit(1);
	}
    
	m_pLoadText = TTF_RenderText_Blended( m_pGlobal->GetFont( SMALL_FONT ), "Load", m_pGlobal->GetColor( WHITE_COLOUR ) );
	if( m_pLoadText == NULL ){
		printf("Could not render load text: %s\n", TTF_GetError());
		exit(1);
	};
    
    m_pPlayText = TTF_RenderText_Blended( m_pGlobal->GetFont( SMALL_FONT ), "Play", m_pGlobal->GetColor( WHITE_COLOUR ) );
	if( m_pPlayText == NULL ){
		printf("Could not render play text: %s\n", TTF_GetError());
		exit(1);
	};
    
    m_pGenText = TTF_RenderText_Blended( m_pGlobal->GetFont( SMALL_FONT ), "Generate", m_pGlobal->GetColor( WHITE_COLOUR ) );
	if( m_pGenText == NULL ){
		printf("Could not render generate text: %s\n", TTF_GetError());
		exit(1);
	};
    
	m_pGameOverText = TTF_RenderText_Blended( m_pGlobal->GetFont( SMALL_FONT ), "SCORE:" , m_pGlobal->GetColor( WHITE_COLOUR ) );
	if( m_pGameOverText == NULL ){
		printf("Could not render game over: %s\n", TTF_GetError());
		exit(1);
	}
	
//	m_pTryAgainText = TTF_RenderText_Solid( m_pGlobal->getFont(SMALL_FONT), "Press any key to try again" , m_pGlobal->getColor(WHITE) );
//	if( m_pTryAgainText == NULL ){
//		printf("Could not render try again: %s\n", TTF_GetError());
//		exit(1);
//	}
    
	m_pScore = TTF_RenderText_Blended(  m_pGlobal->GetFont( SMALL_FONT ), "0" , m_pGlobal->GetColor( WHITE_COLOUR ));
	if( m_pScore == NULL ){
		printf( "Could not render score: %s\n", TTF_GetError() );
		exit(1);
	}
    
    m_pMult = TTF_RenderText_Blended(  m_pGlobal->GetFont( SMALL_FONT ), "0" , m_pGlobal->GetColor( WHITE_COLOUR ) );
	if( m_pMult == NULL ){
		printf( "Could not render score: %s\n", TTF_GetError() );
		exit(1);
	}
    
	m_pMutedIcon = TTF_RenderText_Blended( m_pGlobal->GetFont( SMALL_FONT ), "M" , m_pGlobal->GetColor( WHITE_COLOUR ) );
	if( m_pMutedIcon == NULL ){
		printf( "Could not render mutedIcon: %s\n", TTF_GetError() );
		exit(1);
	}
    
    m_pPlayer = Global::SharedGlobal()->LoadImage( RESOURCE_PLAYER );
}

#pragma mark -
#pragma mark Updates


void SceneManager::UpdateInLevel( float dt, float fps, int sc, int mult )
{
    UpdateFrameRate( fps );
    UpdateMultiplier( mult );
    UpdateScore( sc );
}


void SceneManager::ResetScene()
{
    m_bgX = m_bgY = 0;
    UpdateFrameRate( 0 );
    UpdateScore( 0 );
}


//Update the frame rate
void SceneManager::UpdateFrameRate( float fps )
{
	std::stringstream stream;
	stream << std::setprecision(2) << fps;
	std::string mystr = stream.str();
    
    SDL_FreeSurface( m_pFrames );
	m_pFrames = TTF_RenderText_Blended( m_pGlobal->GetFont( SMALL_FONT ), mystr.c_str(), m_pGlobal->GetColor( WHITE_COLOUR ) );
}

//Update the multiplier
void SceneManager::UpdateMultiplier( int mult )
{
	std::stringstream stream;
	stream << "x" << mult;
	std::string mystr = stream.str();
    
    SDL_FreeSurface( m_pMult );
	m_pMult = TTF_RenderText_Blended( m_pGlobal->GetFont( SMALL_FONT ), mystr.c_str(), m_pGlobal->GetColor( WHITE_COLOUR ) );
}

//Update the frame score
void SceneManager::UpdateScore( int sc )
{
	std::stringstream stream;
	stream << sc;
	std::string mystr = stream.str();
    
    SDL_FreeSurface( m_pScore );
	m_pScore = TTF_RenderText_Blended( m_pGlobal->GetFont( SMALL_FONT ), mystr.c_str(), m_pGlobal->GetColor( WHITE_COLOUR ) );
}


#pragma mark -
#pragma mark RenderFunctions

// Renders the background in the main menu
// -- too many magic numbers in this function?
void SceneManager::RenderInMainMenu( SDL_Surface* pScreen, int option )
{
    float color = SDL_MapRGB( pScreen->format, 100, 30, 30 );
    SDL_FillRect( pScreen, NULL , color );
	
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pFeverText->w)/2, (SCREEN_HEIGHT - m_pFeverText->h)/2 - 100, m_pFeverText, pScreen );
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pLoadText->w)/2, (SCREEN_HEIGHT - m_pLoadText->h)/2 + 50, m_pLoadText, pScreen );
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pPlayText->w)/2, (SCREEN_HEIGHT - m_pPlayText->h)/2 + 100, m_pPlayText, pScreen );
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pGenText->w)/2, (SCREEN_HEIGHT - m_pGenText->h)/2 + 150, m_pGenText, pScreen );
    
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pGenText->w)/2 - 120, (SCREEN_HEIGHT - m_pPlayer->h)/2 + (50*(option+1)), m_pPlayer, pScreen );
	
    if( SDL_Flip( pScreen ) == -1 )
		exit(1);
}

// Renders the scrolling background for in-game
void SceneManager::RenderInLevel( SDL_Surface* pScreen )
{
    //Render the frame rate and score
    m_pGlobal->ApplySurface( 10, 0, m_pMult, pScreen );
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pScore->w)/2, 0, m_pFrames, pScreen );
    m_pGlobal->ApplySurface(  SCREEN_WIDTH - m_pScore->w - 10, 0, m_pScore, pScreen );
    
    //Render the mute icon if necessary
    if ( m_pGlobal->IsMuted() ) 
    {
        m_pGlobal->ApplySurface( SCREEN_WIDTH - m_pMutedIcon->w - 12, SCREEN_HEIGHT - 50, m_pMutedIcon, pScreen );
    }
}


void SceneManager::RenderLevelOver( SDL_Surface* pScreen )
{   
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pGameOverText->w)/2, (SCREEN_HEIGHT - m_pGameOverText->h)/2 - 50, m_pGameOverText, pScreen );
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pScore->w)/2, (SCREEN_HEIGHT - m_pScore->h)/2 + 10, m_pScore, pScreen );
}