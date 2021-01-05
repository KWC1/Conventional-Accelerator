#include <vector>
#include <algorithm>
#include <fstream>

SC_MODULE(IDPManager){

}
void decodeData(std::pair<int, double>& decoded, std::vector<double>& NZVL, std::vector<bool>& sparseMap){
    auto currentVal = NZVL.begin();
    for(int i = 0; i < sparseMap.size(); i++) {
        if (sparseMap[i]) decoded.push_back(std::make_pair(i, *(currentVal++)));
    }
}

//Currently, we only support raw image
bool loadImageRAW(std::string filename){
    cout << "Loading file: " << filename << endl;
    std::ifstream inFile(filename);
    if(!inFile.is_open()) return false;
    std::vector<int> originalImage;

    while(!inFile.eof()){

    }
}

bool loadImage