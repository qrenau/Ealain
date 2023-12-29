namespace ealain {

    namespace domain {

        template<class D>
        D cost_to_proba(const D& cost)
        {
            D domain = cost;
            for(auto it = ealain::begin(domain); it != ealain::end(domain); ++it) {
                *it = cost_to_proba(*it);
            }
            return domain;
        }

        template<class D>
        D proba_to_cost(const D& proba)
        {
            D domain = proba;
            for(auto it = ealain::begin(domain); it != ealain::end(domain); ++it) {
                *it = proba_to_cost(*it);
            }
            return domain;
        }

    } // domain

    template<class D>
    typename D::iterator begin(D& domain)
    {
        return D::iterator::begin(domain);
    }

    template<class D>
    typename D::iterator end(D& domain)
    {
        return D::iterator::end(domain);
    }

} // ealain
