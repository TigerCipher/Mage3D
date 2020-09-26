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
 * File Name: winwrapper.h
 * Date File Created: 9/11/2020 at 3:00 PM
 * Author: Matt
 */

#ifndef MAGE3DX_WINWRAPPER_H
#define MAGE3DX_WINWRAPPER_H

//// target Windows 7 or later
//#define _WIN32_WINNT 0x0601
#include <sdkddkver.h>

#ifndef FULL_WINTARD
#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMEMMGR
#define NOMETAFILE
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE
#endif

#define NOMINMAX


#include <Windows.h>

#include <wrl.h>

template<typename T>
using COMPtr = Microsoft::WRL::ComPtr<T>;

template<typename T>
using COMptr = Microsoft::WRL::ComPtr<T>;

template<typename T>
using comptr = Microsoft::WRL::ComPtr<T>;

#include <memory>

template<typename T, typename... Args>
constexpr COMptr<T> createCOM(Args&& ...args)
{
    return Microsoft::WRL::Make<T>(std::forward<Args>(args)...);
}

#endif //MAGE3DX_WINWRAPPER_H
