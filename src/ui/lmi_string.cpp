/* ***** BEGIN LICENSE BLOCK *****
 *
 *   Copyright (C) 2014, Peter Hatina <phatina@redhat.com>
 *                       Martin Hatina <mhatina@redhat.com>
 *
 *   This library is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as
 *   published by the Free Software Foundation, either version 2.1 of the
 *   License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *   MA 02110-1301 USA
 *
 * ***** END LICENSE BLOCK ***** */

#include "lmi_string.h"

String::String() :
    std::string()
{
}

String::String(const char *str) :
    std::string(str)
{
}

String::String(const char *str, size_t pos, size_t len) :
    std::string(str, pos, len)
{
}

String::String(const std::string &str) :
    std::string(str)
{
}

String::String(const Pegasus::String &str) :
    std::string(str.getCString())
{
}

String::String(const QString &str) :
    std::string(str.toStdString())
{
}

const char *String::asConstChar() const
{
    return this->c_str();
}

std::string String::asStdString() const
{
    return *this;
}

Pegasus::String String::asPegasusString() const
{
    return Pegasus::String(c_str());
}

QString String::asQString() const
{
    return QString(c_str());
}

String::operator const char *() const
{
    return asConstChar();
}

String::operator Pegasus::String() const
{
    return asPegasusString();
}

String::operator QString() const
{
    return asQString();
}

std::ostream &operator<<(std::ostream &o, const String &str)
{
    o.write(str.data(), str.size());
    return o;
}

String &String::operator=(const char *rhs)
{
    std::string::operator=(rhs);
    return *this;
}

String &String::operator=(const String &rhs)
{
    std::string::operator=(rhs);
    return *this;
}

String &String::operator=(const std::string &rhs)
{
    std::string::operator=(rhs);
    return *this;
}

String &String::operator=(const Pegasus::String &rhs)
{
    std::string::operator=(rhs.getCString());
    return *this;
}

String &String::operator=(const QString &rhs)
{
    std::string::operator=(rhs.toStdString());
    return *this;
}

String &String::operator+=(const char *rhs)
{
    std::string::operator+=(rhs);
    return *this;
}

String &String::operator+=(const Pegasus::String &rhs)
{
    std::string::operator+=(rhs.getCString());
    return *this;
}

String &String::operator+=(const QString &rhs)
{
    std::string::operator+=(rhs.toStdString());
    return *this;
}

String &String::operator+=(const String &rhs)
{
    std::string::operator+=(rhs);
    return *this;
}

String &String::operator+(const char *rhs)
{
    return operator +=(rhs);
}

String &String::operator+(const String &rhs)
{
    return operator +=(rhs);
}

String &String::operator+(const std::string &rhs)
{
    return operator +=(rhs);
}

String &String::operator+(const Pegasus::String &rhs)
{
    return operator +=(rhs);
}

String &String::operator+(const QString &rhs)
{
    return operator +=(rhs);
}

String &operator+(const char *rhs, String &str)
{
    str.insert(0, rhs);
    return str;
}

String &operator+(const std::string &rhs, String &str)
{
    str.insert(0, rhs);
    return str;
}

String &operator+(const Pegasus::String &rhs, String &str)
{
    str.insert(0, rhs.getCString());
    return str;
}

String &operator+(const QString &rhs, String &str)
{
    str.insert(0, rhs.toStdString());
    return str;
}
