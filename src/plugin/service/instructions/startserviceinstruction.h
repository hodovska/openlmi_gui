/* ***** BEGIN LICENSE BLOCK *****
 *
 *   Copyright (C) 2013-2014, Martin Hatina <mhatina@redhat.com>
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

#ifndef STARTSERVICEINSTRUCTION_H
#define STARTSERVICEINSTRUCTION_H

#include "instructions/serviceinstruction.h"
#include "lmi_string.h"

/**
 * @brief The StartServiceInstruction class
 *
 * Start service.
 */
class StartServiceInstruction : public ServiceInstruction
{
public:
    /**
     * @brief Constructor
     * @param client -- see CIMClient
     * @param name -- name of service
     */
    StartServiceInstruction(CIMClient *client, String name);
    IInstruction::Subject getSubject();
    String toString();
    void run();
};

#endif // STARTSERVICEINSTRUCTION_H
