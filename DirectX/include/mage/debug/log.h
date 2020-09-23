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
 * File Name: log.h
 * Date File Created: 9/16/2020 at 2:29 PM
 * Author: Matt
 */

#ifndef MAGE3DX_LOG_H
#define MAGE3DX_LOG_H

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#define SPDLOG_DEBUG_ON
#define SPDLOG_TRACE_ON

#include "mage/common.h"
#include <spdlog/spdlog.h>

namespace mage
{
    class Log
    {
    public:
        static void init();
        static SharedPtr<spdlog::logger>& getLogger() { return s_logger; }

    private:
        static SharedPtr<spdlog::logger> s_logger;
    };

}


#define LOG_TRACE(...) SPDLOG_LOGGER_TRACE(mage::Log::getLogger(), __VA_ARGS__)
#define LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(mage::Log::getLogger(), __VA_ARGS__)
#define LOG_INFO(...) SPDLOG_LOGGER_INFO(mage::Log::getLogger(), __VA_ARGS__)
#define LOG_WARN(...) SPDLOG_LOGGER_WARN(mage::Log::getLogger(), __VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_LOGGER_ERROR(mage::Log::getLogger(), __VA_ARGS__)
#define LOG_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(mage::Log::getLogger(), __VA_ARGS__)



#endif //MAGE3DX_LOG_H
