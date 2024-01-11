#include <tuple>

#include "../map/geom.h"
#include "../utils.h"
#include "sensor.h"
#include "situated.h"

namespace ealain {
namespace sensor {

void Situated::update()
{

    // Set _visibility to zero, then compute visibility_map with overwrite = false
    for( auto& v : _visibility.data()) {
        std::fill(ALL(v), 0);
    }

    std::vector<double> xy = {static_cast<double>(x),static_cast<double>(y)};
    if (!_bit)
    {
        std::vector<size_t> ij = _proj(xy);
        geom::visibility_map_2D_ray_tracing(_visibility.data(),_map,  ij[0], ij[1], false);
    }
    else
    {
        geom::visibility_map_2D_ray_tracing(_visibility.data(),_map,  xy[0], xy[1], false);
    }


    _has_visibility = true;
}

double Situated::sense(const Position& position)
{
    assert(position.size() >= 2);
    if(not _has_visibility) {
        update();
    }

    std::vector<double> xy = {position[0],position[1]};
    std::vector<size_t> ij = _proj(xy);

    return _visibility[ij];
}

const domain::PlanT<char>& Situated::visibility()
{
    if( not _has_visibility) {
        update();
    }
    return _visibility;
}

const inst::Map& Situated::map() const
{
    return _map;
}

} // sensor
} // ealain
