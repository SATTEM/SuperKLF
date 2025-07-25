# SuperKLF
SuperKLF是出于学习目的实现steam游戏《超级键盘侠》基本框架的项目
## 目录
- [基本信息](#info)
- [编译](#setup)
- [游戏运行流程](#game)
- [组件概要](#struct)
	+ [控制管理组件](#control)
		- [流程控制类](#flow)
		- [关卡管理类](#levelMgr)
		- [资源管理类](#resMgr)
		- [数据管理类](#dataMgr)
		- [效果管理类](#effectMgr)
		- [信号系统类](#signal)
	+ [游戏对象](#obj)
		- [关卡类](#level)
		- [关卡界面类](#levelUI)
		- [实体类](#entity)
		- [奖励类](#reward)
		- [子弹类](#bullet)
		- [子弹特效类](#bulletEffect)
		- [即时效果类](#instantEffect)
		- [遗物类](#relicEffect)

## 基本信息<a id="info"></a>
- 平台：linuxmint(原生)/windows11(交叉编译)
- 语言标准：C++ 17
- IDE: vscode
- 使用库和工具：
[nlohmann::json](https://github.com/nlohmann/json)
[raylib](https://github.com/raysan5/raylib)
[CMake](https://cmake.org/)
- 创意模板：
[超级键盘侠](https://store.steampowered.com/app/2719030/)
## 编译运行<a id="setup"></a>
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
## 游戏运行流程<a id="game"></a>
1. 启动游戏：获取[资源](#resMgr)，初始化各种组件，设置日志等级，注册[即时特效](#instantEffect)和[遗物特效](#relicEffect)，读取各[配置文件](/outline)创建游戏对象
2. 进入游戏主循环：由[流程控制类](#flow)调用实时关卡的update()方法，更新游戏状态
3. 退出游戏主循环：[数据管理类](#dataMgr)存档，清理[资源](#resMgr)
## 组件概要<a id="struct"></a>
> 下面是游戏主要组件的概述，忽略了部分细节
### 控制管理组件<a id="control"></a>
- #### 流程控制类StageController<a id="flow"></a>
  + 控制游戏流程的单例类
  + 游戏开始时进行设置日志等级、注册[即时特效](#instantEffect)和[遗物特效](#relicEffect)等初始化工作
  + 未接收到关闭信号时，从[LevelManager关卡管理类](#levelMgr)获得实时[关卡Level](#level)并调用其成员方法更新游戏状态
- #### 关卡管理类LevelManager<a id="levelMgr"></a>
  + 管理静态和动态[关卡](#level)，并负责切换关卡的单例类
  + 在初始化时创建主菜单/胜利/失败/商店静态类，同时读取[关卡配置文件](/res/data/Levels.json)，生成战役和事件关卡
- #### 资源管理类ResourceManager<a id="resMgr"></a>
  + 管理纹理和字体资源的单例类
  + 内部管理一个<string,Texture2D>哈希表，并提供加载/卸载/缩放/获取纹理的方法
  + 管理游戏字体
  > 由于raylib对中文支持有限，游戏需在raylib库的基础上实现[新的文字绘制函数](/doc/raylibDrawChinese.pdf)
- #### 数据管理类DataManager<a id="dataMgr"></a>
  + 管理游戏数据的单例类
  + 维护玩家和敌人[实体](#entity)、游戏数值数据（如[奖励](#reward)）
  + 承担存档和读档职能
- #### 效果管理类EffectManager<a id="effectMgr"></a>
  + 管理即时效果InstantEffect和遗物效果RelicEffect的单例类
  + 为两种效果分别维护一个效果池，通过抽象工厂模式，由[效果配置文件](/res/data/Effects.json)动态创建游戏初始化时注册的效果
- #### 事件（信号）系统类EventSystem<a id="signal"></a>
  + 用于实现遗物机制的单例类，采用观察者模式
  + 提供绑定信号和遗物、广播信号的接口
### 游戏对象<a id="obj"></a>
- #### 关卡类Level<a id="level"></a>
  + Level为抽象基类，其派生类实现具体的关卡逻辑
  + 每个关卡类都有对应的[关卡界面类LevelUI](#levelUI)，并根据从接口获得的信号执行对应逻辑
- #### 关卡界面类LevelUI<a id="levelUI"></a>
  + BaseUI为抽象基类，其派生类实现具体界面绘制逻辑
  + 每个关卡界面类都向对应[关卡类](#level)暴露查询组件状态的接口方法
- #### 实体类Entity<a id="entity"></a>
  + 表示实体对象的基类，派生类有玩家类Player和敌人类Enemy，规定了基本数据成员行为
- #### 奖励类Reward<a id="reward"></a>
  + 表示玩家可获得奖励的基类，由[数据管理类](#dataMgr)根据[奖励配置文件](/res/data/Rewards.json)动态生成
  + 奖励机制由[即时效果](#instantEffect)对象实现
- #### 子弹类Bullet<a id="buller"></a>
  + 子弹基类，规定了子弹的基本数据成员和行为
  + 子弹功能通过触发一个或多个[子弹特效](#bulletEffect)实现
- #### 子弹特效类BulletEffect<a id="bulletEffect"></a>
  + 子弹效果的基类，派生类分别实现不同种类的子弹特效
- #### 即时效果类InstantEffect<a id="instantEffect"></a>
  + 即时效果基类，应用时立即对玩家产生效果
- #### 遗物类RelicEffect<a id="relicEffect"></a>
  + 遗物类，只有当被[触发](#signal)时才对[实体](#entity)产生效果