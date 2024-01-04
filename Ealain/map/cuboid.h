#ifndef __EALAIN_CUBOID_H__
#define __EALAIN_CUBOID_H__

#include "domain.h"

namespace ealain {

    namespace domain {

        // Forward declaration.
        template<typename T=double>
        class Cuboid_iterator;

        // 3D cuboid domain.
        template<typename T=double>
        class CuboidT : public Domain<3,T>
        {
            public:
                using iterator = Cuboid_iterator<T>;
                using data_type = typename Domain<3,T>::data_type;

                CuboidT( data_type data );

                CuboidT(std::size_t lines, std::size_t columns, std::size_t frames, T fill = 0);

                CuboidT(std::array<std::size_t,3> sizes, T fill = 0);

                CuboidT(proj::Projection<T,size_t> p, T fill = 0);

                const T& at(std::vector<std::size_t> coords) const;
                T& at(std::vector<std::size_t> coords);

                virtual std::size_t size() const;

                virtual std::array<std::size_t,3> sizes() const;

                iterator begin();

                iterator end();

        };

        using Cuboid = CuboidT<double>;

        // Extract a slice of the 2D+0 domain given an angle index.
        template<typename T=double>
        std::vector<std::vector<double>> slice_at_angle(const CuboidT<T>& domain, unsigned int angle_idx);

        // An iterator over a whole Cuboid.
        template<typename T/*=double*/>
        class Cuboid_iterator : public std::iterator<std::bidirectional_iterator_tag, double>
        {
        protected:
            CuboidT<T>* _domain;
            std::size_t _i_cut;
            std::size_t _j_row;
            std::size_t _k_col;

            Cuboid_iterator(
                    CuboidT<T>* cuboid = nullptr,
                    std::vector<std::size_t> coords = {0,0,0}
                );

        public:

            Cuboid_iterator<T>() = default;

            bool operator==(const Cuboid_iterator<T>& other) const;

            bool operator!=(const Cuboid_iterator<T>& other) const;

            Cuboid_iterator<T> operator++(int);

            Cuboid_iterator<T> operator--(int);

            static Cuboid_iterator<T> begin(CuboidT<T>& plan);

            static Cuboid_iterator<T> end(CuboidT<T>& plan);

            double& operator*();

            const double& operator*() const;

            Cuboid_iterator<T>& operator++();

            Cuboid_iterator<T>& operator--();

            std::size_t operator()(std::size_t dimension) const;

        };

    } // domain
} // ealain

#include "cuboid.hpp"

#endif // __EALAIN_CUBOID_H_
