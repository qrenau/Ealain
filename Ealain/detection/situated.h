#ifndef __EALAIN_SITUATED_H__
#define __EALAIN_SITUATED_H__

#include <cmath>
#include <map>
#include <functional>

#include <cassert>
#include "../map/plan.h"
#include "../map/geom.h"
#include "../map/instance.h"
#include "../map/projection.h"

namespace ealain {
    namespace sensor {

        // Minimal geo-Situated model of a generic sensor that computes visibility maps.
        class Situated : public Sensor<2>
        {
            protected:

                // Cache flag.
                bool _has_visibility;

                // Note: do not use `bool` because the STL's specialized vector of bool cannot return reference to items.
                domain::PlanT<char> _visibility;

                const inst::Map& _map;

                bool _bit;

            public:
                double x;
                double y;

                Situated(
                        const inst::Map& map,
                        const proj::Projection<double,size_t>& p
                    ) :
                    Sensor<2>(p),
                    _map(map),
                    _has_visibility(false),
                    _bit(false),
                    _visibility(p[0].range_idx().max()+1, p[1].range_idx().max()+1, 0)
                {}

                Situated(
                        const inst::Map& map,
                        const proj::Projection<double,size_t>& p,
                        const bool bit,
                        const double x_,
                        const double y_
                    ) :
                    Sensor<2>(p),
                    _map(map),
                    _has_visibility(false),
                    _bit(bit),
                    _visibility(p[0].range_idx().max()+1, p[1].range_idx().max()+1, 0),
                    x(x_),
                    y(y_)
                {}

                Situated(
                        const inst::Map& map,
                        const proj::Projection<double,size_t>& p,
                        const double x_,
                        const double y_
                    ) :
                    Sensor<2>(p),
                    _map(map),
                    _has_visibility(false),
                    _bit(false),
                    _visibility(p[0].range_idx().max()+1, p[1].range_idx().max()+1, 0),
                    x(x_),
                    y(y_)
                {}

                // Internal interface implemented by this subclass.
                virtual double sense(const Position& position);

                // Return the current visibility map cache.
                const domain::PlanT<char>& visibility();

                // Update the internal visibility map cache.
                void update();

                const inst::Map& map() const;
        };

    } // sensor
} // ealain

#endif // __EALAIN_SITUATED_H__
