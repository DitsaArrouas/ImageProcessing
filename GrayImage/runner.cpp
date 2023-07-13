#include <iostream>
#include "GrayImage.h"
#include <cmath>


int main()
{
	GrayImage image(1000, 1000);
	for (int y = 0; y < image.getHeight(); y++) {
		for (int x = 0; x < image.getWidth(); x++) {
			image.setPixel(y, x, (uint8_t)(tan(x / 20.0) * tan(y / 20.0) * 255.));
		}
	}

	image.saveToBMP("C:\\Users\\Dell Vostro\\Desktop\\לימודים\\BOOTCAMP\\C++\\saveBMPImage.bmp");

	GrayImage imageView(image,50,100,500,500);
	GrayImage imageViewV(imageView, 50, 100, 250, 250);
	GrayImage newImage(image);
	GrayImage ImageAdd = newImage + image;
	uint8_t a = 105;
	GrayImage ImageTry(100,100);
	imageView = a;
	GrayImage ImageSub = newImage - image;
	GrayImage ImageDouble = newImage * 2;
	GrayImage ImageMultMatrix = newImage * image;
	GrayImage ImageMult = newImage.mul(image);
	GrayImage ImageAbs = newImage.absdiff(newImage);

	//imageViewV.copyTo(ImageMult, 50, 50);
	//std::string path = "C:\\Users\\Dell Vostro\\Desktop\\לימודים\\BOOTCAMP\\C++\\saveImage.bin";
	//image.save(path);
	//image.load(path);
	return 0;
}