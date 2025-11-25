#include <numeric>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <random>
#include <iterator>

#include <Ealain/io.h>
#include <Ealain/constraint.h>
#include <Ealain/cost.h>
#include <Ealain/map/plan.h>
#include <Ealain/map/cuboid.h>
#include <Ealain/map/geom.h>
#include <Ealain/map/projection.h>
#include <Ealain/detection/group.h>
#include <Ealain/detection/camera.h>

/**
 * Create a single objective instance
 */

// How to launch:
// ./example_co instance_size nb_cameras x0 y0 ...

int main(int argc, char* argv[])
{
    double min_proba = 0.5; // minimum detection proba
    double max_angle_direction = 2*M_PI;

    // Read parameters
    unsigned int n; // # of points in each dimension
    n = atoi(argv[1]);
    int nb_cameras = atoi(argv[2]);
    int range = atoi(argv[3]);

    // Set map
    int m = static_cast<int>(n);
    std::pair<ealain::inst::Map,ealain::proj::Projection<double,size_t>> d = ealain::inst::rectangle(m,m,n,n);
    ealain::inst::Map map = d.first;
    ealain::proj::Projection<double,size_t> p_map = d.second;
    ealain::camera::Unidir::Domain domain(p_map);
    //ealain::camera::Unibinary::Domain domain(p_map);

    // Read coordinates and set cam
    std::vector<std::vector<double>> coordinates;
    std::vector<double> point = {0,0,0};

    ealain::group::proba::AtLeastOne group(p_map); //Change here for aggregation
    std::vector<ealain::camera::Unidir> cameras;
    //std::vector<ealain::camera::Unibinary> cameras;

    int k = 0;
    for (int i=0; i<nb_cameras; i++)
    {
        point[0] = (n-1)*atof(argv[4+k]);
        k++;
        point[1] = (n-1)*atof(argv[4+k]);
        k++;
        point[2] = max_angle_direction*atof(argv[4+k]);
        k++;
        ealain::camera::Unidir camera(map, p_map, point[0], point[1], point[2], n/range);
        //ealain::camera::Unibinary camera(map, p_map, point[0], point[1], n/range);
        cameras.push_back(camera);
    }

    for (int i=0; i<nb_cameras; i++) // Has to be done outside
        group.bind(cameras[i]);

    auto cover = ealain::cost::make_coverage(domain, min_proba);
    double sum = cover(group);
    int total_pixels = ealain::size::items(map);

    std::cout << total_pixels - sum << std::endl;
}


