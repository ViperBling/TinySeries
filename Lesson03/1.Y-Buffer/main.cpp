#include <vector>
#include <cmath>
#include <iostream>
#include <limits>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);
const TGAColor blue  = TGAColor(0,   0,   255, 255);

const int width  = 800;
const int height = 500;

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

Vec3f barycentric(Vec2i *pts, Vec2i P) { 
	Vec3f A = Vec3f(pts[2][0] - pts[0][0], pts[1][0] - pts[0][0], pts[0][0] - P[0]);
	Vec3f B = Vec3f(pts[2][1] - pts[0][1], pts[1][1] - pts[0][1], pts[0][1] - P[1]);
    Vec3f u = A ^ B;			// geometry.h中重载了^运算符表示外积
    /* `pts` and `P` has integer value as coordinates
       so `abs(u[2])` < 1 means `u[2]` is 0, that means
       triangle is degenerate, in this case return something with negative coordinates */
    if (std::abs(u[2])<1) return Vec3f(-1,1,1);
    return Vec3f(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z); 
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

 void rasterize(Vec2i p0, Vec2i p1, TGAImage& image, TGAColor color, int ybuffer[]) {
    if (p0.x > p1.x) {
        std::swap(p0, p1);
    }
    for (int x = p0.x; x < p1.x; ++x) {
        // 从左到右，首先计算“斜率”
        float t = (x - p0.x) / float(p1.x - p0.x);
        // 根据插值计算y值，也就是绘制一条直线
        int y = p0.y * (1. - t) + p1.y * t + 0.5;
        // 如果当前深度值小于y，说明要被覆盖
        if (ybuffer[x] < y) {
            ybuffer[x] = y;
            image.set(x, 0, color);
        }
    }
}

int main(int argc, char** argv) {
    { 
        // just dumping the 2d scene (yay we have enough dimensions!)
        TGAImage scene(width, height, TGAImage::RGB);

        // scene "2d mesh"
        line(Vec2i(20, 34),   Vec2i(744, 400), scene, red);
        line(Vec2i(120, 434), Vec2i(444, 400), scene, green);
        line(Vec2i(330, 463), Vec2i(594, 200), scene, blue);

        // screen line，对应最下面那条白色线
        line(Vec2i(10, 10), Vec2i(790, 10), scene, white);

        scene.flip_vertically(); // i want to have the origin at the left bottom corner of the image
        scene.write_tga_file("scene.tga");
    }

    {
        TGAImage render(width, 16, TGAImage::RGB);
        int ybuffer[width];
        for (int i = 0; i < width; i++) {
            ybuffer[i] = std::numeric_limits<int>::min();
        }
        rasterize(Vec2i(20, 34),   Vec2i(744, 400), render, red,   ybuffer);
        rasterize(Vec2i(120, 434), Vec2i(444, 400), render, green, ybuffer);
        rasterize(Vec2i(330, 463), Vec2i(594, 200), render, blue,  ybuffer);

        // 1-pixel wide image is bad for eyes, lets widen it
        for (int i = 0; i < width; i++) {
            for (int j = 1; j < 16; j++) {
                render.set(i, j, render.get(i, 0));
            }
        }
        render.flip_vertically(); // i want to have the origin at the left bottom corner of the image
        render.write_tga_file("render.tga");
    }

    return 0;
}