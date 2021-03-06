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

#include "labeledlineedit.h"
#include "logger.h"
#include "ui_labeledlineedit.h"

LabeledLineEdit::LabeledLineEdit(String objectName, String label,
                                 String text, bool key) :
    QWidget(0),
    m_text(text),
    m_ui(new Ui::LabeledLineEdit)
{
    Logger::getInstance()->debug("LabeledLineEdit::LabeledLineEdit(String objectName, String label, String text, bool key)");
    m_ui->setupUi(this);
    setLabelText(label);
    setText(text);
    if (key) {
        m_ui->lineEdit->setReadOnly(true);
        m_ui->lineEdit->setStyleSheet("QLineEdit{background: pink;}");
    }


    setObjectName(objectName.c_str());

    connect(
        m_ui->lineEdit,
        SIGNAL(textChanged(QString)),
        this,
        SLOT(textChanged()));
    connect(
        m_ui->lineEdit,
        SIGNAL(editingFinished()),
        this,
        SLOT(itemChanged()));
}

LabeledLineEdit::~LabeledLineEdit()
{
    Logger::getInstance()->debug("LabeledLineEdit::~LabeledLineEdit()");
    delete m_ui;
}

int LabeledLineEdit::getLabelWidth()
{
    Logger::getInstance()->debug("LabeledLineEdit::getLabelWidth()");
    return m_ui->label->fontMetrics().boundingRect(m_ui->label->text()).width();
}

String LabeledLineEdit::getLabelText()
{
    Logger::getInstance()->debug("LabeledLineEdit::getLabelText()");
    return m_ui->label->text();
}

String LabeledLineEdit::getObjectName()
{
    Logger::getInstance()->debug("LabeledLineEdit::getObjectName()");
    return objectName();
}

String LabeledLineEdit::getText()
{
    Logger::getInstance()->debug("LabeledLineEdit::getText()");
    return m_ui->lineEdit->text();
}

void LabeledLineEdit::setAlignment(int alignment)
{
    Logger::getInstance()->debug("LabeledLineEdit::setAlignment(int alignment)");
    int tmp = m_ui->label->fontMetrics().boundingRect(m_ui->label->text()).width();
    if (alignment - tmp > 0) {
        m_ui->label->setMinimumWidth(alignment);
    }
}

void LabeledLineEdit::setLabelText(String text)
{
    Logger::getInstance()->debug("LabeledLineEdit::setLabelText(String text)");
    m_ui->label->setText(text);
}

void LabeledLineEdit::setReadOnly(bool state)
{
    Logger::getInstance()->debug("LabeledLineEdit::setReadOnly(bool state)");
    m_ui->lineEdit->setReadOnly(state);
}

void LabeledLineEdit::setText(String text)
{
    Logger::getInstance()->debug("LabeledLineEdit::setText(String text)");
    m_text = text;
    m_ui->lineEdit->setText(text.c_str());
}

void LabeledLineEdit::itemChanged()
{
    Logger::getInstance()->debug("LabeledLineEdit::itemChanged()");
    if (String(m_ui->lineEdit->text()) != m_text) {
        emit itemChanged(this);
    }
}

void LabeledLineEdit::textChanged()
{
    Logger::getInstance()->debug("LabeledLineEdit::textChanged()");
    m_ui->lineEdit->setStyleSheet("QLineEdit{background: yellow;}");
}
