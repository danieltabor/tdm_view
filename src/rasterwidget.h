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
#ifndef RASTERWIDGET_H
#define RASTERWIDGET_H

#include <QWidget>
#include <QString>
#include <QBitArray>
#include <QProgressDialog>
#include "capturefile.h"

class RasterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RasterWidget(QWidget *parent = 0);
    void setCaptureFile(CaptureFile* captureFile);
    void setTimeSlots(unsigned int ts);
    void setBitsPerTimeSlot(unsigned int bpts);
    void setFramesPerLine(unsigned int fpl);
    void setFileOffset(unsigned int offset);
    void setZoom(unsigned int zoom);
    void setBitsPerPixels(unsigned int rbpp, unsigned int gbpp, unsigned int bbpp);
    unsigned int horizontalMaximum();
    unsigned int verticalMaximum();

    void saveViewableRaster(QString path, QProgressDialog* dlg = 0);
    void saveHorizontalRaster(QString path, QProgressDialog* dlg = 0);
    void saveVerticalRaster(QString path, QProgressDialog* dlg = 0);
    void saveEntireRaster(QString path, QProgressDialog* dlg = 0);

    void saveHorizontalCSV(QString path, QBitArray *tsIncl = 0, QProgressDialog* dlg = 0);
    void saveEntireCSV(QString path, QBitArray *tsIncl = 0, QProgressDialog* dlg = 0);

    void saveHorizontalTimeSlots(QString path, QBitArray *tsIncl = 0, QProgressDialog* dlg = 0);
    void saveEntireTimeSlots(QString path, QBitArray *tsIncl = 0, QProgressDialog* dlg = 0);

signals:
    void info(QString label,QString data);

public slots:
    void setHorizontalOffset(int offset);
    void setVerticalOffset(int offset);


protected:
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent* event);

private:
    CaptureFile* m_captureFile;
    unsigned int m_ts;      //Number of time slots
    unsigned int m_bpts;    //Bits per time slot
    unsigned int m_fpl;     //Frames per line
    unsigned int m_foffset; //File offset
    unsigned int m_zoom;
    unsigned int m_rbpp;    //Red bits per pixel
    unsigned int m_gbpp;    //Green bits per pixel
    unsigned int m_bbpp;    //Blue bits per pixel
    unsigned int m_hoffset;
    unsigned int m_voffset;

    unsigned int m_totalBitWidth;     //Total bits represented per line
    unsigned int m_tsBitWidth;        //Total bits represented in a single timeslot per line
    unsigned int m_frameBitWidth;     //Total bits that represent a single frame in the file
    unsigned int m_totalBitsPerPixel; //Total number of bits represented by each pixel
    unsigned int m_tsPixelWidth;      //Total pixels needed to represent each timeslot (with 1 for a buffer)
    unsigned int m_totalPixelWidth;   //Total pixels per line
    unsigned int m_totalPixelHeight;  //Total lines

    void calculateSizes();
    void paintRaster(QPaintDevice* target, size_t vOffset, size_t hOffset, size_t zoom, QProgressDialog* dlg = 0);
    void saveCSV(QString path, QBitArray *tsIncl, size_t lineOffset, size_t lineCount, QProgressDialog* dlg = 0);
    void saveTimeSlots(QString path, QBitArray *tsIncl, size_t lineOffset, size_t lineCount, QProgressDialog* dlg = 0);
};

#endif // RASTERWIDGET_H
