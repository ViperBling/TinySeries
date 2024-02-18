#include <vector>
#include <cmath>
#include <iostream>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);

const int width  = 200;
const int height = 200;

Model *model = nullptr;

/* TEST 3 */
void line(Vec2i p0, Vec2i p1, TGAImage& image, TGAColor color) {
	bool steep = false;
	if (std::abs(p0.x - p1.x) < std::abs(p0.y - p1.y)) { // 如果斜率大于1，翻转直线
		std::swap(p0.x, p0.y);
		std::swap(p1.x, p1.y);
		steep = true;
	}
	if (p0.x > p1.x) {		// 确保从左往右绘制
		std::swap(p0, p1);
	}

	int dx = p1.x - p0.x;
	int dy = p1.y - p0.y;
	float derror2 = std::abs(dy) * 2; 		// 以2代替1/dx
	float error2 = 0;						// error2 = error * dx * 2
	int y = p0.y;

	for (int x = p0.x; x <= p1.x; ++x) {
		if (steep)
			image.set(y, x, color); // 如果直线被翻转了，那么绘制的时候再翻回来
		else
			image.set(x, y, color);

		error2 += derror2;
		if (error2 > dx) {
			y += (p1.y > p0.y ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color) {

	if (t0.y>t1.y) std::swap(t0, t1); 
    if (t0.y>t2.y) std::swap(t0, t2); 
    if (t1.y>t2.y) std::swap(t1, t2);

	int total_height = t2.y - t0.y;				// 三角形的总高度
	for (int i = 0; i < total_height; ++i) {
		bool second_half = i > t1.y - t0.y || t1.y == t0.y;								// 判断是在上部分还是下部分，在上部分为真
		int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;					// 水平分割后的下半部分高度
		float alpha = (float)i / total_height;											// 当前y坐标占总高度比例
		float beta  = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height;	// 当前y坐标占半高度比例
		Vec2i A = t0 + (t2 - t0) * alpha;												// 缩放到对等长度，确保在同一水平线
		Vec2i B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;
		if (A.x > B.x) std::swap(A, B);			// 从左往右绘制
		for (int j = A.x; j <= B.x; ++j) 		// 画水平直线
			image.set(j, t0.y + i, color);
	}
}

int main(int argc, char** argv) {
    
    TGAImage image(width, height, TGAImage::RGB);

	Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)};
    Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)};
    Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};

    triangle(t0[0], t0[1], t0[2], image, red);
    triangle(t1[0], t1[1], t1[2], image, white);
    triangle(t2[0], t2[1], t2[2], image, green);

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    return 0;
}