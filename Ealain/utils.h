#ifndef __EALAIN_UTILS_H__
#define __EALAIN_UTILS_H__

#include <iterator>
#include <string>
#include <vector>
#include <cstddef>
#include <sys/stat.h>
#include <cassert>

#define ALL(p) std::begin(p), std::end(p)
#define IN(p) std::begin(p), std::end(p)

namespace ealain {

    // Return true if the given number is neither NaN nor INF.
    template<class T>
    bool is_real(const T number);

    // Return true if the given number is between 0 and 1.
    template<class T>
    bool is_proba(const T p);

    // Return true if the given number is in radians range.
    template<class T>
    bool is_angle(const T a);

    namespace size {
        // Return the size of the first depth of a container.
        template<class T>
        unsigned int rows(const T& array);

        // Return the size of the second depth of a container of containers.
        template<class T>
        unsigned int cols(const T& array);

        // Return (rows * cols) of the given container.
        template<class T>
        unsigned int items(const T& array);
    } // size

    // Add space separators to a number representation.
    std::string format(unsigned int number);

    // Operator that cast a type to another, useful in an std::transform.
    template <typename From, typename To>
    struct static_caster {
        // Functor interface.
        To operator()(From x);
    };


} // ealain

#include "utils.hpp"

#endif // __EALAIN_UTILS_H__
