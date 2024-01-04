
namespace ealain {
namespace domain {

// Plan

template<typename T>
PlanT<T>::PlanT( data_type data ) : Domain<2,T>(data) {}

template<typename T>
PlanT<T>::PlanT(std::size_t lines, std::size_t columns, T fill) :
    Domain<2,T>(lines, columns, fill)
{}

template<typename T>
PlanT<T>::PlanT(std::array<std::size_t,2> sizes, T fill) :
    Domain<2,T>(sizes[0], sizes[1], fill)
{}

template<typename T>
PlanT<T>::PlanT(proj::Projection<T,size_t> p, T fill) :
    Domain<2,T>(p, fill)
{}


template<typename T>
T& PlanT<T>::at(std::vector<std::size_t> coords)
{
    return this->_data[ coords[0] ][ coords[1] ];
}

template<typename T>
const T& PlanT<T>::at(std::vector<std::size_t> coords) const
{
    return this->_data[ coords[0] ][ coords[1] ];
}

template<typename T>
std::size_t PlanT<T>::size() const
{
    assert(this->_data.size() > 0);
    return this->_data.size() * this->_data.at(0).size();
}

template<typename T>
std::array<std::size_t,2> PlanT<T>::sizes() const
{
    assert(this->_data.size() > 0);
    return { this->_data.size(), this->_data.at(0).size() };
}

template<typename T>
typename PlanT<T>::iterator PlanT<T>::begin()
{
    return ealain::begin(*this);
}

template<typename T>
typename PlanT<T>::iterator PlanT<T>::end()
{
    return ealain::end(*this);
}

// Plan_iterator

template<typename T>
Plan_iterator<T>::Plan_iterator(
        PlanT<T>* plan,
        std::vector<std::size_t> coords
    ) :
    _domain(plan),
    _i_row(coords.at(0)),
    _j_col(coords.at(1))
{
    assert(coords.size() == 2);
}

template<typename T>
Plan_iterator<T> Plan_iterator<T>::begin(PlanT<T>& plan) {
    return Plan_iterator(&plan, {0, 0});
}

template<typename T>
Plan_iterator<T> Plan_iterator<T>::end(PlanT<T>& plan) {
    return Plan_iterator(&plan, {plan.data().size(), 0});
}

template<typename T>
T& Plan_iterator<T>::operator*()
{
    return const_cast<T&>(static_cast<const PlanT<T>&>(*_domain).at({_i_row,_j_col}));
}

template<typename T>
const T& Plan_iterator<T>::operator*() const
{
    return _domain->at({_i_row,_j_col});
}

template<typename T>
Plan_iterator<T>& Plan_iterator<T>::operator++()
{
    if(_j_col + 1 < _domain->data()[_i_row].size()) {
        _j_col++;
    } else {
        do {
            _i_row++;
        } while(_i_row < _domain->data().size() and _domain->data()[_i_row].empty());

        _j_col = 0;
    }
    return *this;
}

template<typename T>
Plan_iterator<T>& Plan_iterator<T>::operator--()
{
    if(_j_col > 0) {
        _j_col--;
    } else {
        do {
            _i_row--;
        } while( _domain->data()[_i_row].empty());
        _j_col = _domain->data()[_i_row].size() - 1;
    }
    return *this;
}

template<typename T>
Plan_iterator<T> Plan_iterator<T>::operator++(int)
{
    auto prev = *this;
    ++(*this);
    return prev;
}

template<typename T>
Plan_iterator<T> Plan_iterator<T>::operator--(int)
{
    auto prev = *this;
    --(*this);
    return prev;
}

template<typename T>
bool Plan_iterator<T>::operator==(const Plan_iterator<T>& other) const
{
    return other._domain  == _domain
       and other._i_row   == _i_row
       and other._j_col   == _j_col;
}

template<typename T>
bool Plan_iterator<T>::operator!=(const Plan_iterator<T>& other) const
{
    return not (*this == other);
}

template<typename T>
std::size_t Plan_iterator<T>::operator()(std::size_t dimension) const
{
    std::array<std::size_t,2> coords = {_i_row,_j_col};
    return coords[dimension];
}

} // domain

} // ealain

