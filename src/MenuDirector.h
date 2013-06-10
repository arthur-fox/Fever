//
//  MenuDirector.h
//  Fever
//
//  Created by Arthur Fox on 22/02/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#ifndef Fever_MenuDirector_h
#define Fever_MenuDirector_h

#include "Global.h"
#include "SceneManager.h"

// MenuDirector knows how to run the main menu

class MenuDirector 
{
public:
    
    static void Generate();
    
    MenuDirector( SDL_Surface* pScreen );
    ~MenuDirector();
	bool Run( std::string* pLvlname );
    void CleanUp();
    
private:
    
    static Global* ms_pGlobal; //Needs to be static because of the Event Filters
    
    static std::string ms_spath, ms_sname, ms_lpath; // HACK: because C++11 stuff wasn't playing ball! I statically give these values over to generate function.
    static int ms_genOption;
    
    SceneManager* m_pSceneManager;
    SDL_Event m_event;
	SDL_Surface* m_pScreen;
    int m_menuOption, m_currScoreScreen, m_totalScoreScreens;
    bool m_displayScores;
    std::string m_loadedLevel;
    
    
    static int MainMenuEventFilter( const SDL_Event* pEvent );
    
    bool HandleOption();
    bool LoadLevel();
    bool PlayLevel();
    bool GenLevel();
    bool HandleGenOption();
    bool HighScores();
    int CalcTotalScoreScreens();
};

#endif
