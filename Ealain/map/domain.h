#ifndef __EALAIN_DOMAIN_H__
#define __EALAIN_DOMAIN_H__

#include <vector>
#include <array>

#include <cassert>
#include "projection.h"

namespace ealain {

    template<class D>
    typename D::iterator begin(D& domain);

    template<class D>
    typename D::iterator end(D& domain);

    //Discretized domains of various dimensions.
    namespace domain {

        //Convert a (additive) cost to a (multiplicative) probability.
        double cost_to_proba(const double cost);

        //Convert a (multiplicative) probability to a (additive) cost.
        double proba_to_cost(const double proba);


        //Base class for domain interfaces.
        template<std::size_t DIM, typename T=double>
        class DomainBase
        {
            public:
                //The targeted number of dimension of the domain.
                static const std::size_t dimension = DIM;

                /** Accessor to an item.
                 *
                 * coords coordinate vector of the item, can be initialized with an explicit list.
                 * returns a reference to the item.
                 */
                virtual const
                T& at(std::vector<std::size_t> coords) const = 0;

                virtual
                T& at(std::vector<std::size_t> coords) = 0;

                //Alias to DomainBase::at.
                T& operator[](std::vector<std::size_t> coords)
                {return this->at(coords);}

                //Alias to DomainBase::at (const version).
                const
                T& operator[](std::vector<std::size_t> coords) const
                {return this->at(coords);}

                /** Accessor to an item
                 *
                 * params DIM number of indices arguments.
                 * returns a reference to the item
                 */
                template<class... Ts>
                T& operator()(Ts... params)
                {
                    static_assert(sizeof...(params) == DIM, "Wrong number of dimensions");
                    const std::vector<std::size_t> coords = {params...};
                    return this->at(coords);
                }

                //Number of items in the whole domain.
                virtual std::size_t size() const = 0;

                /** Size in number of items of each axis.
                 *
                 * returns an array of DIM sizes.
                 */
                virtual std::array<std::size_t, DIM> sizes() const = 0;

                //Size of the given axis
                std::size_t box_size(std::size_t dimension) const
                {
                    return this->sizes().at(dimension);
                }

        };

        //Dummy class indicating no data type.
        struct None { };

        /** Interface for domains
         *
         * Defines the embedded data structure that actually store data.
         * If you want to create your own multi-dimensional domain, you should inherit from a specialization of this
         * class.
         */
        template<std::size_t DIM, typename T=double>
        class Domain : public DomainBase<DIM,T>
        {
            public:
                //Data type of the internal container.
                using data_type = None;
            protected:
                data_type _data;
            public:
                Domain<DIM,T>(data_type data) : _data(data) {}

                //Accessor to the internal container.
                data_type& data() {return _data;}

                //Accessor to the internal container (const version).
                const data_type& data() const {return _data;}
        };

        //Specialization of Domain for 2D.
        template<typename T>
        class Domain<2,T> : public DomainBase<2,T>
        {
            public:
                using data_type = std::vector<std::vector<T>>;

            protected:
                data_type _data;

            public:
                Domain<2,T>(data_type data) : _data(data) {}

                Domain<2,T>(std::size_t lines, std::size_t columns, T fill = 0) :
                    _data(lines, std::vector<T>(columns, fill))
                {}

                Domain<2,T>(proj::Projection<T,size_t> m, T fill = 0) :
                    //Plus one, because the semantic of underlying ranges
                    //is to be closed intervals.
                    _data(m[0].range_idx().max()+1,
                            std::vector<T>(m[1].range_idx().max()+1,
                                fill))
                {
                    assert(_data.size() == m[0].range_idx().max()+1);
                    assert(_data.size() > 0);
                    assert(_data[0].size() == m[1].range_idx().max()+1);
                }

                data_type& data() {return _data;}
                const data_type& data() const {return _data;}
        };

        //Specialization of Domain for 3D.
        template<typename T>
        class Domain<3,T> : public DomainBase<3,T>
        {
            public:
                using data_type = std::vector<std::vector<std::vector<T>>>;
            protected:
                data_type _data;
            public:
                Domain<3,T>(data_type data) : _data(data) {}

                Domain<3,T>(std::size_t lines, std::size_t columns, std::size_t frames, T fill = 0) :
                    _data(lines, std::vector<std::vector<T>>(columns, std::vector<T>(frames, fill)))
                {}

                Domain<3,T>(proj::Projection<T,size_t> m, T fill = 0) :
                    //Plus one, because the semantic of underlying ranges
                    //is to be closed intervals.
                    _data(m[0].range_idx().max()+1,
                            std::vector<std::vector<T>>(m[1].range_idx().max()+1,
                                std::vector<T>(m[2].range_idx().max()+1,
                                    fill)))
                {}
                data_type& data() {return _data;}
                const data_type& data() const {return _data;}
        };

        //Call the atomic cost_to_proba on a whole domain.
        template<class D>
        D cost_to_proba(const D& cost);

        //Call the atomic proba_to_cost on a whole domain.
        template<class D>
        D proba_to_cost(const D& proba);

    } // domain

} // EALAIN

#include "domain.hpp"

#endif //__EALAIN_DOMAIN_H__
