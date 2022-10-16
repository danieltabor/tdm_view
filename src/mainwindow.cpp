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
#include "mainwindow.h"
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include "channelselectiondialog.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    m_central = new CentralWidget(this);
    setCentralWidget(m_central);

    QAction* action;
    QMenu* fileMenu = menuBar()->addMenu("&File");
    action = fileMenu->addAction("&Open");
    connect(action,SIGNAL(triggered()),this,SLOT(openFile()));
    fileMenu->addSeparator();
    m_byte_per_bit = fileMenu->addAction("File is Byte Per Bit");
    m_byte_per_bit->setCheckable(true);
    m_byte_per_bit->setChecked(false);
    connect(m_byte_per_bit,SIGNAL(triggered()),this,SLOT(setFileType()));
    m_bit_per_bit = fileMenu->addAction("File is Bit Per Bit");
    m_bit_per_bit->setCheckable(true);
    m_bit_per_bit->setChecked(true);
    connect(m_bit_per_bit,SIGNAL(triggered()),this,SLOT(setFileType()));
    m_file_type_group = new QActionGroup(this);
    m_file_type_group->addAction(m_byte_per_bit);
    m_file_type_group->addAction(m_bit_per_bit);
    fileMenu->addSeparator();
    m_invert = fileMenu->addAction("Invert Bits");
    m_invert->setCheckable(true);
    connect(m_invert,SIGNAL(triggered()),this,SLOT(setInvert()));
    fileMenu->addSeparator();
    action = fileMenu->addAction("E&xit");
    connect(action,SIGNAL(triggered()),this,SLOT(close()));

    QMenu* rasterMenu = menuBar()->addMenu("&Raster");
    action = rasterMenu->addAction("Save &Viewable Raster");
    connect(action,SIGNAL(triggered()),this,SLOT(saveViewableRaster()));
    action = rasterMenu->addAction("Save &Horizontal Raster");
    connect(action,SIGNAL(triggered()),this,SLOT(saveHorizontalRaster()));
    action = rasterMenu->addAction("Save Ver&tical Raster");
    connect(action,SIGNAL(triggered()),this,SLOT(saveVerticalRaster()));
    action = rasterMenu->addAction("Save Entire &Raster");
    connect(action,SIGNAL(triggered()),this,SLOT(saveEntireRaster()));

    QMenu* csvMenu = menuBar()->addMenu("&CSV");
    action = csvMenu->addAction("Save &Horizontal CSV");
    connect(action,SIGNAL(triggered()),this,SLOT(saveHorizontalCSV()));
    action = csvMenu->addAction("Save &CSV");
    connect(action,SIGNAL(triggered()),this,SLOT(saveEntireCSV()));

    QMenu* binaryMenu = menuBar()->addMenu("&Binary");
    action = binaryMenu->addAction("Save Horizontal TimeSlots");
    connect(action,SIGNAL(triggered()),this,SLOT(saveHorizontalTS()));
    action = binaryMenu->addAction("Save Entire &TimeSlots");
    connect(action,SIGNAL(triggered()),this,SLOT(saveEntireTS()));

    QMenu* uiMenu = menuBar()->addMenu("&UI");
    m_auto_update = uiMenu->addAction("&Auto Update");
    m_auto_update->setCheckable(true);
    m_auto_update->setChecked(false);
    connect(m_auto_update,SIGNAL(triggered()),this,SLOT(setAutoUpdate()));
    uiMenu->addSeparator();
    m_enable_colors = uiMenu->addAction("&Color Channels");
    m_enable_colors->setCheckable(true);
    m_enable_colors->setChecked(false);
    connect(m_enable_colors,SIGNAL(triggered()),this,SLOT(setEnableColors()));
    uiMenu->addSeparator();
    m_tdm_mode = uiMenu->addAction("&TDM Mode");
    m_tdm_mode->setCheckable(true);
    m_tdm_mode->setChecked(true);
    connect(m_tdm_mode,SIGNAL(triggered()),this,SLOT(setSettingsMode()));
    m_bin_mode = uiMenu->addAction("&Binary Mode");
    m_bin_mode->setCheckable(true);
    m_bin_mode->setChecked(false);
    connect(m_bin_mode,SIGNAL(triggered()),this,SLOT(setSettingsMode()));
    m_mode_group = new QActionGroup(this);
    m_mode_group->addAction(m_tdm_mode);
    m_mode_group->addAction(m_bin_mode);

    //QMenu* analysisMenu = menuBar()->addMenu("&Anaylsis");
    //action = analysisMenu->addAction("&Find Sync");
    //connect(action,SIGNAL(triggered()),this,SLOT("openFile"));
    //action = analysisMenu->addAction("&Sync");
    //connect(action,SIGNAL(triggered()),this,SLOT("openFile"));

    connect(m_central->raster(),SIGNAL(info(QString,QString)),this,SLOT(showInfo(QString,QString)));

    m_captureFile = 0;
    resize(800,600);
}

MainWindow::~MainWindow()
{

}

SettingsWidget* MainWindow::settings() {
    return m_central->settings();
}

void MainWindow::setInvert(bool invert) {
        m_invert->setChecked(invert);
}

void MainWindow::setBitPerByte() {
    m_bit_per_bit->setChecked(true);
    m_byte_per_bit->setChecked(false);
}
void MainWindow::setBytePerByte() {
    m_bit_per_bit->setChecked(false);
    m_byte_per_bit->setChecked(true);
}

void MainWindow::setAutoUpdate(bool autoUpdate) {
    m_auto_update->setChecked(autoUpdate);
    m_central->settings()->setAutoUpdate(autoUpdate);
}

void MainWindow::setEnableColors(bool enableColors) {
    m_enable_colors->setChecked(enableColors);
    m_central->settings()->setEnableColors(enableColors);
}

void MainWindow::setTdmMode() {
    m_tdm_mode->setChecked(true);
    m_bin_mode->setChecked(false);
    setSettingsMode();
}

void MainWindow::setBinMode() {
    m_tdm_mode->setChecked(false);
    m_bin_mode->setChecked(true);
    setSettingsMode();
}

void MainWindow::openFile() {
    QString path = QFileDialog::getOpenFileName(this,"Open Capture");
    if( path.length() ) {
        openSpecifiedFile(path);
    }
}

void MainWindow::openSpecifiedFile(QString path) {
    m_path = path;
    if( m_captureFile ) {
        delete m_captureFile;
    }
    if( m_byte_per_bit->isChecked() ) {
        m_captureFile = (CaptureFile*)new CaptureFile_BytePerBit(path,m_invert->isChecked());
    }
    else {
        m_captureFile = (CaptureFile*)new CaptureFile_BitPerBit(path,m_invert->isChecked());
    }
    setWindowTitle(m_captureFile->fileName());
    m_central->setCaptureFile(m_captureFile);
}

void MainWindow::saveViewableRaster() {
    QString path = QFileDialog::getSaveFileName(this,"Save Viewable Raster");
    if( path.length() ) {
        m_savePath = path;
        m_progress = new QProgressDialog("Saving Viewable Raster","Cancel",0,100,this);
        m_progress->setWindowModality(Qt::WindowModal);
        m_central->raster()->saveViewableRaster(m_savePath,m_progress);
        m_progress->close();
        delete m_progress;
    }
}

void MainWindow::saveHorizontalRaster() {
    QString path = QFileDialog::getSaveFileName(this,"Save Horizontal Raster");
    if( path.length() ) {
        m_savePath = path;
        m_progress = new QProgressDialog("Saving Horizontal Raster","Cancel",0,100,this);
        m_progress->setWindowModality(Qt::WindowModal);
        m_central->raster()->saveHorizontalRaster(m_savePath,m_progress);
        m_progress->close();
        delete m_progress;
    }
}

void MainWindow::saveVerticalRaster() {
    QString path = QFileDialog::getSaveFileName(this,"Save Vertical Raster");
    if( path.length() ) {
        m_savePath = path;
        m_progress = new QProgressDialog("Saving Vertical Raster","Cancel",0,100,this);
        m_progress->setWindowModality(Qt::WindowModal);
        m_central->raster()->saveVerticalRaster(m_savePath,m_progress);
        m_progress->close();
        delete m_progress;
    }
}

void MainWindow::saveEntireRaster() {
    QString path = QFileDialog::getSaveFileName(this,"Save Entire Raster");
    if( path.length() ) {
        m_savePath = path;
        m_progress = new QProgressDialog("Saving Entire Raster","Cancel",0,100,this);
        m_progress->setWindowModality(Qt::WindowModal);
        m_central->raster()->saveEntireRaster(m_savePath,m_progress);
        m_progress->close();
        delete m_progress;
    }
}

void MainWindow::saveHorizontalCSV() {
    ChannelSelectionDialog sel(settings()->ts(),this);
    if( sel.exec() ) {
        QString path = QFileDialog::getSaveFileName(this,"Save Horizontal CSV");
        if( path.length() ) {
            m_savePath = path;
            m_saveSelect = sel.selected();
            m_progress = new QProgressDialog("Saving Horizontal CSV","Cancel",0,100,this);
            m_progress->setWindowModality(Qt::WindowModal);
            m_central->raster()->saveHorizontalCSV(m_savePath,m_saveSelect,m_progress);
            m_progress->close();
            delete m_progress;
            delete m_saveSelect;
        }
    }
}

void MainWindow::saveEntireCSV() {
    ChannelSelectionDialog sel(settings()->ts(),this);
    sel.exec();
    qDebug() << sel.result();
    if( sel.result() ) {
        QString path = QFileDialog::getSaveFileName(this,"Save Entire CSV");
        if( path.length() ) {
            m_savePath = path;
            m_saveSelect = sel.selected();
            m_progress = new QProgressDialog("Saving Entire CSV","Cancel",0,100,this);
            m_progress->setWindowModality(Qt::WindowModal);
            m_progress->setBar(0);
            m_central->raster()->saveEntireCSV(m_savePath,m_saveSelect,m_progress);
            m_progress->close();
            delete m_progress;
            delete m_saveSelect;
        }
    }
}

void MainWindow::saveHorizontalTS() {
    ChannelSelectionDialog sel(settings()->ts(),this);
    if( sel.exec() ) {
        QString path = QFileDialog::getSaveFileName(this,"Save Horizontal Time Slots");
        if( path.length() ) {
            m_savePath = path;
            m_saveSelect = sel.selected();
            m_progress = new QProgressDialog("Saving Horizontal Time Slots","Cancel",0,100,this);
            m_progress->setWindowModality(Qt::WindowModal);
            m_central->raster()->saveHorizontalTimeSlots(m_savePath,m_saveSelect,m_progress);
            m_progress->close();
            delete m_progress;
            delete m_saveSelect;
        }
    }
}

void MainWindow::saveEntireTS() {
    ChannelSelectionDialog sel(settings()->ts(),this);
    sel.exec();
    qDebug() << sel.result();
    if( sel.result() ) {
        QString path = QFileDialog::getSaveFileName(this,"Save Entire Time Slots");
        if( path.length() ) {
            m_savePath = path;
            m_saveSelect = sel.selected();
            m_progress = new QProgressDialog("Saving Entire Time Slots","Cancel",0,100,this);
            m_progress->setWindowModality(Qt::WindowModal);
            m_progress->setBar(0);
            m_central->raster()->saveEntireTimeSlots(m_savePath,m_saveSelect,m_progress);
            m_progress->close();
            delete m_progress;
            delete m_saveSelect;
        }
    }
}

void MainWindow::findSync() {

}

void MainWindow::setFileType() {
    if( m_path.length() != 0 ) {
        openSpecifiedFile(m_path);
    }
}

void MainWindow::setInvert() {
    if( m_path.length() != 0 ) {
        openSpecifiedFile(m_path);
    }
}

void MainWindow::setAutoUpdate() {
    m_central->settings()->setAutoUpdate(m_auto_update->isChecked());
}

void MainWindow::setEnableColors() {
    m_central->settings()->setEnableColors(m_enable_colors->isChecked());
}

void MainWindow::setSettingsMode() {
    if( m_tdm_mode->isChecked() ) {
        m_central->settings()->setTdmMode();
    }
    else {
        m_central->settings()->setBinMode();
    }
}

void MainWindow::showInfo(QString label, QString data) {
    m_info.setInfo(label,data);
}

void MainWindow::closeEvent( QCloseEvent* event ) {
    m_info.close();
    event->accept();
}
