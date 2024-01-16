#include <sstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <iostream>

#include <Ealain/io.h>
#include <Ealain/utils.h>
#include <Ealain/map/instance.h>
#include <Ealain/map/geom.h>
#include <Ealain/map/projection.h>
#include <Ealain/detection/group.h>
#include <Ealain/detection/camera.h>
#include <Ealain/cost.h>
#include <Ealain/map/plan.h>
#include <Ealain/map/cuboid.h>

int main()
{
    size_t n = 20;
    double m = 20;
    double min_proba = 0.2;
    using Domain = ealain::camera::Omnidir::Domain;

    std::pair<ealain::inst::Map,ealain::proj::Projection<double,size_t>> d = ealain::inst::rectangle(n,n,m,m);
    ealain::inst::Map map = d.first;
    ealain::proj::Projection<double,size_t> p_map = d.second;


    Domain dom(n,n,0);

    ealain::camera::Omnidir camera_0(map, p_map, 0, 12, m/2);
    ealain::camera::Omnidir camera_1(map, p_map, 12, 0, m/2);
    ealain::group::proba::AtLeastOne group(p_map,{camera_0, camera_1});
    auto cover = ealain::cost::make_coverage(dom, min_proba);
    double sum = cover(group);
    std::cout << sum << std::endl;

    dom = group(dom);
    std::cout<<"DETECTION: "<<std::endl;
    ealain::sav::img::ascii(dom.data(), std::clog);
}

