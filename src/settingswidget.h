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
#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include<QWidget>
#include<QLabel>
#include<QLineEdit>
#include<QSpinBox>
#include<QString>
#include<QPushButton>

class SettingsWidget: public QWidget
{
    Q_OBJECT

public:
    SettingsWidget(QWidget* parent=0);
    int ts();
    void setTs(int ts);
    int bpts();
    void setBpts(int bpts);
    int bpl();
    void setBpl(int bpl);
    int fpl();
    void setFpl(int fpl);
    int offset();
    void setOffset(int offset);
    //QString sync();
    int zoom();
    void setZoom(int zoom);
    bool autoUpdate();
    void setAutoUpdate(bool autoUpdate);
    void setTdmMode();
    void setBinMode();

public slots:
    void updateHandler();
    void updateEmit();


signals:
    void update();

private:
    QLabel* m_tsl;
    QLineEdit* m_tsw;
    int m_ts;
    QLabel* m_bptsl;
    QLineEdit* m_bptsw;
    int m_bpts;
    QLabel* m_fpll;
    QLineEdit* m_fplw;
    int m_fpl;
    QLabel* m_offsetl;
    QLineEdit* m_offsetw;
    int m_offset;
    //QLineEdit* m_syncw;
    //QString m_syncv;
    QLabel* m_zooml;
    QSpinBox* m_zoomw;
    QPushButton* m_updateButton;
    QPalette *m_errpal;
    QPalette *m_okpal;
    bool m_autov;
    bool m_tdmv;
};

#endif // SETTINGSWIDGET_H
