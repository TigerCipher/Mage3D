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
 * File Name: callstack.cpp
 * Date File Created: 9/24/2020 at 10:10 AM
 * Author: Matt
 */
//#include "pch.h" -intellisense works better with force include being used
#include "Callstack.h"
#include "3rdParty/StackWalker/StackWalker.h"


class StackWalkerImpl : public StackWalker
{
public:
    explicit StackWalkerImpl(int numDiscard) :
        StackWalker(StackWalker::RetrieveVerbose | StackWalker::SymBuildPath), m_discardEntries(numDiscard)
    { }
    ~StackWalkerImpl() override = default;

    list<Entry> getStack() { return m_stack; }

protected:
    void OnCallstackEntry(CallstackEntryType eType, CallstackEntry& entry) override
    {
        if (!entry.lineFileName[ 0 ]) m_discardEntries = -1;
        if (m_discardEntries > 0) m_discardEntries--;
        else if (m_discardEntries == 0)
        {
            Entry e;
            e.file = entry.lineFileName;
            e.line = entry.lineNumber;
            e.function = entry.name;
            m_stack.push_back(e);
        }
    }

    void OnOutput(LPCSTR szText) override { }
    void OnSymInit(LPCSTR szSearchPath, DWORD symOptions, LPCSTR szUserName) override { }
    void OnLoadModule(LPCSTR img, LPCSTR mod, DWORD64 baseAddr, DWORD size, DWORD result, LPCSTR symType,
                      LPCSTR pdbName, ULONGLONG fileVersion) override
    { }
    void OnDbgHelpErr(LPCSTR szFuncName, DWORD gle, DWORD64 addr) override { }

private:
    list<Entry> m_stack;
    int m_discardEntries;
};

Callstack::Callstack(int numDiscard)
{
    StackWalkerImpl sw(numDiscard);
    sw.ShowCallstack();
    m_stack = sw.getStack();
}
