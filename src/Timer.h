//
//  Timer.h
//  Fever
//
//  Created by Arthur Fox on 14/02/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#ifndef Fever_Timer_h
#define Fever_Timer_h

#include "SDL.h"

// Timer allows for timing things in-game eg. delta time step

class Timer
{
public:
	//Initializes variables
	Timer();
	
	//The various clock actions
	void Start();
	void Stop();
	void Pause();
	void Unpause();
	
	//Gets the timer's time
    int GetTicks() const;
	
	//Checks the status of the timer
	inline bool IsStarted() const {return m_started;}
	inline bool IsPaused()  const {return m_paused; }
    
private:
	//The clock time when the timer started, and stored when paused
	int m_startTicks, m_pausedTicks;
	
	//The timer status
	bool m_started, m_paused;
};

#endif