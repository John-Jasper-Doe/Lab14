/**
 * @file reducer.hpp
 * @brief Definition of the class "Reducer".
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

#ifndef CORE_REDUCER_HPP_
#define CORE_REDUCER_HPP_

#include <functional>
#include <future>
#include <thread>
#include <vector>

/** @brief The namespace of the MAP REDUCE project */
namespace yamr {
/** @brief The namespace of the Core */
namespace core {

/** @brief Alias of the pointer on function for "Reducer" class. */
template <class DATA_TYPE, class OUT_TYPE>
using rfunc_ptr_t = std::function<OUT_TYPE(std::vector<DATA_TYPE>&&)>;

/** @brief The reducer class */
template <class DATA_TYPE, class OUT_TYPE>
class reducer {
  using thread_ptr_t = std::unique_ptr<std::thread, std::function<void(std::thread*)>>;

  /** @brief Reduce function. */
  rfunc_ptr_t<DATA_TYPE, OUT_TYPE> function_;

public:
  explicit reducer(rfunc_ptr_t<DATA_TYPE, OUT_TYPE> reduce_func) noexcept
    : function_{reduce_func} {}

  /**
   * @brief Function to execute.
   * @param [in] input - input data.
   * @return Processed data.
   */
  std::vector<OUT_TYPE> exec(std::vector<std::vector<DATA_TYPE>>&& input) noexcept {
    std::vector<OUT_TYPE> res;

    std::vector<std::promise<OUT_TYPE>> promises(input.size());
    std::vector<std::future<OUT_TYPE>> futures;
    for (size_t i = 0; i != promises.size(); ++i) {
      futures.push_back(promises[i].get_future());
    }

    /* worker function */
    auto worker = [this](std::promise<OUT_TYPE>&& promise, std::vector<DATA_TYPE>&& arg) {
      OUT_TYPE res = function_(std::move(arg));
      promise.set_value(std::move(res));
    };

    std::vector<thread_ptr_t> workers;
    for (size_t i = 0; i != promises.size(); ++i) {
      workers.emplace_back(
          thread_ptr_t(new std::thread(worker, std::move(promises[i]), std::move(input[i])),
                       [](std::thread* t) { t->join(); }));
    }

    for (size_t i = 0; i != futures.size(); ++i) {
      res.push_back(futures[i].get());
    }

    return res;
  }
};

} /* core:: */
} /* yamr:: */

#endif /* CORE_REDUCER_HPP_ */
