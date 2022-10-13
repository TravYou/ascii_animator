#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <fstream>
#include <unistd.h>

using std::cerr;
using std::cout;
using std::endl;
using std::vector;
using std::ifstream;
using std::filesystem::path;
using std::filesystem::exists;
using std::filesystem::directory_iterator;

int main(int argc, char ** argv) {
    if (argc != 2) {
        cerr << "ERROR: Wrong argument number" << endl;
        return 0;
    }
    if (exists(argv[1])){
        vector<path> pathvec;
        copy(directory_iterator(argv[1]), directory_iterator(), back_inserter(pathvec));
        sort(pathvec.begin(), pathvec.end());
        for (auto it = pathvec.begin(); it != pathvec.end(); ++it) {
            ifstream content(*it);
            std::cout << content.rdbuf();
            usleep(30200);
        }
    }
}