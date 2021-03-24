#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char *filename) : verts_(), faces_() {
    std::ifstream in(filename, std::ifstream::in);
    if (in.is_open()) {
        std::string line;
        while (!in.eof()) {
            std::getline(in, line);
            // 将字符串转换成字符输入流
            std::istringstream iss(line.c_str());
            char trash;                                         // 用于丢掉空格和/符号
            // 定位顶点数据位置，line下标为0-2的字符串和"v "比较
            if (!line.compare(0, 2, "v ")) {
                iss >> trash;
                Vec3f v;
                for (int i = 0; i < 3; ++i) iss >> v[i];    // 读取三个顶点
                verts_.emplace_back(v);
            // 定位三角面数据位置
            } else if (!line.compare(0, 2, "f ")) {
                std::vector<int> f;
                int itrash, idx;
                iss >> trash;
                while (iss >> idx >> trash >> itrash >> trash >> itrash) {
                    idx--;
                    f.emplace_back(idx);
                }
                faces_.emplace_back(f);
            }
        }
    }
    in.close();
    
    std::cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

std::vector<int> Model::face(int idx) {
    return faces_[idx];
}

Vec3f Model::vert(int i) {
    return verts_[i];
}
