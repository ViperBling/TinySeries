#include <vector>
#include <cmath>
#include <iostream>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);

const int width  = 800;
const int height = 800;

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

int main(int argc, char** argv) {
    if (2 == argc)
		model = new Model(argv[1]);
	else 
		model = new Model("obj/african_head.obj");

	TGAImage image(width, height, TGAImage::RGB);
	Vec3f light_dir(0, 0, -1);

	for (int i = 0; i < model->nfaces(); ++i) {
		std::vector<int> face = model->face(i);
		Vec2i screen_coords[3];			// 像素坐标
		Vec3f world_coords[3];			// 三维坐标
		for (int j = 0; j < 3; ++j) {
			Vec3f v = model->vert(face[j]);
			screen_coords[j] = Vec2i((v.x + 1.) * width / 2., (v.y + 1.) * height / 2.);
			world_coords[j] = v;		// 三维坐标就是顶点位置，用他们计算法线方向
		}
		// 这里^就是外积
		Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
		n.normalize();
		float intensity = n * light_dir;		// 通过内积得到光强度

		if (intensity > 0)
			triangle(screen_coords[0], screen_coords[1], screen_coords[2], image, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255));
	}

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");

	delete model;
    return 0;
}