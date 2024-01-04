#ifndef __EALAIN_PLAN_H__
#define __EALAIN_PLAN_H__

#include "domain.h"

namespace ealain {
    namespace domain {

        // Forward declaration.
        template<typename T=double>
        class Plan_iterator;

        // 2D rectangle domain.
        template<typename T=double>
        class PlanT : public Domain<2,T>
        {
            public:
                using iterator = Plan_iterator<T>;
                using data_type = typename Domain<2,T>::data_type;

                PlanT( data_type data );

                PlanT(std::size_t lines, std::size_t columns, T fill = 0);

                PlanT(std::array<std::size_t,2> sizes, T fill = 0);

                PlanT(proj::Projection<T,size_t> p, T fill = 0);

                const T& at(std::vector<std::size_t> coords) const;
                T& at(std::vector<std::size_t> coords);

                std::size_t size() const;

                std::array<std::size_t,2> sizes() const;

                iterator begin();

                iterator end();

        };

        using Plan = PlanT<double>;

        // An iterator over a whole Plan.
        template<typename T/*=double*/>
        class Plan_iterator : public std::iterator<std::bidirectional_iterator_tag, T>
        {
        protected:
            PlanT<T>* _domain;
            std::size_t _i_row;
            std::size_t _j_col;

            Plan_iterator(
                    PlanT<T>* plan = nullptr,
                    std::vector<std::size_t> coords = {0,0}
                );

        public:

            Plan_iterator<T>() = default;

            bool operator==(const Plan_iterator<T>& other) const;

            bool operator!=(const Plan_iterator<T>& other) const;

            Plan_iterator<T> operator++(int);

            Plan_iterator<T> operator--(int);

            static Plan_iterator<T> begin(PlanT<T>& plan);

            static Plan_iterator<T> end(PlanT<T>& plan);

            T& operator*();

            const T& operator*() const;

            Plan_iterator<T>& operator++();

            Plan_iterator<T>& operator--();

            std::size_t operator()(std::size_t dimension) const;

        };

    } // domain
} // ealain

#include "plan.hpp"

#endif // __EALAIN_PLAN_H_
