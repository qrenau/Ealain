#include "../utils.h"
#include "../map/geom.h"
#include "camera.h"

namespace ealain {
namespace camera {

double Omnidir::sense(const Position& position)
{
    assert(position.size() >= 2);
    const double target_x = position[0];
    const double target_y = position[1];
    const double radar_x = geo.x;
    const double radar_y = geo.y;

    const double d = _eps + geom::ground_distance(target_x, target_y, radar_x, radar_y);

    // 2D visibility map
    const double v = geo({position[0],position[1]});
    assert(v==0 or v==1);

    if(v == 0 or d > range) {
        return 0;
    } else {
        double p = (range-d)/range;
        // In case output > 1
        if(p>1) {
            return 1;
        } else {
            assert(is_proba(p));
            return p;
        }
    }
}

double Omnibinary::sense(const Position& position)
{
    assert(position.size() >= 2);
    const double target_x = position[0];
    const double target_y = position[1];
    const double radar_x = geo.x;
    const double radar_y = geo.y;

    const double d = _eps + geom::ground_distance(target_x, target_y, radar_x, radar_y);

    // 2D visibility map (0 or 1),
    const double v = geo({position[0],position[1]});
    assert(v==0 or v==1);

    if(v == 0 or d > range) {
        return 0;
    } else {
        return 1;
        }
}

} // camera
} // ealain
