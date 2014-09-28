/*
 * =====================================================================================
 *
 *       Filename:  tga.c
 *
 *    Description:  tga library routines.
 *
 *        Version:  1.0
 *        Created:  09/26/2014 04:01:56 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Benjamin F. Arbe (ba), benarbe@tuckerenergy.com
 *        Company:  Tucker Energy, Inc.
 *
 * =====================================================================================
 */
#define _LIB_TGA_
#include "tga.h"


void
tga_read_header(FILE *fptr, TGAHEADER *header) {
    header->idlength = fgetc (fptr);
    header->colourmaptype = fgetc (fptr);
    header->datatypecode = fgetc (fptr);
    fread (&header->colourmaporigin, 2, 1, fptr);
    fread (&header->colourmaplength, 2, 1, fptr);
    header->colourmapdepth = fgetc(fptr);
    fread (&header->x_origin, 2, 1, fptr);
    fread (&header->y_origin, 2, 1, fptr);
    fread (&header->width, 2, 1, fptr);
    fread (&header->height, 2,1, fptr);
    header->bitsperpixel = fgetc(fptr);
    header->imagedescriptor = fgetc(fptr);
}

