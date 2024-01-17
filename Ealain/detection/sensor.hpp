#include "../map/projection.h"

namespace ealain {
namespace sensor {

    template<class D>
    D Detector::operator()(const D& domain)
    {
        assert(domain.dimension == _proj.size());
        D out = domain;
        for(auto it=ealain::begin(out); it != ealain::end(out); ++it) {
            std::vector<size_t> position_discr;
            std::vector<double> position_num;
            position_discr.reserve(domain.dimension);
            position_num.reserve(domain.dimension);
            for(std::size_t d = 0; d < domain.dimension; ++d) {
                position_discr.push_back(it(d));
            }
            position_num = _proj(position_discr);
            *it = this->sense(position_num);
        }
        return out;
    }

} // sensor
} // ealain
