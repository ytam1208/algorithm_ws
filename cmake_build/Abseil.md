## Abseil install
```
git clone https://github.com/abseil/abseil-cpp.git

mkdir build 

cd build 

cmake -DABSL_BUILD_TESTING=ON -DABSL_USE_GOOGLETEST_HEAD=ON ..

make -j

ctest

sudo make install

```

## Available Abseil CMake Public Targets
```
absl::algorithm
absl::base
absl::debugging
absl::flat_hash_map
absl::flags
absl::memory
absl::meta
absl::numeric
absl::random_random
absl::strings
absl::synchronization
absl::time
absl::utility
```