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


/* Generate instances with walls
 * 
 */
int main(int argc, char* argv[])
{
    double min_proba = 0.5; // minimum detection proba
    // Read parameters
    unsigned int n; // # of points in each dimension
    std::string inst_name = argv[1];
    n = atoi(argv[2]);
    int nb_cameras = atoi(argv[3]);
    int range = atoi(argv[4]);


    // Set the map
    double m = static_cast<double>(n);
    std::string instance = "../Instances/"+inst_name+".csv";
    std::pair<ealain::inst::Map,ealain::proj::Projection<double,size_t>> d = ealain::inst::rectangle_walls(instance,m,m,n,n);
    ealain::inst::Map map = d.first;
    ealain::proj::Projection<double,size_t> p_map = d.second;
    ealain::camera::Omnidir::Domain domain(p_map);

    // Read coordinates and set cam
    std::vector<std::vector<double>> coordinates;
    std::vector<double> point = {0,0};

    ealain::group::proba::AtLeastOne group(p_map); //Change here for aggregation
    std::vector<ealain::camera::Omnidir> cameras;

    int k = 0;
    for (int i=0; i<nb_cameras; i++)
    {
        point[0] = (n-1)*atof(argv[5+k]);
        k++;
        point[1] = (n-1)*atof(argv[5+k]);
        k++;
        ealain::camera::Omnidir camera(map, p_map, point[0], point[1], n/range);
        cameras.push_back(camera);
    }

    for (int i=0; i<nb_cameras; i++) // Has to be done outside
        group.bind(cameras[i]);


    // Cover the domain
    auto cover_any = ealain::cost::make_coverage(domain, min_proba);
    double sum_any = cover_any(group);

    int total_pixels = ealain::size::items(map);
    std::cout << total_pixels - sum_any << std::endl;

}


