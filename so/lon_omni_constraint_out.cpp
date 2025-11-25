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

using Shape = std::vector<std::vector<double>>;

Shape n_points_circle(std::vector<double> center,int size, double radius, int nb_points)
{
    std::vector<std::vector<double>> circle;
    std::vector<double> point {-1,-1};
    int side = std::floor(360/nb_points);
    for (int i=0; i<nb_points; i++)
    {
        int nb_loop =0;
        do
        {
            int index = (rand() % side) + (i+1)*side;
            point[0] = center[0]+radius*std::cos(index*M_PI/180);
            point[1] = center[1]+radius*std::sin(index*M_PI/180);
            nb_loop++;
            if ((point[0] < 0) || (point[1] < 0) || (point[0] > size) || (point[1] >size)) {
                std::cout << " Out of bounds"<< std::endl;
            }
        }
        while ((point[0] < 0) || (point[1] < 0) || (point[0] > size) || (point[1] >size));
        circle.push_back(point);

    }
    return circle;
}

Shape set_shape(int vertex_number, int size, int radius, int seed)
{
    std::vector<double> center;
    Shape coordinates;

    srand(seed);
    center.push_back(rand() % size);
    center.push_back(rand() % size);

    coordinates = n_points_circle(center, size, radius, vertex_number);
    return coordinates;
}

Shape set_definite_shape(int shape_number, int domain_size)
{
    int size = static_cast<int>(domain_size/10);
    //std::vector<double> x{1/domain_size, 9/10, 1/2, 7/10, 1/domain_size, 1/2, 9/10, 1/domain_size, 3/10, 3/10};
    std::vector<double> x{1/domain_size, 0.85, 0.5, 0.7, 1/domain_size, 0.5, 0.85, 1/domain_size, 0.3, 0.3};
    //std::vector<double> y{1/domain_size, 9/10, 1/2, 1/2, 9/10, 3/10, 1/domain_size, 1/2, 3/10, 7/10};
    std::vector<double> y{1/domain_size, 0.85, 0.5, 0.5, 0.85, 0.3, 1/domain_size, 0.5, 0.3, 0.7};
    Shape shape;
    std::vector<double> point {-1,-1};

    double x0,y0, x1,y1, x2,y2, x3,y3;
    x0 = x[shape_number]*domain_size;
    y0 = y[shape_number]*domain_size;
    x1 = x[shape_number]*domain_size+size;
    y1 = y0;
    x2 = x0;
    y2 = y[shape_number]*domain_size+size;
    x3 = x[shape_number]*domain_size+size;
    y3 = y[shape_number]*domain_size+size;
    point[0] = x0;
    point[1] = y0;
    shape.push_back(point);
    point[0] = x1;
    point[1] = y1;
    shape.push_back(point);
    point[0] = x3;
    point[1] = y3;
    shape.push_back(point);
    point[0] = x2;
    point[1] = y2;
    shape.push_back(point);


    return shape;
}


int main(int argc, char* argv[])
{

    double min_proba = 0.5; // minimum detection proba
    bool log = false;
    if (atoi(argv[argc-1]) == 1)
        log = true;
    // Read parameters
    unsigned int n;// # of points in each dimension
    n = atoi(argv[1]);
    int nb_cameras = atoi(argv[2]);
    int nb_constraints = atoi(argv[3]);
    int nb_vertex = atoi(argv[4]);
    int seed = atoi(argv[5]);
    int initial_seed = seed;

    // Set map
    double m = static_cast<double>(n);
    std::pair<ealain::inst::Map,ealain::proj::Projection<double,size_t>> d = ealain::inst::rectangle(m,m,n,n);
    ealain::inst::Map map = d.first;
    ealain::proj::Projection<double,size_t> p_map = d.second;
    ealain::camera::Omnidir::Domain domain(p_map);

    // Read coordinates and set cam
    std::vector<std::vector<double>> coordinates;
    std::vector<double> point = {0,0};

    ealain::group::proba::AtLeastOne group(p_map);
    std::vector<ealain::camera::Omnidir> cameras;

    int k = 0;
    for (int i=0; i<nb_cameras; i++)
    {
        point[0] = (n-1)*atof(argv[6+k]);
        k++;
        point[1] = (n-1)*atof(argv[6+k]);
        k++;
        ealain::camera::Omnidir camera(map, p_map, point[0], point[1], n/2);
        cameras.push_back(camera);
    }

    for (int i=0; i<nb_cameras; i++) // Has to be done outside
        group.bind(cameras[i]);

    auto cover_any = ealain::cost::make_coverage(domain, min_proba);
    double sum_any = cover_any(group);

    //Constraints
    std::vector<Shape> available_shapes;
    int total_pixels = ealain::size::items(map);
    int penalty = 0;

    //TODO restrict location of centers ! -> bias on constraint locations ! -> do it by hand !?
    for (int j=0; j<nb_constraints; j++)
    {
        //Shape shape = set_shape(nb_vertex,n-1,n/6,seed);
        Shape shape = set_definite_shape(j,n);
        seed++;
        for (int i=0; i<nb_cameras; i++)
        {
            ealain::constraint::InPolygon cons(cameras[i].geo.x, cameras[i].geo.y, shape);
            if (!cons().first)
            {
                penalty += total_pixels;
                break;
            }
        }

        available_shapes.push_back(shape);
    }

    if (log)
    {
        std::ofstream logFile;

        logFile.open("../Instances/instance_info.csv", std::ios::out);
        logFile << n << "," << initial_seed << "," << nb_cameras<< "," << nb_constraints << "," << nb_vertex << ",";
        for (size_t s=0; s<available_shapes.size(); s++)
        {
            for (int v=0; v<nb_vertex; v++)
            {
                logFile << available_shapes[s][v][0] << "," << available_shapes[s][v][1]  << ",";
            }
        }
        for (int c=0; c<nb_cameras; c++)
        {
            logFile << cameras[c].geo.x << "," << cameras[c].geo.y << "," ;
        }
        logFile << std::endl;
        logFile.close();
    }


    std::cout << total_pixels - sum_any + penalty << std::endl;
    //domain = network(domain);
    //ealain::sav::img::ascii(domain.data(), std::clog);

}


