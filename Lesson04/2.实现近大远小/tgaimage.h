#ifndef __TGAIMAGE_H__
#define __TGAIMAGE_H__

#include <fstream>

// 每个特定平台上的编译器都有自己的默认“对齐系数”(也叫对齐模数)。程序员可以通过预编译命令#pragma pack(n)，n=1,2,4,8,16来改变这一系数，其中的n就是你要指定的“对齐系数”
// #pragma pack(n)			将当前字节对齐值设为n
// #pragma pack()			将当前字节对齐值设为默认值（通常是8）
// #pragma pack(push, 1)	先将当前字节对齐值压入编译栈栈顶, 然后再将n设为当前值
// #pragma pack(pop)		将编译栈栈顶的字节对齐值弹出, 然后丢弃

#pragma pack(push, 1)
/*
	TGA
*/
struct TGA_Header
{
	char  idlength;					// 图像信息字段长度
	char  colormaptype;				// 颜色表类型，0 = None, 1 = paletted，有颜色
	char  datatypecode;				// 图像类型码，0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
	short colormaporigin;			// 颜色表首址，First colour map entry
	short colormaplength;			// 颜色表的长度 Number of Colors
	char  colormapdepth;			// 颜色表项位数，bits per palette entry
	short x_origin;					// image x origin
	short y_origin;					// image y origin
	short width;					// width in pixels
	short height;					// height in pixels
	char  bitsperpixel;				// 图像每像素存储占用位数 (8, 16, 24, 32)
	char  imagedescriptor;			// image descriptor
};
#pragma pack(pop)

struct TGAColor {
	union {
		struct {
			unsigned char b, g, r, a;
		};
		unsigned char raw[4];
		unsigned int val;
	};
	int bytespp;

	TGAColor() : val(0), bytespp(1) {
	}

	TGAColor(unsigned char R, unsigned char G, unsigned char B, unsigned char A=255) : b(B), g(G), r(R), a(A), bytespp(4) {
	}

	TGAColor(int v, int bpp) : val(v), bytespp(bpp) {
	}

	TGAColor(const TGAColor &c) : val(c.val), bytespp(c.bytespp) {
	}

	TGAColor(const unsigned char *p, int bpp) : val(0), bytespp(bpp) {
		for (int i=0; i<bpp; i++) {
			raw[i] = p[i];
		}
	}

	TGAColor & operator =(const TGAColor &c) {
		if (this != &c) {
			bytespp = c.bytespp;
			val = c.val;
		}
		return *this;
	}
};



class TGAImage {
protected:
	unsigned char* data;
	int width;
	int height;
	int bytespp;

	// RLE是TGA的压缩算法
	bool   load_rle_data(std::ifstream& in);
	bool unload_rle_data(std::ofstream& out);

public:
	enum Format {
		GRAYSCALE = 1, RGB = 3, RGBA = 4
	};

	TGAImage();
	TGAImage(int w, int h, int bpp);
	TGAImage(const TGAImage& img);
	bool read_tga_file(const char* filename);							// 读取TGA文件
	bool write_tga_file(const char* filename, bool rle = true);			// 写入TGA文件
	bool flip_horizontally();											// 水平翻转
	bool flip_vertically();												// 垂直翻转
	bool scale(int w, int h);											// 缩放
	TGAColor get(int x, int y);											// 获取指定位置颜色
	bool set(int x, int y, TGAColor c);								// 设置颜色
    // bool set(int x, int y, const TGAColor& c);
	~TGAImage();														// 析构函数
	TGAImage & operator =(const TGAImage& img);							// 重载赋值运算符
	int get_width();													// 获取图像维度
	int get_height();
	int get_bytespp();
	unsigned char* buffer();											// ֱ读取图像数据
	void clear();														// 
};

#endif // __TGAIMAGE_H__
