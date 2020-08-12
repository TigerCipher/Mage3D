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
 * File Name: profiler.h
 * Date File Created: 8/9/2020 at 2:14 PM
 * Author: Matt
 */

#ifndef BMD_PROFILER_H
#define BMD_PROFILER_H

#ifndef BMD_PROFILE
	#define BMD_PROFILE 0
#endif

#include "timer.h"

void start_profiler_internal(const char* name);
void stop_profiler_internal();

#ifdef __cplusplus

	#include "strutil.h"

class Profiler
{
public:
	explicit Profiler(const char* name, const char* suffix)
	{
		copyStr(m_name, name);
		concatStr(m_name, suffix);
		initTimer(&m_timer);
		m_stopped = false;
	}

	~Profiler()
	{
		if (!m_stopped)
			stop();
	}

private:
	Profiler() = default;

	void stop()
	{
		tickTimer(&m_timer);
		int64_f s = seconds(&m_timer, m_timer.elapsedTime);
		int64_f ms = milliseconds(&m_timer, m_timer.elapsedTime);
		int64_f us = microseconds(&m_timer, m_timer.elapsedTime);
		char abrev[3] = { (char) 230, 's', 0 };
		if (us >= 1000)
		{
			us %= 1000;
			if (ms >= 1000)
			{
				ms %= 1000;
			}
		}

		if (s >= 1)
		{
			if (ms >= 1 && us < 1)
				fprintf(stderr,
						"Profiler [%s] Duration: %lli s %lli ms -- Start time: %lli -- End time: %lli\n",
						m_name, s, ms, m_timer.startTime, m_timer.currentTime);
			else if (ms >= 1 && us >= 1)
				fprintf(stderr,
						"Profiler [%s] Duration: %lli s %lli ms %lli %s -- Start time: %lli -- End time: %lli\n",
						m_name, s, ms, us, abrev, m_timer.startTime, m_timer.currentTime);
			else if (ms < 1 && us < 1)
				fprintf(stderr, "Profiler [%s] Duration: %lli s -- Start time: %lli -- End time: %lli\n",
						m_name, s, m_timer.startTime, m_timer.currentTime);
			else if (ms < 1 && us >= 1)
				fprintf(stderr,
						"Profiler [%s] Duration: %lli s %lli %s -- Start time: %lli -- End time: %lli\n",
						m_name, s, us, abrev, m_timer.startTime, m_timer.currentTime);
		} else if (ms >= 1)
		{
			if (us >= 1)
				fprintf(stderr,
						"Profiler [%s] Duration: %lli ms %lli %s -- Start time: %lli -- End time: %lli\n",
						m_name, ms, us, abrev, m_timer.startTime, m_timer.currentTime);
			else
				fprintf(stderr, "Profiler [%s] Duration: %lli ms -- Start time: %lli -- End time: %lli\n",
						m_name, ms, m_timer.startTime, m_timer.currentTime);
		} else if (us >= 1)
		{
			fprintf(stderr, "Profiler [%s] Duration: %lli %s -- Start time: %lli -- End time: %lli\n",
					m_name, us, abrev, m_timer.startTime, m_timer.currentTime);
		}

		m_stopped = true;
	}

	timer_t m_timer;
	char m_name[512];
	bool m_stopped;
};
#endif


#if BMD_PROFILE
	#define PROFILER_START(name) start_profiler_internal(name)
	#define PROFILER_END stop_profiler_internal()
	#define PROFILER_WRAP(functionCall, ...) start_profiler_internal(#functionCall); \
functionCall (__VA_ARGS__);                                                         \
stop_profiler_internal()
	#ifdef __cplusplus
		#define PROFILER_SCOPE(n) Profiler prof_##n(__FUNCTION__ , #n)
	#endif // __cplusplus
#else
	#define PROFILER_START(...)
	#define PROFILER_END
	#define PROFILER_WRAP(...)
#ifdef __cplusplus
	#define PROFILER_SCOPE
#endif // __cplusplus
#endif

#ifdef BMD_HEADERS_ONLY
	#ifndef BMD_PROFILER_IMPL
		#define BMD_PROFILER_IMPL

		#if !BMD_DEBUGGING || !BMD_VERBOSE

			#include <stdio.h>

		#endif

const char* g_name;
timer_t g_timer;

void start_profiler_internal(const char* name)
{
	g_name = name;
	initTimer(&g_timer);
}

void stop_profiler_internal()
{
	tickTimer(&g_timer);
	int64_f s = seconds(&g_timer, g_timer.elapsedTime);
	int64_f ms = milliseconds(&g_timer, g_timer.elapsedTime);
	int64_f us = microseconds(&g_timer, g_timer.elapsedTime);
	char abrev[3] = { (char) 230, 's', 0 };
	if (us >= 1000)
	{
		us %= 1000;
		if (ms >= 1000)
		{
			ms %= 1000;
		}
	}

	if (s >= 1)
	{
		if (ms >= 1 && us < 1)
			fprintf(stderr, "Profiler [%s] Duration: %lli s %lli ms -- Start time: %lli -- End time: %lli\n",
					g_name, s, ms, g_timer.startTime, g_timer.currentTime);
		else if (ms >= 1 && us >= 1)
			fprintf(stderr,
					"Profiler [%s] Duration: %lli s %lli ms %lli %s -- Start time: %lli -- End time: %lli\n",
					g_name, s, ms, us, abrev, g_timer.startTime, g_timer.currentTime);
		else if (ms < 1 && us < 1)
			fprintf(stderr, "Profiler [%s] Duration: %lli s -- Start time: %lli -- End time: %lli\n",
					g_name, s, g_timer.startTime, g_timer.currentTime);
		else if (ms < 1 && us >= 1)
			fprintf(stderr, "Profiler [%s] Duration: %lli s %lli %s -- Start time: %lli -- End time: %lli\n",
					g_name, s, us, abrev, g_timer.startTime, g_timer.currentTime);
	} else if (ms >= 1)
	{
		if (us >= 1)
			fprintf(stderr, "Profiler [%s] Duration: %lli ms %lli %s -- Start time: %lli -- End time: %lli\n",
					g_name, ms, us, abrev, g_timer.startTime, g_timer.currentTime);
		else
			fprintf(stderr, "Profiler [%s] Duration: %lli ms -- Start time: %lli -- End time: %lli\n",
					g_name, ms, g_timer.startTime, g_timer.currentTime);
	} else if (us >= 1)
	{
		fprintf(stderr, "Profiler [%s] Duration: %lli %s -- Start time: %lli -- End time: %lli\n",
				g_name, us, abrev, g_timer.startTime, g_timer.currentTime);
	}


}

	#endif
#endif


#endif //BMD_PROFILER_H
