#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"

class Model { 
private:
	std::vector<Vec3f> verts_;
	std::vector<std::vector<int> > faces_;
public:
	Model(const char *filename);
	~Model();
	int nverts();							// 返回定点数
	int nfaces();							// 三角面的数量
	Vec3f vert(int i);						// 查询顶点
	std::vector<int> face(int idx);			// 查询三角面
};

#endif //__MODEL_H__