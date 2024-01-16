#include <numeric>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>

#include <Ealain/io.h>
#include <Ealain/cost.h>
#include <Ealain/map/plan.h>
#include <Ealain/map/cuboid.h>
#include <Ealain/map/geom.h>
#include <Ealain/map/projection.h>
#include <Ealain/detection/group.h>
#include <Ealain/detection/camera.h>

int main()
{
    size_t n = 20;
    double m = 20;
    using Domain = ealain::camera::Omnidir::Domain;

    std::pair<ealain::inst::Map,ealain::proj::Projection<double,size_t>> d = ealain::inst::rectangle(n,n,m,m);
    ealain::inst::Map map = d.first;
    ealain::proj::Projection<double,size_t> p_map = d.second;

    Domain dom(n,n,0);

    ealain::camera::Omnibinary camera_0(map, p_map, m/2, m/2, m/2);
    dom = camera_0(dom);
    ealain::group::Additive group(p_map,{camera_0});
    ealain::sav::img::ascii(dom.data(), std::clog);
    double min_proba = 0.5;
    auto cover = ealain::cost::make_coverage(dom, min_proba);
    double sum = cover(group);
    std::cout << sum << std::endl;
}

