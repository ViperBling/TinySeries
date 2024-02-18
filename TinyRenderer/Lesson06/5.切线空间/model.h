/*
 * @Author: Zero
 * @Date: 2021-03-23 18:34:21
 * @LastEditTime: 2021-04-02 16:57:32
 * @Description: 
 */
#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"
#include "tgaimage.h"

class Model {
    
private:
	std::vector<Vec3f> verts_;
	std::vector<Vec3f> norms_;					// 法向
	std::vector<std::vector<Vec3i>> faces_;
	std::vector<Vec2f> uvs_;					// UV坐标

	TGAImage diffusemap_;						// 材质贴图
	TGAImage normalmap_;						// 法线贴图
	TGAImage specularmap_;						// 高光贴图
	void loadTexture_(std::string filename, const char * suffix, TGAImage& image);

public:
	Model(const char *filename);
	~Model();
	int nverts();								// 返回定点数
	int nfaces();								// 三角面的数量
	Vec3f vert(int i);							// 查询顶点
	Vec3f vert(int iface, int nthvert);
	Vec3f normal(const Vec2f& uv);				// 根据uv查询法线贴图
	Vec3f normal(int iface, int nthvert);		// 根据三角面和顶点查询法线
	Vec2f uv(int iface, int nthvert);			// 查询uv，第iface个三角面的第nvert个顶点
	TGAColor diffuse(const Vec2f& uv);			// 根据uv得到颜色
	float specular(const Vec2f& uv);			// 高光系数
	std::vector<int> face(int idx);				// 查询三角面
	
};

#endif //__MODEL_H__