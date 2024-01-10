#ifndef __ealain_SENSOR_H__
#define __ealain_SENSOR_H__

#include <cmath>
#include <map>
#include <functional>

//#include <aion/param.h>

#include "../debug.h"
#include "../map/plan.h"
#include "../map/cuboid.h"
#include "../map/geom.h"
#include "../map/instance.h"
#include "../map/projection.h"

namespace ealain {

    //! Core classes of the generic sensor models.
    namespace sensor {

        /** Generic sensor interface
         *
         * Interface defining a sensor as something that computes the cost associated with a given position.
         *
         * @ingroup Interfaces
         * @ingroup Sensors
         */
        class Detector
        {
            protected:
                /** proj from coordinates in the real sensor space (as double)
                 * to coordinates in the indices spaces of the domain (as long int).
                 */
                const proj::Projection<double,size_t>& _proj;

            public:
                /** External interface: compute a cost for a normalized position.
                 *
                 * @param pos a vector representing the position of the cell in the related domain.
                 * @returns The value associated with this position by the sensor (e.g. the probability of detection on this pixel).
                 */
                double operator()(const Position& pos);

                /** Call this detector on all cells of the given domain.
                 *
                 * This calls the (position) interface using iterators on the domain.
                 */
                template<class D>
                D operator()(const D& domain);

                Detector(const proj::Projection<double,size_t>& p) : _proj(p) {};

                const proj::Projection<double,size_t>& projection() const;

                /** Set of proxies toward Projection's interface @{ */
                std::vector<double> proj(std::vector<size_t> x) const;
                std::vector<size_t> proj(std::vector<double> x) const;
                std::vector<std::vector<double>> proj(std::vector<std::vector<size_t>> x) const;
                std::vector<std::vector<size_t>> proj(std::vector<std::vector<double>> x) const;
                /** @} */

                virtual ~Detector() {};

            protected:
                //! Internal interface to be implemented by subclasses.
                virtual double sense(const Position& pos) = 0;
        };


        //! Non-instanciable class used to indicate that there is no default Domain type for the Sensor interface.
        class None
        {
            private:
                None() {}; // private constructor => non instanciable class
        };

        /** The interface from which any kind of sensor should inherit.
         *
         * The template is used to indicate the dimension of the domain.
         * That is, the size of the vector it expect in the Detector::sense interface.
         * That is, the number of scalar it uses to represent a position.
         *
         * @tparam DIM the dimension of the domain.
         *
         * @ingroup Interfaces
         * @ingroup Sensors
         */
        template<unsigned int DIM>
        class Sensor : public Detector
        {
            public:
                //! The dimension on which the sensor operates.
                unsigned int dimension = DIM;

                //! Default to no domain type.
                using Domain = None;
                Sensor(
                        const proj::Projection<double,size_t>& p
                    ) :
                    Detector(p)
                {
                    assert(m.size() == DIM);
                };

                virtual ~Sensor() {}
        };

        /** For sensors expecting a 2D domains, the type is Plan.
         *
         * @ingroup Interfaces
         * @ingroup Sensors
         */
        template<>
        class Sensor<2> : public Detector
        {
            public:
                unsigned int dimension = 2;
                using Domain = ealain::domain::Plan;
                Sensor<2>(
                        const proj::Projection<double,size_t>& p
                    ) :
                    Detector(p)
                {
                    assert(m.size() == 2);
                };
        };

        /** For sensors expecting a 3D domains, the type is Cuboid.
         *
         * @ingroup Interfaces
         * @ingroup Sensors
         */
        template<>
        class Sensor<3> : public Detector
        {
            public:
                unsigned int dimension = 3;
                using Domain = ealain::domain::Cuboid;
                Sensor<3>(
                        const proj::Projection<double,size_t>& p
                    ) :
                    Detector(p)
                {
                    assert(m.size() == 3);
                };
        };

    } // sensor
} // ealain

#include "sensor.hpp"

#endif // __ealain_SENSOR_H__
