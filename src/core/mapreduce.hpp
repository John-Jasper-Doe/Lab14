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

/** @brief The namespace of the MAP REDUCE project */
namespace yamr {
/** @brief The namespace of the Core */
namespace core {

/** @brief The map_reduce class */
class map_reduce {
  std::size_t mnum_;
  std::size_t rnum_;

public:
  explicit map_reduce(std::size_t mnum, std::size_t rnum) noexcept
    : mnum_{mnum}, rnum_{rnum} {}

  void run() noexcept;
};

} /* core:: */
} /* yamr:: */

#endif /* CORE_MAPREDUCE_HPP_ */
