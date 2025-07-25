mkdir build-win
cd build-win
cmake -DCMAKE_TOOLCHAIN_FILE=../mingw_toolchain.cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . -j16