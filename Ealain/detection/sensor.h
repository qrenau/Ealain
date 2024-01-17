#ifndef __EALAIN_SENSOR_H__
#define __EALAIN_SENSOR_H__

#include <cmath>
#include <map>
#include <functional>

#include <cassert>
#include "../map/plan.h"
#include "../map/cuboid.h"
#include "../map/geom.h"
#include "../map/instance.h"
#include "../map/projection.h"

namespace ealain {

    namespace sensor {

        /** Generic sensor interface
         * Interface defining a sensor as something that computes the cost associated with a given position.
         */
        class Detector
        {
            protected:
                const proj::Projection<double,size_t>& _proj;

            public:
                // Compute a cost for a normalized position.
                double operator()(const Position& pos);

                // Call this detector on all cells of the given domain.
                template<class D>
                D operator()(const D& domain);

                Detector(const proj::Projection<double,size_t>& p) : _proj(p) {};

                const proj::Projection<double,size_t>& projection() const;

                // Set of proxies toward Projection's interface
                std::vector<double> proj(std::vector<size_t> x) const;
                std::vector<size_t> proj(std::vector<double> x) const;
                std::vector<std::vector<double>> proj(std::vector<std::vector<size_t>> x) const;
                std::vector<std::vector<size_t>> proj(std::vector<std::vector<double>> x) const;

                virtual ~Detector() {};

            protected:
                // Internal interface to be implemented by subclasses.
                virtual double sense(const Position& pos) = 0;
        };


        // Non-instanciable class used to indicate that there is no default Domain type for the Sensor interface.
        class None
        {
            private:
                None() {}; // private constructor as it is a non instanciable class
        };

        /** The interface from which any kind of sensor should inherit.
         *
         * The template is used to indicate the dimension of the domain.
         */
        template<unsigned int DIM>
        class Sensor : public Detector
        {
            public:
                // The dimension on which the sensor operates.
                unsigned int dimension = DIM;

                // Default to no domain type.
                using Domain = None;
                Sensor(
                        const proj::Projection<double,size_t>& p
                    ) :
                    Detector(p)
                {
                    assert(p.size() == DIM);
                };

                virtual ~Sensor() {}
        };

        // For sensors in 2D, the domain type is Plan.
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
                    assert(p.size() == 2);
                };
        };

        // For sensors in 3D, the domain type is Cuboid.
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
                    assert(p.size() == 3);
                };
        };

    } // sensor
} // ealain

#include "sensor.hpp"

#endif // __EALAIN_SENSOR_H__
