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
#include "channelselectiondialog.h"
#include <QLabel>
#include <QBoxLayout>
#include <QPushButton>
#include <QGridLayout>
#include <QTextStream>
#include <QDebug>

ChannelSelectionDialog::ChannelSelectionDialog(unsigned int channelCount, QWidget *parent, Qt::WindowFlags f) : QDialog(parent,f)
{
    unsigned int i;
    QBoxLayout* mainLayout = new QBoxLayout(QBoxLayout::TopToBottom,this);
    setLayout(mainLayout);
    mainLayout->addWidget(new QLabel("Select Channels"),0,Qt::AlignCenter);
    m_list = new QListWidget(this);
    m_list->setSortingEnabled(false);
    m_list->setSelectionMode(QAbstractItemView::ExtendedSelection);
    for( i=0; i<channelCount; i++ ) {
        QString label;
        QTextStream s(&label);
        s << "TS:" << i;
        m_list->addItem(label);
    }
    mainLayout->addWidget(m_list,1);

    QGridLayout *buttonLayout = new QGridLayout(this);
    mainLayout->addLayout(buttonLayout,0);

    QPushButton* selAll = new QPushButton("Select All",this);
    connect(selAll,SIGNAL(clicked()),this,SLOT(onSelAll()));
    buttonLayout->addWidget(selAll,0,0);
    QPushButton* selNone = new QPushButton("Select None",this);
    connect(selNone,SIGNAL(clicked()),this,SLOT(onSelNone()));
    buttonLayout->addWidget(selNone,0,1);
    QPushButton* ok = new QPushButton("Ok",this);
    connect(ok,SIGNAL(clicked()),this,SLOT(onOk()));
    buttonLayout->addWidget(ok,1,0);
    QPushButton* cancel = new QPushButton("Cancel",this);
    connect(cancel,SIGNAL(clicked()),this,SLOT(onCancel()));
    buttonLayout->addWidget(cancel,1,1);


    setModal(true);
    setMinimumHeight(400);
    setMinimumWidth(200);

    m_selected = new QBitArray(channelCount);
}

QBitArray* ChannelSelectionDialog::selected() {
    return m_selected;
}

void ChannelSelectionDialog::onSelAll() {
    int i;
    for( i=0; i<m_list->count(); i++) {
        m_list->item(i)->setSelected(true);
    }
}

void ChannelSelectionDialog::onSelNone() {
    int i;
    for( i=0; i<m_list->count(); i++) {
        m_list->item(i)->setSelected(false);
    }
}

void ChannelSelectionDialog::onOk() {
    int i;
    for( i=0; i<m_list->count(); i++ ) {
        //qDebug() << i << " " << m_list->item(i)->isSelected() << " " << m_selected->testBit(i)
        m_selected->setBit(i,m_list->item(i)->isSelected());
    }
    accept();
}
void ChannelSelectionDialog::onCancel() {
    reject();
}
