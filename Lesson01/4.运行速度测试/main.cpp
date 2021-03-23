#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

/* TEST1 */
// void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
// 	bool steep = false;
// 	if (std::abs(x0 - x1) < std::abs(y0 - y1)) { // 如果斜率大于1，翻转直线
// 		std::swap(x0, y0);
// 		std::swap(x1, y1);
// 		steep = true;
// 	}
// 	if (x0 > x1) {		// 确保从左往右绘制
// 		std::swap(x0, x1);
// 		std::swap(y0, y1);
// 	}
// 	for (int x = x0; x <= x1; ++x) {
// 		float t = (x - x0) / (float)(x1 - x0);
// 		int y = y0 * (1. - t) + y1 * t;
// 		if (steep)
// 			image.set(y, x, color); // 如果直线被翻转了，那么绘制的时候再翻回来
// 		else
// 			image.set(x, y, color);
// 	}
// }

/* TEST 2 */
// void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
// 	bool steep = false;
// 	if (std::abs(x0 - x1) < std::abs(y0 - y1)) { // 如果斜率大于1，翻转直线
// 		std::swap(x0, y0);
// 		std::swap(x1, y1);
// 		steep = true;
// 	}
// 	if (x0 > x1) {		// 确保从左往右绘制
// 		std::swap(x0, x1);
// 		std::swap(y0, y1);
// 	}

// 	int dx = x1 - x0;
// 	int dy = y1 - y0;
// 	float derror = std::abs(dy / float(dx));
// 	float error = 0;
// 	int y = y0;

// 	for (int x = x0; x <= x1; ++x) {
// 		if (steep)
// 			image.set(y, x, color); // 如果直线被翻转了，那么绘制的时候再翻回来
// 		else
// 			image.set(x, y, color);
// 		error += derror;

// 		if (error > .5) {
// 			y += (y1 > y0 ? 1 : -1);
// 			error -= 1.;
// 		}
// 	}
// }

/* TEST 3 */
void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1)) { // 如果斜率大于1，翻转直线
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1) {		// 确保从左往右绘制
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int dx = x1 - x0;
	int dy = y1 - y0;
	float derror2 = std::abs(dy) * 2; 		// 以2代替1/dx
	float error2 = 0;						// error2 = error * dx * 2
	int y = y0;

	for (int x = x0; x <= x1; ++x) {
		if (steep)
			image.set(y, x, color); // 如果直线被翻转了，那么绘制的时候再翻回来
		else
			image.set(x, y, color);

		error2 += derror2;
		if (error2 > dx) {
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

int main(int argc, char** argv) {
    TGAImage image(100, 100, TGAImage::RGB);
    for (int i = 0; i < 1000000; i++) {
        line(13, 20, 80, 40, image, white);
        line(20, 13, 40, 80, image, red);
        line(80, 40, 13, 20, image, red);
    }
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    return 0;
}