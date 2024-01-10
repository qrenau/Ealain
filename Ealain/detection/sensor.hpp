#include "../map/projection.h"

namespace ealain {
namespace sensor {

    template<class D>
    D Detector::operator()(const D& domain)
    {
        assert(domain.dimension == _proj.size());
        D out = domain;
        for(auto it=ealain::begin(out); it != ealain::end(out); ++it) {
            // The underlying model assumes normalized coordinates ou pas !
            std::vector<size_t> position_idx;
            std::vector<double> position_irl;
            position_idx.reserve(domain.dimension);
            position_irl.reserve(domain.dimension);
            for(std::size_t d = 0; d < domain.dimension; ++d) {
                position_idx.push_back(it(d));
            }
            position_irl = _proj(position_idx);
            *it = this->sense(position_irl);
        }
        return out;
    }

} // sensor
} // ealain
