#ifndef __EALAIN_GROUP_H__
#define __EALAIN_GROUP_H__

#include "../map/geom.h"
#include "../map/projection.h"
#include "sensor.h"
#include <limits>
#include <cassert>

namespace ealain {

    namespace group {

        /** Group of sensors
         *
         * Detector returning a cost when called on a position.
         */
        class Group : public sensor::Detector, protected std::vector<std::reference_wrapper<sensor::Detector>>
        {
            public:
                // Empty constructor.
                Group(proj::Projection<double,size_t>& p);

                // Constructor with mandatory list of references.
                Group(proj::Projection<double,size_t>& p,
                        std::initializer_list<std::reference_wrapper<sensor::Detector>> detectors ) :
                    sensor::Detector(p),
                    std::vector<std::reference_wrapper<sensor::Detector>>(detectors)
                {}

                // Just a proxy to push_back
                void bind(sensor::Detector& sensor);

                virtual ~Group() {};

        };

        namespace proba {

            /** Group of sensors computing probabilities of detection under independency assumption.
             *
             * One want to have the probability of having a least one detection across the Group.
             * Under the assumption that those events are independant.
             *
             * Group aggregates them by multiplying the probabilities of non-detection
             * Given a set of sensors \f$s\in S\f$, it returns $f:x \mapsto [0,1]$:
             *      f(x) = 1 - \prod_{s \in S} (1 - s(x))
             *
             */
            class AtLeastOne : public Group
            {
                protected:
                    const double _min;
                    const double _max;

                public:
                    AtLeastOne(proj::Projection<double,size_t>& p,
                            const double proba_min = 0.0, const double proba_max = 1.0) :
                        Group(p),
                        _min(proba_min), _max(proba_max)
                    {
                        assert(_min < _max);
                        assert(is_proba(_min));
                        assert(is_proba(_max));
                    }

                    AtLeastOne(proj::Projection<double,size_t>& p,
                            std::initializer_list<std::reference_wrapper<sensor::Detector>> detectors,
                            const double proba_min = 0.0, const double proba_max = 1.0
                        ) :
                        Group(p, detectors),
                        _min(proba_min), _max(proba_max)
                    {
                        assert(_min < _max);
                        assert(is_proba(_min));
                        assert(is_proba(_max));
                    }


                    virtual double sense(const Position& position);
            };

        } // proba

        /** A Group which cost is computed as an aggregation of its sensors.
         *
         * In this low-level interface, you can indicate your own aggregation function.
         *
         * Most probably use one of the derived classes,
         * with implicit parameters: Additive, Multiplicative, Min, Max, etc.
         *
         * The aggregation is done via a generic function,
         * in the spirit of the `std::accumulate` function:
         * cost = func(cost, sense(position));
         *
         * The signature should be equivalent to the following:
         * double fun(const double & lhs, const double & rhs);
         */
        class Aggregate : public Group
        {
            public:
                using Function = std::function<double(const double&, const double&)>;

            protected:
                const double _init;
                const Function _func;

            public:
                Aggregate(proj::Projection<double,size_t>& p,
                        const double init, const Function func) :
                    Group(p),
                    _init(init),
                    _func(func)
                { }

                Aggregate(proj::Projection<double,size_t>& p,
                        std::initializer_list<std::reference_wrapper<sensor::Detector>> detectors,
                        const double init, const Function func) :
                    Group(p,detectors),
                    _init(init),
                    _func(func)
                {}

            protected:
                virtual double sense(const Position& position);
        };

        // Group that add costs of each sensor.
        class Additive : public Aggregate
        {
            public:
                Additive(proj::Projection<double,size_t>& p, const double init = 0) :
                    Aggregate(p,init,std::plus<double>())
                {}

                Additive(proj::Projection<double,size_t>& p,std::initializer_list<std::reference_wrapper<sensor::Detector>> detectors, const double init = 0) :
                    Aggregate(p,detectors,init,std::plus<double>())
                {}
        };

        // Group that multiply costs of each sensor.
        class Multiplicative : public Aggregate
        {
            public:
                Multiplicative(proj::Projection<double,size_t>& p) :
                    Aggregate(p,1,std::multiplies<double>())
                {}

                Multiplicative(proj::Projection<double,size_t>& p,std::initializer_list<std::reference_wrapper<sensor::Detector>> detectors) :
                    Aggregate(p,detectors,1,std::multiplies<double>())
                {}
        };

        // Group that consider the minimum cost of all sensors.
        class Min : public Aggregate
        {
            public:
                Min(proj::Projection<double,size_t>& p) :
                    Aggregate(p,std::numeric_limits<double>::max(),std::less<double>())
                {}

                Min(proj::Projection<double,size_t>& p,std::initializer_list<std::reference_wrapper<sensor::Detector>> detectors) :
                    Aggregate(p,detectors,std::numeric_limits<double>::max(),std::less<double>())
                {}
        };

        // Group that consider the maximum cost of all sensors.
        class Max : public Aggregate
        {
            public:
                Max(proj::Projection<double,size_t>& p) :
                    Aggregate(p,std::numeric_limits<double>::min(),std::greater<double>())
                {}

                Max(proj::Projection<double,size_t>& p,std::initializer_list<std::reference_wrapper<sensor::Detector>> detectors) :
                    Aggregate(p,detectors,std::numeric_limits<double>::min(),std::greater<double>())
                {}
        };

        // Group that outputs either one or zeros.
        class Binary : public Group

        {
            protected:
                double _threshold;

            public:
                Binary(proj::Projection<double,size_t>& p, double threshold = 0) :
                    Group(p),
                    _threshold(threshold)
                {}

                Binary(proj::Projection<double,size_t>& p, std::initializer_list<std::reference_wrapper<sensor::Detector>> detectors, double threshold = 0) :
                    Group(p, detectors),
                    _threshold(threshold)
                {}

                virtual double sense(const Position& position);
        };

    } // net
} // ealain


#endif // __EALAIN_GROUP_H__
