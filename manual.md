## 必看说明

**请务必看完整个说明文件**

该C++项目分为两个部分：C++调试、WASM调用

C++调试，是先写好C++的算法，然后写测试代码，并编译为exe可执行程序，用来测试算法是否能正常使用

> C++调试，需要安装[MinGW](https://sourceforge.net/projects/mingw/files/)

WASM调用，是将封装C++的算法，通过`Emscripten`编译成`wasm`，然后在前端进行调用



## 安装Emscripten

Emscripten 是一个强大的工具，可以将 C 和 C++ 代码编译为 WebAssembly (Wasm) 和 JavaScript，以便在 Web 浏览器中运行。以下是安装 Emscripten 的步骤：

**1. 安装 Emscripten SDK (emsdk)**

Emscripten 使用自己的 SDK (emsdk) 来管理安装和更新。可以通过克隆 GitHub 仓库获取 SDK。

```shell
# 克隆 Emscripten SDK 仓库
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
```

**2. 安装和激活最新版本的 Emscripten**

一旦克隆了 emsdk 仓库，就可以使用 emsdk 脚本安装和激活 Emscripten。

```shell
# 安装最新的 SDK
./emsdk install latest

# 激活最新的 SDK
./emsdk activate latest
```

**3. 将 Emscripten 添加到环境变量**

激活 SDK 后，运行以下命令将 Emscripten 环境变量添加到当前 shell 会话。

```shell
source ./emsdk_env.sh
```

**4.验证安装**

成功安装后，可以运行 `emcc` 来确认 Emscripten 是否正常工作。

```shell
emcc -v
```



## 编译调试程序的方式

生成动态链接库`dll`文件

```shell
g++ -shared -o ibtet.dll tetgen.cxx predicates.cxx
```

假设测试代码是`delaunay_test.cpp`，那么对应的编译命令为：

`delaunay_test.cpp`是测试代码文件，`delaunay.cpp`是它依赖的文件，`-L. -ltet`是指定动态链接库`libtet.dll`

```shell
# delaunay
g++ -o delaunay delaunay_test.cpp delaunay.cpp -L. -ltet
#alphaShape
g++ -o alphaShape alphaShape_test.cpp alphaShape.cpp  delaunay.cpp calcTetRadius.cpp -L. -ltet  
```

执行完成后，会在`test`目录下生成`delaunay.exe`

直接运行它没什么用，需要在test目录下，打开CMD，然后输入`delaunay.exe`

如果法线乱码 需要执行`chcp 65001`



## 生成wasm的方式

先用`emscripten`生成`.o`中间文件，是单个静态库

```shell
# -O3：启用编译优化，提升算法性能
# -msimd128：启用 WebAssembly 的 SIMD 指令集，适用于处理密集型计算。
emcc -c tetgen.cxx -O3 -msimd128
emcc -c predicates.cxx -O3 -msimd128
```

再将单个库合成完整的`.a`静态链接库

```shell
emar rcs libtet.a tetgen.o predicates.o
```

`.o`文件就可以删除了，只剩下`libtet.a`

最后执行编译命令：

```shell
emcc libtet.a delaunay.cpp -o ./wasm/delaunay.js -s EXPORTED_FUNCTIONS="['_delaunay', '_malloc', '_free']" -s MODULARIZE=1 -s EXPORT_ES6=1
```

命令有点长，我们一步步理解：

```shell
# 这一步很好理解，就是指明连接库以及输出文件名
emcc libtet.a delaunay.cpp -o ./wasm/delaunay.js

# 这一步是导出delaunay函数，另外的malloc和free是给前端用来管理内存的
-s EXPORTED_FUNCTIONS="['_delaunay', '_malloc', '_free']"

# 这一步是指要导出为esm模块化的形式
-s MODULARIZE=1 -s EXPORT_ES6=1
```



## 扩大初始内存

因为delaunay算法的输入数据量非常大，wasm的初始内存可能不够用，所以需要增大初始内存

因此，需要在编译命令后添加：

```shell
# INITIAL_MEMORY=100mb：初始内存1024M
# ALLOW_MEMORY_GROWTH：允许内存自动增长
-s INITIAL_MEMORY=100mb -s ALLOW_MEMORY_GROWTH
```

完整的命令为：

```shell
emcc libtet.a delaunay.cpp -o ./wasm/delaunay.js -s EXPORTED_FUNCTIONS="['_delaunay', '_malloc', '_free']" -s MODULARIZE=1 -s EXPORT_ES6=1 -s INITIAL_MEMORY=1024mb -s ALLOW_MEMORY_GROWTH
```

如果发现还是不够，则需要进一步增大初始内存



## 启用O3优化

在tetgen文档中有这么一段话：`TetGen 的旧版本是由 GNU gcc/g++ 版本 4.7.2 编译的 64 位版本，带有 −*O*3（优化）开关`

我们可以在编译命令结尾加上`-O3`的选项，这样该算法的运行速度会得到明显提升

```shell
emcc libtet.a delaunay.cpp -o ./wasm/delaunay.js -s EXPORTED_FUNCTIONS="['_delaunay', '_malloc', '_free']" -s MODULARIZE=1 -s EXPORT_ES6=1 -s INITIAL_MEMORY=1024mb -s ALLOW_MEMORY_GROWTH -O3
```



## 内存超过限制问题

除了wasm分配的初始内存不够之外，还有可能因为栈内存不够导致报错

这个时候就需要分配更大的栈内存

```shell
# 设置程序的最大堆栈大小为 64 MB，确保可以处理较大的递归调用或局部变量分配需求
# 设置初始堆栈大小为 10 MB。这个值为程序启动时分配的堆栈内存量。
-s TOTAL_STACK=64mb -s STACK_SIZE=10mb
```

最终的完整命令为：

```shell
emcc libtet.a delaunay.cpp -o ./wasm/delaunay.js -s EXPORTED_FUNCTIONS="['_delaunay', '_malloc', '_free']" -s MODULARIZE=1 -s EXPORT_ES6=1 -s INITIAL_MEMORY=1024mb -s ALLOW_MEMORY_GROWTH -O3 -s TOTAL_STACK=64mb -s STACK_SIZE=10mb
```

