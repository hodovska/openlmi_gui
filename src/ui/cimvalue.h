/* ***** BEGIN LICENSE BLOCK *****
 *   Copyright (C) 2013-2014, Peter Hatina <phatina@redhat.com>,
 *                            Martin Hatina <mhatina@redhat.com>
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 2.1 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 * ***** END LICENSE BLOCK ***** */

#ifndef LMIWBEM_VALUE_H
#define LMIWBEM_VALUE_H

#include "lmi_string.h"

#include <Pegasus/Common/CIMValue.h>

/**
 * @brief The CIMValue class
 *
 * Convert CIMValue to better String
 */
class CIMValue
{

public:
    static String decode_values(Pegasus::CIMProperty property);
    /**
     * @brief Convert CIMValue
     * @param value -- value to convert
     * @return String representation of value
     */
    static String to_string(const Pegasus::CIMValue &value);
    static String get_property_value(Pegasus::CIMInstance instance,
                                          String propertyName, Pegasus::CIMProperty *property = NULL);
    static String convert_values(String value, String unit);
    static Pegasus::CIMValue to_cim_value(Pegasus::CIMType type, String value,
                                          bool isArray = false);

};


#endif // LMIWBEM_VALUE_H
