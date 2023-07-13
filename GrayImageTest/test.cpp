#include "pch.h"
#include "../GrayImage/GrayImage.h"
#include <functional>

using namespace std;

TEST(GrayImage, Ctor_from_width_height_sets_values_correctly) {
	// Arrange
	int w = 5, h = 10;
	GrayImage img(w, h);
	// Act, Assert
	EXPECT_EQ(w, img.getWidth());
	EXPECT_EQ(w, img.getStride());
	EXPECT_EQ(h, img.getHeight());
	EXPECT_EQ(1, img.getRefCount());
}

TEST(GrayImage, Ctor_of_view_keeps_original_stride) {
	// Arrange
	int w = 5, h = 10;
	GrayImage img(w, h);
	// Act
	GrayImage view(img, 1, 1, 2, 2);

	// Assert
	EXPECT_EQ(2, view.getWidth());
	EXPECT_EQ(view.getStride(), img.getStride());
	EXPECT_EQ(2, view.getHeight());
}

TEST(GrayImage, Ctor_width_height_vector__sets_image_correctly) {
	// Arrange
	int w = 5, h = 10;
	vector<uint8_t> data(w * h);
	int value = 0;

	generate(data.begin(), data.end(), [&value]() {return value++; });

	// Act
	GrayImage img(w, h, data);

	// Assert
	for (auto ptr = img.data(); ptr < img.data() + w * h; ptr++)
	{
		auto index = ptr - img.data();
		auto value = *ptr;
		ASSERT_EQ(index, value);
	}
}

TEST(GrayImage, Equality_with_value) {
	// Arrange
	int w = 5, h = 10;
	uint8_t value = 123;
	vector<uint8_t> data(w * h, value);
	GrayImage img(w, h, data);

	// Act, Assert
	ASSERT_TRUE(img == value);
}

TEST(GrayImage, Equality_with_other_image) {
	// Arrange
	int w = 5, h = 10;
	vector<uint8_t> data(w * h);
	// fill with random numbers:
	generate(data.begin(), data.end(),
		[]() {return rand() % 256; });

	GrayImage img(w, h, data);
	GrayImage other(w, h, data);

	// Act, Assert
	ASSERT_TRUE(img == other);
}

TEST(GrayImage, Assignment_To_value) {
	// Arrange
	int w = 5, h = 10;
	vector<uint8_t> data(w * h);
	generate(data.begin(), data.end(),
		[]() {return rand() % 256; });
	GrayImage img(w, h, data);
	uint8_t expected = 65;

	// Act
	img = expected;

	// Assert
	ASSERT_TRUE(img == expected);
}

TEST(GrayImage, Assignment_to_other_image) {
	// Arrange
	int w = 5, h = 10;
	vector<uint8_t> data(w * h);
	generate(data.begin(), data.end(),
		[]() {return rand() % 256; });
	GrayImage img(w, h, data);
	GrayImage other(w, h, data);

	uint8_t expected = 65;
	other = expected;
	// Act
	img = other;

	// Assert
	ASSERT_TRUE(img == expected);
}

void test_img_op(
	std::function<uint8_t(uint8_t, uint8_t)> const& scalar_op,
	std::function<GrayImage(GrayImage&, GrayImage&)> const& img_op)
{
	// Arrange
	int w = 5, h = 10;
	GrayImage img(w, h);
	GrayImage other(w, h);
	uint8_t img_value = 4, other_value = 48;
	uint8_t sum_value = scalar_op(img_value, other_value);
	img = img_value;
	other = other_value;

	// Act
	auto actual = img_op(img, other);

	// Assert
	ASSERT_TRUE(actual == sum_value);

}
TEST(GrayImage, Operator_plus) {
	test_img_op(
		[](uint8_t a, uint8_t b) {return a + b; },
		[](GrayImage& a, GrayImage& b) {return a + b; }
	);
}

TEST(GrayImage, Operator_minus) {
	test_img_op([](uint8_t a, uint8_t b) {return a - b; },
		[](GrayImage& a, GrayImage& b) {return a - b; }
	);
}
TEST(GrayImage, mul) {
	test_img_op([](uint8_t a, uint8_t b) {return a * b; },
		[](GrayImage& a, GrayImage& b) {return a.mul(b); }
	);
}

TEST(GrayImage, absdiff) {
	test_img_op([](uint8_t a, uint8_t b) {return abs(a - b); },
		[](GrayImage& a, GrayImage& b) {return a.absdiff(b); }
	);
}

TEST(GrayImage, Operator_product_with_image) {
	// Arrange
	GrayImage A(2, 3,
		{ 1,2
		,3,4
		,5,6 });
	GrayImage B(5, 2,
		{ 1,2,3,4,5
		,6,7,8,9,10 });
	GrayImage AxB(5, 3,
		{ 13 ,16 ,19 ,22 ,25
		,27 ,34 ,41 ,48 ,55
		,41 ,52 ,63 ,74 ,85 });

	// Act
	auto actual = A * B;

	// Assert
	ASSERT_TRUE(actual == AxB);
}


TEST(GrayImage, Operator_product_with_scalar) {
	// Arrange
	GrayImage A(2, 3,
		{ 1,2
		,3,4
		,5,6 });

	uint8_t B = 2;

	GrayImage AxB(2, 3,
		{ 2,4
		,6,8
		,10,12 });

	// Act
	auto actual = A * B;

	// Assert
	ASSERT_TRUE(actual == AxB);
}

TEST(GrayImage, view_ctor_extracts_correct_indexes) {
	// Arrange
	int w = 5, h = 5;
	uint8_t O = 156, V = 2;
	GrayImage img(w, h,
		//0 1 2 3 4
		{ O,O,O,O,O // 0
		 ,O,O,O,O,O // 1
		 ,O,O,V,V,O // 2
		 ,O,O,V,V,O // 3
		 ,O,O,O,O,O // 4
		});

	int view_row = 2, view_col = 2,
		view_w = 2, view_h = 2;

	// Act
	GrayImage view(img,
		view_row, view_col,
		view_w, view_h);

	// Assert
	vector<uint8_t> view_values = view;
	ASSERT_EQ(view_values, vector<uint8_t>({ V, V, V, V }));
}

TEST(GrayImage, view_supports_value_assignment) {
	// Arrange
	int w = 5, h = 5;
	GrayImage orig(w, h);
	uint8_t O = 156, V = 2;
	orig = O;

	int view_row = 2, view_col = 2,
		view_w = 2, view_h = 2;

	GrayImage view(orig,
		view_row, view_col,
		view_w, view_h);

	GrayImage expected(w, h,
		//0 1 2 3 4
		{ O,O,O,O,O // 0
		 ,O,O,O,O,O // 1
		 ,O,O,V,V,O // 2
		 ,O,O,V,V,O // 3
		 ,O,O,O,O,O // 4
		});

	// Act
	view = V; // changing buffer of orig image as well.

	// Assert
	ASSERT_TRUE(orig == expected);
}

TEST(GrayImage, CopyConstructor_does_not_allocate_data) {
	// Arrange
	GrayImage img(10, 10);

	// Act
	GrayImage other(img);

	// Assert
	ASSERT_EQ(img.data(), other.data());
}


TEST(GrayImage, CopyConstructor_increase_ref_count) {
	// Arrange
	GrayImage img(10, 10);
	auto origRefCount = img.getRefCount();

	// Act
	GrayImage other(img);

	// Assert
	ASSERT_EQ(other.getRefCount(), img.getRefCount());

	auto afterCopyCtorRefCount = img.getRefCount();
	ASSERT_EQ(afterCopyCtorRefCount, origRefCount + 1);
}


TEST(GrayImage, MoveConstructor_nullifies_orig_data_and_does_not_increase_ref_count) {
	// Arrange
	GrayImage img(10, 10);
	auto origRefCount = img.getRefCount();

	// Act
	// call move constructor explicitly:
	GrayImage other = std::move(img);

	// Assert
	auto afterCopyCtorRefCount = other.getRefCount();
	ASSERT_EQ(img.data(), nullptr);
	ASSERT_EQ(afterCopyCtorRefCount, origRefCount);
}


TEST(GrayImage, View_increases_ref_count) {
	// Arrange
	GrayImage img(10, 10);
	auto origRefCount = img.getRefCount();

	// Act
	GrayImage view(img, 1, 1, 2, 3);

	// Assert
	ASSERT_EQ(view.getRefCount(), img.getRefCount());
	auto afterAssignmentRefCount = img.getRefCount();

	ASSERT_TRUE(img.data() < view.data() && view.data() < img.data() + img.getWidth() * img.getHeight());
	ASSERT_EQ(afterAssignmentRefCount, origRefCount + 1);
}

TEST(GrayImage, Assignment_increases_ref_count) {
	// Arrange
	GrayImage img(10, 10);
	auto origRefCount = img.getRefCount();

	// Act
	GrayImage other = img;

	// Assert
	ASSERT_EQ(other.getRefCount(), img.getRefCount());
	auto afterAssignmentRefCount = other.getRefCount();

	ASSERT_EQ(img.data(), other.data());
	ASSERT_EQ(afterAssignmentRefCount, origRefCount + 1);
}

TEST(GrayImage, dtor_decreases_ref_count) {
	// Arrange
	GrayImage refHolder(1, 1);
	uint64_t before_img_dtor_ref_count = 0;

	// Act
	{
		GrayImage img(10, 20);
		refHolder = img;
		before_img_dtor_ref_count = refHolder.getRefCount();

	} // dtor of img is called here.

	// Assert
	EXPECT_EQ(refHolder.getWidth(), 10);
	EXPECT_EQ(refHolder.getHeight(), 20);
	EXPECT_EQ(before_img_dtor_ref_count, 2);
	EXPECT_EQ(refHolder.getRefCount(), 1);
}


TEST(GrayImage, Assignment_of_image_decreases_ref_count_of_original_image) {
	// Arrange
	GrayImage orig_img(10, 10);
	GrayImage orig_refHolder(1, 1);
	orig_refHolder = orig_img;
	uint64_t before_assignment_ref_count = orig_refHolder.getRefCount();

	// Act
	GrayImage overriding_img(100, 100);
	orig_img = overriding_img;

	// Assert
	uint64_t after_assignment_ref_count = orig_refHolder.getRefCount();
	EXPECT_EQ(before_assignment_ref_count, 2);
	EXPECT_EQ(after_assignment_ref_count, 1);
}

TEST(GrayImage, Assignment_of_value_does_NOT_decreases_ref_count_of_original_image) {
	// Arrange
	GrayImage orig_img(10, 10);
	GrayImage orig_refHolder(1, 1);
	orig_refHolder = orig_img;
	uint64_t before_assignment_ref_count = orig_refHolder.getRefCount();

	// Act
	orig_img = 5;

	// Assert
	uint64_t after_assignment_ref_count = orig_refHolder.getRefCount();
	EXPECT_EQ(before_assignment_ref_count, 2);
	EXPECT_EQ(after_assignment_ref_count, 2);
}