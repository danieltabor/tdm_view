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
#include "rasterwidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QImage>
#include <QTextStream>
#include <QDebug>
#include <QApplication>

RasterWidget::RasterWidget(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);

    m_ts = 1;
    m_bpts = 1;
    m_fpl = 1;
    m_foffset = 0;
    m_zoom = 1;
    m_hoffset = 0;
    m_voffset = 0;
    m_captureFile = 0;
    calculateSizes();
}

void RasterWidget::setCaptureFile(CaptureFile* captureFile) {
    m_captureFile = captureFile;
    calculateSizes();
}

void RasterWidget::setTimeSlots(unsigned int ts) {
    if( ts > 0 && ts != m_ts ) {
        m_ts = ts;
        calculateSizes();
    }
}

void RasterWidget::setBitsPerTimeSlot(unsigned int bpts) {
    if( bpts > 0 && bpts != m_bpts ) {
        m_bpts = bpts;
        calculateSizes();
    }
}

void RasterWidget::setFramesPerLine(unsigned int fpl) {
    if( fpl > 0 && fpl != m_fpl ) {
        m_fpl = fpl;
        calculateSizes();
    }
}

void RasterWidget::setFileOffset(unsigned int offset) {
    if( offset != m_foffset ) {
        m_foffset = offset;
        calculateSizes();
    }
}

void RasterWidget::setZoom(unsigned int zoom) {
    if( zoom > 0 ) {
        m_zoom = zoom;
        repaint();
    }
}

void RasterWidget::setHorizontalOffset(int offset) {
    if( (unsigned int)offset != m_hoffset ) {
        m_hoffset = (unsigned int)offset;
        repaint();
    }
}

void RasterWidget::setVerticalOffset(int offset) {
    if( (unsigned int)offset != m_voffset ) {
        m_voffset = (unsigned int)offset;
        repaint();
    }
}

unsigned int RasterWidget::horizontalMaximum() {
    return m_totalPixelWidth;
}

unsigned int RasterWidget::verticalMaximum() {
    return m_totalPixelHeight;
}

void RasterWidget::calculateSizes() {
    if( m_captureFile == 0 ) {
        m_totalBitWidth = 0;
        m_tsBitWidth = 0;
        m_frameBitWidth = 0;
        m_tsPixelWidth = 1;
        m_totalPixelWidth = 0;
        m_totalPixelHeight = 0;
    }
    else {
        m_totalBitWidth = m_bpts*m_ts*m_fpl;
        m_tsBitWidth = m_bpts*m_fpl;
        m_frameBitWidth = m_ts*m_bpts;
        m_tsPixelWidth = m_tsBitWidth+1;
        m_totalPixelWidth = m_tsPixelWidth*m_ts-1;
        m_totalPixelHeight = (m_captureFile->sizebit()-m_foffset) / m_totalBitWidth;
    }
    repaint();
}

void RasterWidget::mouseMoveEvent(QMouseEvent* event) {
    QString tip;
    unsigned int ts = ( (event->x()/m_zoom) + m_hoffset) / m_tsPixelWidth;
    if( ts < m_ts ) {
        QTextStream tipstream(&tip);
        tipstream << "TS:" << ts;
    }
    setToolTip(tip);
    event->accept();
}

void RasterWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    size_t line = (event->y()/m_zoom)+m_voffset;
    size_t ts = ((event->x()/m_zoom)+m_hoffset) / m_tsPixelWidth;
    qDebug() << event->x() << " " << event->x()/m_zoom << " " << m_hoffset;
    size_t fileLineOffset = m_foffset + line*m_totalBitWidth;
    size_t frame;
    int i;
    QBitArray data(m_bpts*m_fpl,false);
    QBitArray *bits;

    for( frame=0; frame<m_fpl; frame++ ) {
         m_captureFile->seekbit(fileLineOffset + frame*m_frameBitWidth + ts*m_bpts );
         bits = m_captureFile->readbit(m_bpts);
         for( i=0; i<bits->size(); i++ ) {
             data.setBit(frame*m_bpts+i,bits->testBit(i));
         }
         delete bits;
    }

    QString labelstr;
    QTextStream labelstream(&labelstr);
    QString datastr;
    QTextStream datastream(&datastr);

    labelstream << "File:" << m_captureFile->fileName() << " " << "TS:" << ts << " " << "Line:" << line;
    for( i=0; i<data.size(); i++ ) {
        if( data.testBit(i) ) {
            datastream << "1";
        }
        else {
            datastream << "0";
        }
    }
    emit info(labelstr,datastr);
}

void RasterWidget::paintEvent(QPaintEvent* event) {
    paintRaster(this,m_voffset,m_hoffset,m_zoom);
    event->accept();
}

void RasterWidget::saveViewableRaster(QString path, QProgressDialog* dlg) {
    QImage target(QSize(width(),height()),QImage::Format_RGB16);
    paintRaster(&target,m_voffset,m_hoffset,m_zoom,dlg);
    target.save(path);
}

void RasterWidget::saveHorizontalRaster(QString path, QProgressDialog* dlg) {
    QImage target(QSize(m_totalPixelWidth,height()/m_zoom),QImage::Format_RGB16);
    paintRaster(&target,m_voffset,0,1,dlg);
    target.save(path);
}

void RasterWidget::saveVerticalRaster(QString path, QProgressDialog* dlg) {
    QImage target(QSize(width()/m_zoom,m_totalPixelHeight),QImage::Format_RGB16);
    paintRaster(&target,0,m_hoffset,1,dlg);
    target.save(path);
}

void RasterWidget::saveEntireRaster(QString path, QProgressDialog* dlg) {
    QImage target(QSize(m_totalPixelWidth,m_totalPixelHeight),QImage::Format_RGB16);
    paintRaster(&target,0,0,1,dlg);
    target.save(path);
}

void RasterWidget::paintRaster(QPaintDevice* target, size_t vOffset, size_t hOffset, size_t zoom, QProgressDialog* dlg) {
    unsigned int y,x,line,frame,ts;
    int i;
    size_t baseFileOffset = m_foffset+m_totalBitWidth*vOffset;
    size_t lineOffset;
    unsigned int minVisibleTS = hOffset / m_tsPixelWidth;
    unsigned int maxVisibleTS = minVisibleTS;
    for( i=((minVisibleTS*m_tsPixelWidth) - hOffset)*zoom;
         i<target->width() && maxVisibleTS < m_ts;
         i=i+m_tsPixelWidth*zoom, maxVisibleTS++);
    unsigned int visibleLineCount = (target->height() / zoom)+1;
    QBitArray* bits;

    if( dlg != 0 ) {
        dlg->setMinimum(0);
        dlg->setMaximum(visibleLineCount);
        dlg->setValue(0);
    }

    QPainter painter;
    painter.begin(target);

    QBrush blackBrush(QColor(0,0,0,0xFF));
    QBrush whiteBrush(QColor(0xFF,0xFF,0xFF,0xFF));
    QBrush greenBrush(QColor(0,0xFF,00,0xFF));
    QBrush grayBrush(QColor(0x80,0x80,0x80,0xFF));

    painter.fillRect(0,0,target->width(),target->height(),grayBrush);

    if( m_captureFile ){
        //Draw white lines to seperate timeslots
        for( ts=minVisibleTS; ts<maxVisibleTS; ts++ ) {
            if( ts < m_ts-1 ) {
                x = (((ts+1)*m_tsPixelWidth)-1-hOffset)*zoom;
                painter.fillRect(x,0,zoom,target->height(),whiteBrush);
            }
        }
        //Draw green pixels (with zoom)
        for( line=0; line<visibleLineCount; line++ ) {
            if( dlg != 0 ) {
                dlg->setValue(line);
                if( dlg->wasCanceled() ) {
                    break;
                }
            }
            y = line*zoom;
            lineOffset = baseFileOffset + line*m_totalBitWidth;
            if( lineOffset > m_captureFile->sizebit() ) {
                painter.fillRect(0,y,target->width(),target->height()-y,grayBrush);
                break;
            }
            for( frame=0; frame<m_fpl; frame++ ) {
                for( ts=minVisibleTS; ts<maxVisibleTS; ts++ ) {
                     m_captureFile->seekbit(lineOffset + frame*m_frameBitWidth + ts*m_bpts );
                     bits = m_captureFile->readbit(m_bpts);
                     x = ((ts*m_tsPixelWidth + frame*m_bpts) - hOffset)*zoom;
                     for( i=0; i<bits->size(); i++, x=x+zoom ) {
                         if( bits->testBit(i) ) {
                            painter.fillRect(x,y,zoom,zoom,greenBrush);
                         }
                         else {
                            painter.fillRect(x,y,zoom,zoom,blackBrush);
                         }
                     }
                     delete bits;
                }
            }
        }
    }
    painter.end();
}

void RasterWidget::saveHorizontalCSV(QString path, QBitArray *tsIncl, QProgressDialog* dlg) {
    saveCSV(path,tsIncl,m_hoffset,height()/m_zoom,dlg);
}

void RasterWidget::saveEntireCSV(QString path, QBitArray *tsIncl, QProgressDialog* dlg) {
    saveCSV(path,tsIncl,0,m_totalPixelHeight,dlg);
}

void RasterWidget::saveCSV(QString path, QBitArray *tsIncl, size_t lineOffset, size_t lineCount, QProgressDialog* dlg) {
    QFile outFile(path);
    outFile.open(QFile::WriteOnly | QFile::Truncate);
    QTextStream out(&outFile);
    QBitArray allYes(m_ts,true);
    size_t fileLineOffset;
    unsigned int line,frame,ts;
    int i;

    if( ! outFile.isOpen() ) {
        return;
    }

    if( dlg != 0 ) {
        dlg->setMinimum(lineOffset);
        dlg->setMaximum(lineOffset+lineCount);
        dlg->setValue(lineOffset);
    }

    if( tsIncl == 0 ) {
        tsIncl = &allYes;
    }

    bool first = true;
    for( i=0; i<(int)m_ts; i++ ) {
        if( tsIncl->testBit(i) ) {
            if( ! first ) { out << ","; }
            else { first = false; }
            out<<"TS:"<<i;
        }
    }
    out << "\n";

    QBitArray data(m_bpts*m_fpl);
    QBitArray *bits;
    for( line=lineOffset; line<lineOffset+lineCount && line<m_totalPixelHeight; line++ ) {
        if( dlg != 0 ) {
            dlg->setValue(line);
            if( dlg->wasCanceled() ) {
                break;
            }
        }
        fileLineOffset = m_foffset + line*m_totalBitWidth;
        first = true;
        for( ts=0; ts<m_ts; ts++ ) {
            if( ! tsIncl->testBit(ts) ) {
                continue;
            }
            data.fill(false);
            for( frame=0; frame<m_fpl; frame++ ) {
                 m_captureFile->seekbit(fileLineOffset + frame*m_frameBitWidth + ts*m_bpts );
                 bits = m_captureFile->readbit(m_bpts);
                 for( i=0; i<bits->size(); i++ ) {
                     data.setBit(frame*m_bpts+i,bits->testBit(i));
                 }
                 delete bits;
            }
            if( ! first ) { out << ","; }
            else { first = false; }
            for( i=0; i<data.size(); i++ ) {
                if( data.testBit(i) ) {
                    out << "1";
                }
                else {
                    out << "0";
                }
            }
        }
        out << "\n";
    }
}

void RasterWidget::saveHorizontalTimeSlots(QString path, QBitArray *tsIncl, QProgressDialog* dlg) {
    saveTimeSlots(path,tsIncl,m_hoffset,height()/m_zoom,dlg);
}

void RasterWidget::saveEntireTimeSlots(QString path, QBitArray *tsIncl, QProgressDialog* dlg) {
    saveTimeSlots(path,tsIncl,0,m_totalPixelHeight,dlg);
}

void RasterWidget::saveTimeSlots(QString path, QBitArray *tsIncl, size_t lineOffset, size_t lineCount, QProgressDialog* dlg) {
    FILE* fp;
    unsigned char databyte = 0;
    size_t databytelen = 0;
    QBitArray allYes(m_ts,true);
    unsigned int line,frame,ts;
    int i;

    fp = fopen(path.toStdString().c_str(),"wb");
    if( ! fp ) {
        return;
    }

    if( dlg != 0 ) {
        dlg->setMinimum(lineOffset);
        dlg->setMaximum(lineOffset+lineCount);
        dlg->setValue(lineOffset);
    }

    if( tsIncl == 0 ) {
        tsIncl = &allYes;
    }

    QBitArray *bits;
    for( line=lineOffset; line<lineOffset+lineCount && line<m_totalPixelHeight; line++ ) {
        if( dlg != 0 ) {
            dlg->setValue(line);
            if( dlg->wasCanceled() ) {
                break;
            }
        }
        lineOffset = m_foffset + line*m_totalBitWidth;
        for( frame=0; frame<m_fpl; frame++ ) {
            for( ts=0; ts<m_ts; ts++ ) {
                if( ! tsIncl->testBit(ts) ) {
                    continue;
                }
                m_captureFile->seekbit(lineOffset + frame*m_frameBitWidth + ts*m_bpts );
                bits = m_captureFile->readbit(m_bpts);
                for( i=0; i<bits->size(); i++ ) {
                    if( bits->testBit(i) ) {
                        databyte = databyte | (1<<(7-databytelen));
                    }
                    databytelen++;
                    if( databytelen == 8 ) {
                        fwrite(&databyte,1,1,fp);
                        databyte = 0;
                        databytelen = 0;
                    }
                }
                delete bits;
            }
        }
    }
    fclose(fp);
}
