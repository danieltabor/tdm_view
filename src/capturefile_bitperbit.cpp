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
#include "capturefile_bitperbit.h"

CaptureFile_BitPerBit::CaptureFile_BitPerBit(QString path, bool invert): CaptureFile(path)
{
    m_fp = fopen(path.toStdString().c_str(),"rb");
    fseek(m_fp,0,SEEK_END);
    m_fileSize = ftell(m_fp)*8;
    fseek(m_fp,0,SEEK_SET);
    m_invert = invert;
    fread(&m_currentByte,1,1,m_fp);
    m_bitOffset = 0;
    m_invert = invert;
}

CaptureFile_BitPerBit::~CaptureFile_BitPerBit() {
    fclose(m_fp);
}

size_t CaptureFile_BitPerBit::tellbit() {
    return ((ftell(m_fp)-1)*8) + m_bitOffset;
}
void CaptureFile_BitPerBit::seekbit(size_t offset) {
    fseek(m_fp,offset/8,SEEK_SET);
    fread(&m_currentByte,1,1,m_fp);
    m_bitOffset = offset%8;
}

size_t CaptureFile_BitPerBit::sizebit() {
    return m_fileSize;
}

QBitArray* CaptureFile_BitPerBit::readbit(size_t readlen) {
    char bit;
    size_t bits_size = 0;
    QBitArray *bits = new QBitArray(readlen);
    while( bits_size < readlen ) {
        bit = (m_currentByte >> (7-m_bitOffset)) & 1;
        if( (bit && !m_invert) ||
            (!bit && m_invert) ) {
            bits->setBit(bits_size,true);
        }
        bits_size++;
        m_bitOffset++;
        if( m_bitOffset == 8 ) {
            m_bitOffset = 0;
            m_currentByte = 0;
            fread(&m_currentByte,1,1,m_fp);
        }
    }
    return bits;
}
