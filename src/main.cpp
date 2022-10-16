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
#include <QApplication>
#include <QFileInfo>
#include <QDir>
#include <stdio.h>

void usage(char* cmd) {
    fprintf(stderr,"Usage:\n");
    fprintf(stderr,"%s [-h] [-ts ts] [[-bpts bpts] | [-bpl bpl]] [-fpl fpl] [-offset offset]\n",cmd);
    fprintf(stderr,"    [-zoom zoom] [-auto] [-tdm | -bin] [-invert] [-rbpp rbpp] [-gbpp gbpp]\n");
    fprintf(stderr,"    [-bbpp bbpp] [-bit | -byte] [-file file]\n");
    fprintf(stderr,"\n");
    fprintf(stderr,"  ts     : Number of time slots (used with -tdm)\n");
    fprintf(stderr,"  bpts   : Bits per time slot (used with -tdm)\n");
    fprintf(stderr,"  bpl    : Bits per line (used with -bin)\n");
    fprintf(stderr,"  fpl    : Frames displayed per line (used with -tdm)\n");
    fprintf(stderr,"  offset : Bit offset into file (default 0)\n");
    fprintf(stderr,"  zoom   : Zoom factor (default 1)\n");
    fprintf(stderr,"  auto   : Enable auto raster update\n");
    fprintf(stderr,"  tdm    : Set to Time Division Multiplex mode (default)\n");
    fprintf(stderr,"  bin    : Set to generic Binary mode\n");
    fprintf(stderr,"  invert : Invert bits\n");
    fprintf(stderr,"  rbpp   : Red bits per pixel (default 0)\n");
    fprintf(stderr,"  gbpp   : Green bits per pixel (default 1)\n");
    fprintf(stderr,"  bbpp   : Blue bits per pixel (default 0)\n");
    fprintf(stderr,"  bit    : File is Bit per Byte\n");
    fprintf(stderr,"  byte   : File is Byte per Byte (default)\n");
    fprintf(stderr,"  file   : File to analyze\n");
    exit(1);
}

int main(int argc, char *argv[])
{
    bool fileTypeSet = false;
    char* path = 0;
    int i;

    QApplication::addLibraryPath(QFileInfo(QString(argv[0])).absoluteDir().filePath("plugins"));

    QApplication a(argc, argv);
    MainWindow w;

    for( i=1; i<argc; i++ ) {
        if( strcmp(argv[i],"-h") == 0 ) {
            usage(argv[0]);
        }
        else if( strcmp(argv[i],"-ts") == 0 ) {
            if( i<argc-1 ) {
                w.settings()->setTs(atoi(argv[(i++)+1]));
            }
            else { usage(argv[0]); }
        }
        else if( strcmp(argv[i],"-bpts") == 0 ) {
            if( i<argc-1 ) {
                w.settings()->setBpts(atoi(argv[(i++)+1]));
            }
            else { usage(argv[0]); }
        }
        else if( strcmp(argv[i],"-bpl") == 0 ) {
            if( i<argc-1 ) {
                w.settings()->setBpl(atoi(argv[(i++)+1]));
            }
            else { usage(argv[0]); }
        }
        else if( strcmp(argv[i],"-fpl") == 0) {
            if( i<argc-1 ) {
                w.settings()->setFpl(atoi(argv[(i++)+1]));
            }
            else { usage(argv[0]); }
        }
        else if( strcmp(argv[i],"-offset") == 0) {
            if( i<argc-1 ) {
                w.settings()->setOffset(atoi(argv[(i++)+1]));
            }
            else { usage(argv[0]); }
        }
        else if( strcmp(argv[i],"-zoom") == 0) {
            if( i<argc-1 ) {
                w.settings()->setZoom(atoi(argv[(i++)+1]));
            }
            else { usage(argv[0]); }
        }
        else if( strcmp(argv[i],"-auto") == 0) {
            w.setAutoUpdate(true);
        }
        else if( strcmp(argv[i],"-rbpp") == 0) {
            w.settings()->setRbpp(atoi(argv[(i++)+1]));
        }
        else if( strcmp(argv[i],"-gbpp") == 0) {
            w.settings()->setGbpp(atoi(argv[(i++)+1]));
        }
        else if( strcmp(argv[i],"-bbpp") == 0) {
            w.settings()->setBbpp(atoi(argv[(i++)+1]));
        }
        else if( strcmp(argv[i],"-tdm") == 0) {
            w.setTdmMode();
        }
        else if( strcmp(argv[i],"-bin") == 0) {
            w.setBinMode();
        }
        else if( strcmp(argv[i],"-invert") == 0) {
            w.setInvert(true);
        }
        else if( strcmp(argv[i],"-bit") == 0) {
            if( fileTypeSet ) { usage(argv[0]); }
            else {
                fileTypeSet = true;
                w.setBitPerByte();
            }
        }
        else if( strcmp(argv[i],"-byte") == 0) {
            if( fileTypeSet ) { usage(argv[0]); }
            else {
                fileTypeSet = true;
                w.setBytePerByte();
            }
        }
        else if( strcmp(argv[i],"-file") == 0) {
            if( i<argc-1 ) {
                path = argv[(i++)+1];
            }
            else { usage(argv[0]); }
        }
    }
    if( path != 0 ) {
        w.openSpecifiedFile(QString(path));
    }


    w.show();
    return a.exec();
}
