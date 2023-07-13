#include "pch.h"
#include "../GrayImage/GrayImage.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(GrayImage, Ctor_from_width_height_sets_values_correctly) {
	// Arrange
	int w = 5, h = 10;
	GrayImage img;
	// Act, Assert
	EXPECT_EQ(w, img.getWidth());
	EXPECT_EQ(w, img.getStride());
	EXPECT_EQ(h, img.getHeight());
	EXPECT_EQ(1, img.getRefCount());
}