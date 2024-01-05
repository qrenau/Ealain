#ifndef __EALAIN_RASTER_H__
#define __EALAIN_RASTER_H__

#include <cmath>
#include <limits>
#include <algorithm>

#include "instance.h"

namespace ealain {

        // Bresenham algorithm.
    namespace raster {
        // namespace line {

            // Coordinate of a point in an image, using row/column
            using Point = std::pair<int,int>;

            // Point with a value.
            using Pix = std::pair<Point,float>;

            // Make a Pix
            Pix pixel(Point p, float value = 0);

            // Extract the Point in a Pix.
            Point point(Pix p);

            // Extract the row coordinate of a Point.
            float row(Point p);

            // Extract the row coordinate of a Pix.
            float row(Pix p);

            // Extract the column coordinate of a Point.
            float col(Point p);

            // Extract the column coordinate of a Pix.
            float col(Pix p);

            // Extract the value of a Pix.
            float shade(Pix p);
        // } // line

        // A line is a vector of pixels
        using Line = std::vector<Pix>;

        namespace line {

            float  fpart(float i);

            float rfpart(float i);

            /** Add the given point to the given line (if its value is greater than a threshold).
             *
             * return true if the point was added, else false.
             */
            bool add(Line& line, Point p, float thickness=1, float thickness_threshold=0);

            /** Bresenham's algorithm for drawing line of pixels.
             *
             * Derived from: Bresenham, Jack E., "Algorithm for computer control of a digital plotter", IBM Sjstems Journal, vol. 4, 1, 1965, p. 25–30, doi:10.1147/sj.41.0025
             *
             * return The computed line.
             */
            Line bresenham( float i1, float j1, float i2, float j2);

        } // line

        /** Compute the list of euclidean integer pixels covering the given segment.
         *
         * return The computed line.
         */
        Line pixels_line( float i1, float j1, float i2, float j2 );

        namespace poly {

            struct RowCmp
            {
                bool operator()(const Pix& lhs, const Pix& rhs) const;
            };

            // Rasterize a polygon given in pixel coordinates.
            std::vector<Pix> rasterize(
                    const std::vector<std::vector<size_t>>& polygon);

        } // poly
    } // raster
} // ealain
#endif // __EALAIN_RASTER_H__
