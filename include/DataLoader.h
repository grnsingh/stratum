#pragma once

#include <vector>
#include <string>
#include <memory>

struct Point {
    float x , y, z;
};

class DataLoader {
    public:
    static std::unique_ptr<std::vector<Point>> loadTerrain(const std::string& filepath);
};