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
#include "capturefile_byteperbit.h"

CaptureFile_BytePerBit::CaptureFile_BytePerBit(QString path, bool invert): CaptureFile(path)

{
    m_fp = fopen(path.toStdString().c_str(),"rb");
    fseek(m_fp,0,SEEK_END);
    m_fileSize = ftell(m_fp);
    fseek(m_fp,0,SEEK_SET);
    m_invert = invert;
}

CaptureFile_BytePerBit::~CaptureFile_BytePerBit() {
    fclose(m_fp);
}

size_t CaptureFile_BytePerBit::tellbit() {
    return ftell(m_fp);
}
void CaptureFile_BytePerBit::seekbit(size_t offset) {
    fseek(m_fp,offset,SEEK_SET);
}

size_t CaptureFile_BytePerBit::sizebit() {
    return m_fileSize;
}

QBitArray* CaptureFile_BytePerBit::readbit(size_t readlen) {
    char buf[1024];
    size_t block_size;
    size_t bits_size = 0;
    size_t read_size;
    size_t i;
    QBitArray *bits = new QBitArray(readlen);
    while( bits_size < readlen ) {
        if( readlen-bits_size < 1024 ) {
            block_size = readlen-bits_size;
        } else {
            block_size = 1024;
        }
        memset(buf,0,1024);
        read_size = fread(buf,1,block_size,m_fp);
        for( i=0; i<read_size; i++ ) {
            if( (buf[i] && !m_invert) ||
                (!buf[i] && m_invert) ) {
                bits->setBit(bits_size+i,true);
            }
        }
        bits_size = bits_size + read_size;
    }
    return bits;
}
