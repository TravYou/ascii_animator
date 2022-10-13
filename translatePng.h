#ifndef TRANSLATEPNG_H
#define TRANSLATEPNG_H

#include <png.h>
#include <iostream>

typedef struct {
    png_structp png_ptr;
    png_infop info_ptr;
    png_bytep * rowpointers;
    png_byte color_type;
    png_byte bit_depth;
    int width;
    int height;
} imgPng;

typedef struct{
    int width;
    int height;
    char* data;
} charDisplay;


imgPng* read_png_file(const char* file_name);

charDisplay* readArrayToText(imgPng* img, int displayWidth, int displayHeight);

int calculateGrayScale(png_byte* pixel);

char findchar(int grayScale);

void writeTextToFile(const char * file_name, charDisplay* display);

void freeAll(imgPng* img, charDisplay* display);

#endif