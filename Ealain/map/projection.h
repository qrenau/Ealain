#ifndef __EALAIN_PROJECTION_H__
#define __EALAIN_PROJECTION_H__

#include <cstddef>
#include <vector>
#include <memory>

#include <cassert>

namespace ealain {
namespace proj {

     template<class T>
     struct Range
     {
            const T _mini;
            const T _maxi;
            const T min() const;
            const T max() const;
            Range(const T min, const T max);
     };

    //Interface for a one-dimensional Projection from/to numerical space to/from discrete space.
    template<class IRL=double, class IDX=size_t>
    class Proj
    {
       protected:
           const Range<IRL> _range_irl;
           const Range<IDX> _range_idx;

       public:
           using RealLifeType = IRL;
           using IndexType = IDX;

           /** Constructor
            *
            * range_irl [min,max] bounds for the numerical space.
            * range_idx [min,max] boumds for the discrete space.
            */
           Proj(Range<IRL> range_irl, Range<IDX> range_idx);

           //Map from discrete space to numerical space.
           virtual IRL operator()(IDX x) const = 0;

           //Map from numerical space to discrete space.
           virtual IDX operator()(IRL x) const = 0;

           //Accessor to the real space range.
           const Range<IRL>& range_irl() const;

           //Accessor to the indexed space range.
           const Range<IDX>& range_idx() const;

           virtual ~Proj() {}
    };

    /** Identity mapping from/to numerical space to/from discrete space.
     *
     * e.g. given range_idx := [a,b] ⊂ ℤ, apply a mapping x ∈ [a,b] ⊂ ℝ → y ∈ [a,b] ⊂ ℤ.
     */
    template<class IRL=double, class IDX=size_t>
    class Identity : public Proj<IRL,IDX>
    {
        public:
           Identity();

           //Map from discrete space to numerical space.
           virtual IRL operator()(IDX x) const;

           //Map from numerical space to discrete space.
           virtual IDX operator()(IRL x) const;

    };

    /** Linear mapping from/to numerical space to/from discrete space.
     *
     * e.g. given range_irl := [a,b] ⊂ ℝ and range_idx := [c,d] ⊂ ℤ, apply a mapping x ∈ [a,b] → y ∈ [c,d].
     */
    template<class IRL=double, class IDX=size_t>
    class Linear : public Proj<IRL,IDX>
    {
        public:
           Linear(Range<IRL> range_irl, Range<IDX> range_idx);

           //Map from discrete space to numerical space.
           virtual IRL operator()(IDX x) const;

           //Map from numerical space to discrete space.
           virtual IDX operator()(IRL x) const;
    };

    /** Interface for n-dimensional Projections.
     *
     * Holds a set of (pointers to) one-dimensional Proj objects.
     */
    template<class IRL=double, class IDX=size_t>
    class Projection
    {
       protected:
           std::vector<std::shared_ptr<Proj<IRL,IDX>>> _projs;

           /** Apply the ith-Projection to each ith-element in vx.
            *
            * Use the templates I/O to decide which Projection to apply.
            */
           template<class I, class O>
           std::vector<O> proj( std::vector<I> vx) const;

           /** Apply the ith-Projection to each vector in a set of vectors.
            *
            * Use the templates I/O to decide which Projection to apply.
            */
           template<class I, class O>
           std::vector<std::vector<O>> proj( std::vector<std::vector<I>> d) const;

       public:
           using RealLifeType = IRL;
           using IndexType = IDX;

           /** Empty constructor
            *
            * Useful when declaring first, then instanciating with std::tie.
            */
           Projection();

           /** Constructor taking a set of pointers to one-dimensional Proj instances.
            *
            * Example:
            * std::shared_ptr<Proj<double,size_t>> lm1
            *     = std::make_shared<Linear<double,size_t>>({0,1},{0,100});
            * std::shared_ptr<Proj<double,size_t>> lm2
            *     = std::make_shared<Linear<double,size_t>>({0,1},{0,1000});
            * auto m = Projection<double,size_t>({lm1,lm2});
            */
           Projection(std::vector<std::shared_ptr<Proj<IRL,IDX>>> projs);

           //Add a (shared pointer to a) Proj instance.
           void add(std::shared_ptr<Proj<IRL,IDX>> proj);

           //Map a vector from discrete space to numerical space.
           virtual std::vector<IRL> operator()(std::vector<IDX> x) const;

           //Map a vector from numerical space to discrete space.
           virtual std::vector<IDX> operator()(std::vector<IRL> x) const;

           //Map a set of vectors from discrete space to numerical space.
           virtual std::vector<std::vector<IRL>> operator()(std::vector<std::vector<IDX>> x) const;

           //Map a set of vectors from numerical space to discrete space.
           virtual std::vector<std::vector<IDX>> operator()(std::vector<std::vector<IRL>> x) const;

           //Accessor to the Proj used for dimension d.
           const Proj<IRL,IDX>& operator[](size_t d) const;

           //Accessor to the set of {min,max} pairs for the real space, indexed by dimension.
           std::vector<std::pair<IRL,IRL>> ranges_irl() const;

           //Accessor to the set of {min,max} pairs for the indexed space, indexed by dimension.
           std::vector<std::pair<IDX,IDX>> ranges_idx() const;

           //Returns the number of dimensions.
           size_t size() const;
    };

    /** Helper function that create a n-dimensional Projection with the same ranges on all dimensions, without having to instanciate ranges objects.
     *
     * Example:
     * auto m = Proj::make<Linear<>>(5, {0.1,0.5}, {0,100});
     */
    template<class M>
    Projection<typename M::RealLifeType,typename M::IndexType>
    make(
            std::pair<typename M::RealLifeType,typename M::RealLifeType> ranges_irl,
            std::pair<typename M::IndexType   ,typename M::IndexType>    ranges_idx,
            size_t n = 1
        );

    /** Helper function that create a n-dimensional Linear map with the same range on all dimensions, without having to instanciate ranges objects.
     *
     * Example:
     * auto m = Proj::make_linear<>(5, {0.1,0.5}, {0,100});
     */
    template<class IRL = double, class IDX = size_t>
    Projection<IRL,IDX> make_linear(std::pair<IRL,IRL> ranges_irl, std::pair<IDX,IDX> ranges_idx, size_t n=1);

    /** Helper function that create a n-dimensional normalization map with [0,1] range on all dimensions, without having to instanciate ranges objects.
     *
     * Example:
     * auto m = Proj::make_linear<>(5, {0,100});
     */
    template<class IRL = double, class IDX = size_t>
    Projection<IRL,IDX> make_normalizer(std::pair<IDX,IDX> ranges_idx, size_t n=1);

    /** Helper function that create a n-dimensional normalization map from/to a Domain of dimension d to/from [0,1]^d.
     */
    template<class D>
    Projection<double,size_t> make_normalizer(const D& domain);

    /** Helper function that create a n-dimensional normalization map from a list of max indices [m_0,…,m_i]^n
     * creates a map from/to [0,m_i]^n to/from [0-1]^n, without having to instanciate ranges objects.
     */
    template<class IRL = double, class IDX = size_t>
    Projection<IRL,IDX> make_normalizer_bounded(std::vector<IDX> max_bounds);

    /** Add a new dimension to an existant Projection
     */
    template<class IRL, class IDX>
    Projection<IRL,IDX> lift(Projection<IRL,IDX>  proj, Range<IRL> range_irl, Range<IDX> range_idx);

    /** Remove last dimension of an existant Projection
     */
    template<class IRL, class IDX>
    Projection<IRL,IDX> to_subspace(Projection<IRL,IDX>  proj);

} //proj

} //ealain

#include "projection.hpp"

#endif //__EALAIN_PROJECTION_H__
