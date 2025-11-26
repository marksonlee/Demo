### ROS编程小细节

#### cpp细节

1.关于main函数形参argc和argv

解释：main函数的参数，用于接收命令行输入的传参

argc：命令行运行代码传入的参数总个数

argv：传入的参数的具体值，用数组保存下来了

例子：**（rosrun 包名 节点 A B）**（此处A，B是命令行运行传入的实参，一般用来传__name和__ns）；当输入上述命令后，此时argc=3；因为传入了三个命令行实参。第一个是节点的可执行文件的启动路径；第二个是A；第三个是B。此时argv[0] = 启动路径；argv[1] = A ; argv[2] = B

一般此处的A,B多传入节点名和命名空间



2.启动文件launch

启动文件中的type，name很容易分不清，这下作解释。

type是可执行文件，name是ros启动时的节点名称，要求是在该命名空间ns中是唯一的

参考这段cmake代码：add_executable(client_node src/client_node.cpp)

意思是将src目录下的.cpp文件生成一个可执行文件，这个可执行文件的名称就是：client_node 

而在具体编写cpp文件中，ros::init(argc,argv,"client_node");初始化节点，让节点的名称也设置为了client_node，所以此时cpp可执行文件名称和节点名称相同。节点名称是client_node，这个时候运行ros的话就要在该ns空间下是要唯一的了



3.关于ns：namespace

为什么要设置ns？答：因为个人demo和企业批量化制造有着不同的区别，有了ns可以方便区分是哪一台机器。比如说企业批量走货，一年发货100台，每一台机器人的功能都是相同的，中心服务器位置位于广州，我想在广州同时分管这100台发往世界各地机器，所以需要用到namespace。将100台机器设置为robot1，robot2，……，robot100的命名空间。这样就可以实现代码的复用了。在一个ros master管理下。一个cmd话题就可以分成/robot1/cmd_vel，……/robot100/cmd_vel。可以精确地控制不同车辆的速度话题和参数了

如果不设置ns的值，默认就是"/"，意思就是根目录下，对于个人demo来说是符合的，但是批量的话，就需要使用到ns了



#### cmake小知识

##### 1.查找依赖包

```
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  message_generation
)
```

- **为什么需要？**  
  你的包依赖这些 ROS 功能：
  - `roscpp`：写 C++ 节点
  - `rospy`：写 Python 节点
  - `std_msgs`：使用标准消息类型（如 `std_msgs/String`），也是你自定义 msg 的依赖
  - `message_generation`：**关键！** 用于从 `.msg`/`.srv` 文件生成 C++/Python 代码

> 如果你不用 `message_generation`，`addint.srv` 就不会变成 `#include <demo/addint.h>`，编译会报错：“找不到 demo/addint.h”



##### 2.消息依赖

设置消息依赖，可能不同的自定义消息都有着不同的依赖，这是常见的信息格式，依赖了std_msgs，可能会有更加复杂的消息格式，依赖的可能更加复杂，可能会存在层层依赖现象，但是终归可以找到基本的依赖项

```
generate_messages(
  DEPENDENCIES
  std_msgs
)
```



详细说明：

### 🔹 第一部分：基础声明

```
cmake_minimum_required(VERSION 3.0.2)
project(demo)
```

- **作用**：
  - `cmake_minimum_required`：指定最低 CMake 版本（ROS Noetic 要求 ≥3.0.2）
  - `project(demo)`：项目名为 `demo`，后续可用 `${PROJECT_NAME}` 引用（即 `"demo"`）

> ✅ 就像 Python 的 `__name__ == "__main__"`，这是 CMake 的“入口声明”。

---

### 🔹 第二部分：查找依赖包

```
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  message_generation
)
```

- **为什么需要？**  
  你的包依赖这些 ROS 功能：
  - `roscpp`：写 C++ 节点（如 `pub_node.cpp`）
  - `rospy`：写 Python 节点（如 `pub_node.py`）
  - `std_msgs`：使用标准消息类型（如 `std_msgs/String`），也是你自定义 msg 的依赖
  - `message_generation`：**关键！** 用于从 `.msg`/`.srv` 文件生成 C++/Python 代码

> 🌰 例子：  
> 如果你不用 `message_generation`，`addint.srv` 就不会变成 `#include <demo/addint.h>`，编译会报错：“找不到 demo/addint.h”。

---

### 🔹 第三部分：声明自定义消息和服务

#### ✅ 消息（Message）

```
add_message_files(
  FILES
  person.msg
)
```

- **作用**：告诉 catkin “我要用 `msg/person.msg` 定义新消息类型”
- **前提**：必须在 `package.xml` 中声明 `<build_depend>message_generation</build_depend>`

> 📁 文件位置：`demo/msg/person.msg`  
> 内容可能像：
> 
> msg
> 
> 编辑
> 
> ```
> string name
> int32 age
> ```

#### ✅ 服务（Service）

```
add_service_files(
  FILES
  addint.srv
)
```

- **作用**：告诉 catkin “我要用 `srv/addint.srv` 定义新服务类型”
- 同样需要 `message_generation`

> 📁 文件位置：`demo/srv/addint.srv`  
> 内容：
> 
> srv
> 
> 编辑
> 
> ```
> int64 num1
> int64 num2
> ---
> int64 sum
> ```

---

### 🔹 第四部分：生成消息代码

```
generate_messages(
  DEPENDENCIES
  std_msgs
)
```

- **为什么需要？**  
  你的 `person.msg` 或 `addint.srv` 中如果用了 `std_msgs` 里的类型（比如 `string` 实际是 `std_msgs/String` 的简化），就必须在这里声明依赖。

- **效果**：  
  编译时自动生成：
  
  - C++ 头文件：`devel/include/demo/person.h`、`demo/addint.h`
  - Python 模块：`devel/lib/python3/dist-packages/demo/msg/_person.py` 等

> ⚠️ 如果你用了 `geometry_msgs/Pose`，这里就要加 `geometry_msgs`。

---

### 🔹 第五部分：声明对外依赖（给其他包用）

```
catkin_package(
  CATKIN_DEPENDS roscpp rospy std_msgs message_runtime
)
```

- **关键区别**：
  - `message_generation` → **编译时**需要（写 `.msg` 时用）
  - `message_runtime` → **运行时**需要（别人用你的 `demo::person` 时用）

> 🤔 为什么不是 `message_generation`？  
> 因为其他包只需要“使用”你生成的消息，不需要“重新生成”，所以依赖 `message_runtime`。

✅ 这行确保：当别人 `find_package(catkin REQUIRED demo)` 时，能自动找到你的消息头文件和库。

---

### 🔹 第六部分：包含头文件路径

```
include_directories(
  ${catkin_INCLUDE_DIRS}
)
```

- **作用**：让编译器能找到 ROS 头文件，比如：
  - `#include <ros/ros.h>`
  - `#include <demo/addint.h>` ← 这个就是靠 `${catkin_INCLUDE_DIRS}` 找到的！

> 💡 `${catkin_INCLUDE_DIRS}` 自动包含：
> 
> - `/opt/ros/noetic/include`
> - `devel/include`（你自己的 `demo/addint.h` 就在这里）

---

### 🔹 第七部分：编译可执行文件（核心！）

#### 1. 声明目标

```
add_executable(pub_node src/pub_node.cpp)
add_executable(sub_node src/sub_node.cpp)
add_executable(server_node src/server_node.cpp)
add_executable(client_node src/client_node.cpp)
```

- **作用**：告诉 CMake “我要从这些 `.cpp` 文件编译出可执行程序”
- **输出位置**：`devel/lib/demo/pub_node` 等

> 📌 注意：**名字 `pub_node` 就是 launch 文件中的 `type="pub_node"`**

#### 2. 设置依赖（极其重要！）

```
add_dependencies(pub_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
# ... 其他节点同理
```

- **为什么需要？**  
  确保 **先生成 `demo/addint.h`，再编译 `client_node.cpp`**。  
  否则编译 `client_node.cpp` 时会报错：“找不到 demo/addint.h”。

> 🔧 `${demo_EXPORTED_TARGETS}` = “所有由本包生成的消息/服务目标”  
> `${catkin_EXPORTED_TARGETS}` = “所有依赖包生成的消息目标”

#### 3. 链接库

```
target_link_libraries(pub_node ${catkin_LIBRARIES})
# ... 其他节点同理
```

- **作用**：链接 ROS 库（如 `libroscpp.so`），否则会报“undefined reference to ros::init”

> 📚 `${catkin_LIBRARIES}` 包含：
> 
> - `roscpp`
> - `rosconsole`
> - `rostime`
> - 以及其他你在 `find_package` 中声明的 C++ 库

---

### 🔹 第八部分：安装 Python 脚本



```
catkin_install_python(PROGRAMS
  scripts/pub_node.py
  scripts/sub_node.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
```

- **作用**：把 Python 脚本复制到 `devel/lib/demo/` 并设为可执行
- **前提**：脚本要有 `#!/usr/bin/env python3` 和 `chmod +x`

> ✅ 这样你才能 `rosrun demo pub_node.py`

---

### 🔹 补充：为什么没有 `install(TARGETS ...)`？

你注释掉了 C++ 可执行文件的 `install()`，但在 **开发阶段（catkin_make）**，可执行文件会自动放到 `devel/` 目录，`rosrun` 能直接找到。

> 📦 只有当你 `catkin_make install` 时，才需要 `install(TARGETS ...)` 把文件复制到 `install/` 目录。

---

## 🎯 总结：你的 CMakeLists.txt 为什么需要这些？

| 步骤                                     | 作用               | 不做的后果                     |
| -------------------------------------- | ---------------- | ------------------------- |
| `find_package(... message_generation)` | 支持自定义 msg/srv    | 无法生成 `demo/addint.h`      |
| `add_service_files(...)`               | 声明 `.srv` 文件     | 忽略你的服务定义                  |
| `generate_messages(...)`               | 生成 C++/Python 接口 | 编译时报“找不到头文件”              |
| `catkin_package(... message_runtime)`  | 让别人能用你的消息        | 其他包编译失败                   |
| `add_dependencies(...)`                | 确保“先生成，再编译”      | 随机编译失败（依赖顺序错）             |
| `target_link_libraries(...)`           | 链接 ROS 库         | 链接错误（undefined reference） |

---

### 💡 最佳实践建议

1. **自定义 msg/srv 时**，务必检查：
   - `CMakeLists.txt` 中 `add_*_files` + `generate_messages`
   - `package.xml` 中 `build_depend` 和 `exec_depend`
2. **只要用了自定义消息**，C++ 节点**必须加 `add_dependencies`**
3. **Python 脚本**记得 `catkin_install_python`，否则 `rosrun` 找不到
