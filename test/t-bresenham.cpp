/**
 * Compute the sequence of pixels forming a line between two points in an image.
 */
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>

#include <Ealain/map/raster.h>

void print(const ealain::raster::Line& line, std::ostream& out)
{
    out << line.size() << " pixels" << std::endl;

    for(auto&& p : line) {
        out << p.first.first << "," << p.first.second << " ";
    } out << std::endl;

    int i,j;
    int pi=0, pj=0;
    std::vector<std::string> shades = {"░","▒","▓","█"};
    for(auto&& p : line) {
        std::tie(j,i) = p.first;
        if(i>pi) {
            for(int ii=pi; ii<i; ++ii) {
                out << std::endl;
            }
            pj=0;
        }
        if(j>pj) {
            for(int jj=pj; jj<j; ++jj) {
                out << " ";
            }
        }

        out << shades[std::round(p.second*10/4)];
        pi=i; pj=j+1;
    }
    out << std::endl;

}

void print(const ealain::raster::Line& seg, const unsigned int x_max, const unsigned int y_max, std::ostream& out)
{
    out << seg.size() << " pixels" << std::endl;

    for(auto&& p : seg) {
        out << ealain::raster::row(p) << "," << ealain::raster::col(p) << " ";
    } out << std::endl;

    // Convert the line to a full boolean matrix.
    std::vector<std::vector<bool>> mat;
    mat.reserve(x_max);
    for(unsigned int i=0; i < x_max; ++i) {
        mat.push_back( std::vector<bool>(y_max,false) );
    }

    for(auto&& p : seg) {
        unsigned int x = ealain::raster::row(p);
        unsigned int y = ealain::raster::col(p);
        mat.at(x).at(y) = true;
    }

    // Columns labels
    out << "x\\y";
    for(unsigned int j=0; j < mat[0].size(); ++j) {
        std::ostringstream m;
        m << j;
        out << std::setw(2) << m.str().back();
    }
    out << std::endl;

    // Print matrix with x on rows and y on columns.
    for(unsigned int i=0; i < mat.size(); ++i) {
        out << std::setw(2) << i << ": ";
        for(unsigned int j=0; j < mat[i].size(); ++j) {
            if(       i==ealain::raster::row(seg.front())
                  and j==ealain::raster::col(seg.front())) {
                out << "@@";
            } else if(i==ealain::raster::row(seg.back() )
                  and j==ealain::raster::col(seg.back() )) {
                out << "**";
            } else {
                out << (mat[i][j] ? "██" : "  ");
            }
        }
        out << std::endl;
    }
}



// Sort on row (y, second element), then on column (x, first element).
bool row_first(const ealain::raster::Pix& lhs, const ealain::raster::Pix& rhs)
{
    if(lhs.first.second != rhs.first.second) {
        return lhs.first.second < rhs.first.second;
    } else {
        return lhs.first.first < rhs.first.first;
    }
}


int main()
{

    float pi = 3.1415926535;
    float RAD_TO_DEG = 57.295779513082320876798154814105;
    int cx = 11;
    int cy = 11;
    int r = 10;
    for(float a=0; a <= 2*pi; a+=pi/12) {
        std::clog << "-------------------------------" << std::endl;
        int px = cx + std::floor(r*std::cos(a));
        int py = cy + std::floor(r*std::sin(a));
        std::clog << a*RAD_TO_DEG << "°: " << cx << "," << cy << " -> "  << px << "," << py << std::endl;
        ealain::raster::Line line = ealain::raster::line::bresenham(cx,cy, px,py);
        print(line, cx+r+1, cy+r+1, std::clog);
    }
}

