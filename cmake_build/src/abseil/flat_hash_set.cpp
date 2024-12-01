#include "absl/log/log.h"
#include "absl/container/flat_hash_set.h"

int main(int argc, char** argv)
{
  absl::flat_hash_set<std::string> set1;

  // Initializer List constructor
  absl::flat_hash_set<std::string> set2 = {{"huey"}, {"dewey"}, {"louie"},};

  // Copy constructor
  absl::flat_hash_set<std::string> set3(set2);

  // Copy assignment operator
  // Hash functor and Comparator are copied as well
  absl::flat_hash_set<std::string> set4;
  set4 = set3;

  // Move constructor
  // Move is guaranteed efficient
  absl::flat_hash_set<std::string> set5(std::move(set4));

  // Move assignment operator
  // May be efficient if allocators are compatible
  absl::flat_hash_set<std::string> set6;
  set6 = std::move(set5);

  // Range constructor
  std::vector<std::string> v1 = {"a", "b"};
  absl::flat_hash_set<std::string> set7(v1.begin(), v1.end());
  auto find_set_it = set7.find("a");
  if (find_set_it != set7.end())
  {
    LOG(INFO) << "Found: " << *find_set_it;
  }

  return 0;
}