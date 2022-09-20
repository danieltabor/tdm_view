/*
 * Copyright (c) 2022, Daniel Tabor
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "settingswidget.h"
#include <QGridLayout>
#include <QIntValidator>
#include "tinyexpr.h"

SettingsWidget::SettingsWidget(QWidget *parent):
    QWidget(parent)
{
    QGridLayout* layout = new QGridLayout(this);
    setLayout(layout);

    m_tsl = new QLabel("Timeslots:",this);
    layout->addWidget(m_tsl,0,0);
    m_tsw = new QSpinBox(this);
    m_tsw->setMinimum(1);
    m_tsw->setMaximum(INT_MAX);
    m_tsw->setValue(32);
    connect(m_tsw,SIGNAL(valueChanged(QString)),this,SLOT(updateHandler()));
    layout->addWidget(m_tsw,0,1);

    m_bptsl = new QLabel("Bits per Timeslot:",this);
    layout->addWidget(m_bptsl,1,0);
    m_bptsw = new QSpinBox(this);
    m_bptsw->setMinimum(1);
    m_bptsw->setMaximum(INT_MAX);
    m_bptsw->setValue(1);
    connect(m_bptsw,SIGNAL(valueChanged(QString)),this,SLOT(updateHandler()));
    layout->addWidget(m_bptsw,1,1);

    m_fpll = new QLabel("Frames per Line:",this);
    layout->addWidget(m_fpll,0,2);
    m_fplw = new QSpinBox(this);
    m_fplw->setMinimum(1);
    m_fplw->setMaximum(INT_MAX);
    m_fplw->setValue(15);
    connect(m_fplw,SIGNAL(valueChanged(QString)),this,SLOT(updateHandler()));
    layout->addWidget(m_fplw,0,3);

    m_offsetl = new QLabel("Offset:",this);
    layout->addWidget(m_offsetl,1,2);
    m_offsetw = new QSpinBox(this);
    m_offsetw->setMinimum(0);
    m_offsetw->setMaximum(INT_MAX);
    m_offsetw->setValue(0);
    connect(m_offsetw,SIGNAL(valueChanged(QString)),this,SLOT(updateHandler()));
    layout->addWidget(m_offsetw,1,3);

    //layout->addWidget(new QLabel("Sync Pattern:",this),0,4);
    //m_syncw = new QLineEdit(this);
    //layout->addWidget(m_syncw,0,5);
    //m_syncv = QString("");

    m_zooml = new QLabel("Zoom",this);
    layout->addWidget(m_zooml,1,4);
    m_zoomw = new QSpinBox(this);
    m_zoomw->setMinimum(1);
    m_zoomw->setMaximum(INT_MAX);
    m_zoomw->setValue(1);
    connect(m_zoomw,SIGNAL(valueChanged(QString)),this,SLOT(updateHandler()));
    layout->addWidget(m_zoomw,1,5);

    m_updateButton = new QPushButton("Update",this);
    layout->addWidget(m_updateButton,1,6);
    connect(m_updateButton,SIGNAL(clicked()),this,SLOT(updateEmit()));
    m_updateButton->setEnabled(false);

    m_autov = false;
}

void SettingsWidget::updateHandler() {
    if( m_autov ) {
        updateEmit();
    }
    else {
        m_updateButton->setEnabled(true);
    }
}

void SettingsWidget::updateEmit() {
    m_updateButton->setEnabled(false);
    emit update();
}

int SettingsWidget::ts() {
    return m_tsw->value();
}


void SettingsWidget::setTs(int ts) {
    if( ts != m_tsw->value() && ts >= m_tsw->minimum() ) {
        m_tsw->setValue(ts);
        emit update();
    }
}

int SettingsWidget::bpts() {
    return m_bptsw->value();
}

void SettingsWidget::setBpts(int bpts) {
    if( bpts != m_bptsw->value() && bpts >= m_bptsw->minimum() ) {
        m_bptsw->setValue(bpts);
        emit update();
    }
}

int SettingsWidget::bpl() {
    return bpts();
}

void SettingsWidget::setBpl(int bpl) {
    setBpts(bpl);
}

int SettingsWidget::fpl() {
    return m_fplw->value();
}

void SettingsWidget::setFpl(int fpl) {
    if( fpl != m_fplw->value() && fpl >= m_fplw->minimum() ) {
        m_fplw->setValue(fpl);
        emit update();
    }
}

int SettingsWidget::offset() {
    return m_offsetw->value();
}

void SettingsWidget::setOffset(int offset) {
    if( offset != m_offsetw->value() && offset >= m_offsetw->minimum() ) {
        m_offsetw->setValue(offset);
        emit update();
    }
}

//QString SettingsWidget::sync() {
//    return m_syncw->value;
//}

int SettingsWidget::zoom() {
    return m_zoomw->value();
}

void SettingsWidget::setZoom(int zoom) {
    if( zoom != m_zoomw->value() && zoom >= m_zoomw->minimum() ) {
        m_zoomw->setValue(zoom);
        emit update();
    }
}

bool SettingsWidget::autoUpdate() {
    return m_autov;
}

void SettingsWidget::setAutoUpdate(bool autoUpdate) {
    m_autov = autoUpdate;
}

void SettingsWidget::setTdmMode() {
    m_tsl->setVisible(true);
    m_tsw->setVisible(true);
    m_fpll->setVisible(true);
    m_fplw->setVisible(true);
    m_bptsl->setText("Bits per Timeslot:");
}


void SettingsWidget::setBinMode() {
    m_tsw->setValue(1);
    m_tsl->setVisible(false);
    m_tsw->setVisible(false);
    m_fplw->setValue(1);
    m_fpll->setVisible(false);
    m_fplw->setVisible(false);
    m_bptsl->setText("Bits per Line:");
    emit update();
}
