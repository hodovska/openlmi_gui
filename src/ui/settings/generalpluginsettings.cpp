/* ***** BEGIN LICENSE BLOCK *****
 *   Copyright (C) 2013-2014, Martin Hatina <mhatina@redhat.com>
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

#include "generalpluginsettings.h"
#include "logger.h"
#include "ui_generalpluginsettings.h"

GeneralPluginSettings::GeneralPluginSettings(QWidget *parent) :
    ISettings(parent),
    m_ui(new Ui::GeneralPluginSettings)
{
    m_ui->setupUi(this);
}

GeneralPluginSettings::~GeneralPluginSettings()
{
    if (m_ui != NULL) {
            delete m_ui;
            m_ui = NULL;
        }
}

std::string GeneralPluginSettings::title()
{
    return "General plugin";
}

void GeneralPluginSettings::init()
{
    m_ui->overview->setChecked(true);
    m_ui->account->setChecked(true);
    m_ui->hardware->setChecked(true);
    m_ui->network->setChecked(true);
    m_ui->service->setChecked(true);
    m_ui->software->setChecked(true);
}

void GeneralPluginSettings::load(QFile &file)
{
    bool found = false;
    QXmlStreamReader in(&file);
    if (in.hasError()) {
        Logger::getInstance()->error(in.errorString().toStdString(), false);
        return;
    }

    std::string t = title();
    t.erase(remove_if(t.begin(), t.end(), isspace), t.end());

    while (!in.atEnd()) {
        QXmlStreamReader::TokenType token = in.readNext();

        if (in.name().toString().toStdString().empty() ||
            in.isEndElement()) {
            continue;
        }

        if (token == QXmlStreamReader::StartDocument) {
            continue;
        } else if (in.name().toString().toStdString() == t) {
            found = true;
        } else if (in.name() == "overviewAutoRefresh") {
            QXmlStreamAttributes attr = in.attributes();
            bool enabled = (attr.value("enabled").toString().toStdString() == "true");
            m_ui->overview->setChecked(enabled);
        } else if (in.name() == "accountAutoRefresh") {
            QXmlStreamAttributes attr = in.attributes();
            bool enabled = (attr.value("enabled").toString().toStdString() == "true");
            m_ui->account->setChecked(enabled);
        } else if (in.name() == "hardwareAutoRefresh") {
            QXmlStreamAttributes attr = in.attributes();
            bool enabled = (attr.value("enabled").toString().toStdString() == "true");
            m_ui->hardware->setChecked(enabled);
        } else if (in.name() == "networkAutoRefresh") {
            QXmlStreamAttributes attr = in.attributes();
            bool enabled = (attr.value("enabled").toString().toStdString() == "true");
            m_ui->network->setChecked(enabled);
        } else if (in.name() == "serviceAutoRefresh") {
            QXmlStreamAttributes attr = in.attributes();
            bool enabled = (attr.value("enabled").toString().toStdString() == "true");
            m_ui->service->setChecked(enabled);
        } else if (in.name() == "softwareAutoRefresh") {
            QXmlStreamAttributes attr = in.attributes();
            bool enabled = (attr.value("enabled").toString().toStdString() == "true");
            m_ui->software->setChecked(enabled);
        }
    }

    if (in.hasError()) {
        Logger::getInstance()->error(in.errorString().toStdString(), false);
        return;
    }

    if (!found) {
        init();
    }
}

void GeneralPluginSettings::save(QXmlStreamWriter &writer)
{
    writer.writeStartElement("overviewAutoRefresh");
    writer.writeAttribute("enabled",
                          m_ui->overview->checkState() == Qt::Checked ? "true" : "false");
    writer.writeEndElement();

    writer.writeStartElement("accountAutoRefresh");
    writer.writeAttribute("enabled",
                          m_ui->account->checkState() == Qt::Checked ? "true" : "false");
    writer.writeEndElement();

    writer.writeStartElement("hardwareAutoRefresh");
    writer.writeAttribute("enabled",
                          m_ui->hardware->checkState() == Qt::Checked ? "true" : "false");
    writer.writeEndElement();

    writer.writeStartElement("networkAutoRefresh");
    writer.writeAttribute("enabled",
                          m_ui->network->checkState() == Qt::Checked ? "true" : "false");
    writer.writeEndElement();

    writer.writeStartElement("serviceAutoRefresh");
    writer.writeAttribute("enabled",
                          m_ui->service->checkState() == Qt::Checked ? "true" : "false");
    writer.writeEndElement();

    writer.writeStartElement("softwareAutoRefresh");
    writer.writeAttribute("enabled",
                          m_ui->software->checkState() == Qt::Checked ? "true" : "false");
    writer.writeEndElement();
}
