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
    const double camera_x = geo.x;
    const double camera_y = geo.y;

    const double d = _eps + geom::ground_distance(target_x, target_y, camera_x, camera_y);

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
    const double camera_x = geo.x;
    const double camera_y = geo.y;

    const double d = _eps + geom::ground_distance(target_x, target_y, camera_x, camera_y);

    // 2D visibility map (0 or 1),
    const double v = geo({position[0],position[1]});
    assert(v==0 or v==1);

    if(v == 0 or d > range) {
        return 0;
    } else {
        return 1;
        }
}

double Unidir::sense(const Position& position)
{
    assert(position.size() >= 2);
    const double target_x = position[0];
    const double target_y = position[1];
    const double camera_x = geo.x;
    const double camera_y = geo.y;

    const double d = _eps + geom::ground_distance(target_x, target_y, camera_x, camera_y);
    double alpha = geom::azimuth(target_x, target_y, camera_x, camera_y);
    double delta_angle = aperture/2;
    double visible_angle = geom::angle_trunc(direction - delta_angle);
    alpha = geom::angle_trunc(alpha - visible_angle);

    // 2D visibility map
    const double v = geo({position[0],position[1]});
    assert(v==0 or v==1);

    if(v == 0 or d > range or alpha > 2*delta_angle) {
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

double Unibinary::sense(const Position& position)
{
    assert(position.size() >= 2);
    const double target_x = position[0];
    const double target_y = position[1];
    const double camera_x = geo.x;
    const double camera_y = geo.y;

    const double d = _eps + geom::ground_distance(target_x, target_y, camera_x, camera_y);
    double alpha = geom::azimuth(target_x, target_y, camera_x, camera_y);
    double delta_angle = aperture/2;
    double visible_angle = geom::angle_trunc(direction - delta_angle);
    alpha = geom::angle_trunc(alpha - visible_angle);

    // 2D visibility map
    const double v = geo({position[0],position[1]});
    assert(v==0 or v==1);

    if(v == 0 or d > range or alpha > 2*delta_angle) {
        return 0;
    } else {
        return 1;
        }

}

} // camera
} // ealain
