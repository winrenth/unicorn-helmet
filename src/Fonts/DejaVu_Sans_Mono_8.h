// Created by http://oleddisplay.squix.ch/ Consider a donation
// In case of problems make sure that you are using the font file with the correct version!
const uint8_t DejaVu_Sans_Mono_8Bitmaps[] PROGMEM = {

    // Bitmap Data:
    0x00, // ' '
    0xF4, // '!'
    0xB4, // '"'
    0x32,0xBE,0xAF,0xB2,0x80, // '#'
    0x27,0xE8,0xE2,0xFC,0x80, // '$'
    0xE5,0x3C,0xF2,0x9C, // '%'
    0x74,0x6B,0xA7, // '&'
    0xC0, // '''
    0x6A,0xA4, // '('
    0x95,0x58, // ')'
    0xAB,0x9D,0x50, // '*'
    0x21,0x3E,0x42,0x00, // '+'
    0xC0, // ','
    0xC0, // '-'
    0x80, // '.'
    0x12,0x24,0x44,0x80, // '/'
    0x69,0xD9,0x96, // '0'
    0xC9,0x25,0xC0, // '1'
    0xE1,0x12,0x4F, // '2'
    0xE1,0x16,0x1F, // '3'
    0x26,0x6A,0xF2, // '4'
    0xF8,0xE1,0x1E, // '5'
    0x7C,0x8F,0x97, // '6'
    0xF1,0x22,0x24, // '7'
    0x69,0x96,0x9F, // '8'
    0xE9,0xF1,0x3E, // '9'
    0x90, // ':'
    0x98, // ';'
    0x1E,0xC3, // '<'
    0xF0,0xF0, // '='
    0x87,0x3C, // '>'
    0xED,0x20,0x80, // '?'
    0x69,0xBB,0x86, // '@'
    0x66,0x66,0xF9, // 'A'
    0xE9,0x9E,0x9F, // 'B'
    0x78,0x88,0x87, // 'C'
    0xE9,0x99,0x9E, // 'D'
    0xF8,0x8F,0x8F, // 'E'
    0xF8,0x8F,0x88, // 'F'
    0x78,0x8B,0x97, // 'G'
    0x99,0x9F,0x99, // 'H'
    0xE9,0x25,0xC0, // 'I'
    0x64,0x93,0xC0, // 'J'
    0x9A,0xCA,0xA9, // 'K'
    0x88,0x88,0x8F, // 'L'
    0x9F,0xFF,0x99, // 'M'
    0x9D,0xDB,0xB9, // 'N'
    0x69,0x99,0x96, // 'O'
    0xF9,0xF8,0x88, // 'P'
    0x69,0x99,0x96,0x10, // 'Q'
    0xF4,0xB9,0x69,0x44, // 'R'
    0x78,0xE3,0x1F, // 'S'
    0xF9,0x08,0x42,0x10, // 'T'
    0x99,0x99,0x96, // 'U'
    0x99,0x66,0x66, // 'V'
    0x8C,0x6A,0xA5,0x28, // 'W'
    0x96,0x66,0x69, // 'X'
    0x8A,0x88,0x42,0x10, // 'Y'
    0xF2,0x24,0x4F, // 'Z'
    0xEA,0xAC, // '['
    0x84,0x44,0x22,0x10, // '\'
    0xD5,0x5C, // ']'
    0x69, // '^'
    0xF8, // '_'
    0x80, // '`'
    0xFF,0x9F, // 'a'
    0x88,0x8E,0x99,0xE0, // 'b'
    0x72,0x30, // 'c'
    0x11,0x17,0x99,0x70, // 'd'
    0x7F,0x87, // 'e'
    0x34,0x4F,0x44,0x40, // 'f'
    0x79,0x97,0x1E, // 'g'
    0x88,0x8F,0x99,0x90, // 'h'
    0x40,0x64,0xB8, // 'i'
    0x20,0x32,0x49,0xE0, // 'j'
    0x88,0x8B,0xEE,0xB0, // 'k'
    0xC4,0x44,0x44,0x70, // 'l'
    0xFD,0x6B,0x50, // 'm'
    0xF9,0x99, // 'n'
    0x69,0x96, // 'o'
    0xE9,0x9E,0x88, // 'p'
    0x79,0x97,0x11, // 'q'
    0xF2,0x40, // 'r'
    0xFE,0x1F, // 's'
    0x4F,0x44,0x70, // 't'
    0x99,0x9F, // 'u'
    0x96,0x66, // 'v'
    0x8D,0x5C,0xA0, // 'w'
    0xF6,0x6F, // 'x'
    0x96,0x64,0x4C, // 'y'
    0xF6,0x4F, // 'z'
    0x69,0x44,0x98, // '{'
    0xFF, // '|'
    0xC9,0x14,0xB0 // '}'
};
const GFXglyph DejaVu_Sans_Mono_8Glyphs[] PROGMEM = {
// bitmapOffset, width, height, xAdvance, xOffset, yOffset
      {     0,   1,   1,   6,    0,    0 }, // ' '
      {     1,   1,   6,   6,    2,   -6 }, // '!'
      {     2,   3,   2,   6,    1,   -6 }, // '"'
      {     3,   5,   7,   6,    0,   -7 }, // '#'
      {     8,   5,   7,   6,    0,   -6 }, // '$'
      {    13,   5,   6,   6,    0,   -6 }, // '%'
      {    17,   4,   6,   6,    1,   -6 }, // '&'
      {    20,   1,   2,   6,    2,   -6 }, // '''
      {    21,   2,   7,   6,    2,   -7 }, // '('
      {    23,   2,   7,   6,    1,   -7 }, // ')'
      {    25,   5,   4,   6,    0,   -6 }, // '*'
      {    28,   5,   5,   6,    0,   -5 }, // '+'
      {    32,   1,   2,   6,    2,   -1 }, // ','
      {    33,   2,   1,   6,    1,   -3 }, // '-'
      {    34,   1,   1,   6,    2,   -1 }, // '.'
      {    35,   4,   7,   6,    0,   -6 }, // '/'
      {    39,   4,   6,   6,    1,   -6 }, // '0'
      {    42,   3,   6,   6,    1,   -6 }, // '1'
      {    45,   4,   6,   6,    1,   -6 }, // '2'
      {    48,   4,   6,   6,    1,   -6 }, // '3'
      {    51,   4,   6,   6,    1,   -6 }, // '4'
      {    54,   4,   6,   6,    1,   -6 }, // '5'
      {    57,   4,   6,   6,    1,   -6 }, // '6'
      {    60,   4,   6,   6,    1,   -6 }, // '7'
      {    63,   4,   6,   6,    1,   -6 }, // '8'
      {    66,   4,   6,   6,    1,   -6 }, // '9'
      {    69,   1,   4,   6,    2,   -4 }, // ':'
      {    70,   1,   5,   6,    2,   -4 }, // ';'
      {    71,   4,   4,   6,    1,   -4 }, // '<'
      {    73,   4,   3,   6,    0,   -4 }, // '='
      {    75,   4,   4,   6,    1,   -4 }, // '>'
      {    77,   3,   6,   6,    1,   -6 }, // '?'
      {    80,   4,   6,   6,    1,   -5 }, // '@'
      {    83,   4,   6,   6,    1,   -6 }, // 'A'
      {    86,   4,   6,   6,    1,   -6 }, // 'B'
      {    89,   4,   6,   6,    1,   -6 }, // 'C'
      {    92,   4,   6,   6,    1,   -6 }, // 'D'
      {    95,   4,   6,   6,    1,   -6 }, // 'E'
      {    98,   4,   6,   6,    1,   -6 }, // 'F'
      {   101,   4,   6,   6,    1,   -6 }, // 'G'
      {   104,   4,   6,   6,    1,   -6 }, // 'H'
      {   107,   3,   6,   6,    1,   -6 }, // 'I'
      {   110,   3,   6,   6,    1,   -6 }, // 'J'
      {   113,   4,   6,   6,    1,   -6 }, // 'K'
      {   116,   4,   6,   6,    1,   -6 }, // 'L'
      {   119,   4,   6,   6,    1,   -6 }, // 'M'
      {   122,   4,   6,   6,    1,   -6 }, // 'N'
      {   125,   4,   6,   6,    1,   -6 }, // 'O'
      {   128,   4,   6,   6,    1,   -6 }, // 'P'
      {   131,   4,   7,   6,    1,   -6 }, // 'Q'
      {   135,   5,   6,   6,    1,   -6 }, // 'R'
      {   139,   4,   6,   6,    1,   -6 }, // 'S'
      {   142,   5,   6,   6,    0,   -6 }, // 'T'
      {   146,   4,   6,   6,    1,   -6 }, // 'U'
      {   149,   4,   6,   6,    1,   -6 }, // 'V'
      {   152,   5,   6,   6,    0,   -6 }, // 'W'
      {   156,   4,   6,   6,    1,   -6 }, // 'X'
      {   159,   5,   6,   6,    0,   -6 }, // 'Y'
      {   163,   4,   6,   6,    1,   -6 }, // 'Z'
      {   166,   2,   7,   6,    2,   -7 }, // '['
      {   168,   4,   7,   6,    0,   -6 }, // '\'
      {   172,   2,   7,   6,    1,   -7 }, // ']'
      {   174,   4,   2,   6,    0,   -6 }, // '^'
      {   175,   5,   1,   6,    0,    1 }, // '_'
      {   176,   2,   1,   6,    1,   -6 }, // '`'
      {   177,   4,   4,   6,    1,   -4 }, // 'a'
      {   179,   4,   7,   6,    1,   -7 }, // 'b'
      {   183,   3,   4,   6,    1,   -4 }, // 'c'
      {   185,   4,   7,   6,    1,   -7 }, // 'd'
      {   189,   4,   4,   6,    1,   -4 }, // 'e'
      {   191,   4,   7,   6,    1,   -7 }, // 'f'
      {   195,   4,   6,   6,    1,   -4 }, // 'g'
      {   198,   4,   7,   6,    1,   -7 }, // 'h'
      {   202,   3,   7,   6,    1,   -7 }, // 'i'
      {   205,   3,   9,   6,    0,   -7 }, // 'j'
      {   209,   4,   7,   6,    1,   -7 }, // 'k'
      {   213,   4,   7,   6,    1,   -7 }, // 'l'
      {   217,   5,   4,   6,    1,   -4 }, // 'm'
      {   220,   4,   4,   6,    1,   -4 }, // 'n'
      {   222,   4,   4,   6,    1,   -4 }, // 'o'
      {   224,   4,   6,   6,    1,   -4 }, // 'p'
      {   227,   4,   6,   6,    1,   -4 }, // 'q'
      {   230,   3,   4,   6,    1,   -4 }, // 'r'
      {   232,   4,   4,   6,    1,   -4 }, // 's'
      {   234,   4,   5,   6,    1,   -5 }, // 't'
      {   237,   4,   4,   6,    1,   -4 }, // 'u'
      {   239,   4,   4,   6,    1,   -4 }, // 'v'
      {   241,   5,   4,   6,    0,   -4 }, // 'w'
      {   244,   4,   4,   6,    1,   -4 }, // 'x'
      {   246,   4,   6,   6,    1,   -4 }, // 'y'
      {   249,   4,   4,   6,    1,   -4 }, // 'z'
      {   251,   3,   7,   6,    1,   -7 }, // '{'
      {   254,   1,   8,   6,    2,   -7 }, // '|'
      {   255,   3,   7,   6,    1,   -7 } // '}'
};
const GFXfont DejaVu_Sans_Mono_8 PROGMEM = {
(uint8_t  *)DejaVu_Sans_Mono_8Bitmaps,(GFXglyph *)DejaVu_Sans_Mono_8Glyphs,0x20, 0x7E, 10};