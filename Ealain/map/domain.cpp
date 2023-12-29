#include <cmath>

#include "domain.h"

namespace ealain {

    namespace domain {

        double cost_to_proba(const double cost)
        {
            return 1 - std::exp(-cost);
        }

        double proba_to_cost(const double proba)
        {
            return -std::log(1 - proba);
        }

    } // domain

} // ealain
