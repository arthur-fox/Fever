//
//  Global.cpp
//  Fever
//
//  Created by Arthur Fox on 14/02/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#include "Global.h"
#include "SDL_mixer.h"
#include "Path.h"
#include <iostream>
#include <thread>
#include <unistd.h>

#pragma mark -
#pragma mark SingletonPattern

Global* Global::ms_pGlobal(0);
bool Global::ms_genThreadRunning;

Global* Global::SharedGlobal()
{	
	if(ms_pGlobal == 0)
		ms_pGlobal = new Global();
	
	return ms_pGlobal;
}

Global::Global(){}

#pragma mark -
#pragma mark Initialisation & CleanUp

void Global::Initialisation()
{
    InitSDL();
    InitFontsColors();
    InitMusic();
    if (!MATLAB_OFF) InitMatlab();
    
    m_dispFPS = false;
}

void Global::CleanUp()
{
    if (!MATLAB_OFF)
    {
        while ( IsThreadRunning() )
            std::this_thread::sleep_for( std::chrono::seconds(1) );
        
        int close = engClose( m_pMateng );
        printf( "Value of engClose: %d \n", close );
    }
    
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
}

void Global::InitSDL() const
{
	if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
		printf( "Could not initialise SDL: %s\n", SDL_GetError() );
		exit(1);
	}
}

void Global::InitMatlab() 
{
    if ( !(m_pMateng = engOpen(NULL)) ) {
		printf( "Cannot initialise MATLAB engine\n" );
		exit(1);
	}
    
    // NOTE: The TOOLBOX and the DYLIBS need to be saved inside the final app hierachy!!
    engEvalString( m_pMateng, "addpath /Applications/MATLAB_R2012b.app/toolbox/AuditoryToolbox;" ); 
    engEvalString( m_pMateng, "addpath /Applications/MATLAB_R2012b.app/toolbox/netlab;" );
    engEvalString( m_pMateng, "addpath /Applications/MATLAB_R2012b.app/toolbox/somtoolbox;" );
    engEvalString( m_pMateng, "addpath /Applications/MATLAB_R2012b.app/toolbox/MIRToolbox;" );
    engEvalString( m_pMateng, (std::string("addpath ") + LEVEL_PATH).c_str() );
}

void Global::InitFontsColors()
{
	if ( TTF_Init() == -1 ){
		printf( "Could not initialise TTF: %s\n", TTF_GetError() );
		exit(1);
	}
    
    //NOT QUITE SURE HOW THIS IS NOT DESTROYED BY THE FUNCTION STACK BEING DESTROYED?
    //MAYBE A COPY CONSTRUCTOR IS USED IN m_whiteColour = white ?
    SDL_Color white = { 255, 255, 255 };
    m_whiteColour = white;
    
    std::string font = ms_pPath->PathForFile( RESOURCE_TITLE_FONT );
    m_pTitleFont  = TTF_OpenFont( font.c_str(), 220 );
    font = ms_pPath->PathForFile( RESOURCE_FONT );
    m_pTinyFont   = TTF_OpenFont( font.c_str(), 28 );
    m_pSmallFont  = TTF_OpenFont( font.c_str(), 36 );
    m_pMediumFont = TTF_OpenFont( font.c_str(), 56 );
    m_pLargeFont  = TTF_OpenFont( font.c_str(), 112 );
    if( m_pSmallFont == NULL || m_pMediumFont == NULL || m_pLargeFont == NULL ){
		printf( "Could not load font: %s\n", TTF_GetError() );
		exit(1);
	}
}

void Global::InitMusic()
{
    if ( Mix_OpenAudio( MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ) { 
        exit(1);
    }
    m_muted = false;

    sucChunk = Mix_LoadWAV(MUSIC_SUCCESS);
    Mix_VolumeChunk(sucChunk, 80);
    failChunk = Mix_LoadWAV(MUSIC_FAIL);
    Mix_VolumeChunk(failChunk, 80);
    scrollChunk = Mix_LoadWAV(MUSIC_UI_SCROLL);
    Mix_VolumeChunk(scrollChunk, 80);
    acceptChunk = Mix_LoadWAV(MUSIC_UI_ACCEPT);
    Mix_VolumeChunk(acceptChunk, 80);
    recordChunk = Mix_LoadWAV(MUSIC_NEW_RECORD);
    Mix_VolumeChunk(recordChunk, 80);
}

#pragma mark -
#pragma mark FontFunctions

TTF_Font* Global::GetFont( int font ) const
{
    switch ( font ) 
    {
        case TINY_FONT:
            return m_pTinyFont;
            break;
            
        case SMALL_FONT:
            return m_pSmallFont;
            break;
            
        case MEDIUM_FONT:
            return m_pMediumFont;
            break;
            
        case LARGE_FONT:
            return m_pLargeFont;
            break;
            
        case TITLE_FONT:
            return m_pTitleFont;
            break;
            
        default:
            return m_pSmallFont;
            break;
    }
}

SDL_Color Global::GetColor( int color ) const
{
    switch ( color ) 
    {
        case WHITE_COLOUR:
            return m_whiteColour;
            break;
            
        default:
            return m_whiteColour;
            break;
    }
}


#pragma mark -
#pragma mark GlobalHelpers

SDL_Surface* Global::LoadImage( const std::string& rFilename ) const
{
	SDL_Surface* pLoadedImage = NULL;
	SDL_Surface* pOptimizedImage = NULL;
	
    std::string path = ms_pPath->PathForFile( rFilename );
    pLoadedImage = IMG_Load( path.c_str() );
    
	if( pLoadedImage != NULL )
	{
		pOptimizedImage = SDL_DisplayFormatAlpha( pLoadedImage );
		SDL_FreeSurface( pLoadedImage );
	}
	else 
    {
		std::cout << "ERROR: Loading image " << rFilename << " has failed" << std::endl;
	}
	
	return pOptimizedImage;
}


void Global::ApplySurface( int x, int y, SDL_Surface* pSource, SDL_Surface* pDestination, SDL_Rect* pClip ) const
{
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	
    //CHECK: KNOWN TO LEAK MEMORY
	SDL_BlitSurface( pSource, pClip, pDestination, &offset );
}

void Global::PlaySound(const char* sound)
{
    std::string chosenSound = std::string(sound);
    
    if ( !IsMuted() )
    {
        if ( chosenSound.compare(MUSIC_SUCCESS) == 0 )
            Mix_PlayChannel(1, sucChunk, 0);
            
        else if ( chosenSound.compare(MUSIC_FAIL) == 0 )
            Mix_PlayChannel(1, failChunk, 0);
        
        else if ( chosenSound.compare(MUSIC_UI_SCROLL) == 0 )
            Mix_PlayChannel(1, scrollChunk, 0);
        
        else if ( chosenSound.compare(MUSIC_UI_ACCEPT) == 0 )
            Mix_PlayChannel(1, acceptChunk, 0);
        
        else if ( chosenSound.compare(MUSIC_NEW_RECORD) == 0 )
            Mix_PlayChannel(1, recordChunk, 0);
    }
}

#pragma mark -
#pragma mark MatlabFunctions

void Global::EvalMatlabString( const char* str )
{
    engEvalString( m_pMateng, str ); 
}