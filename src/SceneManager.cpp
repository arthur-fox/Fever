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
SceneManager::SceneManager(float levelSpeed)
{
    m_pGlobal = Global::SharedGlobal();
    m_levelSpeed = levelSpeed;
    m_bgX = m_bgY = 0;
    
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
	
	m_pPrevScoreText = TTF_RenderText_Solid( m_pGlobal->GetFont(SMALL_FONT), "HIGH SCORE:" , m_pGlobal->GetColor( WHITE_COLOUR ) );
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

    m_pRandomText = TTF_RenderText_Blended( m_pGlobal->GetFont( SMALL_FONT ), LEVEL_RANDOM , m_pGlobal->GetColor( WHITE_COLOUR ) );
	if( m_pRandomText == NULL ){
		printf( "Could not render random text: %s\n", TTF_GetError() );
		exit(1);
	}
    
    m_pAmplitudeText = TTF_RenderText_Blended( m_pGlobal->GetFont( SMALL_FONT ), LEVEL_AMPLITUDE , m_pGlobal->GetColor( WHITE_COLOUR ) );
	if( m_pAmplitudeText == NULL ){
		printf( "Could not render amplitude text: %s\n", TTF_GetError() );
		exit(1);
	}
    
    m_pFrequencyText = TTF_RenderText_Blended( m_pGlobal->GetFont( SMALL_FONT ), LEVEL_FREQUENCY, m_pGlobal->GetColor( WHITE_COLOUR ) );
	if( m_pFrequencyText == NULL ){
		printf( "Could not render frequency text: %s\n", TTF_GetError() );
		exit(1);
	}
    
    m_pEnergyText = TTF_RenderText_Blended( m_pGlobal->GetFont( SMALL_FONT ), LEVEL_ENERGY , m_pGlobal->GetColor( WHITE_COLOUR ) );
	if( m_pEnergyText == NULL ){
		printf( "Could not render energy text: %s\n", TTF_GetError() );
		exit(1);
	}
    
    m_pTempText = TTF_RenderText_Blended( m_pGlobal->GetFont( TINY_FONT ), "EMPTY" , m_pGlobal->GetColor( WHITE_COLOUR ) );
	if( m_pTempText == NULL ){
		printf( "Could not render temp text: %s\n", TTF_GetError() );
		exit(1);
	}
    
    m_pPlayer = Global::SharedGlobal()->LoadImage( RESOURCE_PLAYER );
    m_pLeftArrow = Global::SharedGlobal()->LoadImage( RESOURCE_LEFT_ARROW );
    m_pRightArrow = Global::SharedGlobal()->LoadImage( RESOURCE_RIGHT_ARROW );
    m_pBackground = Global::SharedGlobal()->LoadImage( RESOURCE_BACKGROUND );
}

#pragma mark -
#pragma mark Updates


void SceneManager::UpdateInLevel( float dt, float fps, int sc, int mult )
{
    m_bgX -= (m_levelSpeed/LEVEL_SPEED_FACTOR) * (dt/1000.f);
    if( m_bgX <= -LEVEL_WIDTH )
        m_bgX = 0;
    
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
	stream << "FPS: " << std::setprecision(2) << fps;
	std::string mystr = stream.str();
    
    SDL_FreeSurface( m_pFrames );
	m_pFrames = TTF_RenderText_Blended( m_pGlobal->GetFont( SMALL_FONT ), mystr.c_str(), m_pGlobal->GetColor( WHITE_COLOUR ) );
}

//Update the multiplier
void SceneManager::UpdateMultiplier( int mult )
{
	std::stringstream stream;
	stream << "MULT: x" << mult;
	std::string mystr = stream.str();
    
    SDL_FreeSurface( m_pMult );
	m_pMult = TTF_RenderText_Blended( m_pGlobal->GetFont( SMALL_FONT ), mystr.c_str(), m_pGlobal->GetColor( WHITE_COLOUR ) );
}

//Update the frame score
void SceneManager::UpdateScore( int sc )
{
	std::stringstream stream;
	stream << "SCORE: " << sc;
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
//    Uint32 mainMenuColour = SDL_MapRGB( pScreen->format, 100, 30, 30 );
    Uint32 mainMenuColour = SDL_MapRGB( pScreen->format, 50, 50, 120 );
    SDL_FillRect( pScreen, NULL , mainMenuColour );
    m_pGlobal->ApplySurface(  0, 0, m_pBackground, pScreen );
	
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pFeverText->w)/2,    (SCREEN_HEIGHT - m_pFeverText->h)/2 - 100,  m_pFeverText,   pScreen );
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pLoadText->w)/2,     (SCREEN_HEIGHT - m_pLoadText->h )/2 + 50,   m_pLoadText,    pScreen );
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pPlayText->w)/2,     (SCREEN_HEIGHT - m_pPlayText->h )/2 + 100,  m_pPlayText,    pScreen );
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pGenText->w)/2,      (SCREEN_HEIGHT - m_pGenText->h  )/2 + 150,  m_pGenText,     pScreen );
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pHighScoreText->w)/2,(SCREEN_HEIGHT - m_pHighScoreText->h)/2+200,m_pHighScoreText,pScreen);
    
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pHighScoreText->w)/2 - 120, (SCREEN_HEIGHT - m_pPlayer->h)/2 + (50*(option+1)), m_pPlayer, pScreen );
	
    //Render the mute icon if necessary
    if ( m_pGlobal->IsMuted() )
    {
        m_pGlobal->ApplySurface( SCREEN_WIDTH - m_pMutedIcon->w - 12, SCREEN_HEIGHT - 50, m_pMutedIcon, pScreen );
    }
    
    if( SDL_Flip( pScreen ) == -1 )
		exit(1);
}

void SceneManager::RenderInGenOptions( SDL_Surface* pScreen, int option )
{
//    Uint32 genMenuColour = SDL_MapRGB( pScreen->format, 100, 30, 30 );
    Uint32 genMenuColour = SDL_MapRGB( pScreen->format, 50, 50, 120 );
    SDL_FillRect( pScreen, NULL , genMenuColour );
    m_pGlobal->ApplySurface(  0, 0, m_pBackground, pScreen );

    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pRandomText->w)/2,     (SCREEN_HEIGHT - m_pRandomText->h)/2 - 150,       m_pRandomText, pScreen );
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pAmplitudeText->w)/2,  (SCREEN_HEIGHT - m_pAmplitudeText->h)/2 - 50,     m_pAmplitudeText, pScreen );
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pFrequencyText->w)/2,  (SCREEN_HEIGHT - m_pFrequencyText->h )/2 +50,     m_pFrequencyText, pScreen );
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pEnergyText->w)/2,     (SCREEN_HEIGHT - m_pEnergyText->h )/2 + 150,      m_pEnergyText,    pScreen );
    
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pAmplitudeText->w)/2 - 120, (SCREEN_HEIGHT - m_pPlayer->h)/2 + ((option-2)*100 + 50), m_pPlayer, pScreen );
	
    //Render the mute icon if necessary
    if ( m_pGlobal->IsMuted() )
    {
        m_pGlobal->ApplySurface( SCREEN_WIDTH - m_pMutedIcon->w - 12, SCREEN_HEIGHT - 50, m_pMutedIcon, pScreen );
    }
    
    if( SDL_Flip( pScreen ) == -1 )
		exit(1);
}

// Renders the background for in-game
void SceneManager::RenderInLevel( SDL_Surface* pScreen, std::string levelName )
{
    //Render scrolling background
    m_pGlobal->ApplySurface(  m_bgX, m_bgY, m_pBackground, pScreen );
    m_pGlobal->ApplySurface(  m_bgX + LEVEL_WIDTH, m_bgY, m_pBackground, pScreen );
    
    //Render the multiplier and score
    m_pGlobal->ApplySurface( 10, 0, m_pMult, pScreen );
    m_pGlobal->ApplySurface(  SCREEN_WIDTH - m_pScore->w - 10, 0, m_pScore, pScreen );
    
    //Render the frame rate
    if (m_pGlobal->IsFPS())
        m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pScore->w)/2, m_pFrames->h, m_pFrames, pScreen );
    
    //Level name
    SDL_FreeSurface( m_pTempText );
	m_pTempText = TTF_RenderText_Blended( m_pGlobal->GetFont( SMALL_FONT ), levelName.c_str(), m_pGlobal->GetColor( WHITE_COLOUR ) );
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pTempText->w)/2, 0, m_pTempText, pScreen );
    
    //Render the mute icon if necessary
    if ( m_pGlobal->IsMuted() ) 
    {
        m_pGlobal->ApplySurface( SCREEN_WIDTH - m_pMutedIcon->w - 12, SCREEN_HEIGHT - 50, m_pMutedIcon, pScreen );
    }
}

// Renders the scores screen
void SceneManager::RenderInScores( SDL_Surface* pScreen, int currScreen, int totalScreens )
{
//    Uint32 scoresColour = SDL_MapRGB( pScreen->format, 100, 30, 30 );
    Uint32 scoresColour = SDL_MapRGB( pScreen->format, 50, 50, 120 );
    SDL_FillRect( pScreen, NULL , scoresColour );
    m_pGlobal->ApplySurface(  0, 0, m_pBackground, pScreen );
    
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pHighScoreText->w)/2, m_pHighScoreText->h, m_pHighScoreText, pScreen );
    
    std::ifstream scoresFile;
    scoresFile.open( LEVEL_SCORES );
    
    scoresFile.seekg(0, scoresFile.end);
    int length = (int) scoresFile.tellg();
    scoresFile.seekg(0, scoresFile.beg);
    
    //Skip over a certain number of scores
    for ( int i = 0; i < currScreen*SCORES_PER_SCREEN; i++)
    {
        std::string levelAndScore;
        std::getline(scoresFile, levelAndScore);
    }
    
    //Print high scores on this page
    int textPos = 2.5 * m_pHighScoreText->h;
    for ( int i = 0;  i < SCORES_PER_SCREEN && scoresFile.tellg() != length; i++)
    {
        std::string levelAndScore;
        std::getline(scoresFile, levelAndScore);
        
        SDL_FreeSurface( m_pTempText );
        m_pTempText = TTF_RenderText_Blended( m_pGlobal->GetFont( TINY_FONT ), levelAndScore.c_str(), m_pGlobal->GetColor( WHITE_COLOUR ) );
        m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pTempText->w)/2, textPos, m_pTempText, pScreen );
        
        textPos += m_pTempText->h;
    }
    
    //Display Arrows
    if (totalScreens > 1)
    {
        m_pGlobal->ApplySurface( m_pRightArrow->w*0.5, (SCREEN_HEIGHT - m_pLeftArrow->h)/2, m_pLeftArrow , pScreen );
        m_pGlobal->ApplySurface( SCREEN_WIDTH - m_pLeftArrow->w*1.5, (SCREEN_HEIGHT - m_pRightArrow->h)/2, m_pRightArrow, pScreen );
    }
    
    //Render the mute icon if necessary
    if ( m_pGlobal->IsMuted() )
    {
        m_pGlobal->ApplySurface( SCREEN_WIDTH - m_pMutedIcon->w - 12, SCREEN_HEIGHT - 50, m_pMutedIcon, pScreen );
    }
    
    if( SDL_Flip( pScreen ) == -1 )
		exit(1);
}


void SceneManager::RenderLevelOver( SDL_Surface* pScreen, std::string levelName, int sc )
{
    m_pGlobal->ApplySurface(  0, 0, m_pBackground, pScreen );
    
    //Score
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pShowScoreText->w)/2, (SCREEN_HEIGHT - m_pShowScoreText->h)/2 - 110, m_pShowScoreText, pScreen );
    std::stringstream stream;
	stream << sc;
	std::string mystr = stream.str();
    SDL_FreeSurface( m_pScore );
	m_pScore = TTF_RenderText_Blended( m_pGlobal->GetFont( SMALL_FONT ), mystr.c_str(), m_pGlobal->GetColor( WHITE_COLOUR ) );
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pScore->w)/2, (SCREEN_HEIGHT - m_pScore->h)/2 - 50, m_pScore, pScreen );
    
    //High Score
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pPrevScoreText->w)/2, (SCREEN_HEIGHT - m_pPrevScoreText->h)/2 + 50, m_pPrevScoreText, pScreen );
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pTempText->w)/2, (SCREEN_HEIGHT - m_pTempText->h)/2 + 110, m_pTempText, pScreen );
    
    //Level name
    SDL_FreeSurface( m_pMult );
	m_pMult = TTF_RenderText_Blended( m_pGlobal->GetFont( SMALL_FONT ), levelName.c_str(), m_pGlobal->GetColor( WHITE_COLOUR ) );
    m_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pMult->w)/2, m_pMult->h, m_pMult, pScreen );
    
    //Render the mute icon if necessary
    if ( m_pGlobal->IsMuted() )
    {
        m_pGlobal->ApplySurface( SCREEN_WIDTH - m_pMutedIcon->w - 12, SCREEN_HEIGHT - 50, m_pMutedIcon, pScreen );
    }
}