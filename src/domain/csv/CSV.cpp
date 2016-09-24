#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "CSV.h"

using namespace std;

bool CSV::read(string filename, int16_t* p, int* servo_size, int* motion_size, int* pose_size) {
    fstream file;
    string str;
    int id = 0;

    file.open(filename.c_str(), ios::in);
    if(! file.is_open()) {
        return false;
    }

    while(getline(file, str)) {
        vector<string> str_line = split(str, (string)",");
        
        if (str_line[0] == "servo_size") {
            stringstream sstr;
            sstr << str_line[1];
            sstr >> *servo_size;
        } else if (str_line[0] == "motion_size") {
            stringstream sstr;
            sstr << str_line[1];
            sstr >> *motion_size; 
        } else if (str_line[0] == "pose_size") {
            stringstream sstr;
            sstr << str_line[1];
            sstr >> pose_size[id]; 
            ++ id;
        } else {
            int size = str_line.size();
            for(int i = 0; i < size; ++i) {
                stringstream sstr;
                sstr << str_line[i];
                sstr >> p[i];
            }
            p += size;
        }
    }
    file.close();
    return true;
}

std::vector<string> CSV::split(string &src, string key) {

    string str = src;
    vector<string> str_line;
    int str_len = str.length();
    int key_len = key.length();
    
    int index = 0;
    while(index < str_len) {
        int oldindex = index;
        index = str.find(key, index);
        index = (index == string::npos ? str_len : index);
        string tmp = str.substr(oldindex, index - oldindex);
        str_line.push_back(tmp);
        index += key_len;
    }
    return str_line;
}