#ifndef __tuple__
#define __tuple__

#include <iostream>

namespace LAZ {
    template<typename First, typename... Rest>
    struct Tuple : public Tuple<Rest...> {
    public:
        constexpr Tuple() {}
        constexpr Tuple(First first, Rest... rest) : Tuple<Rest...>(rest...),
                                                     _first{first} 
        {}
        constexpr Tuple(const Tuple<First, Rest...>& oth) : Tuple<Rest...>(oth), 
                                                            _first{oth._first} 
        {}
        constexpr Tuple(Tuple<First, Rest...>&& oth) : Tuple<Rest...>(oth),
                                                       _first{oth._first} 
        {}
    public:
        Tuple<First, Rest...>& operator=(const Tuple<First, Rest...>&);
        Tuple<First, Rest...>& operator=(Tuple<First, Rest...>&&) = default;
    public:
        First _first {};
    };

    template<typename First>
    struct Tuple<First> {
    public:
        constexpr Tuple() {}
        constexpr Tuple(First first) : _first{first} {}
        constexpr Tuple(const Tuple<First>& oth) : _first{oth._first} 
        {}
        constexpr Tuple(Tuple<First>&& oth) : _first{oth._first} 
        {}
    public:
        Tuple<First>& operator=(const Tuple<First>&);
        Tuple<First>& operator=(Tuple<First>&&) = default;
    public:
        First _first {};
    };

    template<typename First, typename... Rest>
    Tuple<First, Rest...>& Tuple<First, Rest...>::operator=(const Tuple<First, Rest...>& rhs) {
        *this = Tuple<First, Rest...>(rhs);
        _first = rhs._first;
        return *this;
    }

    template<typename First>
    Tuple<First>& Tuple<First>::operator=(const Tuple<First>& rhs) {
        _first = rhs._first;
        return *this;
    }
    
    // GET
    template<int N,typename First, typename... Rest>
    struct doGet {
        static decltype(auto) value(const Tuple<First, Rest...>& t) {
            return doGet<N - 1, Rest...>::value(t);
        }
    };

    template<typename First, typename... Rest>
    struct doGet<0, First, Rest...> {
        static decltype(auto) value(const Tuple<First, Rest...>& t) {
            return t._first;
        }
    };

    template<int N, typename... T>
    decltype(auto) get(Tuple<T...> t) {
        return doGet<N, T...>::value(t);
    }

    // SIZE
    template<int N, typename First, typename... T>
    struct TupleSize {
        enum { value = TupleSize<N + 1, T...>::value };
    };

    template<int N, typename First>
    struct TupleSize<N, First> {
        enum { value = N };
    };

    template<typename... T>
    constexpr int size(Tuple<T...> t) {
        return TupleSize<1, T...>::value;
    }

    // MAKE PAIR
    template<typename... T>
    Tuple<T...> makePair(T... args) {
        return Tuple<T...>(args...);
    }
};

#endif