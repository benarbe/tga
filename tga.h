#ifndef _TGA_H
    #define _TGA_H
#include <stdint.h>   
#include <stdio.h>
typedef struct _TgaHeader {
    uint8_t idlength;           /* 00h Size of Image ID field */
    uint8_t colourmaptype;      /* 01h Color map type */
    uint8_t datatypecode;       /* 02h Image type code */
    uint16_t colourmaporigin;   /* 03h Color map origin */
    uint16_t colourmaplength;   /* 05h Color map length */
    uint8_t colourmapdepth;     /* 07h Depth of color map entries */
    uint16_t x_origin;          /* 08h X origin of image */
    uint16_t y_origin;          /* 0Ah Y origin of image */
    uint16_t width;             /* 0Ch Width of image */
    uint16_t height;            /* 0Eh Height of image */
    uint8_t bitsperpixel;       /* 10h Image pixel size */
    uint8_t imagedescriptor;    /* 11h Image descriptor byte */
} TGAHEADER;

typedef struct _pixel {
    uint8_t r, g, b, a;
} PIXEL;

void tga_read_header (FILE *, TGAHEADER *);

#endif

