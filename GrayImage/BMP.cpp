#include "BMP.h"
#include <fstream>
#include <string>
#include <vector>

using namespace std;

template<typename T>
void WriteBytes(std::ofstream& ofs, T t)
{
	ofs.write((char*)(&t), sizeof(T));
}

void BMP_WriteHeader(int width, int height, std::ofstream& ofs)
{
	int numPixels = width * height;
	const int NUM_SAVED_CHANNELS = 3;
	// BMP Header
	WriteBytes(ofs, uint16_t(0x4D42)); // signature
	WriteBytes(ofs, uint32_t(0x36 + numPixels)); // size of the bmp file (all bytes)
	WriteBytes(ofs, uint16_t(0));      // unused
	WriteBytes(ofs, uint16_t(0));      // unused
	WriteBytes(ofs, uint32_t(0x36));   // Offset where the pixel array begins (size of both headers)
	// DIB Header
	WriteBytes(ofs, uint32_t(0x28));   // Number of bytes in DIB header (without this field)
	WriteBytes(ofs, uint32_t(width));  // width
	WriteBytes(ofs, uint32_t(height)); // height
	WriteBytes(ofs, uint16_t(1));      // number of planes used
	WriteBytes(ofs, uint16_t(NUM_SAVED_CHANNELS * 8)); // bit-depth
	WriteBytes(ofs, uint32_t(0));      // no compression
	WriteBytes(ofs, uint32_t(numPixels)); // Size of raw bitmap data (including padding)
	WriteBytes(ofs, uint32_t(0xB13));  // print resolution pixels/meter X
	WriteBytes(ofs, uint32_t(0xB13));  // print resolution pixels/meter Y
	WriteBytes(ofs, uint32_t(0));      // 0 colors in the color palette
	WriteBytes(ofs, uint32_t(0));;     // 0 means all colors are important
}

