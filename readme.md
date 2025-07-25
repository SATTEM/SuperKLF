# SuperKLF
SuperKLF是出于学习目的实现steam游戏《超级键盘侠》基本框架的项目
## 基本信息
- 平台：linuxmint(原生)/windows11(交叉编译)
- 语言标准：C++ 17
- IDE: vscode
- 使用库和工具：
[nlohmann::json](https://github.com/nlohmann/json)
[raylib](https://github.com/raysan5/raylib)
[CMake](https://cmake.org/)
- 创意模板：
[超级键盘侠](https://store.steampowered.com/app/2719030/)
- ### 编译Linux版本
根目录下执行
```shell
mkdir build && cd build
cmake ..
cmake --build . -j{n} #其中n为CPU线程数
```

---
或执行/build.sh
```shell
./build.sh #默认16线程构建Release版本
```
编译结果在bin文件夹中
- ### 编译Windows版本
需要安装MinGW-w64交叉编译工具链
在Debian系的系统终端中执行
```shell
sudo apt update
sudo apt install mingw-64
```
根目录下执行
```shell
mkdir build-win && cd build-win
cmake -DCMAKE_TOOLCHAIN_FILE=../mingw-toolchain.cmake ..
cmake --build . -j{n} #其中n为CPU线程数
```
---
或执行/build-win.sh
```shell
./build-win.sh #默认16线程构建Release版本
```
编译结果在bin-win文件夹中