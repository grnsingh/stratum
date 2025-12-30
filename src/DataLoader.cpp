#include "DataLoader.h"

#include <iostream>
#include <cstdio>

std::unique_ptr<std::vector<Point>> DataLoader::loadTerrain(const std::string& filepath){

    std::cout << "[THREAD] Background task started for: " << filepath << std::endl;
    auto pointsCloud = std::make_unique<std::vector<Point>>();

    FILE* file = fopen(filepath.c_str(), "r");
    if(!file){
        std::cerr<<"[ERROR] Could not open file"<<std::endl;
        return pointsCloud;
    }

    // Reserveng atleast 1M ponits
    pointsCloud->reserve(1000000);

    float x, y, z;
    while(fscanf(file, "%f %f %f", &x, &y, &z) == 3){
        Point p;
        p.x =x;
        p.y =y;
        p.z =z;
        pointsCloud->push_back(p);
    }

    fclose(file);

    std::cout<<"[SUCCESS] Loaded "<< pointsCloud->size() <<" points"<<std::endl;
    return pointsCloud;
}