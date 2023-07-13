#include "GrayImage.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include "BMP.h"

GrayImage::GrayImage()
{
	this->_height = 0;
	this->_width = 0;
	this->_stride = _width;
	this->_refCount = new size_t;
	*_refCount = 1;
}

GrayImage::GrayImage(int img_width, int img_height, std::vector<uint8_t> const& vectorData)
	:_width(img_width), _height(img_height)
{
	if (_width <= 0 || _height <= 0)
		throw std::invalid_argument("Bad image dimensions!");
	this->_stride = _width;
	this->_data = new uint8_t[_width * _height];
	if (!vectorData.empty()) {
		for (int y = 0; y < _height; y++) {
			for (int x = 0; x < _width; x++) {
				setPixel(y, x, vectorData[y * _stride + x]);
			}
		}
	}
	this->_refCount = new size_t;
	*_refCount = 1;
}

GrayImage::GrayImage(GrayImage const& other, int row, int col, int ROI_width, int ROI_height) 
	:_width(ROI_width), _height(ROI_height)
{
	this->_stride = other.getStride();
	this->_data = new uint8_t[_width * _height];
	this->_data = other._data + row * _stride + col;
	this->_refCount = other._refCount;
	(*_refCount)++;
}

GrayImage::GrayImage(const GrayImage & other)
{
	std::cout << "copy ctor" << endl;
	this->_width = other.getWidth();
	this->_height = other.getHeight();
	this->_stride = other.getStride();
	if (_refCount != nullptr && (*_refCount) == 1)
		delete[]_data;
	this->_refCount = other._refCount;
	this->_data = other._data;
	(*_refCount)++;

}



GrayImage const& GrayImage::operator=(GrayImage const& other)
{
	if (_data)
		delete[] _data;
	if(--(*_refCount) == 0)
		delete[] _refCount;
	this->_width = other.getWidth();
	this->_height = other.getHeight();
	this->_stride = other.getStride();
	this->_data = other._data;
	this->_refCount = other._refCount;
	(*_refCount)++;
	return *this;
}

GrayImage const& GrayImage::operator=(uint8_t const& value)
{
	uint8_t* dst_ptr, * src1_ptr, * src2_ptr;
	for (int y = 0; y < _height; y++)
	{
		dst_ptr = _data + y * _stride;
		for (int x = 0; x < _width; x++)
		{
			*(dst_ptr++) = value;
		}
	}
	return *this;
}

GrayImage::GrayImage(GrayImage&& other) noexcept
	:_width(other.getWidth()), _height(other.getHeight()), _stride(other.getStride())
{
	std::cout << "move ctor" << endl;
	this->_data = other._data;
	this->_width = other._width;
	this->_height = other._height;
	this->_stride = other._stride;
	this->_refCount = other._refCount;
	if (*other._refCount == 1) {
		other._data = nullptr;
		other._refCount = nullptr;
	}
}

GrayImage::~GrayImage()
{
	if(_refCount != nullptr)
		(*_refCount)--;
	if (_refCount == 0)
		delete[] _data;
}

GrayImage::operator std::vector<uint8_t>() const
{
	vector<uint8_t> dataVec(_width * _height);
	int index = 0;
	for (int row = 0; row < _height; row++) {
		for (int col = 0; col < _width; col++) {
			dataVec[index] = getPixel(row, col);
			index++;
		}
	}
	return dataVec;
}

int GrayImage::getWidth() const
{
	return this->_width;
}

int GrayImage::getHeight() const
{
	return this->_height;
}

int GrayImage::getStride() const
{
	return this->_stride;
}

size_t GrayImage::getRefCount() const
{
	return *_refCount;
}

uint8_t const* GrayImage::data() const
{
	return _data;
}

uint8_t GrayImage::getPixel(int row, int col) const
{
	return this->_data[row * _stride + col];
}

void GrayImage::setPixel(int row, int col, uint8_t value)
{
	this->_data[row * _stride + col] = value;
}

bool GrayImage::operator==(GrayImage const& other) const
{
	bool flag = true;
	for (int y = 0; y < _height; y++) {
		for (int x = 0; x < _width; x++) {
			if (getPixel(y, x) != other.getPixel(y, x))
				flag = false;
		}
	}
	return (_height == other._height && _width == other._width && flag);
}

bool GrayImage::operator ==(uint8_t const& value) const
{
	for (int y = 0; y < _height; y++) {
		for (int x = 0; x < _width; x++) {
			if (getPixel(y, x) != value)
				return false;
		}
	}
	return true;
}

GrayImage GrayImage::operator +(GrayImage const& other) const
{
	if (_width != other.getWidth() || _height != other.getHeight())
		throw std::runtime_error("The requested operation cannot be performed");
	GrayImage newImage(_width, _height);
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < _height; i++)
	{
		uint8_t* dstPtr = newImage._data + i * newImage._stride;
		const uint8_t* src1Ptr = this->_data + i * _stride;
		const uint8_t* src2Ptr = other._data + i * other._stride;
		for (int j = 0; j < _width; j++)
		{
			*(dstPtr++) = *(src1Ptr++) + *(src2Ptr++);
		}
	}
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << "Execution time: " << duration << " milliseconds" << std::endl;

	return newImage;
}

GrayImage GrayImage::operator -(GrayImage const& other) const
{
	if (_width != other.getWidth() || _height != other.getHeight())
		throw std::runtime_error("The requested operation cannot be performed");
	GrayImage newImage(_width, _height);
	for (int y = 0; y < _height; y++) {
		for (int x = 0; x < _width; x++) {
			newImage.setPixel(y, x, getPixel(y, x) - other.getPixel(y, x));
		}
	}
	return newImage;
}

GrayImage GrayImage::operator *(uint8_t const& value) const
{
	GrayImage newImage(_width, _height);
	for (int y = 0; y < _height; y++) {
		for (int x = 0; x < _width; x++) {
			newImage.setPixel(y, x, getPixel(y, x) * value);
		}
	}
	return newImage;
}

GrayImage GrayImage::operator *(GrayImage const& other) const
{
	if (_width != other._height) {
		throw std::runtime_error("The requested operation cannot be performed");
	}
	int newImageWidth = other._width;
	int newImageHeight = _height;
	GrayImage newImage(newImageWidth, newImageHeight);
	for (int i = 0; i < newImageHeight; i++) {
		for (int j = 0; j < newImageWidth; j++) {
			uint8_t value = 0;
			for (int k = 0; k < _width; k++) {
				value+= getPixel(i, k) * other.getPixel(k, j);
			}
			newImage.setPixel(i, j, value);
		}
	}
	return newImage;
}

GrayImage GrayImage::mul(GrayImage const& other) const
{
	if (_width != other.getWidth() || _height != other.getHeight())
		throw std::runtime_error("The requested operation cannot be performed");
	GrayImage newImage(_width, _height);
	for (int y = 0; y < _height; y++) {
		for (int x = 0; x < _width; x++) {
			newImage.setPixel(y, x, getPixel(y, x) * other.getPixel(y, x));
		}
	}
	return newImage;
}

GrayImage GrayImage::absdiff(GrayImage const& other) const
{
	if (_width != other.getWidth() || _height != other.getHeight())
		throw std::runtime_error("The requested operation cannot be performed");
	GrayImage newImage(_width, _height);
	for (int y = 0; y < _height; y++) {
		for (int x = 0; x < _width; x++) {
			newImage.setPixel(y, x, abs(getPixel(y, x) - other.getPixel(y, x)));
		}
	}
	return newImage;
}

void GrayImage::saveToBMP(std::string const& path) const
{

	std::ofstream ofs(path, std::ios::binary);
	BMP_WriteHeader(_width, _height, ofs);
	for (int row = _height - 1; row >= 0; row--) {
		for (int col = 0; col < _width; col++) {
			uint8_t color = getPixel(row, col);
			for (int k = 0; k < 3; k++) {
				//WriteBytes(ofs, color);
				ofs.write(reinterpret_cast<const char*>(&color), sizeof color);
			}
		}
	}
}


//void GrayImage::copyTo(GrayImage& other, int x, int y) const
//{
//	for (int i = x; i < height && i < other.getHeight(); i++) {
//		for (int j = y; j < width && j < other.getWidth(); j++) {
//			other.setPixel(i, j, getPixel(i - x,j - y));
//		}
//	}
//}

//void GrayImage::save(std::string const& path) const {
//	std::ofstream ofs;
//	ofs.open(path, std::ios::binary);
//
//	if (!ofs.is_open()) {
//		std::cout << "Failed to open the file: " << path << std::endl;
//		return;
//	}
//	ofs.write((char*)(&width), sizeof width);
//	ofs.write((char*)(&height), sizeof height);
//	ofs.write((char*)(image.data()), sizeof(uint8_t) * height * width);
//
//	ofs.flush();
//	ofs.close();
//}
//
//void GrayImage::load(std::string const& path)
//{
//	std::ifstream ifs;
//	ifs.open(path, std::ios::binary);
//	if (!ifs.is_open()) {
//		std::cout << "Failed to open the file: " << path << std::endl;
//		return;
//	}
//	image.resize(height * width);
//	ifs.read((char*)(&width), sizeof width);
//	ifs.read((char*)(&height), sizeof height);
//	ifs.read(reinterpret_cast<char*>(image.data()), sizeof(uint8_t) * height * width);
//	ifs.close();
//}