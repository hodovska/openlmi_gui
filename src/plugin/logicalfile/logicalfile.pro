# ***** BEGIN LICENSE BLOCK *****
#
#   Copyright (C) 2013-2014, Martin Hatina <mhatina@redhat.com>
#
#   This library is free software; you can redistribute it and/or modify
#   it under the terms of the GNU Lesser General Public License as
#   published by the Free Software Foundation, either version 2.1 of the
#   License, or (at your option) any later version.
#
#   This library is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#   GNU Lesser General Public License for more details.
#
#   You should have received a copy of the GNU Lesser General Public
#   License along with this program; if not, write to the Free Software
#   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#   MA 02110-1301 USA
#
# ***** END LICENSE BLOCK ***** */

QT              +=      core gui

include(../../../project.pri)

TEMPLATE        =       lib
CONFIG          +=      plugin
INCLUDEPATH     +=      ../../ui ../../ui/uics ../../logger

TARGET          =       $$qtLibraryTarget(lmicclogicalfile)
DESTDIR         =       ../libs

UI_DIR          =       uics
MOC_DIR         =       mocs
OBJECTS_DIR     =       objs

CONFIG(debug, debug|release) {
    target.path =       ../libs
    LIBS        +=      -L../../logger -llmicclogger
} else {
    target.path =       $$PREFIX$$LIB_PATH/lmicc
    manual_install {
        LIBS    +=      -L/usr/lib/lmicc -llmicclogger
    } else {
        LIBS    +=      -llmicclogger
    }
}
INSTALLS        +=      target

HEADERS         +=      logicalfile.h \
                        ../../ui/plugin.h \
                        ../../ui/instructions/connectinstruction.h \
                        ../../ui/instructions/instruction.h \
                        filetree.h \
                        filebrowser.h

SOURCES         +=      logicalfile.cpp \
                        ../../ui/plugin.cpp \
                        ../../ui/instructions/connectinstruction.cpp \
                        ../../ui/instructions/instruction.cpp \
                        ../../ui/cimvalue.cpp \
                        ../../ui/cimdatetimeconv.cpp \
                        filetree.cpp \
                        filebrowser.cpp

FORMS           +=      logicalfile.ui

RESOURCES       +=      ../../../icons/icons.qrc
