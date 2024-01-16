#ifndef __EALAIN_CONSTRAINT_H__
#define __EALAIN_CONSTRAINT_H__

#include <utility>
#include <vector>
#include <random>
#include <limits>

#include "map/geom.h"
#include <cassert>

namespace ealain {

    namespace constraint {
    // Polygonal constraint
    class InPolygon
    {
        protected:
            double _x;
            double _y;
            std::vector<std::vector<double>> _poly;

        public:
            InPolygon(
                    const double& x,
                    const double& y,
                    const std::vector<std::vector<double>>& poly
            ) :
                _x(x),
                _y(y),
                _poly(poly)
            {}

            // Evaluate distance of a point to a polygon
            std::pair<bool,double> operator()();
    };


} // constraint
} // ealain

#endif // __EALAIN_CONSTRAINT_H__
