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
#include "centralwidget.h"
#include <QBoxLayout>
#include <QGridLayout>
#include <QTabWidget>
#include <QWheelEvent>
#include <QPoint>

CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent)
{
    QBoxLayout* mainLayout = new QBoxLayout(QBoxLayout::TopToBottom,this);
    setLayout(mainLayout);

    m_settings = new SettingsWidget(this);
    connect(m_settings,SIGNAL(update()),this,SLOT(newSettings()));
    mainLayout->addWidget(m_settings,0);

    QGridLayout* rasterLayout = new QGridLayout();
    m_raster = new RasterWidget(this);
    m_vscroll = new QScrollBar(Qt::Vertical,this);
    connect(m_vscroll,SIGNAL(valueChanged(int)),m_raster,SLOT(setVerticalOffset(int)));
    m_hscroll = new QScrollBar(Qt::Horizontal,this);
    connect(m_hscroll,SIGNAL(valueChanged(int)),m_raster,SLOT(setHorizontalOffset(int)));
    rasterLayout->addWidget(m_raster,0,0);
    rasterLayout->addWidget(m_vscroll,0,1);
    rasterLayout->addWidget(m_hscroll,1,0);
    m_hscroll->setRange(0,100);
    m_vscroll->setRange(0,100);
    mainLayout->addLayout(rasterLayout,1);

    m_captureFile = 0;
    newSettings();
}

void CentralWidget::setCaptureFile(CaptureFile* captureFile) {
    m_captureFile = captureFile;
    m_raster->setCaptureFile(captureFile);
    calcSizes();
}

RasterWidget* CentralWidget::raster() {
    return m_raster;
}

SettingsWidget* CentralWidget::settings() {
    return m_settings;
}

void CentralWidget::newSettings() {
    m_raster->setTimeSlots(m_settings->ts());
    m_raster->setBitsPerTimeSlot(m_settings->bpts());
    m_raster->setFramesPerLine(m_settings->fpl());
    m_raster->setFileOffset(m_settings->offset());
    m_raster->setZoom(m_settings->zoom());
    m_raster->setBitsPerPixels(m_settings->rbpp(), m_settings->gbpp(), m_settings->bbpp() );
    calcSizes();
}

void CentralWidget::wheelEvent(QWheelEvent* event) {
    int delta = event->angleDelta().y();
    int zoom = m_settings->zoom();
    if( delta > 0 ) { zoom = zoom + 1; }
    else if( delta < 0 ) { zoom = zoom - 1; }
    if( zoom < 1 ) { zoom = 1; }
    m_settings->setZoom(zoom);
}

void CentralWidget::calcSizes() {
    double old_vscroll_ratio = 0.0;
    double old_hscroll_ratio = 0.0;
    if( m_captureFile == 0 ) {
        m_vscroll->setValue(0);
        m_vscroll->setRange(0,0);
        m_hscroll->setValue(0);
        m_hscroll->setRange(0,0);
    }
    else {
        size_t vmax = m_raster->verticalMaximum();
        size_t hmax = m_raster->horizontalMaximum();
        if( m_vscroll->maximum() ) {
            old_vscroll_ratio = (double)m_vscroll->value() / (double)m_vscroll->maximum();
        }
        m_vscroll->setRange(0,vmax);
        m_vscroll->setValue(old_vscroll_ratio*vmax);
        if( m_hscroll->maximum() ) {
            old_hscroll_ratio = (double)m_hscroll->value() / (double)m_hscroll->maximum();
        }
        m_hscroll->setRange(0,hmax);
        m_hscroll->setValue(old_hscroll_ratio*hmax);
    }
}
