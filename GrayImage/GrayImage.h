#pragma once
#include <vector>
#include <string>
using namespace std;

class GrayImage
{
private:
	uint8_t* _data;
	int _width;
	int _height;
	int _stride;
	size_t* _refCount = nullptr;

public:
	GrayImage();
	GrayImage(int img_width, int img_height, std::vector<uint8_t> const& vectorData = {});
	GrayImage(GrayImage const& other, int row, int col, int ROI_width, int ROI_height);
	GrayImage(GrayImage const& other);
	GrayImage const& operator=(GrayImage const& other);
	GrayImage const& operator=(uint8_t const& value);
	GrayImage(GrayImage&& other) noexcept;
	~GrayImage();

	operator std::vector<uint8_t>() const;

	int getWidth() const;
	int getHeight() const;
	size_t getRefCount() const;
	int getStride() const;

	uint8_t const* data() const;
	uint8_t getPixel(int row, int col) const;
	void setPixel(int row, int col, uint8_t value);

	GrayImage operator +(GrayImage const& other) const;
	GrayImage operator -(GrayImage const& other) const;
	GrayImage mul(GrayImage const& other) const;
	GrayImage absdiff(GrayImage const& other) const;
	GrayImage operator *(GrayImage const& other) const;
	GrayImage operator *(uint8_t const& value) const;

	bool operator ==(GrayImage const& other) const;
	bool operator ==(uint8_t const& value) const;

	void saveToBMP(std::string const& filepath) const;

	//void copyTo(GrayImage& image, int x, int y) const;
	//void save(std::string const& path) const;
	//void load(std::string const& path);
};


