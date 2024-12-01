#include "absl/log/log.h"
#include "absl/container/flat_hash_map.h"

/*
  absl::flat_hasp_map is an unordered associative container that is optimized 
  for both speed and memory footprint in most common use cases.
*/

int main(int argc, char** argv)
{
  absl::flat_hash_map<int, std::string> map1;

  // Initializer List constructor
  absl::flat_hash_map<int, std::string> map2 =
      {{1, "huey"}, {2, "dewey"}, {3, "louie"},};

  // Copy constructor
  absl::flat_hash_map<int, std::string> map3(map2);

  // Copy assignment operator
  // Hash functor and Comparator are copied as well
  absl::flat_hash_map<int, std::string> map4;
  map4 = map3;

  // Move constructor
  // Move is guaranteed efficient
  absl::flat_hash_map<int, std::string> map5(std::move(map4));

  // Move assignment operator
  // May be efficient if allocators are compatible
  absl::flat_hash_map<int, std::string> map6;
  map6 = std::move(map5);

  // Range constructor
  std::vector<std::pair<int, std::string>> v2 = {{1, "a"}, {2, "b"}};
  absl::flat_hash_map<int, std::string> map7(v2.begin(), v2.end());

  auto find_map_it = map7.find(1);
  if (find_map_it != map7.end())
  {
    LOG(INFO) << "Found key: " << find_map_it->first << " value: " << find_map_it->second;
  }
  return 0;
}