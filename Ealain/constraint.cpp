#include "constraint.h"

namespace ealain {
namespace constraint {

std::pair<bool,double> InPolygon::operator()()
{
    std::pair<bool,double> res;
    std::vector<double> point {_x,_y};
    res = geom::is_in_polygon(_poly,point);

    // Constraint not violated if point in polygon
    res.first = !res.first;
    return res;
}

} // constraint
} // ealain
