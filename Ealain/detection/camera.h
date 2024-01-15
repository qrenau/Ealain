#ifndef __EALAIN_CAMERA_H__
#define __EALAIN_CAMERA_H__

#include <cassert>
#include "sensor.h"
#include "situated.h"

namespace ealain {


    namespace camera {

        /** Simple camera seing around
         *
         * Probability of detection decrease with range.
         */
        class Omnidir : public sensor::Sensor<2>
        {
            public:
                double range;
                sensor::Situated geo;

                Omnidir(
                    const inst::Map& map,
                    const proj::Projection<double,size_t>& p,
                    const double range_
                ) :
                    sensor::Sensor<2>(p),
                    range(range_),
                    geo(map,p)
                {
                    assert(range >= 0);
                }

                Omnidir(
                    const inst::Map& map,
                    const proj::Projection<double,size_t>& p,
                    const double x,
                    const double y,
                    const double range_
                ) :
                    sensor::Sensor<2>(p),
                    range(range_),
                    geo(map,p,x,y)
                {
                    assert(range >= 0);
                }

                Omnidir(
                    const inst::Map& map,
                    const proj::Projection<double,size_t>& p,
                    const bool bit,
                    const double x,
                    const double y,
                    const double range_
                ) :
                    sensor::Sensor<2>(p),
                    range(range_),
                    geo(map,p,bit,x,y)
                {
                    assert(range >= 0);
                }

            protected:
                const double _eps = 1e-6;

                // Linear function starting at 1 and decreasing to 0 when reaching range.
                virtual double sense(const Position& position);

        };

        /** Simple camera seing around
         *
         * Detection is binary: 1 in range, 0 otherwise.
         */
        class Omnibinary : public sensor::Sensor<2>
        {
            public:
                double range;
                sensor::Situated geo;

                Omnibinary(
                    const inst::Map& map,
                    const proj::Projection<double,size_t>& p,
                    const double range_
                ) :
                    sensor::Sensor<2>(p),
                    range(range_),
                    geo(map,p)
                {
                    assert(range >= 0);
                }

                Omnibinary(
                    const inst::Map& map,
                    const proj::Projection<double,size_t>& p,
                    const double x,
                    const double y,
                    const double range_
                ) :
                    sensor::Sensor<2>(p),
                    range(range_),
                    geo(map,p,x,y)
                {
                    assert(range >= 0);
                }

                Omnibinary(
                    const inst::Map& map,
                    const proj::Projection<double,size_t>& p,
                    const bool bit,
                    const double x,
                    const double y,
                    const double range_
                ) :
                    sensor::Sensor<2>(p),
                    range(range_),
                    geo(map,p,bit,x,y)
                {
                    assert(range >= 0);
                }

            protected:
                const double _eps = 1e-6;

                virtual double sense(const Position& position);

        };

    } // camera
} // ealain

#endif // __EALAIN_CAMERA_H__

