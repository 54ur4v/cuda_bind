#pragma once

namespace cb {
namespace mpl {

  // apply_from_tuple code derived from this Stack Overflow thread:
  // http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer

  template<int...>
  struct indices {};

  template<int N, int... S>
  struct gen : gen<N-1, N-1, S...> {};

  template<int... S>
  struct gen<0, S...> {
    typedef indices<S...> type;
  };

  template<typename F, typename T, int... I>
  __host__ __device__
  auto __delayed_dispatch(F f, T t, indices<I...>) ->
  decltype(f(thrust::get<I>(t)...)) {
    return f(thrust::get<I>(t)...);
  }

  template<typename F, typename... S>
  typename std::result_of<F(S...)>::type
  __host__ __device__
  apply_from_tuple(F f, thrust::tuple<S...> s) {
    return __delayed_dispatch(f, s, typename gen<sizeof... (S)>::type());
  }

} // namespace mpl
} // namespace cb