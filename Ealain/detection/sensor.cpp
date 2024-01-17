#include <tuple>
#include <cassert>

#include "../map/geom.h"
#include "sensor.h"

namespace ealain {
namespace sensor {

double Detector::operator()(const Position& position)
{
    assert(position.size()>0);
    return this->sense(position);
}


const proj::Projection<double,size_t>& Detector::projection() const
{
    return _proj;
}

std::vector<double> Detector::proj(std::vector<size_t> x) const
{
    return _proj(x);
}

std::vector<size_t> Detector::proj(std::vector<double> x) const
{
    return _proj(x);
}

std::vector<std::vector<double>> Detector::proj(std::vector<std::vector<size_t>> x) const
{
    return _proj(x);
}

std::vector<std::vector<size_t>> Detector::proj(std::vector<std::vector<double>> x) const
{
    return _proj(x);
}

} // sensor
} // ealain
