#pragma once

#include <vector>
#include <string>

struct Point {
    float x , y, z;
};

class DataLoader {
    public:
    static void loadTerrain(const std::string& filepath, std::vector<Point>& pointCloud);
};