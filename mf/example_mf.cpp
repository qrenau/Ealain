#include <numeric>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <chrono>

#include <Ealain/io.h>
#include <Ealain/cost.h>
#include <Ealain/map/plan.h>
#include <Ealain/map/cuboid.h>
#include <Ealain/map/geom.h>
#include <Ealain/map/projection.h>
#include <Ealain/detection/group.h>
#include <Ealain/detection/camera.h>


int main(int argc, char* argv[])
{
    using namespace std::chrono;

    double min_proba = 0.5; // minimum detection proba
    // Set the map
    unsigned int n;// # of points in each dimension
    n = atoi(argv[1]);

    // Big discretisation: m = n
    int m1 = n;
    std::pair<ealain::inst::Map,ealain::proj::Projection<double,size_t>> d1 = ealain::inst::rectangle(m1,m1,n,n);
    ealain::inst::Map map1 = d1.first;
    ealain::proj::Projection<double,size_t> p1_map = d1.second;

    // Small discretisation: m = n/2
    int m2 = static_cast<int>(n/2);
    std::pair<ealain::inst::Map,ealain::proj::Projection<double,size_t>> d2 = ealain::inst::rectangle(m2,m2,n,n);
    ealain::inst::Map map2 = d2.first;
    ealain::proj::Projection<double,size_t> p2_map = d2.second;

    ealain::camera::Omnidir::Domain domain1(p1_map);
    ealain::camera::Omnidir::Domain domain2(p2_map);

    // Read coordinates and set cam
    int nb_cameras = atoi(argv[2]);
    std::vector<std::vector<double>> coordinates;
    std::vector<double> point = {0,0};
    std::vector<double> point2 = {0,0};

    ealain::group::proba::AtLeastOne group1(p1_map);
    ealain::group::proba::AtLeastOne group2(p2_map);
    std::vector<ealain::camera::Omnidir> cameras_big;
    std::vector<ealain::camera::Omnidir> cameras_small;

    int k = 0;
    for (int i=0; i<nb_cameras; i++)
    {
        point[0] = (m1-1)*atof(argv[3+k]);
        point2[0] = (m2-1)*atof(argv[3+k]);
        k++;
        point[1] = (m1-1)*atof(argv[3+k]);
        point2[1] = (m2-1)*atof(argv[3+k]);
        k++;
        ealain::camera::Omnidir camera_big(map1, p1_map, point[0], point[1], n/2);
        cameras_big.push_back(camera_big);
        ealain::camera::Omnidir camera_small(map2, p2_map, point2[0], point2[1], n/2);
        cameras_small.push_back(camera_small);
    }

    for (int i=0; i<nb_cameras; i++) // Has to be done outside
    {
        group1.bind(cameras_big[i]);
        group2.bind(cameras_small[i]);
    }


    // Cover the domain
    // Original size
    auto start1 = high_resolution_clock::now();
    auto cover1 = ealain::cost::make_coverage(domain1, min_proba);
    double sum1 = cover1(group1);
    int total_pixels1 = ealain::size::items(map1);
    auto end1 = high_resolution_clock::now();
    auto duration1 = duration_cast<milliseconds>(end1 - start1);
    std::cout << "Fitness: "<< (total_pixels1-sum1)/total_pixels1 << " : Time: " << duration1.count() << "ms" << std::endl;

    // Discretised size
    auto start2 = high_resolution_clock::now();
    auto cover2 = ealain::cost::make_coverage(domain2, min_proba);
    double sum2 = cover2(group2);
    int total_pixels2 = ealain::size::items(map2);
    auto end2 = high_resolution_clock::now();
    auto duration2 = duration_cast<milliseconds>(end2 - start2);
    std::cout << "Fitness: "<< (total_pixels2-sum2)/total_pixels2<< " : Time: " << duration2.count() << "ms" << std::endl;


}


