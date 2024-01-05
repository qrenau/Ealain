#ifndef __EALAIN_INSTANCE_H__
#define __EALAIN_INSTANCE_H__

#include <iostream>
#include <functional>
#include <fstream>
#include <string>
#include <iomanip>


#include "projection.h"

namespace ealain {

    namespace inst {

        using Map = std::vector<std::vector<double>>;

        // Generate a rectangle map, with ranges starting at zero.
        std::pair<Map,proj::Projection<double,size_t>> rectangle(size_t size_i, size_t size_j, double size_x, double size_y);

        // Generate a rectangle map, with complete ranges.
        std::pair<Map,proj::Projection<double,size_t>> rectangle(
                size_t max_i,
                size_t max_j,
                double min_x, double max_x,
                double min_y, double max_y);

        // Generate a rectangle map, loading walls from a csv file of pixels coordiantes of walls.
        std::pair<Map,proj::Projection<double,size_t>> rectangle_walls(std::string filename, size_t size_i, size_t size_j, double size_x, double size_y);
        std::pair<Map,proj::Projection<double,size_t>> rectangle_walls_numeric(std::string filename, size_t size_i, size_t size_j, double size_x, double size_y);

        std::pair<Map,proj::Projection<double,size_t>> rectangle_walls(std::string filename,size_t max_i, size_t max_j, double min_x, double max_x,double min_y, double max_y);
        std::pair<Map,proj::Projection<double,size_t>> rectangle_walls_numeric(std::string filename,size_t max_i, size_t max_j, double min_x, double max_x,double min_y, double max_y);


    } // inst
} // ealain


#endif // __EALAIN_INSTANCE_H
