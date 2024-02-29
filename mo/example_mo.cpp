#include <numeric>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <cstdio>

#include <Ealain/io.h>
#include <Ealain/cost.h>
#include <Ealain/map/plan.h>
#include <Ealain/map/cuboid.h>
#include <Ealain/map/geom.h>
#include <Ealain/map/projection.h>
#include <Ealain/detection/group.h>
#include <Ealain/detection/camera.h>

/**
 * Create MO instance with two cameras models
 * and their associated cost
 */

// How to run: ./example_mo size grid nb_cam_omnidir nb_cam_binary x_omnidir1 y_omnidir1 ... x_bimary1 y_binary1 ...
int main(int argc, char* argv[])
{

    using Domain = ealain::camera::Omnidir::Domain;


    int cost_omnidir = 1;
    int cost_binary = 5;
    double min_proba = 0.1; // minimum detection proba
    // Set the map
    unsigned int n;// # of points in each dimension
    n = atoi(argv[1]);
    double m = static_cast<double>(n);
    std::pair<ealain::inst::Map,ealain::proj::Projection<double,size_t>> d = ealain::inst::rectangle_walls("",n,n,m,m);
    ealain::inst::Map map = d.first;
    ealain::proj::Projection<double,size_t> p_map = d.second;
    Domain domain(n,n,0);

    // Read coordinates and set cam
    int nb_cameras_omnidir = atoi(argv[2]);
    int nb_cameras_binary = atoi(argv[3]);

    std::vector<std::vector<double>> coordinates;
    std::vector<double> point = {0,0};

    ealain::group::proba::AtLeastOne group(p_map);
    std::vector<ealain::camera::Omnidir> cameras_omnidir;
    std::vector<ealain::camera::Omnibinary> cameras_omnibinary;

    int k = 0;
    for (int i=0; i<nb_cameras_omnidir; i++)
    {
        point[0] = (n-1)*atof(argv[4+k]);
        k++;
        point[1] = (n-1)*atof(argv[4+k]);
        k++;
        ealain::camera::Omnidir camera(map, p_map, point[0], point[1], n/4);
        cameras_omnidir.push_back(camera);
    }
    int index = 2*nb_cameras_omnidir+4;
    k = 0;
    for (int i=0; i<nb_cameras_binary; i++)
    {
        point[0] = (n-1)*atof(argv[index+k]);
        k++;
        point[1] = (n-1)*atof(argv[index+k]);
        k++;
        ealain::camera::Omnibinary camera(map, p_map, point[0], point[1], n/6);
        cameras_omnibinary.push_back(camera);
    }

    for (int i=0; i<nb_cameras_omnidir; i++) // Has to be done outside
        group.bind(cameras_omnidir[i]);

    for (int i=0; i<nb_cameras_binary; i++) // Has to be done outside
        group.bind(cameras_omnibinary[i]);


    // Cover the domain
    auto cover = ealain::cost::make_coverage(domain, min_proba);
    double sum = cover(group);

    int total_pixels = ealain::size::items(map);
    std::cout << total_pixels - sum <<" " << nb_cameras_omnidir*cost_omnidir + nb_cameras_binary*cost_binary << std::endl;

}


