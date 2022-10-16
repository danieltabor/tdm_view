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
#include <QColor>
#include "tinyexpr.h"

SettingsWidget::SettingsWidget(QWidget *parent):
    QWidget(parent)
{
    QGridLayout* layout = new QGridLayout(this);
    setLayout(layout);

    m_tsl = new QLabel("Timeslots:",this);
    layout->addWidget(m_tsl,0,0);
    m_tsw = new QLineEdit(this);
    m_ts = 32;
    m_tsw->setText("32");
    connect(m_tsw,SIGNAL(textChanged(QString)),this,SLOT(updateHandler()));
    connect(m_tsw,SIGNAL(returnPressed()),this,SLOT(updateEmit()));
    layout->addWidget(m_tsw,0,1);

    m_bptsl = new QLabel("Bits per Timeslot:",this);
    layout->addWidget(m_bptsl,1,0);
    m_bptsw = new QLineEdit(this);
    m_bpts = 1;
    m_bptsw->setText("1");
    connect(m_bptsw,SIGNAL(textChanged(QString)),this,SLOT(updateHandler()));
    connect(m_bptsw,SIGNAL(returnPressed()),this,SLOT(updateEmit()));
    layout->addWidget(m_bptsw,1,1);

    m_fpll = new QLabel("Frames per Line:",this);
    layout->addWidget(m_fpll,0,2);
    m_fplw = new QLineEdit(this);
    m_fpl = 15;
    m_fplw->setText("15");
    connect(m_fplw,SIGNAL(textChanged(QString)),this,SLOT(updateHandler()));
    connect(m_fplw,SIGNAL(returnPressed()),this,SLOT(updateEmit()));
    layout->addWidget(m_fplw,0,3);

    m_offsetl = new QLabel("Offset:",this);
    layout->addWidget(m_offsetl,1,2);
    m_offsetw = new QLineEdit(this);
    m_offset = 0;
    m_offsetw->setText("0");
    connect(m_offsetw,SIGNAL(textChanged(QString)),this,SLOT(updateHandler()));
    connect(m_offsetw,SIGNAL(returnPressed()),this,SLOT(updateEmit()));
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

    m_rbppl = new QLabel("Red bits per pixel:");
    m_rbppl->setVisible(false);
    layout->addWidget(m_rbppl,2,0);
    m_rbppw = new QSpinBox(this);
    m_rbppw->setVisible(false);
    m_rbppw->setValue(false);
    m_rbppw->setMinimum(0);
    m_rbppw->setMaximum(32);
    m_rbppw->setValue(0);
    m_rbpp = 0;
    connect(m_rbppw,SIGNAL(valueChanged(QString)),this,SLOT(updateHandler()));
    layout->addWidget(m_rbppw,2,1);

    m_gbppl = new QLabel("Green bits per pixel:");
    m_gbppl->setVisible(false);
    layout->addWidget(m_gbppl,2,2);
    m_gbppw = new QSpinBox(this);
    m_gbppw->setVisible(false);
    m_gbppw->setMinimum(0);
    m_gbppw->setMaximum(32);
    m_gbppw->setValue(1);
    m_gbpp = 1;
    connect(m_gbppw,SIGNAL(valueChanged(QString)),this,SLOT(updateHandler()));
    layout->addWidget(m_gbppw,2,3);

    m_bbppl = new QLabel("Blue bits per pixel:");
    m_bbppl->setVisible(false);
    layout->addWidget(m_bbppl,2,4);
    m_bbppw = new QSpinBox(this);
    m_bbppw->setVisible(false);
    m_bbppw->setMinimum(0);
    m_bbppw->setMaximum(32);
    m_bbppw->setValue(0);
    m_bbpp = 0;
    connect(m_bbppw,SIGNAL(valueChanged(QString)),this,SLOT(updateHandler()));
    layout->addWidget(m_bbppw,2,5);

    m_updateButton = new QPushButton("Update",this);
    layout->addWidget(m_updateButton,1,6);
    connect(m_updateButton,SIGNAL(clicked()),this,SLOT(updateEmit()));
    m_updateButton->setEnabled(false);

    m_autov = false;

    m_okpal = new QPalette(m_tsw->palette());
    m_errpal = new QPalette(m_tsw->palette());
    m_errpal->setColor(QPalette::Text,Qt::red);

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
    int error;
    double result;

    result = te_interp(m_tsw->text().toStdString().c_str(), &error);
    if( ! error && result >= 1.0 ) {
        m_ts = (int)result;
        m_tsw->setPalette(*m_okpal);
    } else {
        m_tsw->setPalette(*m_errpal);
    }

    result = te_interp(m_bptsw->text().toStdString().c_str(), &error);
    if( ! error && result >= 1.0 ) {
        m_bpts = (int)result;
        m_bptsw->setPalette(*m_okpal);
    } else {
        m_bptsw->setPalette(*m_errpal);
    }

    result = te_interp(m_fplw->text().toStdString().c_str(), &error);
    if( ! error && result >= 1.0 ) {
        m_fpl = (int)result;
        m_fplw->setPalette(*m_okpal);
    } else {
        m_fplw->setPalette(*m_errpal);
    }

    result = te_interp(m_offsetw->text().toStdString().c_str(), &error);
    if( ! error && result >= 0.0 ) {
        m_offset = (int)result;
        m_offsetw->setPalette(*m_okpal);
    } else {
        m_offsetw->setPalette(*m_errpal);
    }

    if( m_rbppw->value() || m_gbppw->value() || m_bbppw->value() ) {
        m_rbpp = m_rbppw->value();
        m_rbppw->setPalette(*m_okpal);
        m_gbpp = m_gbppw->value();
        m_gbppw->setPalette(*m_okpal);
        m_bbpp = m_bbppw->value();
        m_bbppw->setPalette(*m_okpal);
    } else {
        m_rbppw->setPalette(*m_errpal);
        m_gbppw->setPalette(*m_errpal);
        m_bbppw->setPalette(*m_errpal);
    }

    m_updateButton->setEnabled(false);
    emit update();
}

int SettingsWidget::ts() {
    return m_ts;
}


void SettingsWidget::setTs(int ts) {
    if( ts >= 1 ) {
        m_tsw->setText(QString::number(ts));
        if( ts != m_ts ) {
            updateEmit();
        }
    }
}

int SettingsWidget::bpts() {
    return m_bpts;
}

void SettingsWidget::setBpts(int bpts) {
    if( bpts >= 1 ) {
        m_bptsw->setText(QString::number(bpts));
        if( bpts != m_bpts ) {
            updateEmit();
        }
    }
}

int SettingsWidget::bpl() {
    return bpts();
}

void SettingsWidget::setBpl(int bpl) {
    setBpts(bpl);
}

int SettingsWidget::fpl() {
    return m_fpl;
}

void SettingsWidget::setFpl(int fpl) {
    if( fpl >= 1 ) {
        m_fplw->setText(QString::number(fpl));
        if( fpl != m_fpl ) {
            updateEmit();
        }
    }
}

int SettingsWidget::offset() {
    return m_offset;
}

void SettingsWidget::setOffset(int offset) {
    if( offset >= 0 ) {
        m_offsetw->setText(QString::number(offset));
        if( offset != m_offset ) {
            updateEmit();
        }
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

int SettingsWidget::rbpp() {
    return m_rbpp;
}

void SettingsWidget::setRbpp(int rbpp) {
    if( rbpp != m_rbpp && rbpp >= 0 ) {
        m_rbppw->setValue(rbpp);
        updateEmit();
    }
}

int SettingsWidget::gbpp() {
    return m_gbpp;
}

void SettingsWidget::setGbpp(int gbpp) {
    if( gbpp != m_gbpp && gbpp >= 0 ) {
        m_gbppw->setValue(gbpp);
        updateEmit();
    }
}

int SettingsWidget::bbpp() {
    return m_bbpp;
}

void SettingsWidget::setBbpp(int bbpp) {
    if( bbpp != m_bbpp && bbpp >= 0 ) {
        m_bbppw->setValue(bbpp);
        updateEmit();
    }
}

bool SettingsWidget::autoUpdate() {
    return m_autov;
}

void SettingsWidget::setAutoUpdate(bool autoUpdate) {
    m_autov = autoUpdate;
}

void SettingsWidget::setEnableColors(bool enableColors) {
    m_rbppl->setVisible(enableColors);
    m_rbppw->setVisible(enableColors);
    m_gbppl->setVisible(enableColors);
    m_gbppw->setVisible(enableColors);
    m_bbppl->setVisible(enableColors);
    m_bbppw->setVisible(enableColors);
    if( ! enableColors && (m_rbpp != 0 || m_gbpp != 1 || m_bbpp != 0) ) {
        m_rbpp = 0;
        m_gbpp = 1;
        m_bbpp = 0;
        emit update();
    }
}

void SettingsWidget::setTdmMode() {
    m_tsl->setVisible(true);
    m_tsw->setVisible(true);
    m_fpll->setVisible(true);
    m_fplw->setVisible(true);
    m_bptsl->setText("Bits per Timeslot:");
    emit update();
}


void SettingsWidget::setBinMode() {
    m_ts = 1;
    m_tsw->setText(QString::number(m_ts));
    m_tsl->setVisible(false);
    m_tsw->setVisible(false);
    m_fpl = 1;
    m_fplw->setText(QString::number(m_ts));
    m_fpll->setVisible(false);
    m_fplw->setVisible(false);
    m_bptsl->setText("Bits per Line:");
    emit update();
}

/*
void SettingsWidget::setRGBMode() {
    m_ts = 3;
    m_tsw->setText(QString::number(m_ts));

}
*/

