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
 * File Name: stacktraceexception.h
 * Date File Created: 9/24/2020 at 10:23 AM
 * Author: Matt
 */

#ifndef MAGE3DX_STACKTRACEEXCEPTION_H
#define MAGE3DX_STACKTRACEEXCEPTION_H

#include "callstack.h"
#include <stdexcept>

namespace mage
{
    class StacktraceExceptionBase : public Callstack
    {
    public:
        explicit StacktraceExceptionBase(bool showStack) : Callstack(2), m_showStack(showStack) { }

        [[nodiscard]] virtual const char* what() const noexcept = 0;

        [[nodiscard]] bool shouldShowStack() const { return m_showStack; }

    protected:
        bool m_showStack;
    };

    template<class T>
    class StacktraceException : public T, public StacktraceExceptionBase
    {
    public:
        explicit StacktraceException(const std::string& msg) : T(msg), StacktraceExceptionBase(true) { }
        const char* what() const noexcept override
        {
            if (m_showStack)
            {
                m_buffer = fmt::format("[{}]\n{}", std::string(T::what()), asString());
                return m_buffer.c_str();
            }
            return T::what();
        }

    protected:
        mutable std::string m_buffer;
    };

    typedef StacktraceException<std::runtime_error> stacktraceRuntimeError;
    typedef StacktraceException<std::range_error> stacktraceRangeError;
    typedef StacktraceException<std::overflow_error> stacktraceOverflowError;
    typedef StacktraceException<std::underflow_error> stacktraceUnderflowError;
    typedef StacktraceException<std::logic_error> stacktraceLogicError;
    typedef StacktraceException<std::domain_error> stacktraceDomainError;
    typedef StacktraceException<std::invalid_argument> stacktraceInvalidArgument;
    typedef StacktraceException<std::length_error> stacktraceLengthError;
    typedef StacktraceException<std::out_of_range> stacktraceOutOfRange;

    //typedef StacktraceException<GraphicsException> stacktraceGfx;
    //typedef StacktraceException<GraphicsDeviceRemovedException> stacktraceGfxDeviceRemoved;
    //typedef StacktraceException<TextureException> stacktraceTexture;
    //typedef StacktraceException<DisplayException> stacktraceDisplay;
    //typedef StacktraceException<NoGraphicsException> stacktraceNoGfx;
    //typedef StacktraceException<InfoException> stacktraceInfo;


}// namespace mage


#endif//MAGE3DX_STACKTRACEEXCEPTION_H
