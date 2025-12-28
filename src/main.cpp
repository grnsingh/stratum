#include <iostream>
#include <vector>

struct Point {float x, y, z;};

int main(){
    Point p = {0.1, 0.2, 0.3};
    auto [x,y,z] = p;

    std::cout << "C++ Version: " << __cplusplus << std::endl;
    return 0;
}