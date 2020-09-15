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
 * File Name: mageexception.h
 * Date File Created: 9/12/2020 at 3:01 PM
 * Author: Matt
 */

#ifndef MAGE3DX_MAGEEXCEPTION_H
#define MAGE3DX_MAGEEXCEPTION_H



namespace mage
{
    class MageException : public std::exception
    {
    public:
        MageException(int line, const char* file) noexcept : m_line(line), m_file(file) {}
        [[nodiscard]] const char* what() const noexcept override;
        virtual const char* getType() const noexcept;
        inline int getLine() const noexcept { return m_line; }
        inline const std::string& getFile() const noexcept { return m_file; }
        std::string getOrigin() const noexcept;
    protected:
        mutable std::string m_what;
    private:
        int m_line;
        std::string m_file;
    };

}


#endif //MAGE3DX_MAGEEXCEPTION_H
