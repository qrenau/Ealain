#ifndef __EALAIN_COST_H__
#define __EALAIN_COST_H__

#include "detection/group.h"
#include "map/domain.h"

namespace ealain {

    // Compute cost of domain as a single value
    namespace cost {

        // Interface for cost operators computing the value of a domain.
        template<class D>
        class Cost
        {
            protected:
                D& _domain;

            public:
                using domain_type = D;

                D& domain() {return _domain;}

                Cost(D& domain) : _domain(domain) {}

                virtual double operator()(group::Group& group) = 0;
        };

        // Compute the number of cells which values are greater than or equal to a threshold.
        template<class D>
        class Coverage: public Cost<D>
        {
            protected:
                const double _threshold;
            public:
                Coverage(D& domain, const double cost_threshold = 0) :
                    Cost<D>(domain),
                    _threshold(cost_threshold)
                {}

                virtual double operator()(group::Group& group);
        };

        template<class D>
        Coverage<D> make_coverage(D& domain);

    } // cost

} // ealain

#include "cost.hpp"

#endif // __EALAIN_COST
