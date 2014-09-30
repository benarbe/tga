/*
 * =====================================================================================
 *
 *       Filename:  tga_convert_uncompressed.c
 *
 *    Description:  Converts any TGA file format into TGA uncompressed format
 *
 *        Version:  1.0
 *        Created:  09/26/2014 03:23:24 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Benjamin F. Arbe (ba), benarbe@tuckerenergy.com
 *        Company:  Tucker Energy, Inc.
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "tga.h"


void MergeBytes (PIXEL *, uint8_t *, int);

int
main (int argc, char **argv) {
    FILE *fptr;
    TGAHEADER header;
    PIXEL *pixels;

    if (argc < 2) {
        fprintf (stderr, "Usage: %s tgafile\n", argv[0]);
        exit (-1);
    }

    if ((fptr = fopen(argv[1], "r")) == NULL) {
        fprintf (stderr, "File open failed\n");
        exit (-1);
    }

    /*  Read TGA header */
    tga_read_header (fptr, &header);
    printf ("Converting from type:%d - %s to type 2\n", header.datatypecode, 
            tga_image_type[header.datatypecode]);
    printf ("%s\n", tga_image_encoding[header.datatypecode]);

    /*  Allocate space for the image */
    if ((pixels = malloc (header.width*header.height*sizeof(PIXEL))) == NULL) {
        fprintf (stderr, "malloc of image failed\n");
        exit (-1);
    }
    for (int i=0; i<header.width*header.height; i++) {
        pixels[i].r = 0;
        pixels[i].g = 0;
        pixels[i].b = 0;
        pixels[i].a = 0;
    }

    /*  What can we handle */
    if (header.datatypecode != 2 && header.datatypecode != 10) {
        fprintf (stderr, "Can only handle image type 2 and 10\n");
        exit (-1);
    }

    if (header.bitsperpixel != 16 && header.bitsperpixel != 24 &&
            header.bitsperpixel != 32) {
        fprintf (stderr, "Can only handle pixel depths of 16, 24, and 32\n");
        exit (-1);
    }

    if (header.colourmaptype != 0 && header.colourmaptype !=1) {
        fprintf (stderr, "Can only handle colour map types of 0 and 1\n");
        exit (-1);
    }

    /*  Skip over unnecessary stuff */
    int skipover = 0;
    skipover += header.idlength;
    skipover += header.colourmaptype * header.colourmaplength;
    fprintf (stderr, "Skip over %d bytes\n", skipover);
    fseek (fptr, skipover, SEEK_CUR);

    /* Read the image */
    int bytes2read = header.bitsperpixel / 8;
    uint8_t p[5];

    int n = 0;
    while (n < header.width * header.height) {
        if (header.datatypecode == 2) { /*  Uncompressed */
            if (fread (p, 1, bytes2read, fptr) != bytes2read) {
                fprintf (stderr, "Unexpected end of file at pixel %d\n",n);
                exit (-1);
            }
            MergeBytes(&(pixels[n]), p, bytes2read);
            n++;
    
        } else if (header.datatypecode == 10) { /* Compressed */
            if (fread (p, 1, bytes2read+1, fptr) != bytes2read+1) {
                fprintf (stderr, "Unexpected end of file at pixel %d\n", n);
                exit (-1);
            }
            int j = p[0] & 0x7f;
            MergeBytes (&(pixels[n]),&(p[1]), bytes2read);
            n++;
            if (p[0] & 0x80) {  /*  RLE chunk */
                for (int i=0; i<j;i++) {
                    MergeBytes (&(pixels[n]), &(p[1]), bytes2read);
                    n++;
                }
            } else {        /* Normal chunck */
                for (int i=0; i<j;i++) {
                    if (fread(p,1,bytes2read,fptr) != bytes2read) {
                        fprintf (stderr, "Unexpected end of file at pixel %d\n", n);
                        exit (-1);
                    }
                    MergeBytes (&(pixels[n]), p, bytes2read);
                    n++;
                }
            }
        } 
    } /*  end of while */

    fclose (fptr);

    /*  Write the result as a uncompressed TGA */
    if ((fptr = fopen ("tgatest.tga", "w")) == NULL) {
        fprintf (stderr, "Failed to open outputfile\n");
        exit (-1);
    }
    putc (0, fptr);
    putc (0, fptr);
    putc (2, fptr);     /*  uncompressed RGB */
    putc (0, fptr), putc(0, fptr);
    putc (0, fptr), putc(0, fptr);
    putc (0, fptr);
    putc (0, fptr); putc(0, fptr); /* X origin */
    putc (0, fptr); putc(0, fptr); /* Y origin */
    putc ((header.width & 0x00FF), fptr);
    putc ((header.width & 0xFF00) / 256, fptr);
    putc ((header.height & 0x00FF), fptr);
    putc ((header.height & 0xFF00) / 256, fptr);
    putc (24, fptr);    /*  24 bit bitmap */
    putc (header.imagedescriptor, fptr);
    for (int i=0; i<header.height*header.width; i++) {
        putc (pixels[i].b, fptr);
        putc (pixels[i].g, fptr);
        putc (pixels[i].r, fptr);
        //putc (0, fptr); /* 24 bit bitmap not use this */
    }
    fclose (fptr);


    
    return 0;
}

void
MergeBytes (PIXEL *pixel, uint8_t *p, int bytes) {
    if (bytes == 4) {
        pixel->r = p[2];
        pixel->g = p[1];
        pixel->b = p[0];
        pixel->a = p[3];
    } else if (bytes == 3) {
        pixel->r = p[2];
        pixel->g = p[1];
        pixel->b = p[0];
        pixel->a = 0;
    } else if (bytes == 2) {
        pixel->r = (p[1] & 0x7c) << 1;
        pixel->g = ((p[1] & 0x03) << 6) | ((p[0] & 0xe0) >> 2);
        pixel->b = (p[0] & 0x1f) << 3;
        pixel->a = (p[1] & 0x80);
    }
}

