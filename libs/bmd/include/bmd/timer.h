/*
 * BMD
 * Copyright (C) 2020 Blue Moon Development. All rights reserved.
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact: team@bluemoondev.org
 * 
 * File Name: timer.h
 * Date File Created: 8/7/2020 at 2:00 PM
 * Author: Matt
 */

#ifndef BMD_TIMER_H
#define BMD_TIMER_H

#include "types.h"
#include "platform.h"

#define DEFAULT_TIME_FORMAT "%m-%d-%Y %H:%M:%S"

typedef struct BMDTimer timer_t;


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifdef __cplusplus
void getCurrentTime(char (& timeStr)[100], const char* format);
#else
void getCurrentTime(char timeStr[100], const char* format);
#endif

void startTimer(timer_t* timer);
void initTimer(timer_t* timer);
float totalTime(timer_t* timer);
void resetTimer(timer_t* timer);
void unpauseTimer(timer_t* timer);
void pauseTimer(timer_t* timer);
void tickTimer(timer_t* timer);

float elapsedTime();


int64_f seconds(timer_t* timer, int64_f ticks);
int64_f milliseconds(timer_t* timer, int64_f ticks);
int64_f microseconds(timer_t* timer, int64_f ticks);


void sleepTimer(int ms);


#ifdef __cplusplus
};
#endif // __cplusplus

#ifdef OS_WINDOWS
struct BMDTimer
{

	int64_f freq;

	int64_f startTime;
	int64_f totalIdleTime;
	int64_f pausedTime;
	int64_f currentTime;
	int64_f previousTime;
	int64_f elapsedTime;

	float secondsPerCount;
	float delta;

	int stopped;
};
#endif
// TODO Mac and unix versions!

#ifdef BMD_HEADERS_ONLY
	#ifndef BMD_TIMER_IMPL
		#define BMD_TIMER_IMPL

#include <time.h>

#ifdef __cplusplus

void getCurrentTime(char (& timeStr)[100], const char* format)
#else
void getCurrentTime(char timeStr[100], const char* format)
#endif
{
	time_t rawTime;
	struct tm* timeInfo;
	time(&rawTime);
	timeInfo = localtime(&rawTime);
	strftime(timeStr, 100, format, timeInfo);
}

	#ifdef OS_WINDOWS

void sleepTimer(int ms)
{
	Sleep(ms);
}

float elapsedTime()
{
	static int first = 1;
	static LARGE_INTEGER prev;
	static double factor;
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);

	if (first)
	{
		first = 0;
		prev = now;
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);
		factor = 1.0 / (double) freq.QuadPart;
		return 0;
	}

	float elapsed = (float) ((double) (now.QuadPart - prev.QuadPart) * factor);
	prev = now;
	return elapsed;
}

void initTimer(timer_t* timer)
{
	int64_f freq;
	if(QueryPerformanceFrequency((LARGE_INTEGER*)&freq))
	{
		timer->freq = freq;
		timer->secondsPerCount = 1.0f / (float) freq;
	}

	resetTimer(timer);
}

float totalTime(timer_t* timer)
{
	if(timer->stopped)
		return (float)(timer->pausedTime - timer->startTime - timer->totalIdleTime) * timer->secondsPerCount;
	else return (float)(timer->currentTime - timer->startTime - timer->totalIdleTime) * timer->secondsPerCount;
}

void resetTimer(timer_t* timer)
{
	int64_f nowi;
	if(QueryPerformanceCounter((LARGE_INTEGER*)&nowi))
	{
		timer->startTime = nowi;
		timer->currentTime = nowi;
		timer->previousTime = nowi;
		timer->pausedTime = 0;
		timer->stopped = 0;
	}
}

void unpauseTimer(timer_t* timer)
{
	if(timer->stopped)
	{
		int64_f nowi;
		if(QueryPerformanceCounter((LARGE_INTEGER*)nowi))
		{
			timer->totalIdleTime += (nowi - timer->pausedTime);
			timer->previousTime = nowi;
			timer->pausedTime = 0;
			timer->stopped = 0;
			timer->totalIdleTime = 0;
			timer->delta = 0;
		}
	}
}

void pauseTimer(timer_t* timer)
{
	if(!timer->stopped)
	{
		int64_f nowi;
		if(QueryPerformanceCounter((LARGE_INTEGER*)nowi))
		{
			timer->pausedTime = nowi;
			timer->stopped = 1;
		}
	}
}

void tickTimer(timer_t* timer)
{
	if(timer->stopped)
		timer->delta = 0;
	else
	{
		if(QueryPerformanceCounter((LARGE_INTEGER*)&timer->currentTime))
		{
			timer->delta = (float)(timer->currentTime - timer->previousTime) * timer->secondsPerCount;
			timer->elapsedTime = timer->currentTime - timer->previousTime;
			timer->previousTime = timer->currentTime;
			if(timer->delta < 0)
				timer->delta = 0;
		}
	}
}


int64_f seconds(timer_t* timer, int64_f ticks)
{
	return ticks / timer->freq;
}

int64_f milliseconds(timer_t* timer, int64_f ticks)
{
	return ticks  / (timer->freq / 1000);
}

int64_f microseconds(timer_t* timer, int64_f ticks)
{
	return ticks / (timer->freq / 1000000);
}


	#endif // OS check
#endif // BMD_TIMER_IMPL
	#endif // BMD_HEADERS_ONLY

#endif //BMD_TIMER_H
