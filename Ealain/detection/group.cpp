#include "../utils.h"
#include "group.h"

namespace ealain {
namespace group {

Group::Group(proj::Projection<double,size_t>& p) :
    sensor::Detector(p)
{}


void Group::bind(sensor::Detector& sensor)
{
    assert(sensor.projection().size() == this->_proj.size());
    this->push_back(sensor);
}


namespace proba {

    double AtLeastOne::sense(const Position& position)
    {
        double cost = 1.0;
        // Probability of having no detection whatsoever
        for(auto&& sense : *this) {
            cost = cost * (1 - sense(position));
        }
        if(cost < 0.0) {
            cost = 0.0;
        }
        assert(is_proba(cost));
        // Probability of having at least one detection.
        const double res = 1 - cost;
        assert(is_proba(res));
        if(res < _min) {
            return _min;
        } else if(res > _max) {
            return _max;
        } else {
            return res;
        }
    }

} // proba

double Aggregate::sense(const Position& position)
{
    double cost = _init;
    for(auto&& sense : *this) {
        cost = _func(cost, sense(position));
    }
    return cost;
}

    double Binary::sense(const Position& position)
    {
        double cost = 0;
        for(auto&& sense : *this) {
            if(sense(position) > _threshold) {
                cost = 1;
            }
        }
        assert(cost == 0 or cost == 1);
        return cost;
    }

} // net
} // ealain
