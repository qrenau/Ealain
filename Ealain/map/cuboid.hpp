
namespace ealain {
namespace domain {

// Cuboid

template<typename T>
CuboidT<T>::CuboidT( data_type data ) : Domain<3,T>(data) {}

template<typename T>
CuboidT<T>::CuboidT(std::size_t lines, std::size_t columns, std::size_t frames, T fill) :
    Domain<3,T>(lines, columns, frames, fill)
{}

template<typename T>
CuboidT<T>::CuboidT(std::array<std::size_t,3> sizes, T fill) :
    Domain<3,T>(sizes[0], sizes[1], sizes[2], fill)
{}

template<typename T>
CuboidT<T>::CuboidT(proj::Projection<T,size_t> p, T fill) :
    Domain<3,T>(p,fill)
{}

template<typename T>
const T& CuboidT<T>::at(std::vector<std::size_t> coords) const
{
    return this->_data[coords[0]][coords[1]][coords[2]];
}

template<typename T>
T& CuboidT<T>::at(std::vector<std::size_t> coords)
{
    return this->_data[coords[0]][coords[1]][coords[2]];
}

template<typename T>
std::size_t CuboidT<T>::size() const
{
    assert(this->_data.size() > 0);
    assert(this->_data.at(0).size() > 0);
    return this->_data.size() * this->_data.at(0).size() * this->_data.at(0).at(0).size();
}

template<typename T>
std::array<std::size_t,3> CuboidT<T>::sizes() const
{
    assert(this->_data.size() > 0);
    assert(this->_data.at(0).size() > 0);
    return { this->_data.size(), this->_data.at(0).size(), this->_data.at(0).at(0).size() };
}


template<typename T>
typename CuboidT<T>::iterator CuboidT<T>::begin()
{
    return ealain::begin(*this);
}

template<typename T>
typename CuboidT<T>::iterator CuboidT<T>::end()
{
    return ealain::end(*this);
}


// Free function

template<typename T>
std::vector<std::vector<double>> slice_at_angle(const CuboidT<T>& domain, unsigned int angle_idx)
{
    std::vector<std::vector<double>> slice;
    for(auto&& mat_x : domain.data()) {
        std::vector<double> line;
        for(auto&& line_y : mat_x ) {
            line.push_back(line_y[angle_idx]);
        }
        slice.push_back(line);
    }
    return slice;
}


// Cuboid_iterator

template<typename T>
Cuboid_iterator<T>::Cuboid_iterator(
        CuboidT<T>* cuboid,
        std::vector<std::size_t> coords
    ) :
    _domain(cuboid),
    _i_cut(coords.at(0)),
    _j_row(coords.at(1)),
    _k_col(coords.at(2))
{
    assert(coords.size() == 3);
}

template<typename T>
Cuboid_iterator<T> Cuboid_iterator<T>::begin(CuboidT<T>& cube) {
    return Cuboid_iterator<T>(&cube, {0, 0, 0});
}

template<typename T>
Cuboid_iterator<T> Cuboid_iterator<T>::end(CuboidT<T>& cube) {
    return Cuboid_iterator<T>(&cube, {cube.data().size(), 0, 0});
}

template<typename T>
double& Cuboid_iterator<T>::operator*()
{
    return const_cast<double&>(static_cast<const CuboidT<T>&>(*_domain).at({_i_cut,_j_row,_k_col}));
}

template<typename T>
const double& Cuboid_iterator<T>::operator*() const
{
    return _domain->at({_i_cut,_j_row,_k_col});
}

template<typename T>
Cuboid_iterator<T>& Cuboid_iterator<T>::operator++()
{
    if(_k_col + 1 < _domain->data()[_i_cut][_j_row].size()) {
        _k_col++;
    } else {
        if(_j_row + 1 < _domain->data()[_i_cut].size()) {
            _j_row++;
        } else {
            do {
                _i_cut++;
            } while(_i_cut < _domain->data().size() and _domain->data()[_i_cut].empty());
            _j_row = 0;
        }
        _k_col = 0;
    }
    return *this;
}

template<typename T>
Cuboid_iterator<T>& Cuboid_iterator<T>::operator--()
{
    if(_k_col > 0) {
        _k_col--;
    } else {
        if(_j_row > 0) {
            _j_row--;
        } else {
            do {
                _i_cut--;
            } while(_domain->data()[_i_cut].empty());

            _j_row = _domain->data()[_i_cut].size() - 1;
        }
        _k_col = _domain->data()[_i_cut][_j_row].size() - 1;
    }
    return *this;
}

template<typename T>
Cuboid_iterator<T> Cuboid_iterator<T>::operator++(int)
{
    auto prev = *this;
    ++(*this);
    return prev;
}

template<typename T>
Cuboid_iterator<T> Cuboid_iterator<T>::operator--(int)
{
    auto prev = *this;
    --(*this);
    return prev;
}

template<typename T>
bool Cuboid_iterator<T>::operator==(const Cuboid_iterator<T>& other) const
{
    return other._domain  == _domain
       and other._i_cut   == _i_cut
       and other._j_row   == _j_row
       and other._k_col   == _k_col;
}

template<typename T>
bool Cuboid_iterator<T>::operator!=(const Cuboid_iterator<T> &other) const
{
    return not (*this == other);
}

template<typename T>
std::size_t Cuboid_iterator<T>::operator()(std::size_t dimension) const
{
    std::array<std::size_t,3> coords = {_i_cut,_j_row,_k_col};
    return coords[dimension];
}

} // domain
} // ealain

