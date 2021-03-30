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
const int depth  = 255;

Model *model = nullptr;

Vec3f light_dir(0, 0, -1);
Vec3f camera(0, 0, 3);

Matrix viewport(int x, int y, int w, int h) {
    Matrix m = Matrix::identity();
    m[0][3] = x + w / 2.f;
    m[1][3] = y + h / 2.f;
    m[2][3] = depth / 2.f;

    m[0][0] = w / 2.f;
    m[1][1] = h / 2.f;
    m[2][2] = depth / 2.f;
    return m;
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
                // TGAColor color(intensity * 255, intensity * 255, intensity * 255, 255);
                color = color * intensity;
                zbuffer[int(P.x + P.y * width)] = P.z;
                image.set(P.x, P.y, color);
            }
        }
    }
}

Vec3f world2screen(Vec3f& v, Matrix& Projection, Matrix& ViewPort) {
    Vec4f gl_vertex = embed<4>(v);                      // embed Vec3f to homogenius coordinates
    gl_vertex = ViewPort * Projection * gl_vertex;      // transform it to screen coordinates
    Vec3f v3 = gl_vertex[3] == 0 ? proj<3>(gl_vertex) : proj<3>(gl_vertex/gl_vertex[3]);

    return Vec3f(int(v3.x+.5), int(v3.y+.5), v3.z);
}

Vec3f world2screen(Vec3f& v) {
    // attention we have to change x and y to int otherwise they maybe too close for the for loop
    // we add 0.5 to rounding off
    return Vec3f(int((v.x + 1.) * width / 2. + .5), int((v.y + 1.) * height / 2. + .5), v.z);
}

int main(int argc, char** argv) { 
    if (2 == argc)
        model = new Model(argv[1]);
    else
        model = new Model("../obj/african_head/african_head.obj");
    
    float* zbuffer = new float[width * height];
    for (int i = width * height; i--;) zbuffer[i] = std::numeric_limits<float>::min();

    {
        Matrix Projection = Matrix::identity();
        Matrix ViewPort   = viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
        Projection[3][2] = -1.f / camera.z;

        TGAImage image(width, height, TGAImage::RGB);
        for (int i = 0; i < model->nfaces(); ++i) {
            std::vector<int> face = model->face(i);
            Vec3f world_coords[3];
            Vec3f screen_coords[3];
            Vec2i texts[3];

            for (int j = 0; j < 3; j++) {
                world_coords[j] = model->vert(face[j]);
                screen_coords[j] = world2screen(world_coords[j], Projection, ViewPort);
                // screen_coords[j] = world2screen(world_coords[j]);
                texts[j] = model->uv(i, j);
            }
            Vec3f n = cross(world_coords[2] - world_coords[0], world_coords[1] - world_coords[0]);
            n.normalize();
            float intensity = n * light_dir;
            if (intensity > 0) {
                triangle(screen_coords, texts, zbuffer, image, intensity);
            }
        
        }
        image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
        image.write_tga_file("output.tga");
    }

    {
        TGAImage zbimage(width, height, TGAImage::GRAYSCALE);
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                TGAColor color = zbuffer[int(i + j * width)];
                zbimage.set(i, j, color);
            }
        }
        
        zbimage.flip_vertically(); // i want to have the origin at the left bottom corner of the image
        zbimage.write_tga_file("zbuffer.tga");
    }
    
    delete model;
    delete [] zbuffer;

    return 0; 
}