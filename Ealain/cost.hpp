
namespace ealain {

    namespace cost {

        template<class D>
        double Coverage<D>::operator()(group::Group& group)
        {
            this->_domain = group(this->_domain);

            double n = 0;
            // Loop over each cells of the domain
            for(auto it = ealain::begin(this->_domain); it != ealain::end(this->_domain); it++) {
                if( *it >= _threshold ) {
                    n++;
                }
            }
            assert(n <= this->_domain.size());
            return n;
        }

        template<class D>
        Coverage<D> make_coverage(D& domain, const double cost_threshold)
        {
            return Coverage<D>(domain, cost_threshold);
        }

    } // cost

} // ealain

