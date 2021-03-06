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
 * File Name: pch.h
 * Date File Created: 9/25/2020 at 10:59 PM
 * Author: Matt
 */
#pragma once


// Target Windows 7 and later - for now at least
//#define _WIN32_WINNT 0x0601
//#include <sdkddkver.h>
//#define STRICT
// ^ making these defined on the command line. Being too weird about redefinitions for some reason

#include <bmd/common.h>
#include <bmd/types.h>
#include <bmd/strutil.h>

#include "Common.h"
#include "Log.h"



#include <functional>
#include <exception>
#include <random>
#include <chrono>
#include <string>
#include <sstream>
#include <utility>
#include <memory>
#include <vector>
#include <array>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <variant>
#include <cmath>
#include <iomanip>
#include <queue>
#include <algorithm>

