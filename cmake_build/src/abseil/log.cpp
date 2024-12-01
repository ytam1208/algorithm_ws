#include "system.hpp"
#include "eigen3/Eigen/Dense"
#include "absl/strings/str_join.h"
#include "absl/log/log.h"
#include "absl/log/initialize.h"

int main(int argc, char** argv)
{    
    std::vector<std::string> v = {"foo", "bar", "baz"};
    std::string s = absl::StrJoin(v, "-");

    std::cout << "Joined string: " << s << std::endl;

    LOG(INFO) << "Joined string: " << s;
    LOG(WARNING) << "Joined string: " << s;
    LOG(ERROR) << "Joined string: " << s;
    LOG(FATAL) << "Joined string: " << s;

    return 0;
}
