#include <cmath>

namespace ealain {

    template<class T>
    bool is_real(const T number) {
        if( std::isnan(number) or std::isinf(number) ) {
            return false;
        } else {
            return true;
        }
    }

    template<class T>
    bool is_proba(const T p) {
        if( not is_real(p) or p < 0 or 1 < p ) {
            return false;
        } else {
            return true;
        }
    }

    template<class T>
    bool is_angle(const T a) {
        if( not is_real(a) or a < 0 or a > 2*M_PI) {
            return false;
        } else {
            return true;
        }
    }

namespace size {
    template<class T>
    unsigned int rows(const T& array) {
        return array.size();
    }

    template<class T>
    unsigned int cols(const T& array) {
        return array[0].size();
    }

    template<class T>
    unsigned int items(const T& array) {
        return rows(array) * cols(array);
    }
} // size

template <typename From, typename To>
To static_caster<From,To>::operator()(From x) {return static_cast<To>(x);}


} // ealain

