#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);
const TGAColor blue  = TGAColor(0,   0,   255, 255);
const int width  = 800;
const int height = 800;

Model *model = nullptr;

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

Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P) {
    Vec3f s[2];         // 存储三个向量
    for (int i = 2; i--;) {
        s[i][0] = C[i] - A[i];
        s[i][1] = B[i] - A[i];
        s[i][2] = A[i] - P[i];
    }
    Vec3f u = cross(s[0], s[1]);
    // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
    if (std::abs(u[2]) > 1e-2)
        return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
    // in this case generate negative coordinates, it will be thrown away by the rasterizator
    return Vec3f(-1, 1, 1);
}

void triangle(Vec3f *pts, Vec2i *texts, float *zbuffer, TGAImage& image, float& intensity) {
    Vec2f bboxmin( std::numeric_limits<float>::max(),  std::numeric_limits<float>::max());
    Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    Vec2f clamp(image.get_width() - 1, image.get_height() - 1);
    
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 2; ++j) {
            bboxmin[j] = std::max(0.f,      std::min(bboxmin[j], pts[i][j]));
            bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
        }
    }
    Vec3f P;
    for (P.x = bboxmin.x; P.x <= bboxmax.x; ++P.x) {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; ++P.y) {
            // 重心坐标
            Vec3f bc_screen = barycentric(pts[0], pts[1], pts[2], P);
            // 有小于0的说明点在三角形外
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;

            P.z = 0;
            Vec2i Ptext(0, 0);              // uv坐标
            for (int i = 0; i < 3; ++i) P.z += pts[i][2] * bc_screen[i];
            //  三角面内部的点uv值是由三个顶点插值得到，即通过重心坐标计算当前顶点的贡献然后累加
            for (int i = 0; i < 3; ++i) Ptext[0] += texts[i][0] * bc_screen[i];
            for (int i = 0; i < 3; ++i) Ptext[1] += texts[i][1] * bc_screen[i];

            if (zbuffer[int(P.x + P.y * width)] <= P.z) {
                TGAColor color = model->diffuse(Ptext);
                color = color * intensity;
                zbuffer[int(P.x + P.y * width)] = P.z;
                image.set(P.x, P.y, color);
            }
        }
    }
}

Vec3f world2screen(Vec3f v) {
    // attention we have to change x and y to int otherwise they maybe too close for the for loop
    // we add 0.5 to rounding off
    return Vec3f(int((v.x + 1.) * width / 2. + .5), int((v.y + 1.) * height / 2. + .5), v.z);
}

int main(int argc, char** argv) { 
    if (2 == argc)
        model = new Model(argv[1]);
    else
        model = new Model("obj/african_head/african_head.obj");
    
    float *zbuffer = new float[width * height];
    for (int i = width * height; i--; zbuffer[i] = -std::numeric_limits<float>::max());

    TGAImage image(width, height, TGAImage::RGB);
    Vec3f light_dir(0, 0, -1);

    for (int i = 0; i < model->nfaces(); ++i) {
        std::vector<int> face = model->face(i);
        Vec3f world_coords[3];
        Vec3f screen_coords[3];
        Vec2i texts[3];
        
        for (int j = 0; j < 3; j++) {
            world_coords[j] = model->vert(face[j]);
            screen_coords[j] = world2screen(world_coords[j]);
            texts[j] = model->uv(i, j);
        }
        Vec3f n = cross(world_coords[2] - world_coords[0], world_coords[1] - world_coords[0]);
        n.normalize();
        float intensity = light_dir * n;
        if (intensity > 0) {
            triangle(screen_coords, texts, zbuffer, image, intensity);
        }
    }

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    delete model;

    return 0; 
}