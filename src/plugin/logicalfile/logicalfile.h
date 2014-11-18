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

#ifndef LOGICALFILE_H
#define LOGICALFILE_H

#include "plugin.h"

#include <QtPlugin>

namespace Ui
{
class LogicalFilePlugin;
}

class LogicalFilePlugin : public Engine::IPlugin
{
    Q_OBJECT
    Q_INTERFACES(Engine::IPlugin)

private:
    bool m_changes_enabled;
    int m_file_cnt;
    std::vector<Pegasus::CIMInstance> m_dirs;
    Ui::LogicalFilePlugin *m_ui;

    void getInfo(std::vector<Pegasus::CIMInstance> *data, std::string name, bool dir = true);
    void writeInfo(std::vector<Pegasus::CIMInstance> *data, std::string name);

public:
    explicit LogicalFilePlugin();
    ~LogicalFilePlugin();
    virtual std::string getInstructionText();
    virtual std::string getLabel();
    virtual std::string getRefreshInfo();
    virtual void clear();
    virtual void fillTab(std::vector<void *> *data);
    virtual void getData(std::vector<void *> *data);

private slots:
    void dirClicked(QTreeWidgetItem *item);
    void dirExpanded(QTreeWidgetItem *item);

signals:
    void writeData(std::vector<Pegasus::CIMInstance> *data);

};

#endif // LOGICALFILE_H
