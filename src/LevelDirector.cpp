//
//  LevelDirector.cpp
//  Fever
//
//  Created by Arthur Fox on 21/02/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#include "LevelDirector.h"
#include "MenuDirector.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <map>

Global* LevelDirector::ms_pGlobal(0);

#pragma mark -
#pragma mark Constructor

LevelDirector::LevelDirector(const std::string &pFilepath, SDL_Surface* pScreen) 
{
    ms_pGlobal = Global::SharedGlobal();
    
    m_pScreen = pScreen;
    
    m_pSceneManager = new SceneManager();
    
    m_fps = FRAMES_PER_SECOND;
    m_mult = INIT_MULT;
    m_score = INIT_SCORE;
    
    //Render pause text
	m_pPause = TTF_RenderText_Blended( ms_pGlobal->GetFont(SMALL_FONT), "PAUSED", ms_pGlobal->GetColor(WHITE_COLOUR) );
	if( m_pPause == NULL ){
		printf("Could not render pause: %s\n", TTF_GetError());
		exit(1);
	}
    
    //Initialise things specific to the level being played:
    m_filepath.assign(pFilepath);
    
    std::ifstream lvlfile;
    lvlfile.open( m_filepath.c_str() );
    
    lvlfile >> m_songpath;
    lvlfile >> m_levelSpeed;    
    lvlfile >> m_songDuration;
    lvlfile >> m_noteFreq;
    m_songDuration *= 1000; // Convert to milliseconds
}

LevelDirector::~LevelDirector()
{
	SDL_FreeSurface( m_pPause );
}

#pragma mark -
#pragma mark Run

//Playing loop, returns false is user has X'd out otherwise returns true
bool LevelDirector::Run()
{
	/***** VARIABLES ****/
    bool game = true; //Return value
    bool playing = true; 
    
	Timer frame, delta, song;
    float dtAccumulator = 0, delay = 0;
	int framesPassed = 0;
    
    Floor floor = Floor( m_filepath );
    Player player = Player( &floor, m_levelSpeed);
    Camera camera = Camera();
    
    NoteManager notes = NoteManager( m_levelSpeed, m_noteFreq, SCREEN_WIDTH, floor.GetLastHeight(), &floor );
    ColourManager colours = ColourManager(m_levelSpeed);
    EffectsManager effects = EffectsManager(m_levelSpeed);
    
	/** LOOP **/
    
    //Start playing the song
    Mix_Music* pMusic = Mix_LoadMUS( m_songpath.c_str());
    std::this_thread::sleep_for( std::chrono::milliseconds(100));
    Mix_FadeInMusic( pMusic, 0, 5000 ); 
    song.Start();
    
    m_pSceneManager->ResetScene();
    delta.Start();
    
	while( playing )
	{
		frame.Start();
		
		/****** EVENTS ******/ 
		while (SDL_PollEvent(&m_event))
		{
			if(m_event.type == SDL_KEYDOWN && m_event.key.keysym.sym == SDLK_SPACE)
            {
                song.Pause();
				delta.Pause();
				frame.Pause();
				PauseGame(game, playing);
				frame.Unpause();
				delta.Unpause();
                song.Unpause();
			}
            
			else if( m_event.type == SDL_KEYDOWN && m_event.key.keysym.sym == SDLK_m)
            {                
                ms_pGlobal->MuteUnMute();  
                if (ms_pGlobal->IsMuted()) 
                    Mix_VolumeMusic(0);
                else
                    Mix_VolumeMusic(128);
			}
            
            else if( m_event.type == SDL_KEYDOWN && m_event.key.keysym.sym == SDLK_ESCAPE )
            {
                game = true;
                playing = false;
			}
            
			else if( m_event.type == SDL_QUIT )
            {
				game = playing = false;
			}
		}
		
		/***** LOGIC *******/
        
        if ( !Update(camera, player, floor, notes, colours, effects, delta.GetTicks(), dtAccumulator) )
        {
            playing = false;
            break;
        }
        
        
        if (song.GetTicks() >= m_songDuration - EXTRA_TIME) //Stop notes EXTRA_TIME early so we never get notes before level stopped
        {
            notes.SetCreatingNotes(false);
            
            if (song.GetTicks() >= m_songDuration + EXTRA_TIME) // EXTRA_TIME seconds allow some more time to elapse
            {
                game = EndSequence(camera, player, floor, colours);
                playing = false;
                break;
            }
        }
        
		delta.Start(); 
        
		/****** RENDERING *******/
        Render(camera, player, floor, notes, colours, effects);
		
		/***** HOUSEKEEPING *****/
		//If we need to cap the frame rate sleep the remaining frame time 
		//NOTE: unnecessary at 60fps due to SDL_Flip() and VSYNC ensuring it stays at 60fps
		if ( frame.GetTicks() < 1000/FRAMES_PER_SECOND )
        {
			SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - frame.GetTicks() );
		}
		
		delay += frame.GetTicks();
        
		if ( ++framesPassed >= FRAMES_PER_SECOND )
		{
//            //DEBUG
//            std::cout << (int)(song.GetTicks()/1000) << std::endl;
            
			OncePerSecond( delay, notes );
			framesPassed = delay = 0;
		}
	}
	
    Mix_FreeMusic(pMusic);
    
	return game;
}

//Per second block
void LevelDirector::OncePerSecond( float delay, NoteManager& rNotes )
{	
	m_fps = (FRAMES_PER_SECOND*1000.0f)/delay;
    rNotes.ClearInvisible();
}

#pragma mark -
#pragma mark Update

//Updates game logic with a fixed-time step, returns false if its GAMEOVER
bool LevelDirector::Update(Camera& rCamera, Player& rPlayer, Floor& rFloor, NoteManager& rNotes, ColourManager& rColours, EffectsManager& rEffects,  int dt, float& dtAccumulator)
{
    bool result = true;
    
    if (VARIABLE_TIME_STEP) //VARIABLE_TIME_STEP: More dangerous but does not skip frames hence more accurate
    {
        result = UpdateGame(rCamera, rPlayer, rFloor, rNotes, rColours, rEffects, dt);
    }
    else  //FIXED_TIME_STEP: Safer but skips frames! Hence innacurate.
    {
        //Loop through updating as many times as required, will only update at most LIMIT_UPDATES times
        int updates = 0;
        for (; (dt >= FIXED_TIME_STEP) && (updates < LIMIT_UPDATES) && result; dt -= FIXED_TIME_STEP, updates++)
        {
            result = UpdateGame(rCamera, rPlayer, rFloor, rNotes, rColours, rEffects, FIXED_TIME_STEP);
        }
        //Accumulate remaining ticks left between frames so we don't "play catch up" in the next few frames
        dtAccumulator += dt;
        
        //Once this has accumulated past a FIXED_TIME_STEP, then update the game logic once again
        while ( dtAccumulator >= FIXED_TIME_STEP && result )
        {
            result = UpdateGame(rCamera, rPlayer, rFloor, rNotes, rColours, rEffects, FIXED_TIME_STEP);
            dtAccumulator -= FIXED_TIME_STEP;
        }
    }
    
    return result;
}

// Calls update on player, floor and notes
bool LevelDirector::UpdateGame(Camera& rCamera, Player& rPlayer, Floor& rFloor, NoteManager& rNotes, ColourManager& rColours, EffectsManager& rEffects, float dt )
{
    bool result = true;
    
    rPlayer.HandleInput( dt );
    
    m_pSceneManager->UpdateInLevel( dt, m_fps, m_score, m_mult );
    
    rFloor.Update( dt );
    
    rNotes.Update( dt );
    
    rPlayer.Update( dt );
    
    rCamera.Update( dt, Point(rPlayer.GetX(), rPlayer.GetY()) );
    
    UpdateScore( rPlayer, rNotes, rEffects );
    
    rEffects.Update( dt );
    
    if (!rEffects.IsPlayingFlash())
        rColours.Update( dt );
    
    return result;
}

void LevelDirector::UpdateScore(Player& rPlayer, NoteManager& rNotes, EffectsManager& rEffects)
{   
    int note = rNotes.TouchNote(rPlayer);
    if( note >= 0 )
    {
        m_score += m_mult;
        rNotes.Remove(note);
        rEffects.ResetPlayedFlash();
    }
    
    m_mult = (rNotes.GetTouched()/MULTIPLIER_COINS_NEEDED +1);
    
    //CHECK: Could keep playing screen flash over and over if notes do not change.
    if( rNotes.GetTouched()%MULTIPLIER_COINS_NEEDED == 0 && !rEffects.HasPlayedFlash() && m_mult != 1)
    {
        rEffects.PlayScreenFlash();
    }
}

#pragma mark -
#pragma mark Render

//Renders all the assets as necessary, currently return value is just ignored
bool LevelDirector::Render(Camera& rCamera, Player& rPlayer, Floor& rFloor, NoteManager& rNotes, ColourManager& rColours, EffectsManager& rEffects )
{
    bool result = true;
    
    if (!rEffects.IsPlayingFlash())
        rColours.Render( m_pScreen );
    
    rEffects.Render( m_pScreen );
    
    m_pSceneManager->RenderInLevel( m_pScreen );
    
    rPlayer.Render( m_pScreen, rCamera );
    
    rFloor.Render( m_pScreen, rCamera );
    
    rNotes.Render( m_pScreen, rCamera );
    
    if( SDL_Flip( m_pScreen ) == -1 )
        exit(1);
    
    return result;
}

#pragma mark -
#pragma mark PauseFunctions

//Pause the game, returns false if user Xs out
void LevelDirector::PauseGame( bool& game, bool& playing )
{	
    //Pause the music probably
    Mix_PauseMusic();
	
	//Write pause on the screen
	ms_pGlobal->ApplySurface( (SCREEN_WIDTH - m_pPause->w)/2, (SCREEN_HEIGHT - m_pPause->h)/2, m_pPause, m_pScreen );
	if( SDL_Flip( m_pScreen ) == -1 )
		exit(1);
	
	//Set pauseEventFilter
	SDL_SetEventFilter(PauseEventFilter);
	
	//Wait for an event to pass through, if its a quit event then quit = true
	//Otherwise it will be an unpause, so ignore it
	if(SDL_WaitEvent(&m_event))
	{
		if( m_event.type == SDL_QUIT) 
			game = playing = false;
        else if( m_event.key.keysym.sym == SDLK_ESCAPE )
            playing = false;
	}
	
	//Remove the pauseEventFilter
	SDL_SetEventFilter(NULL);
	
    //Resume song
    Mix_ResumeMusic();
}

// STATIC Event filter for pausing game - filter so that only unpause and quit can pass through
int LevelDirector::PauseEventFilter(const SDL_Event *pEvent)
{
	if( (pEvent->type == SDL_KEYDOWN && (pEvent->key.keysym.sym == SDLK_SPACE || pEvent->key.keysym.sym == SDLK_ESCAPE ))
       || pEvent->type == SDL_QUIT )
		return 1;
    
	return 0;
}

#pragma mark -
#pragma mark LevelEnd

bool LevelDirector::EndSequence(Camera& rCamera, Player& rPlayer, Floor& rFloor, ColourManager& rColours )
{   
    bool game = true;
    
    bool ending = true;
    while ( ending )
    {
        rColours.Render(m_pScreen);
        
        m_pSceneManager->RenderLevelOver( m_pScreen);
        
        rFloor.End(m_pScreen, rCamera);
        
        ending = rPlayer.End(m_pScreen, rCamera);
        
        if( SDL_Flip( m_pScreen ) == -1 )
            exit(1);
        
        SDL_Delay( ( 1000 / (3*FRAMES_PER_SECOND) ));
    }
    
    UpdateHighScores();
    
    std::this_thread::sleep_for( std::chrono::seconds(1) );
    
	SDL_SetEventFilter( EndEventFilter );
    if ( SDL_WaitEvent(&m_event) )
	{
		if( m_event.type == SDL_QUIT )
			game = false;
	}
	SDL_SetEventFilter(NULL);
    
    return game;
}

int LevelDirector::EndEventFilter( const SDL_Event *pEvent )
{
	if ( (pEvent->type == SDL_KEYDOWN) || pEvent->type == SDL_QUIT)
		return 1;
    
	return 0;
}

void LevelDirector::UpdateHighScores()
{
    /* Save high scores into a map */
    std::map<std::string,int> scores;
    
    std::ifstream inHighScores;
    inHighScores.open( LEVEL_SCORES );
    
    std::string level;
    int score = 0;
    
    inHighScores.seekg(0, inHighScores.end);
    int length = (int) inHighScores.tellg();
    inHighScores.seekg(0, inHighScores.beg);
    
    while ( inHighScores.tellg() != length )
    {
        std::getline(inHighScores, level, '\t');
        inHighScores >> score;
        scores.insert( std::pair<std::string,int>(level,score) );
        
        //Skip out the rest of the line
        std::getline(inHighScores, level, '\n');
    }
    inHighScores.close();
    
    /* Update score */
    std::string songName = Path::NameFromPath(m_filepath);
    if ( scores.find(songName) == scores.end() )
    {
        scores.insert( std::pair<std::string,int>(songName,m_score) );
    }
    else if ( scores.at(songName) < m_score )
    {
        scores.at(songName) = m_score;
    }
    
    /* Store high scores back into the file */
    std::ofstream outHighScores;
    outHighScores.open( LEVEL_SCORES, std::ios::trunc );
    for (std::map<std::string,int>::iterator it=scores.begin(); it!=scores.end(); ++it)
    {
        outHighScores << it->first << "\t\t" << it->second << "\n";
    }
    outHighScores.close();
}

#pragma mark -
#pragma mark LevelGeneration
 
//TODO: Finish this function 100%
bool LevelDirector::GenLevel(std::string lvlpath, std::string songpath)
{
    bool ret = true;
    
    ms_pGlobal = Global::SharedGlobal();
    
    // Create file and make PARTIAL_OUTPUT
    std::ofstream lvlfile(LEVEL_TEMP, std::ofstream::trunc);
    lvlfile.close();
    ret &= GenLevelMatlab(songpath);
    
    // With PARTIAL_OUTPUT generate all the other required parts in the FINAL_OUTPUT.
    //  - currently only the floor is generated
    //    still need to do Coins and Colours
    ret &= Floor::GenFloorPoints(lvlpath, songpath);
    
    // Generated FINAL_OUTPUT, so I can now delete PARTIAL_OUTPUT
    // NOTE: Not deleting atm because these values are useful to look at!!!
//    if ( remove(LEVEL_TEMP) != 0)
//        printf("Unable to delete LEVEL_TEMP\n");
    
    return ret;
}

//TEMPORARILY WRITING ALL THE CONTENTS OUT OF MY SCRIPT TO THIS FUNCTION UNTIL 
//I FIGURE OUT HOW TO USE SCRIPT WITHOUT PERMISSION ERRORS
bool LevelDirector::GenLevelMatlab(std::string songpath)
{
//    ms_pGlobal->EvalMatlabString( std::string("GenLevel('" + songpath + ", " + LEVEL_TEMP + "');").c_str() );
    
    ms_pGlobal->EvalMatlabString( std::string("audio    = miraudio('" + songpath + "');").c_str() );
    ms_pGlobal->EvalMatlabString( "energy   = mirenvelope(audio);" );
    ms_pGlobal->EvalMatlabString( "bands    = mirenvelope(mirfilterbank(audio, 'NbChannels', 3));" );
    ms_pGlobal->EvalMatlabString( "tempo    = mirtempo(bands);" );
    ms_pGlobal->EvalMatlabString( "signature= mirkey(bands);" );

    ms_pGlobal->EvalMatlabString( "len = get(audio,'Length');");
    ms_pGlobal->EvalMatlabString( "len = len{1}{1};" );
    ms_pGlobal->EvalMatlabString( "s   = get(audio,'Sampling');");
    ms_pGlobal->EvalMatlabString( "s   = s{1};");
    ms_pGlobal->EvalMatlabString( "duration  = len/s;");
    
    ms_pGlobal->EvalMatlabString( "eSampling = get(energy, 'Sampling');");
    ms_pGlobal->EvalMatlabString( "tempo     = mirgetdata(tempo);");
    ms_pGlobal->EvalMatlabString( "energy    = mirgetdata(energy);");
    ms_pGlobal->EvalMatlabString( "eSize     = length(energy);");
    ms_pGlobal->EvalMatlabString( "signature = mean(mirgetdata(signature))*tempo;");
    ms_pGlobal->EvalMatlabString( "bands     = mirgetdata(bands);");
    
    std::string lvl("lvlpath   = '"); 
    lvl.append(LEVEL_TEMP).append("';");
    ms_pGlobal->EvalMatlabString( lvl.c_str() );
    ms_pGlobal->EvalMatlabString( "dlmwrite(lvlpath, signature);");
    ms_pGlobal->EvalMatlabString( "dlmwrite(lvlpath, tempo, '-append');");
    ms_pGlobal->EvalMatlabString( "dlmwrite(lvlpath, duration, '-append');");
    ms_pGlobal->EvalMatlabString( "dlmwrite(lvlpath, eSampling, '-append');");
    ms_pGlobal->EvalMatlabString( "dlmwrite(lvlpath, eSize, '-append','precision','%.1f');");
    ms_pGlobal->EvalMatlabString( "dlmwrite(lvlpath, energy,'-append','precision','%.10f');");
//    %dlmwrite(lvlpath, bandsSize, '-append'); ???
    ms_pGlobal->EvalMatlabString( "dlmwrite(lvlpath, bands, '-append');");
    ms_pGlobal->EvalMatlabString( "dlmwrite(lvlpath, 'E', '-append');");

    ms_pGlobal->EvalMatlabString( "clearvars audio tempo energy bands signature duration eSampling eSize len s;");
    
    return true;
}

/*
engEvalString(pMateng, "mirverbose(0);");
//engEvalString(mateng, "mirparallel(1);"); ??

engEvalString(pMateng, "a = miraudio('/Users/arthurfox/Documents/MATLAB/MIRtoolbox1.4/MIRToolboxDemos/guitar.wav');");
engEvalString(pMateng, "b = mirgetdata(a);");
mxArray *pArr = engGetVariable(pMateng, "b");
int m = 139062;

//engEvalString(pMateng, "close;");
double *ds;
ds = mxGetPr(pArr);
std::cout << ds[0] << " " << ds[m-1] <<std::endl;
mxDestroyArray(pArr);


// use mirfeatures to analyse features then either export it to file with mirexport, 
// or get them with engGetVariable() and use them directly from matlab engine.

engEvalString(pMateng, "features = mirfeatures('/Users/arthurfox/Documents/MATLAB/MIRtoolbox1.4/MIRToolboxDemos/guitar.wav');");
engEvalString(pMateng, "tempo = mirgetdata(features.rhythm.tempo{1})");
engEvalString(pMateng, "mirexport('guitardata.txt',features)");
engEvalString(pMateng, "mirexport('guitartempo.txt',tempo)");
pArr = engGetVariable(pMateng, "tempo");
*/