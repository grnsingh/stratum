#include <vector>
#include <string>

struct Point {
    float x , y, z;
};

class DataLoader {
    public:
    static std::vector<Point> loadTerrain(const std::string& filepath);
};