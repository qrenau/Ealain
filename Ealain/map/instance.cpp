#include <string>
#include <algorithm>
#include <cmath>
#include <limits>
#include <iostream>
#include <fstream>
#include <tuple>
#include <numeric>
#include <ctime>

#include "../utils.h"
#include "instance.h"
#include "projection.h"

namespace ealain {
namespace inst {
std::pair<Map,proj::Projection<double,size_t>> rectangle(
        size_t max_i,
        size_t max_j,
        double min_x, double max_x,
        double min_y, double max_y)
{
    assert(max_x > min_x);
    assert(max_y > min_y);
    proj::Range<double> range_x(min_x,max_x);
    proj::Range<double> range_y(min_y,max_y);
    proj::Range<size_t> range_i(0,max_i-1);
    proj::Range<size_t> range_j(0,max_j-1);

    std::shared_ptr<proj::Proj<double,size_t>> lm1
     = std::make_shared<proj::Linear<double,size_t>>(range_x,range_i); // Mapping from x to i

    std::shared_ptr<proj::Proj<double,size_t>> lm2
     = std::make_shared<proj::Linear<double,size_t>>(range_y,range_j); // Mapping from y to j

    proj::Projection<double,size_t> p_map = proj::Projection<double,size_t>({lm1,lm2});

    Map map = Map(max_i, std::vector<double>(max_j));
    return {map,p_map};
}

std::pair<Map,proj::Projection<double,size_t>> rectangle(size_t size_i, size_t size_j, double size_x, double size_y)
{
    return rectangle(size_i, size_j, 0,size_x, 0,size_y);
}

std::pair<Map,proj::Projection<double,size_t>> rectangle_walls(
        std::string filename,
        size_t max_i,
        size_t max_j,
        double min_x, double max_x,
        double min_y, double max_y)
{
    // Projection
    assert(max_x > min_x);
    assert(max_y > min_y);
    proj::Range<double> range_x(min_x,max_x);
    proj::Range<double> range_y(min_y,max_y);
    proj::Range<size_t> range_i(0,max_i-1);
    proj::Range<size_t> range_j(0,max_j-1);

    std::shared_ptr<proj::Proj<double,size_t>> lm1
     = std::make_shared<proj::Linear<double,size_t>>(range_x,range_i); // Mapping from x to i

    std::shared_ptr<proj::Proj<double,size_t>> lm2
     = std::make_shared<proj::Linear<double,size_t>>(range_y,range_j); // Mapping from y to j

    proj::Projection<double,size_t> p_map = proj::Projection<double,size_t>({lm1,lm2});

    // Load file and create domain
    Map map = Map(max_i, std::vector<double>(max_j,0)); // Set everything to 0 = can be seen
    std::vector<int> row;
    std::string line, word;
    std::fstream file (filename, std::ios::in);
    while(getline(file, line))
    {
        row.clear();
        std::stringstream str(line);
        while(getline(str, word, ','))
        {
            row.push_back(stoi(word));
        }
        map[row[0]][row[1]] = 1;
    }

    return {map,p_map};
}

std::pair<Map,proj::Projection<double,size_t>> rectangle_walls_numeric(
        std::string filename,
        size_t max_i,
        size_t max_j,
        double min_x, double max_x,
        double min_y, double max_y)
{
    // Projection
    assert(max_x > min_x);
    assert(max_y > min_y);
    proj::Range<double> range_x(min_x,max_x);
    proj::Range<double> range_y(min_y,max_y);
    proj::Range<size_t> range_i(0,max_i-1);
    proj::Range<size_t> range_j(0,max_j-1);

    std::shared_ptr<proj::Proj<double,size_t>> lm1
     = std::make_shared<proj::Linear<double,size_t>>(range_x,range_i); // Mapping from x to i

    std::shared_ptr<proj::Proj<double,size_t>> lm2
     = std::make_shared<proj::Linear<double,size_t>>(range_y,range_j); // Mapping from y to j

    proj::Projection<double,size_t> p_map = proj::Projection<double,size_t>({lm1,lm2});

    // Load file and create domain
    Map map = Map(max_i, std::vector<double>(max_j,0)); // Set everything to 0 = can be seen
    std::vector<size_t> row;
    std::string line, word;
    std::fstream file (filename, std::ios::in);
    std::vector<double> xy;
    int count = 0;
    while(getline(file, line))
    {
        row.clear();
        xy.clear();
        std::stringstream str(line);
        while(getline(str, word, ','))
        {
            if (count == 0)
            {
                xy.push_back(max_x*stof(word)+min_x);
                count += 1;
            }
            else
            {
                xy.push_back(max_y*stof(word)+min_y);
                count = 0;
            }

        }
        row = p_map(xy);
        std::cout<< xy[0] << " : " << xy[1] << std::endl;
        std::cout<< row[0] << " : " << row[1] << std::endl;
        std::cout<< "-----------" << std::endl;
        map[row[0]][row[1]] = 1;
    }

    return {map,p_map};
}


std::pair<Map,proj::Projection<double,size_t>> rectangle_walls(std::string filename, size_t size_i, size_t size_j, double size_x, double size_y)
{
    return rectangle_walls(filename, size_i, size_j, 0,size_x, 0,size_y);
}
std::pair<Map,proj::Projection<double,size_t>> rectangle_walls_numeric(std::string filename, size_t size_i, size_t size_j, double size_x, double size_y)
{
    return rectangle_walls_numeric(filename, size_i, size_j, 0,size_x, 0,size_y);
}

} // map
} // ealain
