#pragma once
#include <fstream>
using namespace std;

template<typename T>
void WriteBytes(ofstream& ofs, T t);
void BMP_WriteHeader(int width, int height, ofstream& ofs);
