#include <iostream>
#include <png.h>
#include "translatePng.h"
#include "Exceptions.h"
#include <filesystem>
#include <string>
#include <unistd.h>
#include <sstream>
#include <sys/ioctl.h>


using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;
using std::stringstream;
using std::filesystem::directory_iterator;
using std::filesystem::exists;
using std::filesystem::create_directory;
using std::filesystem::remove_all;

#define TIOCGWINSZ	0x5413



int main(int argc, char ** argv){
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    if (argc != 3) {
        std::cerr << "ERROR: Wrong argument number" << endl;
        return 0;
    }
    string screenWidth;
    string screenHeight;
    int scrW, scrH;
    std::cout << "Please enter: screenWidth screenHeight\n";
    bool continueLoop = true;
    while (continueLoop) {
        continueLoop = false;
        if (cin >> screenWidth) {
            if(!(stringstream(screenWidth) >> scrW)){
                if (!screenWidth.compare("auto")){
                    scrW = size.ws_col;
                } else {
                    continueLoop = true;
                }
            }
        } else {
            continueLoop = true;
        }
        if (cin >> screenHeight) {
            if(!(stringstream(screenHeight) >> scrH)){
                if (!screenHeight.compare("auto")){
                    scrH = size.ws_row;
                } else {
                    continueLoop = true;
                }
            }
        } else {
            continueLoop = true;
        }

        if (scrW == 0 || scrH == 0) {
            continueLoop = true;
        }
        if (continueLoop == true) {
            cout << "ERROR: incorrect width and height";
        }
    }
    std::cout << "Width: " << scrW << ' ' << "Height: " << scrH << endl;


    string pngfiles{argv[1]};
    string target{argv[2]};
    if (!exists(pngfiles)) {
        cerr << "ERROR: PNG file path not found";
        return 0;
    }
    if (exists(target)) {
        cerr << "ERROR: Output directory already exists" << endl;
        cout << "Delete directory \"" << argv[2] << "\"? : YesYesYes / Any Key to Quit" << endl;
        std::string reply;
        if ((cin >> reply) && !reply.compare("YesYesYes")){
            remove_all(target);
        }else{
            return 0;
        }
    }

    create_directory(target);
    for (const auto & pngfile: directory_iterator(pngfiles)) {
        cout << pngfile.path() << endl;
        string filename{pngfile.path()};
        imgPng* pic = read_png_file(filename.c_str());
        charDisplay *disp = readArrayToText(pic, scrW, scrH);
        filename.erase(filename.end()-4, filename.end());
        string newTxt{target};
        newTxt.append("/").append(pngfile.path().stem()).append(".txt");
        cout << newTxt << endl;
        writeTextToFile(newTxt.c_str(),disp);
        freeAll(pic, disp);
    }
    

    
}