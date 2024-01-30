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

Shape n_points_circle(std::vector<double> center, double radius, int nb_constraints)
{
    Shape circle;
    int step_size = std::floor(360/nb_constraints);
    for (int i=0; i<360; i = i+step_size)
    {
        std::vector<double> point;
        point.push_back(center[0]+radius*std::cos(i*M_PI/180));
        point.push_back(center[1]+radius*std::sin(i*M_PI/180));
        circle.push_back(point);
    }
    return circle;
}

std::vector<Shape> set_shape_old(int nb_constraints, int size, int radius, int square_size)
{
    std::vector<double> center = {std::floor(size/2), std::floor(size/2)};
    std::vector<Shape> shapes;
    Shape circle;

    circle = n_points_circle(center, radius, nb_constraints);
    for (int i=0; i<nb_constraints; i++)
    {
        Shape current_square = {{circle[i][0]-square_size,circle[i][1]-square_size},{circle[i][0]+square_size,circle[i][1]-square_size},
            {circle[i][0]+square_size,circle[i][1]+square_size},{circle[i][0]-square_size,circle[i][1]+square_size}};
        shapes.push_back(current_square);
    }
    return shapes;

}
std::vector<Shape> set_shape(int nb_constraints, int size, int radius, int square_size)
{
    std::vector<double> center = {std::floor(size/2), std::floor(size/2)};
    std::vector<Shape> shapes;
    Shape circle;

    circle = n_points_circle(center, radius, nb_constraints);
    for (int i=0; i<nb_constraints; i++)
    {
        Shape current_square;
        if (square_size == 0)
        {
            current_square = {{circle[i][0]-1,circle[i][1]-1},{circle[i][0],circle[i][1]-1},
                {circle[i][0],circle[i][1]},{circle[i][0]-1,circle[i][1]}};
        }
        else
        {
            current_square = {{circle[i][0]-square_size,circle[i][1]-square_size},{circle[i][0]+square_size,circle[i][1]-square_size},
                {circle[i][0]+square_size,circle[i][1]+square_size},{circle[i][0]-square_size,circle[i][1]+square_size}};
        }
        shapes.push_back(current_square);
    }
    return shapes;
}


std::vector<std::vector<int>> draw_positions(int size, int nb_positions)
{
    std::vector<std::vector<int>> coordinates;
    std::vector<int> point;
    std::vector<double> center = {std::floor(size/2),std::floor(size/2)};
    //Shape circle = n_points_circle(center, size/4,nb_positions);
    Shape circle = n_points_circle(center, size/2.5,nb_positions);

    for (int i=0; i<nb_positions; i++)
    {
        coordinates.push_back({static_cast<int>(circle[i][0]), static_cast<int>(circle[i][1])});
    }
    return coordinates;
}


int main(int argc, char* argv[])
{

    double min_proba = 0.5; // minimum detection proba

    // Read parameters
    unsigned int n;// # of points in each dimension
    n = atoi(argv[1]);
    int nb_cameras = atoi(argv[2]);
    int nb_constraints = atoi(argv[3]);

    // Set map
    int m = static_cast<int>(n);
    std::pair<ealain::inst::Map,ealain::proj::Projection<double,size_t>> d = ealain::inst::rectangle(m,m,n,n);
    ealain::inst::Map map = d.first;
    ealain::proj::Projection<double,size_t> p_map = d.second;
    ealain::camera::Omnidir::Domain domain(p_map);


    std::vector<ealain::camera::Omnidir> cameras;
    std::vector<std::vector<int>> locations;
    if (nb_constraints < nb_cameras)
    {
        // Transform bitstring into camera coordinates
        int i = 4; //Start of the bitstring
        std::vector<std::pair<double,double>> cam_positions;
        for (size_t row=0; row < map.size(); row++)
        {
            for (size_t col=0; col < map[0].size(); col++)
            {
                if (atoi(argv[i]) == 1)
                {
                   std::pair<double,double> coord = std::make_pair(row,col);
                   cam_positions.push_back(coord);
                }
                i++;
            }
        }

        for (int j=0; j<nb_cameras; j++)
        {
            ealain::camera::Omnidir camera(map, p_map, cam_positions[j].first,cam_positions[j].second , n/2);
            cameras.push_back(camera);
        }
    }
    // Case where the number of positions > number of cameras
    else
    {
        locations = draw_positions(n,nb_constraints);
        int start_bitstring = 4;
        for (int i =0; i<nb_constraints; i++)
        {
            if (atoi(argv[i+start_bitstring]) == 1)
            {
                ealain::camera::Omnidir camera(map, p_map, locations[i][0],locations[i][1] , n/2);
                cameras.push_back(camera);
            }
        }

    }

    ealain::group::proba::AtLeastOne group(p_map);
    for (int i=0; i<nb_cameras; i++) // Has to be done outside
        group.bind(cameras[i]);

    auto cover = ealain::cost::make_coverage(domain, min_proba);
    double sum = cover(group);

    //Constraints
    double min_dist_constraint;
    std::vector<Shape> available_shapes;
    if (nb_constraints == 1)
    {
        min_dist_constraint = n*n;
        double pixel_size = 0;
        available_shapes = set_shape(nb_constraints,n,n/2.5,pixel_size);
        std::vector<ealain::constraint::InPolygon> constraints;
        for (int i=0; i<nb_cameras; i++)
        {
            ealain::constraint::InPolygon cons(cameras[i].geo.x, cameras[i].geo.y, available_shapes[0]);
            constraints.push_back(cons);
        }

        double val;
        for (int i=0; i<nb_cameras; i++)
        {
            val = constraints[i]().second;
            min_dist_constraint = std::min(min_dist_constraint,val);
        }
    }

    else if (nb_constraints < nb_cameras)
    {
        min_dist_constraint = 0;
        std::vector<std::vector<ealain::constraint::InPolygon>> constraints;
        double pixel_size = 0;
        available_shapes = set_shape(nb_constraints,n,n/2.5,pixel_size);
        for (int j=0; j<nb_constraints; j++)
        {
            std::vector<ealain::constraint::InPolygon> current_constraint;
            for (int i=0; i<nb_cameras; i++)
            {
                ealain::constraint::InPolygon cons(cameras[i].geo.x, cameras[i].geo.y, available_shapes[j]);
                current_constraint.push_back(cons);
            }
            constraints.push_back(current_constraint);
        }

        for (size_t j=0; j<constraints.size(); j++)
        {
            double value = n*n;
            for (int i=0; i<nb_cameras; i++)
            {
                value = std::min(value,constraints[j][i]().second);
            }
            min_dist_constraint = min_dist_constraint + value;

        }
    }


    int total_pixels = ealain::size::items(map);
    if (min_dist_constraint == 0)
        std::cout << total_pixels - sum + min_dist_constraint << std::endl;
    else
        std::cout << total_pixels + min_dist_constraint << std::endl;

}


