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
 * File Name: debug_info.cpp
 * Date File Created: 9/25/2020 at 11:49 PM
 * Author: Matt
 */
//#include "pch.h" -intellisense works better with force include being used
#include "DebugInfo.h"
#include "DisplayException.h"
#include "Display.h"
#include "GraphicsException.h"


#define GFX_THROW_FAILED(hrcall) \
	if( FAILED( hr = (hrcall) ) ) \
	throw GraphicsException( __LINE__,__FILE__,hr )

DebugInfo::DebugInfo()
{
	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);
	const auto dxgidll = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (!dxgidll)
	{
		throw DISPLAY_LAST_EXCEPTION();
	}

	const auto debugInterface = reinterpret_cast<DXGIGetDebugInterface>(reinterpret_cast<void*>
																		(GetProcAddress(dxgidll,
																		"DXGIGetDebugInterface")));
	if (!debugInterface)
	{
		throw DISPLAY_LAST_EXCEPTION();
	}

	HRESULT hr;
	GFX_THROW_FAILED(debugInterface(__uuidof(IDXGIInfoQueue), &mInfoQueue));
}


void DebugInfo::set() noexcept
{
	mNext = mInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

list<std::string> DebugInfo::getMessages() const
{
	list<std::string> messages;
	const auto end = mInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	for (auto i = mNext; i < end; i++)
	{
		HRESULT hr;
		SIZE_T msgLen;
		GFX_THROW_FAILED(mInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &msgLen));
		auto bytes = createScope<byte[ ]>(msgLen);
		DXGI_INFO_QUEUE_MESSAGE* msg = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
		GFX_THROW_FAILED(mInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, msg, &msgLen));
		messages.emplace_back(msg->pDescription);
	}
	return messages;
}


