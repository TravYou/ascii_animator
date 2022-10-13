#include <iostream>
#include <png.h>
#include "translatePng.h"
#include "Exceptions.h"
#include <filesystem>
#include <string>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;
using std::filesystem::directory_iterator;
using std::filesystem::exists;
using std::filesystem::create_directory;
using std::filesystem::remove_all;

int main(int argc, char ** argv){
    std::cout << "started\n";
    if (argc != 3) {
        std::cerr << "ERROR: Wrong argument number" << endl;
        return 0;
    }
    std::cout << "started2\n";
    int screenWidth;
    int screenHeight;
    std::cout << "started2\n";
    std::cout << "Please enter: screenWidth screenHeight\n";
    std::cout << "started2\n";
    bool continueLoop = true;
    while (continueLoop) {
        if(!(std::cin >> screenWidth >> screenHeight)) {
            continueLoop = true;
            continue;
        }else if (screenHeight == 0 || screenWidth == 0) {
            continueLoop = true;
            continue;
        }
        continueLoop = false;
    }
    std::cout << screenWidth << ' ' << screenHeight << endl;


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
        charDisplay *disp = readArrayToText(pic, screenWidth, screenHeight);
        filename.erase(filename.end()-4, filename.end());
        string newTxt{target};
        newTxt.append("/").append(pngfile.path().stem()).append(".txt");
        cout << newTxt << endl;
        writeTextToFile(newTxt.c_str(),disp);
        freeAll(pic, disp);
    }
    

    
}