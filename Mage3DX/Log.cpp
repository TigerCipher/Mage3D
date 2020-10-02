/*
 * Mage3DX
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
 * File Name: log.cpp
 * Date File Created: 9/16/2020 at 2:29 PM
 * Author: Matt
 */
#include "pch.h"
#include "Log.h"
//#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>


SharedPtr<spdlog::logger> mage::Log::s_logger;

void mage::Log::init()
{
    list<spdlog::sink_ptr> logSinks;
    //logSinks.emplace_back(createRef<spdlog::sinks::stdout_color_sink_mt>());
    logSinks.emplace_back(createRef<spdlog::sinks::basic_file_sink_mt>("mage3dx.log", true));

    //logSinks[0]->set_pattern("%^[%T] [%l] %n: %v%$");
    logSinks[0]->set_pattern("[%m-%d-%Y %H:%M:%S] [%l] [%g:%#] [%!] %n: %v");

    s_logger = createRef<spdlog::logger>("Mage3D", begin(logSinks), end(logSinks));
    spdlog::register_logger(s_logger);
    s_logger->set_level(spdlog::level::trace);
    s_logger->flush_on(spdlog::level::trace);
}
