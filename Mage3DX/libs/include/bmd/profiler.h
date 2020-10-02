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
#include "strutil.h"

#ifdef __cplusplus
extern "C" {
#endif

void start_profiler_internal(const char* name);
void stop_profiler_internal();

#ifdef __cplusplus
};
#endif

#ifdef __cplusplus


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
		double us = microseconds_f(&m_timer, m_timer.elapsedTime);
		double count = us;
		char abrev[3] = { (char) 230, 's', 0 };
		if (us >= 10000)
		{
			copyStr(abrev, "ms");
			count = us / 1000.0;
			if (count >= 5000)
			{
				copyStr(abrev, "s");
				count = count / 1000.0;
			}
		}

		fprintf(stderr, "Profiler [%s] Duration: %f %s -- Start time: %lli -- End time: %lli\n",
				m_name, count, abrev, m_timer.startTime, m_timer.currentTime);

		m_stopped = true;
	}

	timer_t m_timer{};
	char m_name[512]{};
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
	double us = microseconds_f(&g_timer, g_timer.elapsedTime);
	double count = us;
	char abrev[3] = { (char) 230, 's', 0 };
	if (us >= 10000)
	{
		copyStr(abrev, "ms");
		count = us / 1000.0;
		if (count >= 5000)
		{
			copyStr(abrev, "s");
			count = count / 1000.0;
		}
	}

	fprintf(stderr, "Profiler [%s] Duration: %f %s -- Start time: %lli -- End time: %lli\n",
			g_name, count, abrev, g_timer.startTime, g_timer.currentTime);

}

	#endif
#endif


#endif //BMD_PROFILER_H
