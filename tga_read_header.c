/*
 * =====================================================================================
 *
 *       Filename:  tga_read_header.c
 *
 *    Description:  Print information about TGA file header. 
 *
 *        Version:  1.0
 *        Created:  09/26/2014 10:33:55 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Benjamin F. Arbe (ba), benarbe@tuckerenergy.com
 *        Company:  Tucker Energy, Inc.
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "tga.h"


int
main (int argc, char **argv) {
    FILE *fptr;
    TGAHEADER header;

    if (argc < 2) {
        fprintf (stderr, "Usage: %s tgafile\n", argv[0]);
        exit (-1);
    }

    if ((fptr = fopen (argv[1], "r")) == NULL) {
        fprintf (stderr, "File filed to open\n");
        exit (-1);
    }

    /*
     * Display header fields.
     */
    tga_read_header (fptr, &header);

    printf ("ID length: %d\n", header.idlength);
    printf ("Colourmap type: %d\n", header.colourmaptype);
    printf ("Image type: %d ", header.datatypecode);
    switch (header.datatypecode) {
        case 0: printf("(No image data included in file.\n");break;
        case 1: printf("(Colormapped image data.)\n");
                printf("Colormap: yes   Encoding: no\n");
                break;
        case 2: printf("(Truecolor image data.)\n");
                printf("Colormap: no    Encoding: no\n");
                break;
        case 3: printf("(Monochrome image data.)\n");
                printf("Colormap: no    Encoding: no\n");
                break;
        case 9: printf("(Colormapped image data.)\n");
                printf("Colormap: yes   Encoding: yes\n");
                break;
        case 10: printf("(Truecolor image data.)\n");
                 printf("Colormap: no   Encoding: yes\n");
                 break;
        case 11: printf("(Monochrome image data.)\n");
                 printf("Colormap: no   Encoding: yes\n");
                 break;
        default:
                 printf("Unsupported image file.\n");
    }

    printf ("Colour map offset: %d\n", header.colourmaporigin);
    printf ("Colour map length: %d\n", header.colourmaplength);

    printf ("Colour map depth: %d\n", header.colourmapdepth);
    printf ("X origin: %d\n", header.x_origin);
    printf ("Y origin: %d\n", header.y_origin);
    printf ("Width: %d\n", header.width);
    printf ("Height: %d\n", header.height);
    printf ("Bits per pixel: %d\n", header.bitsperpixel);

    printf ("Descriptor: %d\n", header.imagedescriptor);

    fclose (fptr);
}


