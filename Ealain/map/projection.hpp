#include <memory>
#include <cmath>


namespace ealain {
namespace proj {

//Identity

template<class IRL, class IDX>
Identity<IRL,IDX>::Identity() : Proj<IRL,IDX>({0,0},{0,0})
{ }

template<class IRL, class IDX>
IRL Identity<IRL,IDX>::operator()(IDX x) const
{
    return static_cast<IRL>(x);
}

template<class IRL, class IDX>
IDX Identity<IRL,IDX>::operator()(IRL x) const
{
#ifndef NDEBUG
    IRL int_part;
    IRL frac_part = std::modf(x, &int_part);
    assert(frac_part == 0);
#endif
    return static_cast<IDX>(x);
}


//Proj

template<class IRL, class IDX>
Proj<IRL,IDX>::Proj(Range<IRL> range_irl, Range<IDX> range_idx) : _range_irl(range_irl), _range_idx(range_idx)
{ }

template<class IRL, class IDX>
const Range<IRL>& Proj<IRL,IDX>::range_irl() const
{
    return _range_irl;
}

template<class IRL, class IDX>
const Range<IDX>& Proj<IRL,IDX>::range_idx() const
{
    return _range_idx;
}

//Linear

template<class IRL, class IDX>
Linear<IRL,IDX>::Linear(Range<IRL> range_irl, Range<IDX> range_idx) : Proj<IRL,IDX>(range_irl, range_idx)
{ }

template<class IRL, class IDX>
IRL Linear<IRL,IDX>::operator()(IDX x) const
{
    double normalized = (static_cast<IRL>(x)
          - static_cast<IRL>(this->_range_idx.min()))
         / (static_cast<IRL>(this->_range_idx.max())
          - static_cast<IRL>(this->_range_idx.min()));
    IRL result = static_cast<IRL>(this->_range_irl.min()
            + (normalized
                * (   this->_range_irl.max()
                    - this->_range_irl.min())));
    return result;
}

template<class IRL, class IDX>
IDX Linear<IRL,IDX>::operator()(IRL x) const
{
    double normalized = ((x
          - this->_range_irl.min())
         / (this->_range_irl.max()
          - this->_range_irl.min())
         );
    IDX result = static_cast<IDX>( /*std::round(*/
            static_cast<IRL>(this->_range_idx.min())
            + (normalized
                * (   static_cast<IRL>(this->_range_idx.max())
                    - static_cast<IRL>(this->_range_idx.min())))/*)*/);
    return result;
}



//Projection

template<class IRL, class IDX>
Projection<IRL,IDX>::Projection()
{}


template<class IRL, class IDX>
Projection<IRL,IDX>::Projection(std::vector<std::shared_ptr<Proj<IRL,IDX>>> norms )
    : _projs(norms)
{
    assert(_projs.size() > 0);
}

template<class IRL, class IDX>
void Projection<IRL,IDX>::add(std::shared_ptr<Proj<IRL,IDX>> proj)
{
    _projs.push_back(proj);
}


template<class IRL, class IDX>
template<class I, class O>
std::vector<O> Projection<IRL,IDX>::proj(std::vector<I> vx) const
{
    std::vector<O> result;
    result.reserve(vx.size());
    assert(vx.size() > 0);
    assert(vx.size() == _projs.size());
    for(unsigned int i=0; i < vx.size(); ++i) {
        result.push_back( (*(_projs[i]))(vx[i]) );
    }
    return result;
}

template<class IRL, class IDX>
std::vector<IRL> Projection<IRL,IDX>::operator()(std::vector<IDX> vx) const
{
    return this->proj<IDX,IRL>(vx);
}

template<class IRL, class IDX>
std::vector<IDX> Projection<IRL,IDX>::operator()(std::vector<IRL> vx) const
{
    return this->proj<IRL,IDX>(vx);
}

template<class IRL, class IDX>
template<class I, class O>
std::vector<std::vector<O>> Projection<IRL,IDX>::proj( std::vector<std::vector<I>> d) const
{
    std::vector<std::vector<O>> vy;
    vy.reserve(d.size());
    for(auto& vx : d) {
        vy.push_back( (*this)(vx) );
    }
    return vy;
}

template<class IRL, class IDX>
std::vector<std::vector<IDX>> Projection<IRL,IDX>::operator()(std::vector<std::vector<IRL>> d) const
{
    return this->proj<IRL,IDX>(d);
}

template<class IRL, class IDX>
std::vector<std::vector<IRL>> Projection<IRL,IDX>::operator()(std::vector<std::vector<IDX>> d) const
{
    return this->proj<IDX,IRL>(d);
}

template<class IRL, class IDX>
const Proj<IRL,IDX>& Projection<IRL,IDX>::operator[](size_t d) const
{
    return *(_projs[d]);
}

template<class IRL, class IDX>
std::vector<std::pair<IRL,IRL>> Projection<IRL,IDX>::ranges_irl() const
{
    std::vector<std::pair<double,double>> ranges;
    for(const auto& proj : _projs) {
        ranges.push_back(
            proj->range_irl().range()
        );
    }
    return ranges;
}

template<class IRL, class IDX>
std::vector<std::pair<IDX,IDX>> Projection<IRL,IDX>::ranges_idx() const
{
    std::vector<std::pair<IDX,IDX>> ranges;
    for(const auto& proj : _projs) {
        ranges.push_back(
            proj->range_idx().range()
        );
    }
    return ranges;
}

template<class IRL, class IDX>
size_t Projection<IRL,IDX>::size() const
{
    return _projs.size();
}

//Free functions

template<class M>
Projection<typename M::RealLifeType,typename M::IndexType>
make(
        std::pair<typename M::RealLifeType,typename M::RealLifeType> ranges_irl,
        std::pair<typename M::IndexType   ,typename M::IndexType>    ranges_idx,
        size_t n
    )
{
    using IRL = typename M::RealLifeType;
    using IDX = typename M::IndexType;

    const Range<IRL> r_irl(ranges_irl.first, ranges_irl.second);
    const Range<IDX> r_idx(ranges_idx.first, ranges_idx.second);

    std::shared_ptr<Proj<IRL,IDX>> linear
        = std::make_shared<M>(r_irl, r_idx);

    std::vector<std::shared_ptr<Proj<IRL,IDX>>> linears;

    for(unsigned int i=0; i < n; ++i) {
        linears.push_back(linear);
    }

    Projection<IRL,IDX> m(linears);
    return m;
}


template<class IRL, class IDX>
Projection<IRL,IDX> make_linear(std::pair<IRL,IRL> ranges_irl, std::pair<IDX,IDX> ranges_idx, size_t n)
{
    return make< Linear<IRL,IDX> >(ranges_irl, ranges_idx, n);
}

template<class IRL, class IDX>
Projection<IRL,IDX> make_normalizer(std::pair<IDX,IDX> ranges_idx, size_t n)
{
    return make_linear<IRL,IDX>({0.0,1.0}, ranges_idx, n);
}

template<class IRL, class IDX>
Projection<IRL,IDX> make_normalizer_bounded(std::vector<IDX> max_bounds)
{
    assert(max_bounds.size() > 0);

    const Range<double> r_irl(0.0,1.0);
    std::vector<std::shared_ptr<Proj<double,size_t>>> linears;

    for(unsigned int d=0; d < max_bounds.size(); ++d) {
        const Range<size_t> r_idx(std::make_pair(0, max_bounds[d]));

        std::shared_ptr<Proj<double,size_t>> linear
            = std::make_shared< Linear<double,size_t> >(r_irl, r_idx);

        linears.push_back(linear);
    }

    Projection<double,size_t> m(linears);
    return m;
 
}

template<class D>
Projection<double,size_t> make_normalizer(const D& domain)
{
    const Range<double> r_irl(0.0,1.0);

    std::vector<std::shared_ptr<Proj<double,size_t>>> linears;

    for(unsigned int d=0; d < domain.dimension; ++d) {
        const Range<size_t> r_idx(0, domain.box_size(d));

        std::shared_ptr<Proj<double,size_t>> linear
            = std::make_shared< Linear<double,size_t> >(r_irl, r_idx);

        linears.push_back(linear);
    }

    Projection<double,size_t> m(linears);
    return m;
}

template<class IRL, class IDX>
Projection<IRL,IDX> lift(Projection<IRL,IDX> proj, Range<IRL> range_irl, Range<IDX> range_idx)
{
    std::shared_ptr<Proj<IRL,IDX>> lm1
        = std::make_shared<Linear<IRL,IDX>>(range_irl,range_idx);
    proj.add(lm1);
    return proj;
}

template<class IRL, class IDX>
Projection<IRL,IDX> to_subspace(Projection<IRL,IDX> proj)
{
    proj.erase(proj.rbegin());
    return proj;
}

template<class T>
Range<T>::Range(const T min, const T max) : _mini(min), _maxi(max) {}

template<class T>
const T Range<T>::min() const
{
    return _mini;
}

template<class T>
const T Range<T>::max() const
{
    return _maxi;
}
} // Proj

} // ealain

