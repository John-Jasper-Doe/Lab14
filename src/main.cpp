/**
 * @file main.cpp
 * @brief Main file in this project.
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

/* See the license in the file "LICENSE.txt" in the root directory. */

#include <boost/program_options.hpp>
#include <iostream>

namespace {

struct param {
  std::string src{""};
  std::size_t mnum{0};
  std::size_t rnum{0};
};

using param_t = param;

void get_param(const int argc, const char* const argv[], param_t& param) {
  namespace po = boost::program_options;

  // clang-format off
  po::options_description desc("Options: ");
  desc.add_options()
      ("help,h", "this help")
      ("src,s", po::value<std::string>(), "path to file with data")
      ("mnum,m", po::value<std::size_t>(), "number of threads to work with map function")
      ("rnum,r", po::value<std::size_t>(), "number of threads to work with reduce function");
  // clang-format on

  po::variables_map vm;
  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
  }
  catch (boost::program_options::error& err) {
    throw std::invalid_argument(err.what());
  }

  if (vm.count("help") || vm.count("h")) {
    std::cout << desc << "\n";
    exit(0);
  }

  if (vm.count("src"))
    param.src = vm["src"].as<std::string>();
  else if (vm.count("s"))
    param.src = vm["s"].as<std::string>();
  else
    throw std::invalid_argument("Source path was not set");

  if (vm.count("mnum"))
    param.mnum = vm["mnum"].as<std::size_t>();
  else if (vm.count("m"))
    param.mnum = vm["m"].as<std::size_t>();
  else
    throw std::invalid_argument("Number of threads for map was not set");

  if (vm.count("rnum"))
    param.rnum = vm["rnum"].as<std::size_t>();
  else if (vm.count("r"))
    param.rnum = vm["r"].as<std::size_t>();
  else
    throw std::invalid_argument("Number of threads for reduce was not set");
}

} /* :: */

/** @brief Main entry point */
int main(int argc, const char* argv[]) {
  param_t prm;

  try {
    get_param(argc, argv, prm);
  }
  catch (const std::invalid_argument& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}