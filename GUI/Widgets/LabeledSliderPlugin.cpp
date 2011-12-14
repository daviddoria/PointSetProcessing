/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include "LabeledSlider.h"
#include "LabeledSliderPlugin.h"

#include <QtPlugin>

LabeledSliderPlugin::LabeledSliderPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void LabeledSliderPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;

    initialized = true;
}

bool LabeledSliderPlugin::isInitialized() const
{
    return initialized;
}

QWidget *LabeledSliderPlugin::createWidget(QWidget *parent)
{
    return new LabeledSlider(parent);
}

QString LabeledSliderPlugin::name() const
{
    return "LabeledSlider";
}

QString LabeledSliderPlugin::group() const
{
    return "Custom Widgets";
}

QIcon LabeledSliderPlugin::icon() const
{
    return QIcon();
}

QString LabeledSliderPlugin::toolTip() const
{
    return "";
}

QString LabeledSliderPlugin::whatsThis() const
{
    return "";
}

bool LabeledSliderPlugin::isContainer() const
{
    return false;
}

QString LabeledSliderPlugin::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"LabeledSlider\" name=\"labeledSliderPlugin\">\n"

           "  <property name=\"geometry\">\n"
           "   <rect>\n"
           "    <x>0</x>\n"
           "    <y>0</y>\n"
           "    <width>100</width>\n"
           "    <height>100</height>\n"
           "   </rect>\n"
           "  </property>\n"

           "  <property name=\"toolTip\" >\n"
           "   <string>Labeled slider</string>\n"
           "  </property>\n"
           "  <property name=\"whatsThis\" >\n"
           "   <string>The LabeledSlider widget takes the responsibility of diplaying the current value in the label.</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}

QString LabeledSliderPlugin::includeFile() const
{
    return "LabeledSlider.h";
}

Q_EXPORT_PLUGIN2(LabeledSliderPlugin, LabeledSliderPlugin)
