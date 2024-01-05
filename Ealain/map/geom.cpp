#include <cmath>
#include <limits>
#include <algorithm>

#include "../utils.h"
#include "instance.h"
#include "geom.h"
#include "raster.h"

namespace ealain {
namespace geom {

double ground_distance(double target_x, double target_y, double sensor_x, double sensor_y)
{
    return std::sqrt( std::pow(target_x-sensor_x,2) + std::pow(target_y-sensor_y,2) );
}

double spatial_distance(double target_x, double target_y, double target_h, double sensor_x, double sensor_y, double sensor_h)
{
    return std::sqrt( std::pow(target_x-sensor_x,2) + std::pow(target_y-sensor_y,2) + std::pow(target_h-sensor_h,2) );
}

float raster_distance(int target_i, int target_j, int sensor_i, int sensor_j)
{
    return std::sqrt( std::pow(target_i-sensor_i,2) + std::pow(target_j-sensor_j,2) );
}

std::pair<bool,double> is_in_polygon(const std::vector<std::vector<double>>& poly, const std::vector<double>& point)
{
    double distance = std::numeric_limits<double>::max();
    double distance_ortho;
    double u_ortho;
    std::vector<double> u,v;
    int wind = 0;
    int n = poly.size();
    assert(n > 0);

    double d0, d1; //Temporary values to loop over the edges
    double p0 = poly[n-1][0];
    double p1 = poly[n-1][1];
    for (int i=0; i<n; i++) {
        //Check if the point is a vertex of the polygon
        if ((point[0] == poly[i][0]) and (point[1] == poly[i][1])) {
            return std::make_pair(true,0);
        } else {
            d0 = poly[i][0];
            d1 = poly[i][1];

            u = {point[0]-p0, point[1]-p1};
            v = {p0-d0, p1-d1};

            //double norm_v = std::pow(std::sqrt(v[0]*v[0] + v[1]+v[1]),2);
            u_ortho = u[0]*v[0] + u[1]*v[1];
            std::vector<double> projection = {d0+u_ortho, d1+u_ortho};
            if ((projection[0] <= d0) and (projection[0] >= p0) and (projection[1] <= d1) and (projection[1] >= p1)) {
                distance_ortho = ground_distance(point[0],point[1],projection[0],projection[1]);
            } else {
                double distance_vertex1 = ground_distance(point[0],point[1],p0,p1);
                double distance_vertex2 = ground_distance(point[0],point[1],d0,d1);
                if (distance_vertex1 <= distance_vertex2) {
                    distance_ortho = distance_vertex1;
                } else {
                    distance_ortho = distance_vertex2;
                }
            }
            distance = std::min(distance,distance_ortho);

            // Windup number computation
            double A = (p0-point[0])*(d1-point[1]) - (p1-point[1])*(d0-point[0]);
            if (p1 <= point[1]) {
                if ((d1 > point[1]) and (A > 0)) {
                    wind++;
                }
            } else {
                if ((d1 <= point[1]) and (A < 0)) {
                    wind--;
                }
            }
            p0 = d0;
            p1 = d1;
        }
    }
    if ((wind == 1) or (wind == -1)) {
        return std::make_pair(true,0);
    } else if (wind == 0) {
        return std::make_pair(false,distance);
    } else {
        return std::make_pair(false, -1); //Return -1 if the polygon is not simple
    }
}


unsigned int full_visibility_map_2D(std::vector<std::vector<char>>& visibles, const inst::Map& map)
{
    // Visibles pixels assume everything is visible.
    unsigned int i_len=map.size();
    assert(i_len>0);
    unsigned int j_len=map[0].size();
    assert(j_len>0);

    unsigned int counter = 0;
    for (unsigned int i=0; i<i_len; i++)
    {
        for (unsigned int j=0; j<j_len; j++)
        {
            visibles[i][j] = 1;
            counter++;

        }
    }

    return counter;
}

unsigned int visibility_map_2D_ray_tracing(std::vector<std::vector<char>>& visibles, const inst::Map& map, const int sensor_i, const int sensor_j, bool overwrite)
{
    // Visibles pixels.
    unsigned int i_len=map.size();
    assert(i_len>0);
    unsigned int j_len=map[0].size();
    assert(j_len>0);

    // Coordinates of pixels on the border.
    std::vector<raster::Point> border;
    for(unsigned int i=0; i < i_len; ++i) {
        border.push_back(std::make_pair(i,0));
        border.push_back(std::make_pair(i,j_len-1));
    }
    for(unsigned int j=0; j < i_len; ++j) {
        border.push_back(std::make_pair(0,j));
        border.push_back(std::make_pair(i_len-1,j));
    }

    // Populate the array.
    unsigned int counter = 0;
    // If camera on a wall, visibility = 0 everywhere
    if (map[sensor_i][sensor_j] == 1)
        return counter;
    else
    {
        for(auto&& p : border) {

            raster::Line raj = raster::pixels_line(sensor_i, sensor_j, raster::row(p), raster::col(p));

            bool visible = true;
            for(auto&& pii : raj) {
                raster::Point t = raster::point(pii);

                // Because of rounding, the extreme points of lines may fall of out of the domain.
                if( raster::row(t) < 0
                 or raster::col(t) < 0
                 or raster::row(t) >= map.size()
                 or raster::col(t) >= map[raster::row(t)].size()) {
                    // Just discard those points.
                    continue;
                }
                if (map[raster::row(t)][raster::col(t)] == 1)
                {
                    visible = false;
                }
                if (visible)
                {
                    if(overwrite)
                    {
                        visibles[raster::row(t)][raster::col(t)] = 1;
                        counter++;
                    } else {
                        if(visibles[raster::row(t)][raster::col(t)] == 0)
                        {
                            visibles[raster::row(t)][raster::col(t)] = 1;
                            counter++;
                        }
                    }
                }
            } // for t in raj
        } // for p in border
    }
    return counter;
}

} // ealain
} // geom
