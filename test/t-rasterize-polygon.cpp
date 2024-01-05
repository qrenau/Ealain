#include<cmath>
#include<iostream>
#include <cassert>

#include <Ealain/io.h>
#include <Ealain/map/geom.h>
#include <Ealain/map/plan.h>
#include <Ealain/map/raster.h>

using namespace ealain;
using namespace ealain::geom;

const size_t width = 40;

void check_holes(ealain::domain::PlanT<char>& img)
{
    size_t n;
    for(size_t j=0; j < img.box_size(1); ++j) {
        n = 0;
        for(size_t i=1; i < img.box_size(0); ++i) {
            if( img(i,j) != img(i-1,j)) {
                n++;
            }
        } // i
        if(n>1) {
            size_t nb_holes = static_cast<size_t>(std::round(static_cast<float>(n-2)/2));
            assert(nb_holes == 0);
        }
    } // j
}

void show(const std::vector<ealain::raster::Pix>& pixset)
{
    ealain::domain::PlanT<char> img(width,width,0);
    for(const auto& p : pixset) {
        img( static_cast<size_t>(ealain::raster::col(p)), static_cast<size_t>(ealain::raster::row(p)) ) = 1;
    }

    ealain::sav::img::ascii( img.data(), std::clog );
}

int main()
{
    const std::vector<std::vector<size_t>> polygon = {{1,1},{22,14},{12,25},{12,29},{39,39},{8,39},{11,20}};

    std::vector<ealain::raster::Pix> pixels = ealain::raster::poly::rasterize(polygon);

    show(pixels);

    const float pi = M_PI;
    const std::vector<float> polar_polygon = {0, pi/2, pi, 3*pi/2};
    const size_t radius = width/2-1;
    for(float angle=0; angle < pi/2; angle += (pi/2)/10) {
        std::vector<std::vector<size_t>> rotated;
        for(const auto a : polar_polygon) {
            float x = radius+std::round(radius*std::cos(angle+a));
            float y = radius+std::round(radius*std::sin(angle+a));
            rotated.push_back( { static_cast<size_t>(x), static_cast<size_t>(y) } );
        }
        auto pixels = ealain::raster::poly::rasterize(rotated);
        show(pixels);

        ealain::domain::PlanT<char> img(width,width,0);
        for(const auto& p : pixels) {
            img( static_cast<size_t>(ealain::raster::col(p)), static_cast<size_t>(ealain::raster::row(p)) ) = 1;
        }
        check_holes(img);
    }
}
