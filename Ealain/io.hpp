#include <limits>
#include <cmath>
#include <algorithm>

#include "utils.h"

namespace ealain {

namespace sav {

namespace tab {

template<typename T>
void csv(const domain::Domain<2,T>& domain, std::ostream& out,
        std::size_t row, std::size_t col,
        std::size_t width, std::size_t height)
{
    csv(domain.data(), out, row, col, width, height);
}

template<typename T>
void csv(const domain::Domain<3,T>& domain, std::ostream& out,
        std::size_t row, std::size_t col, std::size_t frame,
        std::size_t width, std::size_t height, std::size_t depth)
{
    csv(domain.data(), out, row, col, frame, width, height, depth);
}

template<class T>
void csv(const std::vector<std::vector<T>>& data, std::ostream& out, std::size_t row, std::size_t col, std::size_t width, std::size_t height)
{
    if(width==0) {
        width = data.size();
    }
    if(height==0) {
        assert(data.size()>0);
        height = data.at(0).size();
    }

    out << "i,j,value" << std::endl;
    for(auto i=row; i < row+width; ++i) {
        for(auto j=col; j < col+height; ++j) {
            T val = data[i][j];
            out << i << "," << j << "," << val << std::endl;
        } //col
    } // row
}

template<class T>
void csv(const std::vector<std::vector<std::vector<T>>>& data, std::ostream& out,
        std::size_t row, std::size_t col, std::size_t frame,
        std::size_t width, std::size_t height, std::size_t depth)
{
    if(width==0) {
        width = data.size();
    }
    if(height==0) {
        assert(data.size()>0);
        height = data.at(0).size();
    }
    if(depth==0) {
        assert(data.at(0).size()>0);
        depth = data.at(0).at(0).size();
    }

    out << "i,j,k,value" << std::endl;
    for(auto i=row; i < row+width; ++i) {
        for(auto j=col; j < col+height; ++j) {
            for(auto k=frame; k < frame+depth; ++k) {
                T val = data[i][j][k];
                out << i << "," << j << "," << k << "," << val << std::endl;
            } //col
        } // row
    } // frame
}

} // tab


namespace img {

template<std::size_t DIM, typename T>
void ascii(const domain::Domain<DIM,T>& domain, std::ostream& out,
        std::size_t row, std::size_t col, std::size_t width, std::size_t height)
{
    ascii(domain.data(), out, row, col, width, height);
}

template<std::size_t DIM, typename T>
void text(const domain::Domain<DIM,T>& domain, std::ostream& out, int precision,
        std::size_t row, std::size_t col, std::size_t width, std::size_t height)
{
    text(domain.data(), out, precision, row, col, width, height);
}

template<std::size_t DIM, typename T>
void raw(const domain::Domain<DIM,T>& domain, std::string fname,
        std::size_t row, std::size_t col, std::size_t width, std::size_t height)
{
    // DBGLOG("raw fname " << DIM << "D");
    raw(domain.data(), fname, row, col, width, height);
}

template<std::size_t DIM, typename T>
void raw(const domain::Domain<DIM,T>& domain, std::fstream& out,
        std::size_t row, std::size_t col, std::size_t width, std::size_t height)
{
    raw(domain.data(), out, row, col, width, height);
}


template<class T>
void ascii(const T& data, std::ostream& out, std::size_t row, std::size_t col, std::size_t width, std::size_t height)
{
    float amin = std::numeric_limits<float>::max();
    float amax = std::numeric_limits<float>::min();

    if(width==0) {
        width = data.size();
    }
    if(height==0) {
        assert(data.size()>0);
        height = data.at(0).size();
    }

    // Real min/max
    for(std::size_t i=row; i < row+width; ++i) {
        for(std::size_t j=col; j < col+height; ++j) {
            float alt = data[i][j];
            if( -11074 <= alt and alt <= 8849) {
                amin = alt < amin ? alt : amin;
                amax = alt > amax ? alt : amax;
            }
        }
    }

    /* Unicode block elements:
       U+2591 	░ 	Light shade
       U+2592 	▒ 	Medium shade
       U+2593 	▓ 	Dark shade
       U+2588 	█ 	Full block
       */
    // Two chars wide to stay close to a square ratio.
    std::vector<std::string> symbols = {"‧ ","⁚⁚","⁙⁙","░░","▒▒","▓▓","██"};
    std::string nis = "╳╳";
    float span = (amax-amin)/static_cast<double>(symbols.size());
    out << "Scale: " << amin;

    for(unsigned int i=0; i < symbols.size(); ++i) {
        out << symbols[i] << " " << amin+i*span << " ";
    }
    out << " (" << nis << " = NaN | inf)" << std::endl;

    for(auto i=row; i < row+width; ++i) {
        for(auto j=col; j < col+height; ++j) {
            float alt = data[i][j];
            if( not is_real(alt) ) {
                out << nis;
            } else {
                std::size_t idx = std::floor((alt-amin)/(amax-amin)*(symbols.size()-1));
                if(idx >= symbols.size() ) { idx = symbols.size()-1; }
                // assert(idx < symbols.size());
                if( amin <= alt and alt <= amax ) {
                    out << symbols[idx];
                } else {
                    out << nis;
                }
            } // if nan or inf
        } // col
        out << std::endl;
    } // row
}

template<class T>
void text(const T& data, std::ostream& out, int precision,
        std::size_t row, std::size_t col, std::size_t width, std::size_t height)
{

    if(width==0) {
        width = data.size();
    }
    if(height==0) {
        assert(data.size()>0);
        height = data.at(0).size();
    }

    std::string nis = "╳";

    for(auto i=row; i < row+width; ++i) {
        for(auto j=col; j < col+height; ++j) {
            out << std::setw(precision+2) << std::setprecision(precision) << data[i][j] << " ";
        } // col
        out << std::endl;
    } // row
}

template<class T,
    typename /*= std::enable_if_t<std::is_arithmetic<T>::value>*/>
void raw(const std::vector<std::vector<T>>& data, std::string fname,
        std::size_t row, std::size_t col, std::size_t width, std::size_t height)
{
    auto fd = std::fstream(fname.c_str(), std::ios::out | std::ios::binary);
    raw(data, fd, row, col, width, height);
    fd.close();
}


} // img

} // sav

} // ealain
