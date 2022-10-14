#include <png.h>
#include <iostream>
#include <fstream>
#include "translatePng.h"
#include <exception>
#include <string.h>
#include "Exceptions.h"

using std::ofstream;

imgPng* read_png_file(const char* file_name) {
    unsigned char header[8];
    FILE *fp = fopen(file_name, "rb");
    if (!fp) {
        throw Exception("Invalid file pointer for file", file_name ) ;
    }
    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8)){
        throw Exception("Not a PNG file", file_name );
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        throw Exception("png_ptr failed", file_name);
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        throw Exception("info_ptr failed", file_name);
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        throw Exception("setjmp error", file_name);
    }
    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);

    png_read_info(png_ptr, info_ptr);

    int width = png_get_image_width(png_ptr, info_ptr);
    int height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    png_read_update_info(png_ptr, info_ptr);


    /* read file */
    if (setjmp(png_jmpbuf(png_ptr))){
        throw Exception("error reading image", file_name);
    }
    png_bytep * row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
    for (int y=0; y<height; y++)
            row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));

    png_read_image(png_ptr, row_pointers);


    imgPng* img_ptr = new imgPng;
    img_ptr->png_ptr = png_ptr;
    img_ptr->info_ptr = info_ptr;
    img_ptr->rowpointers = row_pointers;
    img_ptr->height = height;
    img_ptr->width = width;
    img_ptr->bit_depth = bit_depth;
    img_ptr->color_type = color_type;
    //printf("pngwidth: %d, pngheight: %d\n", width, height);

    fclose(fp);
    return img_ptr;

}

charDisplay* readArrayToText(imgPng* img_ptr, int displayWidth, int displayHeight) {
    double heightShrinkRatio = (double) img_ptr->height / displayHeight;
    double widthShrinkRatio = (double)img_ptr->width / displayWidth;
    //std::cout << widthShrinkRatio << ' '<< heightShrinkRatio << '\n';
    double currH = 0;
    double currW = 0;
    charDisplay* pic = new charDisplay;
    pic->width = displayWidth;
    pic->height = displayHeight;
    pic->data = new char[displayHeight * (displayWidth + 1) + 1];
    char* addptr = pic->data;
    for (int h = 0; h < displayHeight; h++) {
        png_byte* row = img_ptr->rowpointers[(int)currH];
        for (int w = 0; w < displayWidth; w++) {
            png_byte* pixel = &(row[((int)currW) * 3]);
            int gray = calculateGrayScale(pixel);
            char res = findchar(gray);
            *addptr = res;
            addptr += 1;
            currW += widthShrinkRatio;
        }
        *addptr = '\n';
        addptr += 1;
        currH += heightShrinkRatio;
        currW = 0;
    }
    *addptr = '\0';
    return pic;
}

int calculateGrayScale(png_byte* pixel) {
    int res = (pixel[0] + pixel[1] + pixel[2]) / 76;
    return res;
};

char findchar(int grayScale) {
    switch (grayScale)
    {
    case 0 : return '@';
        break;
    case 1 : return '%';
        break;
    case 2 : return '#';
        break;
    case 3 : return '&';
        break;
    case 4 : return '+';
        break;
    case 5 : return '=';
        break;
    case 6 : return '^';
        break;
    case 7 : return '-';
        break;
    case 8 : return ':';
        break;
    case 9 : return '.';
        break;
    case 10 : return ' ';
        break;
    default: return ' ';
        break;
    }
}

void writeTextToFile(const char * file_name, charDisplay* display) {
    ofstream output;
    output.open(file_name);
    output << display->data;
    output.close();
}

void freeAll(imgPng *img, charDisplay* display) {
    for (int h = 0; h < img->height; h++) {
        free(img->rowpointers[h]);
    }
    free(img->rowpointers);
    png_destroy_read_struct(&img->png_ptr, &img->info_ptr, nullptr);
    delete img;

    delete[] display->data;
    delete display;
}