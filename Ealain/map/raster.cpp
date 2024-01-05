#include <set>
#include <map>

#include "raster.h"
#include "../utils.h"


namespace ealain {
namespace raster {

    Point point(Pix p){return p.first;}
    float row(Point p){return p.first;}
    float row(Pix p){return row(point(p));}
    float col(Point p){return p.second;}
    float col(Pix p){return col(point(p));}
    float shade(Pix p){return p.second;}

    Pix pixel(int row, int col, float value = 0)
    {
        return std::make_pair(std::make_pair(row,col),value);
    }

namespace line {
    float  fpart(float i) {return i - std::floor(i);}
    float rfpart(float i) {return 1 - fpart(i);}

    bool add(Line& line, std::pair<int, int> p, float thickness, float thickness_threshold)
    {
        if(thickness >= thickness_threshold) {
            line.push_back(std::make_pair(p,thickness));
            return true;
        } else {
            return false;
        }
    }

    Line bresenham( float i1, float j1, float i2, float j2)
    {
        Line line;

        float di = i2 - i1;
        float dj = j2 - j1;
        int is = di > 0 ? 1 : -1;
        int js = dj > 0 ? 1 : -1;
        di = std::abs(di);
        dj = std::abs(dj);

        float ii, ij, ji, jj;
        if(di > dj) {
            ii = is;
            ij = 0;
            ji = 0;
            jj = js;
        } else {
            std::swap(di,dj);
            ii = 0;
            ij = js;
            ji = is;
            jj = 0;
        }

        float err = 2 * dj - di;
        int j = 0;

        for( int i = 0; i < di+1; ++i ) {
            add(line, std::make_pair(i1+i*ii+j*ji, j1+i*ij+j*jj));
            if( err >= 0 ) {
                j += 1;
                err -= 2*di;
            }
            err += 2 * dj;
        }

        return line;
    }

} // line


Line pixels_line( float i1, float j1, float i2, float j2)
{
    return line::bresenham(i1, j1, i2, j2);
}

namespace poly {

    bool RowCmp::operator()(const Pix& lhs, const Pix& rhs) const
    {
        return raster::col(lhs) < raster::col(rhs);
    }

    std::vector<Pix> rasterize(
            const std::vector<std::vector<size_t>>& polygon)
    {
        std::vector<Pix> pixels;

        const size_t drow = 0;
        const size_t dcol = 1;

        assert(polygon.size() >= 3);
        size_t min_row = std::numeric_limits<size_t>::max();
        size_t max_row = std::numeric_limits<size_t>::min();
        size_t min_col = std::numeric_limits<size_t>::max();
        size_t max_col = std::numeric_limits<size_t>::min();
        for(const auto& p : polygon) {
            assert(p.size() == 2);
            min_row = p[drow] < min_row ? p[drow] : min_row;
            max_row = p[drow] > max_row ? p[drow] : max_row;
            min_col = p[dcol] < min_col ? p[dcol] : min_col;
            max_col = p[dcol] > max_col ? p[dcol] : max_col;
        }

        // Loop through rows of pixels.
        for(double current_row = min_row; current_row < max_row; current_row++) {

            // Get all intersection nodes and keep their columns.
            std::vector<size_t> nodes; nodes.reserve(polygon.size());

            size_t j = polygon.size()-1;
            for(size_t i = 0; i < polygon.size(); ++i) {

                double p_row_i = polygon[i][drow];
                double p_row_j = polygon[j][drow];

                if(   p_row_i < current_row and p_row_j >= current_row
                   or p_row_j < current_row and p_row_i >= current_row ) {
                    double p_col_i = polygon[i][dcol];
                    double p_col_j = polygon[j][dcol];
                    nodes.push_back( static_cast<size_t>( std::round(
                        p_col_i + (current_row-p_row_i)/(p_row_j-p_row_i) * (p_col_j-p_col_i)
                    )));
                }
                j = i;
            } // for i

            // Sort nodes (along the column axis).
            std::sort(ALL(nodes));

            // Fill between pairs of pixels
            assert(nodes.size() % 2 == 0);
            for(size_t i=0; i < nodes.size(); i+=2) {
                if(nodes[i  ] >= max_col) {break;}
                if(nodes[i+1] >  min_col) {
                    if(nodes[i  ] < min_col) { nodes[i  ] = min_col; }
                    if(nodes[i+1] > max_col) { nodes[i+1] = max_col; }
                    for(size_t pcol = nodes[i]; pcol < nodes[i+1]; pcol++) {
                        pixels.push_back( raster::pixel(current_row,pcol,1) );
                    } // for pcol
                }
            } // for i

        } // for current_row

        return pixels;
    }

} // poly

} // raster
} // ealain
