/** @file
  FrontPage routines to handle the callbacks and browser calls

Copyright (c) 2004 - 2017, Intel Corporation. All rights reserved.<BR>
(C) Copyright 2018 Hewlett Packard Enterprise Development LP<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "FrontPage.h"
#include "FrontPageCustomizedUi.h"

#include <Protocol/SimplePointer.h>
#include <Protocol/AbsolutePointer.h>


UINTN ScreenWidth, ScreenHeight;  

EFI_GRAPHICS_OUTPUT_PROTOCOL       *GraphicsOutput;
EFI_SIMPLE_POINTER_PROTOCOL        *gMouse;

const UINT8 sASCII[][16] =
{   
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},       //0x00
    {0x00,0x00,0x7E,0x81,0xA5,0x81,0x81,0xBD,0x99,0x81,0x81,0x7E,0x00,0x00,0x00,0x00},       //0x01
    {0x00,0x00,0x7E,0xFF,0xDB,0xFF,0xFF,0xC3,0xE7,0xFF,0xFF,0x7E,0x00,0x00,0x00,0x00},       //0x02
    {0x00,0x00,0x00,0x00,0x6C,0xFE,0xFE,0xFE,0xFE,0x7C,0x38,0x10,0x00,0x00,0x00,0x00},       //0x03
    {0x00,0x00,0x00,0x00,0x10,0x38,0x7C,0xFE,0x7C,0x38,0x10,0x00,0x00,0x00,0x00,0x00},       //0x04
    {0x00,0x00,0x00,0x18,0x3C,0x3C,0xE7,0xE7,0xE7,0x18,0x18,0x3C,0x00,0x00,0x00,0x00},       //0x05
    {0x00,0x00,0x00,0x18,0x3C,0x7E,0xFF,0xFF,0x7E,0x18,0x18,0x3C,0x00,0x00,0x00,0x00},       //0x06
    {0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x3C,0x3C,0x18,0x00,0x00,0x00,0x00,0x00,0x00},       //0x07
    {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE7,0xC3,0xC3,0xE7,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},       //0x08
    {0x00,0x00,0x00,0x00,0x00,0x3C,0x66,0x42,0x42,0x66,0x3C,0x00,0x00,0x00,0x00,0x00},       //0x09
	{0xFF,0xFF,0xFF,0xFF,0xFF,0xC3,0x99,0xBD,0xBD,0x99,0xC3,0xFF,0xFF,0xFF,0xFF,0xFF},       //0x0A
    {0x00,0x00,0x1E,0x0E,0x1A,0x32,0x78,0xCC,0xCC,0xCC,0xCC,0x78,0x00,0x00,0x00,0x00},       //0x0B
    {0x00,0x00,0x3C,0x66,0x66,0x66,0x66,0x3C,0x18,0x7E,0x18,0x18,0x00,0x00,0x00,0x00},       //0x0C
    {0x00,0x00,0x3F,0x33,0x3F,0x30,0x30,0x30,0x30,0x70,0xF0,0xE0,0x00,0x00,0x00,0x00},       //0x0D
    {0x00,0x00,0x7F,0x63,0x7F,0x63,0x63,0x63,0x63,0x67,0xE7,0xE6,0xC0,0x00,0x00,0x00},       //0x0E
    {0x00,0x00,0x00,0x18,0x18,0xDB,0x3C,0xE7,0x3C,0xDB,0x18,0x18,0x00,0x00,0x00,0x00},       //0x0F
    {0x00,0x80,0xC0,0xE0,0xF0,0xF8,0xFE,0xF8,0xF0,0xE0,0xC0,0x80,0x00,0x00,0x00,0x00},       //0x10
    {0x00,0x02,0x06,0x0E,0x1E,0x3E,0xFE,0x3E,0x1E,0x0E,0x06,0x02,0x00,0x00,0x00,0x00},       //0x11
    {0x00,0x00,0x18,0x3C,0x7E,0x18,0x18,0x18,0x7E,0x3C,0x18,0x00,0x00,0x00,0x00,0x00},       //0x12
    {0x00,0x00,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x00,0x66,0x66,0x00,0x00,0x00,0x00},       //0x13
    {0x00,0x00,0x7F,0xDB,0xDB,0xDB,0x7B,0x1B,0x1B,0x1B,0x1B,0x1B,0x00,0x00,0x00,0x00},       //0x14
    {0x00,0x7C,0xC6,0x60,0x38,0x6C,0xC6,0xC6,0x6C,0x38,0x0C,0xC6,0x7C,0x00,0x00,0x00},       //0x15
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFE,0xFE,0xFE,0x00,0x00,0x00,0x00},       //0x16
    {0x00,0x00,0x18,0x3C,0x7E,0x18,0x18,0x18,0x7E,0x3C,0x18,0x7E,0x00,0x00,0x00,0x00},       //0x17
    {0x00,0x00,0x18,0x3C,0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x00},       //0x18
    {0x00,0x00,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x7E,0x3C,0x18,0x00,0x00,0x00,0x00},       //0x19
    {0x00,0x00,0x00,0x00,0x00,0x18,0x0C,0xFE,0x0C,0x18,0x00,0x00,0x00,0x00,0x00,0x00},       //0x1A
    {0x00,0x00,0x00,0x00,0x00,0x30,0x60,0xFE,0x60,0x30,0x00,0x00,0x00,0x00,0x00,0x00},       //0x1B
    {0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xC0,0xC0,0xFE,0x00,0x00,0x00,0x00,0x00,0x00},       //0x1C
    {0x00,0x00,0x00,0x00,0x00,0x28,0x6C,0xFE,0x6C,0x28,0x00,0x00,0x00,0x00,0x00,0x00},       //0x1D
    {0x00,0x00,0x00,0x00,0x10,0x38,0x38,0x7C,0x7C,0xFE,0xFE,0x00,0x00,0x00,0x00,0x00},       //0x1E
    {0x00,0x00,0x00,0x00,0xFE,0xFE,0x7C,0x7C,0x38,0x38,0x10,0x00,0x00,0x00,0x00,0x00},       //0x1F
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},       //0x20' '
    {0x00,0x00,0x18,0x3C,0x3C,0x3C,0x18,0x18,0x18,0x00,0x18,0x18,0x00,0x00,0x00,0x00},       //0x21'!'
    {0x00,0x66,0x66,0x66,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},       //0x22'"'
    {0x00,0x00,0x00,0x6C,0x6C,0xFE,0x6C,0x6C,0x6C,0xFE,0x6C,0x6C,0x00,0x00,0x00,0x00},       //0x23'#'
    {0x18,0x18,0x7C,0xC6,0xC2,0xC0,0x7C,0x06,0x06,0x86,0xC6,0x7C,0x18,0x18,0x00,0x00},       //0x24'$'
    {0x00,0x00,0x00,0x00,0xC2,0xC6,0x0C,0x18,0x30,0x60,0xC6,0x86,0x00,0x00,0x00,0x00},       //0x25'%'
    {0x00,0x00,0x38,0x6C,0x6C,0x38,0x76,0xDC,0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,0x00},       //0x26'&'
    {0x00,0x30,0x30,0x30,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},       //0x27'''
    {0x00,0x00,0x0C,0x18,0x30,0x30,0x30,0x30,0x30,0x30,0x18,0x0C,0x00,0x00,0x00,0x00},       //0x28'('
    {0x00,0x00,0x30,0x18,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x18,0x30,0x00,0x00,0x00,0x00},       //0x29')'
    {0x00,0x00,0x00,0x00,0x00,0x66,0x3C,0xFF,0x3C,0x66,0x00,0x00,0x00,0x00,0x00,0x00},       //0x2A'*'
    {0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x7E,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00},       //0x2B'+'
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x18,0x30,0x00,0x00,0x00},       //0x2C','
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},       //0x2D'-'
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00},       //0x2E'.'
    {0x00,0x00,0x00,0x00,0x02,0x06,0x0C,0x18,0x30,0x60,0xC0,0x80,0x00,0x00,0x00,0x00},       //0x2F'/'
    {0x00,0x00,0x38,0x6C,0xC6,0xC6,0xD6,0xD6,0xC6,0xC6,0x6C,0x38,0x00,0x00,0x00,0x00},       //0x30'0'
    {0x00,0x00,0x18,0x38,0x78,0x18,0x18,0x18,0x18,0x18,0x18,0x7E,0x00,0x00,0x00,0x00},       //0x31'1'
    {0x00,0x00,0x7C,0xC6,0x06,0x0C,0x18,0x30,0x60,0xC0,0xC6,0xFE,0x00,0x00,0x00,0x00},       //0x32'2'
    {0x00,0x00,0x7C,0xC6,0x06,0x06,0x3C,0x06,0x06,0x06,0xC6,0x7C,0x00,0x00,0x00,0x00},       //0x33'3'
    {0x00,0x00,0x0C,0x1C,0x3C,0x6C,0xCC,0xFE,0x0C,0x0C,0x0C,0x1E,0x00,0x00,0x00,0x00},       //0x34'4'
    {0x00,0x00,0xFE,0xC0,0xC0,0xC0,0xFC,0x06,0x06,0x06,0xC6,0x7C,0x00,0x00,0x00,0x00},       //0x35'5'
    {0x00,0x00,0x38,0x60,0xC0,0xC0,0xFC,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00},       //0x36'6'
    {0x00,0x00,0xFE,0xC6,0x06,0x06,0x0C,0x18,0x30,0x30,0x30,0x30,0x00,0x00,0x00,0x00},       //0x37'7'
    {0x00,0x00,0x7C,0xC6,0xC6,0xC6,0x7C,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00},       //0x38'8'
    {0x00,0x00,0x7C,0xC6,0xC6,0xC6,0x7E,0x06,0x06,0x06,0x0C,0x78,0x00,0x00,0x00,0x00},       //0x39'9'
    {0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x00},       //0x3A':'
    {0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x18,0x18,0x30,0x00,0x00,0x00,0x00},       //0x3B';'
    {0x00,0x00,0x00,0x06,0x0C,0x18,0x30,0x60,0x30,0x18,0x0C,0x06,0x00,0x00,0x00,0x00},       //0x3C'<'
    {0x00,0x00,0x00,0x00,0x00,0x7E,0x00,0x00,0x7E,0x00,0x00,0x00,0x00,0x00,0x00,0x00},       //0x3D'='
    {0x00,0x00,0x00,0x60,0x30,0x18,0x0C,0x06,0x0C,0x18,0x30,0x60,0x00,0x00,0x00,0x00},       //0x3E'>'
    {0x00,0x00,0x7C,0xC6,0xC6,0x0C,0x18,0x18,0x18,0x00,0x18,0x18,0x00,0x00,0x00,0x00},       //0x3F'?'
    {0x00,0x00,0x00,0x7C,0xC6,0xC6,0xDE,0xDE,0xDE,0xDC,0xC0,0x7C,0x00,0x00,0x00,0x00},       //0x40'@'
    {0x00,0x00,0x10,0x38,0x6C,0xC6,0xC6,0xFE,0xC6,0xC6,0xC6,0xC6,0x00,0x00,0x00,0x00},       //0x41'A'
    {0x00,0x00,0xFC,0x66,0x66,0x66,0x7C,0x66,0x66,0x66,0x66,0xFC,0x00,0x00,0x00,0x00},       //0x42'B'
    {0x00,0x00,0x3C,0x66,0xC2,0xC0,0xC0,0xC0,0xC0,0xC2,0x66,0x3C,0x00,0x00,0x00,0x00},       //0x43'C'
    {0x00,0x00,0xF8,0x6C,0x66,0x66,0x66,0x66,0x66,0x66,0x6C,0xF8,0x00,0x00,0x00,0x00},       //0x44'D'
    {0x00,0x00,0xFE,0x66,0x62,0x68,0x78,0x68,0x60,0x62,0x66,0xFE,0x00,0x00,0x00,0x00},       //0x45'E'
    {0x00,0x00,0xFE,0x66,0x62,0x68,0x78,0x68,0x60,0x60,0x60,0xF0,0x00,0x00,0x00,0x00},       //0x46'F'
    {0x00,0x00,0x3C,0x66,0xC2,0xC0,0xC0,0xDE,0xC6,0xC6,0x66,0x3A,0x00,0x00,0x00,0x00},       //0x47'G'
    {0x00,0x00,0xC6,0xC6,0xC6,0xC6,0xFE,0xC6,0xC6,0xC6,0xC6,0xC6,0x00,0x00,0x00,0x00},       //0x48'H'
    {0x00,0x00,0x3C,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00},       //0x49'I'
    {0x00,0x00,0x1E,0x0C,0x0C,0x0C,0x0C,0x0C,0xCC,0xCC,0xCC,0x78,0x00,0x00,0x00,0x00},       //0x4A'J'
    {0x00,0x00,0xE6,0x66,0x66,0x6C,0x78,0x78,0x6C,0x66,0x66,0xE6,0x00,0x00,0x00,0x00},       //0x4B'K'
    {0x00,0x00,0xF0,0x60,0x60,0x60,0x60,0x60,0x60,0x62,0x66,0xFE,0x00,0x00,0x00,0x00},       //0x4C'L'
    {0x00,0x00,0xC6,0xEE,0xFE,0xFE,0xD6,0xC6,0xC6,0xC6,0xC6,0xC6,0x00,0x00,0x00,0x00},       //0x4D'M'
    {0x00,0x00,0xC6,0xE6,0xF6,0xFE,0xDE,0xCE,0xC6,0xC6,0xC6,0xC6,0x00,0x00,0x00,0x00},       //0x4E'N'
    {0x00,0x00,0x7C,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00},       //0x4F'O'
    {0x00,0x00,0xFC,0x66,0x66,0x66,0x7C,0x60,0x60,0x60,0x60,0xF0,0x00,0x00,0x00,0x00},       //0x50'P'
    {0x00,0x00,0x7C,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xD6,0xDE,0x7C,0x0C,0x0E,0x00,0x00},       //0x51'Q'
    {0x00,0x00,0xFC,0x66,0x66,0x66,0x7C,0x6C,0x66,0x66,0x66,0xE6,0x00,0x00,0x00,0x00},       //0x52'R'
    {0x00,0x00,0x7C,0xC6,0xC6,0x60,0x38,0x0C,0x06,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00},       //0x53'S'
    {0x00,0x00,0x7E,0x7E,0x5A,0x18,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00},       //0x54'T'
    {0x00,0x00,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00},       //0x55'U'
    {0x00,0x00,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x6C,0x38,0x10,0x00,0x00,0x00,0x00},       //0x56'V'
    {0x00,0x00,0xC6,0xC6,0xC6,0xC6,0xD6,0xD6,0xD6,0xFE,0xEE,0x6C,0x00,0x00,0x00,0x00},       //0x57'W'
    {0x00,0x00,0xC6,0xC6,0x6C,0x7C,0x38,0x38,0x7C,0x6C,0xC6,0xC6,0x00,0x00,0x00,0x00},       //0x58'X'
    {0x00,0x00,0x66,0x66,0x66,0x66,0x3C,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00},       //0x59'Y'
    {0x00,0x00,0xFE,0xC6,0x86,0x0C,0x18,0x30,0x60,0xC2,0xC6,0xFE,0x00,0x00,0x00,0x00},       //0x5A'Z'
    {0x00,0x00,0x3C,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x3C,0x00,0x00,0x00,0x00},       //0x5B'['
    {0x00,0x00,0x00,0x80,0xC0,0xE0,0x70,0x38,0x1C,0x0E,0x06,0x02,0x00,0x00,0x00,0x00},       //0x5C'\'
    {0x00,0x00,0x3C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x3C,0x00,0x00,0x00,0x00},       //0x5D']'
    {0x10,0x38,0x6C,0xC6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},       //0x5E'^'
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00},       //0x5F'_'
    {0x30,0x30,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},       //0x60'`'
    {0x00,0x00,0x00,0x00,0x00,0x78,0x0C,0x7C,0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,0x00},       //0x61'a'
    {0x00,0x00,0xE0,0x60,0x60,0x78,0x6C,0x66,0x66,0x66,0x66,0x7C,0x00,0x00,0x00,0x00},       //0x62'b'
    {0x00,0x00,0x00,0x00,0x00,0x7C,0xC6,0xC0,0xC0,0xC0,0xC6,0x7C,0x00,0x00,0x00,0x00},       //0x63'c'
    {0x00,0x00,0x1C,0x0C,0x0C,0x3C,0x6C,0xCC,0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,0x00},       //0x64'd'
    {0x00,0x00,0x00,0x00,0x00,0x7C,0xC6,0xFE,0xC0,0xC0,0xC6,0x7C,0x00,0x00,0x00,0x00},       //0x65'e'
    {0x00,0x00,0x38,0x6C,0x64,0x60,0xF0,0x60,0x60,0x60,0x60,0xF0,0x00,0x00,0x00,0x00},       //0x66'f'
    {0x00,0x00,0x00,0x00,0x00,0x76,0xCC,0xCC,0xCC,0xCC,0xCC,0x7C,0x0C,0xCC,0x78,0x00},       //0x67'g'
    {0x00,0x00,0xE0,0x60,0x60,0x6C,0x76,0x66,0x66,0x66,0x66,0xE6,0x00,0x00,0x00,0x00},       //0x68'h'
    {0x00,0x00,0x18,0x18,0x00,0x38,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00},       //0x69'i'
    {0x00,0x00,0x06,0x06,0x00,0x0E,0x06,0x06,0x06,0x06,0x06,0x06,0x66,0x66,0x3C,0x00},       //0x6A'j'
    {0x00,0x00,0xE0,0x60,0x60,0x66,0x6C,0x78,0x78,0x6C,0x66,0xE6,0x00,0x00,0x00,0x00},       //0x6B'k'
    {0x00,0x00,0x38,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00},       //0x6C'l'
    {0x00,0x00,0x00,0x00,0x00,0xEC,0xFE,0xD6,0xD6,0xD6,0xD6,0xC6,0x00,0x00,0x00,0x00},       //0x6D'm'
    {0x00,0x00,0x00,0x00,0x00,0xDC,0x66,0x66,0x66,0x66,0x66,0x66,0x00,0x00,0x00,0x00},       //0x6E'n'
    {0x00,0x00,0x00,0x00,0x00,0x7C,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00},       //0x6F'o'
    {0x00,0x00,0x00,0x00,0x00,0xDC,0x66,0x66,0x66,0x66,0x66,0x7C,0x60,0x60,0xF0,0x00},       //0x70'p'
    {0x00,0x00,0x00,0x00,0x00,0x76,0xCC,0xCC,0xCC,0xCC,0xCC,0x7C,0x0C,0x0C,0x1E,0x00},       //0x71'q'
    {0x00,0x00,0x00,0x00,0x00,0xDC,0x76,0x66,0x60,0x60,0x60,0xF0,0x00,0x00,0x00,0x00},       //0x72'r'
    {0x00,0x00,0x00,0x00,0x00,0x7C,0xC6,0x60,0x38,0x0C,0xC6,0x7C,0x00,0x00,0x00,0x00},       //0x73's'
    {0x00,0x00,0x10,0x30,0x30,0xFC,0x30,0x30,0x30,0x30,0x36,0x1C,0x00,0x00,0x00,0x00},       //0x74't'
    {0x00,0x00,0x00,0x00,0x00,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,0x00},       //0x75'u'
    {0x00,0x00,0x00,0x00,0x00,0x66,0x66,0x66,0x66,0x66,0x3C,0x18,0x00,0x00,0x00,0x00},       //0x76'v'
    {0x00,0x00,0x00,0x00,0x00,0xC6,0xC6,0xD6,0xD6,0xD6,0xFE,0x6C,0x00,0x00,0x00,0x00},       //0x77'w'
    {0x00,0x00,0x00,0x00,0x00,0xC6,0x6C,0x38,0x38,0x38,0x6C,0xC6,0x00,0x00,0x00,0x00},       //0x78'x'
    {0x00,0x00,0x00,0x00,0x00,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7E,0x06,0x0C,0xF8,0x00},       //0x79'y'
    {0x00,0x00,0x00,0x00,0x00,0xFE,0xCC,0x18,0x30,0x60,0xC6,0xFE,0x00,0x00,0x00,0x00},       //0x7A'z'
    {0x00,0x00,0x0E,0x18,0x18,0x18,0x70,0x18,0x18,0x18,0x18,0x0E,0x00,0x00,0x00,0x00},       //0x7B'{'
    {0x00,0x00,0x18,0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x00},       //0x7C'|'
    {0x00,0x00,0x70,0x18,0x18,0x18,0x0E,0x18,0x18,0x18,0x18,0x70,0x00,0x00,0x00,0x00},       //0x7D'}'
    {0x00,0x00,0x76,0xDC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},       //0x7E'~'
    {0x00,0x00,0x00,0x00,0x10,0x38,0x6C,0xC6,0xC6,0xC6,0xFE,0x00,0x00,0x00,0x00,0x00},       //0x7F''
    {0x00,0x00,0x3C,0x66,0xC2,0xC0,0xC0,0xC0,0xC2,0x66,0x3C,0x0C,0x06,0x7C,0x00,0x00},       //0x80
    {0x00,0x00,0xCC,0x00,0x00,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,0x00},       //0x81
    {0x00,0x0C,0x18,0x30,0x00,0x7C,0xC6,0xFE,0xC0,0xC0,0xC6,0x7C,0x00,0x00,0x00,0x00},       //0x82
    {0x00,0x10,0x38,0x6C,0x00,0x78,0x0C,0x7C,0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,0x00},       //0x83
    {0x00,0x00,0xCC,0x00,0x00,0x78,0x0C,0x7C,0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,0x00},       
};

/* chinese char */
const UINT8 sChinese[][32] =
{   
    {0x04,0x80,0x0E,0xA0,0x78,0x90,0x08,0x90,0x08,0x84,0xFF,0xFE,0x08,0x80,0x08,0x90,
	 0x0A,0x90,0x0C,0x60,0x18,0x40,0x68,0xA0,0x09,0x20,0x0A,0x14,0x28,0x14,0x10,0x0C}      
};

void CopyColorIntoBuffer(UINT8 *pBuffer, EFI_GRAPHICS_OUTPUT_BLT_PIXEL color, UINT16 x0, UINT16 y0)
{
    pBuffer[y0 * ScreenWidth * 4 + x0 * 4]     = color.Blue;
    pBuffer[y0 * ScreenWidth * 4 + x0 * 4 + 1] = color.Green;
    pBuffer[y0 * ScreenWidth * 4 + x0 * 4 + 2] = color.Red;
    pBuffer[y0 * ScreenWidth * 4 + x0 * 4 + 3] = color.Reserved;

}

void CopyColorIntoBuffer2(UINT8 *pBuffer, EFI_GRAPHICS_OUTPUT_BLT_PIXEL color, UINT16 x0, UINT16 y0)
{
    pBuffer[y0 * 8 * 4 + x0 * 4]     = color.Blue;
    pBuffer[y0 * 8 * 4 + x0 * 4 + 1] = color.Green;
    pBuffer[y0 * 8 * 4 + x0 * 4 + 2] = color.Red;
    pBuffer[y0 * 8 * 4 + x0 * 4 + 3] = color.Reserved;

}

	    
EFI_STATUS DrawPoint(IN EFI_GRAPHICS_OUTPUT_PROTOCOL  *GraphicsOutput,
    IN UINTN x,
    IN UINTN y,
    IN UINTN Width,
    IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL PixelColor)
{
    EFI_STATUS Status;

    Status = GraphicsOutput->Blt(GraphicsOutput,
                				 &PixelColor,
                				 EfiBltVideoFill,
                				 0, 0,
                				 x, y,
                				 Width, Width,
                				 0);        
                
    return Status;
}

INT32 abs(INT32 v)
{
	return (v < 0) ? -v : v;
}

EFI_STATUS DrawLineIntoBuffer(UINT8 *pBuffer,
        IN UINTN x0, UINTN y0, UINTN x1, UINTN y1, 
        IN UINTN BorderWidth,
        IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL BorderColor)
{

    INT32 dx  = abs((int)(x1 - x0));
    INT32 sx  = x0 < x1 ? 1 : -1;
    INT32 dy  = abs((int)(y1-y0)), sy = y0 < y1 ? 1 : -1;
    INT32 err = ( dx > dy ? dx : -dy) / 2, e2;
    
    for(;;)
    {    
        CopyColorIntoBuffer(pBuffer, BorderColor, x0, y0);
        
        if (x0==x1 && y0==y1) break;
    
        e2 = err;
    
        if (e2 > -dx) { err -= dy; x0 += sx; }
        if (e2 <  dy) { err += dx; y0 += sy; }
    }
    return EFI_SUCCESS;
}


EFI_STATUS DrawLine(IN EFI_GRAPHICS_OUTPUT_PROTOCOL  *GraphicsOutput,
        IN UINTN x0, UINTN y0, UINTN x1, UINTN y1, 
        IN UINTN BorderWidth,
        IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL BorderColor)
{

    INT32 dx  = abs((int)(x1 - x0));
    INT32 sx  = x0 < x1 ? 1 : -1;
    INT32 dy  = abs((int)(y1-y0)), sy = y0 < y1 ? 1 : -1;
    INT32 err = ( dx > dy ? dx : -dy) / 2, e2;
    
    for(;;)
    {    
        DrawPoint(GraphicsOutput, x0, y0, BorderWidth, BorderColor);
    
        if (x0==x1 && y0==y1) break;
    
        e2 = err;
    
        if (e2 > -dx) { err -= dy; x0 += sx; }
        if (e2 <  dy) { err += dx; y0 += sy; }
    }
    return EFI_SUCCESS;
}


EFI_STATUS DrawAsciiCharIntoBuffer(UINT8 *pBuffer,
        IN UINTN x0, UINTN y0,UINT8 c,
        IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL BorderColor)
{
    INT8 i;
    UINT8 d;
    
	for(i = 0; i < 16; i++)
	{
		d = sASCII[c][i];
		
		if ((d & 0x80) != 0) 
		    CopyColorIntoBuffer(pBuffer, BorderColor, x0 + 0, y0 + i); 
		
		if ((d & 0x40) != 0) 
            CopyColorIntoBuffer(pBuffer, BorderColor, x0 + 1, y0 + i);
		
		if ((d & 0x20) != 0) 
		    CopyColorIntoBuffer(pBuffer, BorderColor, x0 + 2, y0 + i);
		
		if ((d & 0x10) != 0) 
		    CopyColorIntoBuffer(pBuffer, BorderColor, x0 + 3, y0 + i);
		
		if ((d & 0x08) != 0) 
		    CopyColorIntoBuffer(pBuffer, BorderColor, x0 + 4, y0 + i);
		
		if ((d & 0x04) != 0) 
		    CopyColorIntoBuffer(pBuffer, BorderColor, x0 + 5, y0 + i);
		
		if ((d & 0x02) != 0) 
		    CopyColorIntoBuffer(pBuffer, BorderColor, x0 + 6, y0 + i);
		
		if ((d & 0x01) != 0) 
		    CopyColorIntoBuffer(pBuffer, BorderColor, x0 + 7, y0 + i);
		
	}
	
    return EFI_SUCCESS;
}


EFI_STATUS DrawAsciiChar(IN EFI_GRAPHICS_OUTPUT_PROTOCOL  *GraphicsOutput,
        IN UINTN x0, UINTN y0,UINT8 c,
        IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL BorderColor)
{
    INT8 i;
    UINT8 d;
    
	for(i = 0; i < 16; i++)
	{
		d = sASCII[c][i];
		
		if ((d & 0x80) != 0) 
		{ DrawPoint(GraphicsOutput, x0 + 0, y0 + i, 1, BorderColor); }
		
		if ((d & 0x40) != 0) 
		{ DrawPoint(GraphicsOutput, x0 + 1, y0 + i, 1, BorderColor); }
		
		if ((d & 0x20) != 0) 
		{ DrawPoint(GraphicsOutput, x0 + 2, y0 + i, 1, BorderColor); }
		
		if ((d & 0x10) != 0) 
		{ DrawPoint(GraphicsOutput, x0 + 3, y0 + i, 1, BorderColor); }
		
		if ((d & 0x08) != 0) 
		{ DrawPoint(GraphicsOutput, x0 + 4, y0 + i, 1, BorderColor); }
		
		if ((d & 0x04) != 0) 
		{ DrawPoint(GraphicsOutput, x0 + 5, y0 + i, 1, BorderColor); }
		
		if ((d & 0x02) != 0) 
		{ DrawPoint(GraphicsOutput, x0 + 6, y0 + i, 1, BorderColor); }
		
		if ((d & 0x01) != 0) 
		{ DrawPoint(GraphicsOutput, x0 + 7, y0 + i, 1, BorderColor); }
		
	}
	
    return EFI_SUCCESS;
}

EFI_STATUS DrawAsciiCharUseBuffer(IN EFI_GRAPHICS_OUTPUT_PROTOCOL  *GraphicsOutput,
        IN UINTN x0, UINTN y0, UINT8 c,
        IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL BorderColor)
{
    INT16 i;
    UINT8 d;
    UINT8 pBuffer[16 * 8 * 4];

    for(i = 0; i < 16 * 8 * 4; i++)
	{
	    pBuffer[i] = 0x00;
	}
    
	for(i = 0; i < 16; i++)
	{
		d = sASCII[c][i];
		
		if ((d & 0x80) != 0) 
		    CopyColorIntoBuffer2(pBuffer, BorderColor, 0, i); 
		
		if ((d & 0x40) != 0) 
            CopyColorIntoBuffer2(pBuffer, BorderColor, 1, i);
		
		if ((d & 0x20) != 0) 
		    CopyColorIntoBuffer2(pBuffer, BorderColor, 2, i);
		
		if ((d & 0x10) != 0) 
		    CopyColorIntoBuffer2(pBuffer, BorderColor, 3, i);
		
		if ((d & 0x08) != 0) 
		    CopyColorIntoBuffer2(pBuffer, BorderColor, 4, i);
		
		if ((d & 0x04) != 0) 
		    CopyColorIntoBuffer2(pBuffer, BorderColor, 5, i);
		
		if ((d & 0x02) != 0) 
		    CopyColorIntoBuffer2(pBuffer, BorderColor, 6, i);
		
		if ((d & 0x01) != 0) 
		    CopyColorIntoBuffer2(pBuffer, BorderColor, 7, i);
		
	}
	
    GraphicsOutput->Blt(GraphicsOutput, 
                        (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) pBuffer,
                        EfiBltBufferToVideo,
                        0, 0, 
                        x0, y0, 
                        8, 16, 
                        0);   
                
    return EFI_SUCCESS;
}


EFI_STATUS DrawAsciiCharString(IN EFI_GRAPHICS_OUTPUT_PROTOCOL  *GraphicsOutput,
        IN UINTN x0, UINTN y0,UINT8 *c,
        IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color)
{    
    UINT8 i;
    for (i = 0; i < 20; i++)
        DrawAsciiCharUseBuffer(GraphicsOutput, 20 + (i - 40) * 8, 120, c[i], Color);

    return EFI_SUCCESS;
}


// Draw 8 X 16 point
EFI_STATUS Draw8_16(IN EFI_GRAPHICS_OUTPUT_PROTOCOL  *GraphicsOutput,UINT8 d,
        IN UINTN x0, UINTN y0,
        UINT8 width,
        IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color)
{
    if ((d & 0x80) != 0) 
      DrawPoint(GraphicsOutput, x0 + 0, y0, 1, Color); 
    
    if ((d & 0x40) != 0) 
      DrawPoint(GraphicsOutput, x0 + 1, y0, 1, Color); 
    
    if ((d & 0x20) != 0) 
      DrawPoint(GraphicsOutput, x0 + 2, y0, 1, Color); 
   
    if ((d & 0x10) != 0) 
      DrawPoint(GraphicsOutput, x0 + 3, y0, 1, Color); 
    
    if ((d & 0x08) != 0) 
      DrawPoint(GraphicsOutput, x0 + 4, y0, 1, Color); 
   
    if ((d & 0x04) != 0) 
      DrawPoint(GraphicsOutput, x0 + 5, y0, 1, Color); 
    
    if ((d & 0x02) != 0) 
      DrawPoint(GraphicsOutput, x0 + 6, y0, 1, Color); 
    
    if ((d & 0x01) != 0) 
      DrawPoint(GraphicsOutput, x0 + 7, y0, 1, Color); 

    return EFI_SUCCESS;
}

// Draw 8 X 16 point
EFI_STATUS Draw8_16IntoBuffer(UINT8 *pBuffer,UINT8 d,
        IN UINTN x0, UINTN y0,
        UINT8 width,
        IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color)
{
    if ((d & 0x80) != 0) 
        CopyColorIntoBuffer(pBuffer, Color, x0 + 0, y0 ); 
    
    if ((d & 0x40) != 0) 
        CopyColorIntoBuffer(pBuffer, Color, x0 + 1, y0 );
    
    if ((d & 0x20) != 0) 
        CopyColorIntoBuffer(pBuffer, Color, x0 + 2, y0 );
    
    if ((d & 0x10) != 0) 
        CopyColorIntoBuffer(pBuffer, Color, x0 + 3, y0 );
    
    if ((d & 0x08) != 0) 
        CopyColorIntoBuffer(pBuffer, Color, x0 + 4, y0 );
    
    if ((d & 0x04) != 0) 
        CopyColorIntoBuffer(pBuffer, Color, x0 + 5, y0 );
    
    if ((d & 0x02) != 0) 
        CopyColorIntoBuffer(pBuffer, Color, x0 + 6, y0 );
    
    if ((d & 0x01) != 0) 
        CopyColorIntoBuffer(pBuffer, Color, x0 + 7, y0 );


    return EFI_SUCCESS;
}


EFI_STATUS DrawChineseCharIntoBuffer(UINT8 *pBuffer,
        IN UINTN x0, UINTN y0,UINT8 c,
        IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL BorderColor)
{
    INT8 i;    
    	    
	for(i = 0; i < 32; i += 2)
	{
        Draw8_16IntoBuffer(pBuffer, sChinese[0][i],     x0,     y0 + i / 2, 1, BorderColor);		        
		Draw8_16IntoBuffer(pBuffer, sChinese[0][i + 1], x0 + 8, y0 + i / 2, 1, BorderColor);		
	}
	
    return EFI_SUCCESS;
}


EFI_STATUS DrawChineseChar(IN EFI_GRAPHICS_OUTPUT_PROTOCOL  *GraphicsOutput,
        IN UINTN x0, UINTN y0,UINT8 c,
        IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL BorderColor)
{
    INT8 i;    
    	    
	for(i = 0; i < 32; i += 2)
	{
        Draw8_16(GraphicsOutput, sChinese[0][i],     x0,     y0 + i / 2, 1, BorderColor);		        
		Draw8_16(GraphicsOutput, sChinese[0][i + 1], x0 + 8, y0 + i / 2, 1, BorderColor);		
	}
	
    return EFI_SUCCESS;
}


//Name:  GetKeyEx
//Input:
//Output:
//Descriptor:

EFI_STATUS GetKeyEx(UINT16 *ScanCode, UINT16 *UniChar, UINT32 *ShiftState, EFI_KEY_TOGGLE_STATE * ToggleState)

{

	EFI_STATUS                        Status;
	EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *SimpleEx;
  EFI_KEY_DATA                      KeyData;
  EFI_HANDLE                        *Handles;
  UINTN                             HandleCount;
  UINTN                             HandleIndex;
  UINTN								Index;

  

  Status = gBS->LocateHandleBuffer (
              ByProtocol,
              &gEfiSimpleTextInputExProtocolGuid,
              NULL,
              &HandleCount,
              &Handles
              );

  if(EFI_ERROR (Status))
  	return Status;

  for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) 
  {
    Status = gBS->HandleProtocol (Handles[HandleIndex], &gEfiSimpleTextInputExProtocolGuid, (VOID **) &SimpleEx);
    
    if (EFI_ERROR(Status))	
    	continue;
	else
	{
		gBS->WaitForEvent(1,&(SimpleEx->WaitForKeyEx),&Index);
    	Status = SimpleEx->ReadKeyStrokeEx(SimpleEx,&KeyData);
    	if(!EFI_ERROR(Status))
    	{

    		*ScanCode=KeyData.Key.ScanCode;
  			*UniChar=KeyData.Key.UnicodeChar;
  			*ShiftState=KeyData.KeyState.KeyShiftState;
  			*ToggleState=KeyData.KeyState.KeyToggleState;
  			return EFI_SUCCESS;
  		}
    }

  }	 

  return Status;

}


// fill into rectangle
void RectangleFillIntoBuffer(UINT8 *pBuffer,
        IN UINTN x0, UINTN y0, UINTN x1, UINTN y1, 
        IN UINTN BorderWidth,
        IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color)
{    
	UINT32 i = 0;
	UINT32 j = 0;
    for (j = y0; j < y0 + y1; j++) 
    {
        for (i = x0; i < x0 + x1; i++) 
        {
            pBuffer[(j * ScreenWidth + i) * 4]     =  Color.Blue; //Blue   
            pBuffer[(j * ScreenWidth + i) * 4 + 1] =  Color.Green; //Green 
            pBuffer[(j * ScreenWidth + i) * 4 + 2] =  Color.Red; //Red  
            pBuffer[(j * ScreenWidth + i) * 4 + 3] =  Color.Reserved;
        }
    }

}

//static UINT8 Counter = 0;

EFI_EVENT mProgress1Event;
EFI_EVENT mProgress2Event;

VOID EFIAPI TimeoutSelf(
	IN EFI_EVENT Event,
	IN VOID           *Context
	)
{
    
    DEBUG ((EFI_D_INFO, "Charles SignalEvent mProgress1Event  ...\n"));
    gBS->SignalEvent (mProgress1Event);

    //DEBUG ((EFI_D_INFO, "Charles SignalEvent mProgress2Event  ...\n"));
    //gBS->SignalEvent (mProgress2Event);
	return;
}

EFI_STATUS DisplayMouseMode(void)
{
	Print(L"Print Current Mode of Mouse:\n");
	Print(L"::ResolutionX=0x%x\n",gMouse->Mode->ResolutionX);
	Print(L"::ResolutionY=%d\n",gMouse->Mode->ResolutionY);
	Print(L"::ResolutionZ=%d\n",gMouse->Mode->ResolutionZ);
	Print(L"::LeftButton=%d\n",gMouse->Mode->LeftButton);
	Print(L"::RightButton=%d\n",gMouse->Mode->RightButton);
	
	return EFI_SUCCESS;
}

//Name: ListMouseMessage
//Input: gMouse
//Output: 
//Descriptor:
void ListMouseMessage(void)
{
	
		
}


EFI_STATUS MouseInit()
{
	EFI_STATUS                         Status;
	EFI_HANDLE                         *PointerHandleBuffer = NULL;
	UINTN                              HandleIndex = 0;
	UINTN                              HandleCount = 0;
	//get the handles which supports
	Status = gBS->LocateHandleBuffer(
                             		ByProtocol,
                             		&gEfiSimplePointerProtocolGuid,
                             		NULL,
                             		&HandleCount,
                             		&PointerHandleBuffer
                             		);
		
	if (EFI_ERROR(Status))	return Status;		//unsupport
	
	for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++)
	{
		Status = gBS->HandleProtocol(
			PointerHandleBuffer[HandleIndex],
			&gEfiSimplePointerProtocolGuid,
			(VOID**)&gMouse);
			
		if (EFI_ERROR(Status))	
		    continue;
		
		else
		{
		    Print(L"Call LocateMouse, Find device!\n");
    		DisplayMouseMode();
    		//ListMouseMessage(); 
			return EFI_SUCCESS;
		}
	}	
	
    return EFI_SUCCESS;

}


//static int process1_i = 0;
// for keyboard input event
STATIC
VOID
EFIAPI
Process1 (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
    UINT16 scanCode = 0;
    UINT16 uniChar = 0;
    UINT32 shiftState, process1_i = 0;
    UINT32 count = 0;
    EFI_STATUS Status;

    EFI_KEY_TOGGLE_STATE toggleState;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color;
	//UINT8 *ScreenBuff = NULL;
	//UINT32 i = 0;
	//UINT32 j = 0;

    //ScreenBuff = (UINT8 *)AllocatePool(ScreenWidth * ScreenHeight * 4); 

	++process1_i;
	Color.Blue = 0xFF;
	Color.Red = 0xFF;
	Color.Green = 0xFF;
	Color.Reserved = 0x66;
	
	Status = GetKeyEx(&scanCode, &uniChar, &shiftState, &toggleState);
    if (EFI_ERROR (Status)) 
    {
    	//Print(L"Call GetKeyEx() Error!\n");
    }
    else
    {

    	//Print(L"NO.%08d\n", count);
    	++count;
        ++process1_i;
    	//Print(L"  ScanCode=%04x", scanCode);
    	//Print(L"  ShiftState=%08x", shiftState);
    	//Print(L"  ToggleState=%02x", toggleState);
    	//Print(L"\n");
	    DrawAsciiCharUseBuffer(GraphicsOutput, 20 + process1_i * 8, 40, uniChar, Color);
    	//Print(L"  UnicodeChar=%04x", uniChar);
        /*
        for (j = 0; j < ScreenHeight - 100; j++) 
        {
            for (i = 0; i < ScreenWidth; i++) 
            {
                ScreenBuff[(j * ScreenWidth + i) * 4]     =  0; //Blue   
                ScreenBuff[(j * ScreenWidth + i) * 4 + 1] =  0; //Green 
                ScreenBuff[(j * ScreenWidth + i) * 4 + 2] =  uniChar; //Red  
                ScreenBuff[(j * ScreenWidth + i) * 4 + 3] =  0;
            }
        }*/

    }
    /*
    GraphicsOutput->Blt(
                GraphicsOutput, 
                (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) ScreenBuff,
                EfiBltBufferToVideo,
                0, 0, 
                0, 0, 
                ScreenWidth, ScreenHeight, 0);   
*/
}

 static int process2_i = 1;
STATIC
VOID
EFIAPI
Process2 (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color;

	//++process2_i;

	Color.Blue = 0xFF;
	Color.Red = 0xFF;
	Color.Green = 0xFF;
	Color.Reserved = 0x66;
	
	//for (;;i++)
	    //DrawAsciiCharUseBuffer(GraphicsOutput, 20 + process2_i * 8, 60, process2_i + 60, Color);

    EFI_STATUS Status;
	UINTN Index;
	EFI_SIMPLE_POINTER_STATE State;
	//INTN i;
	
    //Print(L"ResolutionX: %08d", gMouse->Mode->ResolutionX);
    
	//for(i = 0;i < 10000; i++)
	//{
		
	Status = gMouse->GetState(gMouse, &State);
    if (Status == EFI_DEVICE_ERROR)
    {
        return ;
    }
	
	//x:
	//Print(L"X: %08x Y: %08x Z: %08x L: %d R:%d\n", State.RelativeMovementX, State.RelativeMovementY, State.RelativeMovementZ,
	//                           State.LeftButton, State.RightButton);
	DrawAsciiCharUseBuffer(GraphicsOutput, 20 + process2_i * 8, 60, gMouse->Mode->ResolutionX, Color);

    INT32 MovementX;
    //INT32 MovementY;

    
    //X
	if (State.RelativeMovementX >= 0xFFFF)
	{
	    MovementX = State.RelativeMovementX / 0xFFFF;

	    DEBUG ((EFI_D_INFO, "X <-: %08x ", MovementX));
	    DrawAsciiCharUseBuffer(GraphicsOutput, 20 + process2_i * 8, 60, 'L', Color);
    }
    else
    {
	    DrawAsciiCharUseBuffer(GraphicsOutput, 20 + process2_i * 8, 60, 'R', Color);
    }


    //Y
	if (State.RelativeMovementY >= 0xFFFF)
	{
	    //MovementY = State.RelativeMovementY / 0xFFFF;

	    DrawAsciiCharUseBuffer(GraphicsOutput, 20 + process2_i * 8 + 8, 60, 'U', Color);

    }
    else
    {
        DEBUG ((EFI_D_INFO, "Y Move down: %08x ", State.RelativeMovementY));
	    DrawAsciiCharUseBuffer(GraphicsOutput, 20 + process2_i * 8 + 8, 60, 'D', Color);
    }

    //Button
    if (State.LeftButton == 0x01)
    {
        DEBUG ((EFI_D_INFO, "Left button clicked"));
	    DrawAsciiCharUseBuffer(GraphicsOutput, 20 + process2_i * 8 + 16, 60, 'E', Color);
    }
    
    if (State.RightButton == 0x01)
    {
	    DrawAsciiCharUseBuffer(GraphicsOutput, 20 + process2_i * 8 + 16, 60, 'R', Color);
    }
    
    DEBUG ((EFI_D_INFO, "\n"));
    
	gBS->WaitForEvent( 1, &gMouse->WaitForInput, &Index );
	//}	    

}

static int process3_i = 2;
STATIC
VOID
EFIAPI
Process3 (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color;

	++process3_i;

	Color.Blue = 0xFF;
	Color.Red = 0xFF;
	Color.Green = 0xFF;
	Color.Reserved = 0x66;
	
	//for (;;i++)
	DrawAsciiCharUseBuffer(GraphicsOutput, 20 + process3_i * 8, 80, process3_i + 60, Color);

}

 
EFI_STATUS MultiProcessInit ()
{
    EFI_GUID gMultiProcessGuid  = { 0x0579257E, 0x1843, 0x45FB, { 0x83, 0x9D, 0x6B, 0x79, 0x09, 0x38, 0x29, 0xA9 } };

    gBS->CreateEventEx(
                      EVT_NOTIFY_SIGNAL,
                      TPL_NOTIFY,
                      Process3,
                      NULL,
                      &gMultiProcessGuid,
                      &mProgress1Event
                      );

    gBS->CreateEventEx(
                      EVT_NOTIFY_SIGNAL,
                      TPL_NOTIFY,
                      Process1,
                      NULL,
                      &gMultiProcessGuid,
                      &mProgress1Event
                      );
                  
    gBS->CreateEventEx(
                      EVT_NOTIFY_SIGNAL,
                      TPL_NOTIFY,
                      Process2,
                      NULL,
                      &gMultiProcessGuid,
                      &mProgress1Event
                      );
                      

    return EFI_SUCCESS;
}
// https://blog.csdn.net/longsonssss/article/details/80221513


EFI_STATUS TimerCreate()
{
    EFI_STATUS	Status;
	EFI_HANDLE	TimerOne	= NULL;
	//BOOLEAN		ExitMark	= FALSE;
	static const UINTN SecondsToNanoSeconds = 5000000;

	Status = gBS->CreateEvent(
                           		EVT_NOTIFY_SIGNAL | EVT_TIMER,
                           		TPL_CALLBACK,
                           		TimeoutSelf,
                           		NULL,
                           		&TimerOne
                           		);

	if ( EFI_ERROR( Status ) )
	{
		Print( L"Create Event Error! \r\n" );
		return(1);
	}

	Status = gBS->SetTimer(
		TimerOne,
		TimerPeriodic,
		MultU64x32( SecondsToNanoSeconds, 1)
		);

	if ( EFI_ERROR( Status ) )
	{
		Print( L"Set Timer Error! \r\n" );
		return(2);
	}

	while (1)
	{		
	}
	
	gBS->SetTimer( TimerOne, TimerCancel, 0 );
	gBS->CloseEvent( TimerOne );    

	return EFI_SUCCESS;
}

EFI_STATUS ScreenInit(EFI_GRAPHICS_OUTPUT_PROTOCOL   *GraphicsOutput)
{
    UINT8 *ScreenBuff = NULL;
	UINT32 i = 0;
	UINT32 j = 0;
	//UINT32 x = ScreenWidth;
	//UINT32 y = ScreenHeight;
	//UINT8 p[100];
	
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color;
  
    ScreenBuff = (UINT8 *)AllocatePool(ScreenWidth * ScreenHeight * 4); 
    
    for (j = 0; j < ScreenHeight; j++) 
    {
        for (i = 0; i < ScreenWidth; i++) 
        {
            ScreenBuff[(j * ScreenWidth + i) * 4]     =  0; //Blue   
            ScreenBuff[(j * ScreenWidth + i) * 4 + 1] =  0; //Green 
            ScreenBuff[(j * ScreenWidth + i) * 4 + 2] =  0; //Red  
            ScreenBuff[(j * ScreenWidth + i) * 4 + 3] =  0;
        }
    }
    /*
    Color.Red   = 0x00;
    Color.Green = 0x84;
    Color.Blue	= 0x84;
    RectangleFillIntoBuffer(ScreenBuff, 0,     0,      x -  1, y - 29, 1, Color);
    
    Color.Red   = 0xC6;
    Color.Green = 0xC6;
    Color.Blue	= 0xC6;
    RectangleFillIntoBuffer(ScreenBuff, 0,     y - 28, x -  1, y - 28, 1, Color);
    RectangleFillIntoBuffer(ScreenBuff, 0,     y - 26, x -  1, y -  1, 1, Color);
    
    Color.Red   = 0x00;
    Color.Green = 0x00;
    Color.Blue	= 0x00;
    RectangleFillIntoBuffer(ScreenBuff, 2,     y -  3, 59,     y -  3, 1, Color);
    RectangleFillIntoBuffer(ScreenBuff, 60,    y - 24, 60,     y -  3, 1, Color);

    Color.Red   = 0x84;
    Color.Green = 0x84;
    Color.Blue	= 0x84;
    RectangleFillIntoBuffer(ScreenBuff, 3,     y -  4, 59,     y -  4, 1, Color);
    RectangleFillIntoBuffer(ScreenBuff, 59,     y - 23, 59,     y -  5, 1, Color);
    RectangleFillIntoBuffer(ScreenBuff, x - 47, y - 24, x -  4, y - 24, 1, Color);
    RectangleFillIntoBuffer(ScreenBuff, x - 47, y - 23, x - 47, y -  4, 1, Color);
    
	
    Color.Red   = 0xFF;
    Color.Green = 0xFF;
    Color.Blue	= 0xFF;
    RectangleFillIntoBuffer(ScreenBuff, 0,     y - 27, x -  1, y - 27, 1, Color);
    RectangleFillIntoBuffer(ScreenBuff, 3,     y - 24, 59,     y - 24, 1, Color);
        
    Color.Red   = 0xFF;
    Color.Green = 0xFF;
    Color.Blue	= 0xFF;
    RectangleFillIntoBuffer(ScreenBuff, 0, 100, 100, 100, 1, Color);
    
    Color.Red   = 0x00;
    Color.Green = 0x00;
    Color.Blue	= 0xFF;
    RectangleFillIntoBuffer(ScreenBuff, 0, 200, 100, 100, 1, Color);

    
    Color.Red   = 0x00;
    Color.Green = 0xFF;
    Color.Blue	= 0x00;
    RectangleFillIntoBuffer(ScreenBuff, 0, 300, 100, 100, 1, Color);
    
    
    Color.Red   = 0xFF;
    Color.Green = 0x00;
    Color.Blue	= 0x00;
    RectangleFillIntoBuffer(ScreenBuff, 0, 400, 100, 100, 1, Color);
    
    Color.Red  = 0xFF;
    Color.Green = 0x00;
    Color.Blue	= 0xFF;

    DrawLineIntoBuffer(ScreenBuff, 0, 0, 100, 100, 2, Color);
    DrawLineIntoBuffer(ScreenBuff, 100, 0, 0, 100, 1, Color);

    Color.Red  = 0xFF;
    Color.Green = 0xFF;
    Color.Blue	= 0xFF;
*/
    //Display ASCII Char
    //count = 60;
    //for (i = 40; i < 65 + 60; i++)
    //    DrawAsciiCharIntoBuffer(ScreenBuff, 20 + (i - 40) * 8, 20, i, Color);

    DrawChineseCharIntoBuffer(ScreenBuff, 20, 20 + 16, i, Color);
        
    GraphicsOutput->Blt(
                GraphicsOutput, 
                (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) ScreenBuff,
                EfiBltBufferToVideo,
                0, 0, 
                0, 0, 
                ScreenWidth, ScreenHeight, 0);   

    return EFI_SUCCESS;
}

/**
  The user Entry Point for Application. The user code starts with this function
  as the real entry point for the image goes into a library that calls this
  function.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
Main (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
    EFI_HII_HANDLE                     HiiHandle;
    EFI_STATUS                         Status;
    
    //EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color;
    //gBS->SetWatchdogTimer (0x0000, 0x0000, 0x0000, NULL);
    //gST->ConOut->ClearScreen (gST->ConOut);

    //
    // Install customized fonts needed by Front Page
    //
    HiiHandle = ExportFonts ();
    ASSERT (HiiHandle != NULL);


    Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **) &GraphicsOutput);        
    if (EFI_ERROR (Status)) 
    	{
        return EFI_UNSUPPORTED;
    }

    ScreenWidth  = GraphicsOutput->Mode->Info->HorizontalResolution;
    ScreenHeight = GraphicsOutput->Mode->Info->VerticalResolution;

    Print(L"ScreenWidth:%d, ScreenHeight:%d\n\n", ScreenWidth, ScreenHeight);
    
    ScreenInit(GraphicsOutput);
    
    //KeyboardInit(GraphicsOutput);

	//Color.Blue = 0xFF;
	//Color.Red = 0xFF;
	//Color.Green = 0xFF;
	//Color.Reserved = 0x66;
	
    //DrawAsciiCharString(GraphicsOutput, 20 + 8, 100, NULL, Color);
    
	Status = MouseInit();    
	if (EFI_ERROR (Status)) 
    {
        return EFI_UNSUPPORTED;
    }
    
    MultiProcessInit();
    TimerCreate();
    
	
    return EFI_SUCCESS;
}

