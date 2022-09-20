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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QString>
#include <QTimer>
#include <QBitArray>
#include <QProgressDialog>
#include <QCloseEvent>
#include "centralwidget.h"
#include "capturefile.h"
#include "capturefile_bitperbit.h"
#include "capturefile_byteperbit.h"
#include "infodialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    SettingsWidget* settings();
    void setInvert(bool invert);
    void setBitPerByte();
    void setBytePerByte();
    void setAutoUpdate(bool autoUpdate);
    void setTdmMode();
    void setBinMode();
    void openSpecifiedFile(QString path);

public slots:
    void openFile();
    void saveViewableRaster();
    void saveHorizontalRaster();
    void saveVerticalRaster();
    void saveEntireRaster();
    void saveHorizontalCSV();
    void saveEntireCSV();
    void saveHorizontalTS();
    void saveEntireTS();
    void findSync();
    void setFileType();
    void setInvert();
    void setAutoUpdate();
    void setSettingsMode();
    void showInfo(QString label, QString data);

protected:
    virtual void closeEvent(QCloseEvent *event);

private:
    CentralWidget* m_central;
    QAction* m_byte_per_bit;
    QAction* m_bit_per_bit;
    QAction* m_invert;
    QActionGroup* m_file_type_group;
    QAction* m_auto_update;
    QAction *m_tdm_mode;
    QAction *m_bin_mode;
    QActionGroup* m_mode_group;
    QString m_path;
    CaptureFile* m_captureFile;
    InfoDialog m_info;
    QProgressDialog *m_progress;
    QString m_savePath;
    QBitArray *m_saveSelect;
};

#endif // MAINWINDOW_H
