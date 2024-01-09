#ifndef __EALAIN_IO_H__
#define __EALAIN_IO_H__

#include <iostream>
#include <functional>
#include <fstream>
#include <iomanip>

#include "map/domain.h"

namespace ealain {

    // Input/Output
    namespace sav {

        // Tabular formats
        namespace tab {

            template<typename T>
            void csv(const domain::Domain<2,T>& domain, std::ostream& out = std::cout,
                    std::size_t row=0, std::size_t col=0,
                    std::size_t width=0, std::size_t height=0);

            template<typename T>
            void csv(const domain::Domain<3,T>& domain, std::ostream& out = std::cout,
                    std::size_t row=0, std::size_t col=0, std::size_t frame=0,
                    std::size_t width=0, std::size_t height=0, std::size_t depth=0);

            // Write 2D block of data as CSV.
            template<class T>
            void csv(const std::vector<std::vector<T>>& data, std::ostream& out = std::cout,
                    std::size_t row=0, std::size_t col=0,
                    std::size_t width=0, std::size_t height=0);

            // Write 3D block of data as CSV.
            template<class T>
            void csv(const std::vector<std::vector<std::vector<T>>>& data, std::ostream& out = std::cout,
                    std::size_t row=0, std::size_t col=0, std::size_t frame=0,
                    std::size_t width=0, std::size_t height=0, std::size_t depth=0);
        } // tab


        // Pixmap formats
        namespace img {

            // Display the image in ASCII-art
            template<std::size_t DIM, typename T>
            void ascii(const domain::Domain<DIM,T>& domain, std::ostream& out = std::cout,
                    std::size_t row=0, std::size_t col=0, std::size_t width=0, std::size_t height=0);

            // Display the image's pixels numeric values.
            template<std::size_t DIM, typename T>
            void text(const domain::Domain<DIM,T>& domain, std::ostream& out = std::cout, int precision=1,
                    std::size_t row=0, std::size_t col=0, std::size_t width=0, std::size_t height=0);

            template<std::size_t DIM, typename T>
            void raw(const domain::Domain<DIM,T>& domain, std::string fname,
                    std::size_t row=0, std::size_t col=0, std::size_t width=0, std::size_t height=0);

            template<std::size_t DIM, typename T>
            void raw(const domain::Domain<DIM,T>& domain, std::fstream& out,
                    std::size_t row=0, std::size_t col=0, std::size_t width=0, std::size_t height=0);


            template<class T>
            void ascii(const T& data, std::ostream& out = std::cout,
                    std::size_t row=0, std::size_t col=0, std::size_t width=0, std::size_t height=0);

            template<class T>
            void text(const T& data, std::ostream& out = std::cout, int precision=1,
                    std::size_t row=0, std::size_t col=0, std::size_t width=0, std::size_t height=0);

            template<class T,
                typename = std::enable_if_t<std::is_arithmetic<T>::value>>
            void raw(const std::vector<std::vector<T>>& data, std::string fname,
                    std::size_t row=0, std::size_t col=0, std::size_t width=0, std::size_t height=0);


        } // img
    } // sav

} // ealain

#include "io.hpp"

#endif // __EALAIN_IO_H__
