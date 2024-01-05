#ifndef __EALAIN_GEOM_H__
#define __EALAIN_GEOM_H__

#include <cmath>

#include "instance.h"

namespace ealain {

    using Position = std::vector<double>;

    namespace geom {
        const double pi = M_PI;

        // Euclidean distance.
        double ground_distance(double target_i, double target_j, double sensor_i, double sensor_j);

        double spatial_distance(double target_x, double target_y, double target_h, double sensor_x, double sensor_y, double sensor_h);

        // Euclidean distance, in pixels.
        float raster_distance(int target_i, int target_j, int sensor_i, int sensor_j);


        /** Check if a point is in a simple Polygon.
         * Return the distance to the polygon.
         * Use the winding number algorithm.
         * If polygon is not simple, distance is -1
         */
        std::pair<bool,double> is_in_polygon(const std::vector<std::vector<double>>& poly, const std::vector<double>& point);


        // Compute the full visibility map as if everything was visible
        unsigned int full_visibility_map_2D(std::vector<std::vector<char>>& visibles, const inst::Map& map);

        /** Compute the map of pixels that are linear-sight visibles from a given camera.
         *
         * Do not take into account any detection model.
         *
         * return Number of pixels that are visibles.
         */

        unsigned int visibility_map_2D_ray_tracing(std::vector<std::vector<char>>& visibles, const inst::Map& map, const int sensor_i, const int sensor_j, bool overwrite = false);

    } // geom
} // ealain

#endif // __EALAIN_GEOM_H__
