//
//  SceneManager.cpp
//  Fever
//
//  Created by Arthur Fox on 14/02/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#include "SceneManager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>

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
    
	m_pFeverText = TTF_RenderText_Blended( m_pGlobal->GetFont( TITLE_FONT ), GAME_NAME, m_pGlobal->GetColor( WHITE_COLOUR ) );
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
    
    m_pHighScoreText = TTF_RenderText_Blended( m_pGlobal->GetFont( SMALL_FONT ), "High Scores", m_pGlobal->GetColor( WHITE_COLOUR ) );
	if( m_pHighScoreText == NULL ){
		printf("Could not render generate text: %s\n", TTF_GetError());
		exit(1);
	};
    
	m_pShowScoreText = TTF_RenderText_Blended( m_pGlobal->GetFont( SMALL_FONT ), "SCORE:" , m_pGlobal->GetColor( WHITE_COLOUR ) );
	if( m_pShowScoreText == NULL ){
		printf("Could not render game over: %s\n", TTF_GetError());
		exit(1);
	}
	
	m_pPrevScoreText = TTF_RenderText_Solid( m_pGlobal->GetFont(SMALL_FONT), "PREVIOUS SCORE:" , m_pGlobal->GetColor( WHITE_COLOUR ) );
	if( m_pPrevScoreText == NULL ){
		printf("Could not render try again: %s\n", TTF_GetError());
		exit(1);
	}
    
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
    
    m_pTempText = TTF_RenderText_Blended( m_pGlobal->GetFont( TINY_FONT ), "EMPTY" , m_pGlobal->GetColor( WHITE_COLOUR ) );
	if( m_pTempText == NULL ){
		printf( "Could not render temp text: %s\n", TTF_GetError() );
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

//Update the frame score
void SceneManager::UpdatePreviousScore( int sc )
{
	std::stringstream stream;
	stream << sc;
	std::string mystr = stream.str();
    
    SDL_FreeSurface( m_pTempText );
	m_pTempText = TTF_RenderText_Blended( m_pGlobal->GetFont( SMALL_FONT ), mystr.c_str(), m_pGlobal->GetColor( WHITE_COLOUR ) );
}


void SceneManager::ResetScene()
{
    m_bgX = m_bgY = 0;
    UpdateFrameRate( 0 );
    UpdateMultiplier( 1 );
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
    Uint32 menuColour = SDL_MapRGB( pScreen->format, 100, 30, 30 );
    SDL_FillRect( pScreen, NULL , menuColour );
	
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pFeverText->w)/2,    (SCREEN_HEIGHT - m_pFeverText->h)/2 - 100,  m_pFeverText,   pScreen );
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pLoadText->w)/2,     (SCREEN_HEIGHT - m_pLoadText->h )/2 + 50,   m_pLoadText,    pScreen );
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pPlayText->w)/2,     (SCREEN_HEIGHT - m_pPlayText->h )/2 + 100,  m_pPlayText,    pScreen );
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pGenText->w)/2,      (SCREEN_HEIGHT - m_pGenText->h  )/2 + 150,  m_pGenText,     pScreen );
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pHighScoreText->w)/2,(SCREEN_HEIGHT - m_pHighScoreText->h)/2+200,m_pHighScoreText,pScreen);
    
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pHighScoreText->w)/2 - 120, (SCREEN_HEIGHT - m_pPlayer->h)/2 + (50*(option+1)), m_pPlayer, pScreen );
	
    if( SDL_Flip( pScreen ) == -1 )
		exit(1);
}

// Renders the background for in-game
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

// Renders the scores screen
void SceneManager::RenderInScores( SDL_Surface* pScreen )
{
    Uint32 scoresColour = SDL_MapRGB( pScreen->format, 100, 30, 30 );
    SDL_FillRect( pScreen, NULL , scoresColour );
    
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pHighScoreText->w)/2, m_pHighScoreText->h, m_pHighScoreText, pScreen );
    
    std::ifstream scoresFile;
    scoresFile.open( LEVEL_SCORES );
    
    scoresFile.seekg(0, scoresFile.end);
    int length = (int) scoresFile.tellg();
    scoresFile.seekg(0, scoresFile.beg);
    
    int textPos = 2.5 * m_pHighScoreText->h;
    while ( scoresFile.tellg() != length && textPos < SCREEN_HEIGHT )
    {
        std::string levelAndScore;
        std::getline(scoresFile, levelAndScore);
        
        SDL_FreeSurface( m_pTempText );
        m_pTempText = TTF_RenderText_Blended( m_pGlobal->GetFont( TINY_FONT ), levelAndScore.c_str(), m_pGlobal->GetColor( WHITE_COLOUR ) );
        m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pTempText->w)/2, textPos, m_pTempText, pScreen );
        
        textPos += m_pTempText->h;
    }
    
    if( SDL_Flip( pScreen ) == -1 )
		exit(1);
}


void SceneManager::RenderLevelOver( SDL_Surface* pScreen )
{    
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pShowScoreText->w)/2, (SCREEN_HEIGHT - m_pShowScoreText->h)/2 - 110, m_pShowScoreText, pScreen );
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pScore->w)/2, (SCREEN_HEIGHT - m_pScore->h)/2 - 50, m_pScore, pScreen );
    
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pPrevScoreText->w)/2, (SCREEN_HEIGHT - m_pPrevScoreText->h)/2 + 50, m_pPrevScoreText, pScreen );
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pTempText->w)/2, (SCREEN_HEIGHT - m_pTempText->h)/2 + 110, m_pTempText, pScreen );
}