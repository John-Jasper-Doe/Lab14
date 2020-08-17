/**
 * @file mapreduce.hpp
 * @brief Definition of the class "Map Reduce".
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

#ifndef CORE_MAPREDUCE_HPP_
#define CORE_MAPREDUCE_HPP_

#include <string>

#include "mapper.hpp"

#include "common/merge.hpp"
#include "common/split.hpp"

/** @brief The namespace of the MAP REDUCE project */
namespace yamr {
/** @brief The namespace of the Core */
namespace core {

/** @brief The map_reduce class */
template <class DATA_TYPE, class MAPPER_OUT_TYPE>
class map_reduce {
  std::size_t mnum_;
  std::size_t rnum_;

public:
  explicit map_reduce(std::size_t mnum, std::size_t rnum) noexcept : mnum_{mnum}, rnum_{rnum} {}

  void run(std::vector<DATA_TYPE>&& input, mfunc_ptr_t<DATA_TYPE, MAPPER_OUT_TYPE> mfunc) noexcept {
    std::vector<std::vector<DATA_TYPE>> splitted = split(std::move(input), mnum_);

    /* Run MAP */
    core::mapper<DATA_TYPE, MAPPER_OUT_TYPE> mapper(mfunc);
    std::vector<std::vector<MAPPER_OUT_TYPE>> mres = mapper.exec(std::move(splitted));

    /* Sorted */
    std::for_each(mres.begin(), mres.end(),
                  [](std::vector<MAPPER_OUT_TYPE>& vec) { std::sort(vec.begin(), vec.end()); });

    /* MERGE */
    std::vector<MAPPER_OUT_TYPE> smerged = common::merge<MAPPER_OUT_TYPE>(std::move(mres));
  }
};

} /* core:: */
} /* yamr:: */

#endif /* CORE_MAPREDUCE_HPP_ */
