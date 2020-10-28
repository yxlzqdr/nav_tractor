### 功能
小车的导航包

### 目录
.
├── CMakeLists.txt
├── launch
│   └── move_base.launch
├── navigation_params
│   ├── base_local_planner_params.yaml
│   ├── costmap_common_params.yaml
│   ├── costmap_converter_params.yaml
│   ├── global_costmap_params.yaml
│   ├── global_planner_params.yaml
│   └── local_costmap_params.yaml
├── package.xml
└── readme.md

### 启动文件
启动了两个节点
  1.car_cmd节点
  2.move_base节点

### 配置文件
costmap_common_params.yaml--------------------------全局代价地图和局部代价地图的公共参数
global_costmap_params.yaml--------------------------全局代价地图的参数
local_costmap_params.yaml---------------------------局部代价地图的参数
costmap_converter_params.yaml-----------------------
global_planner_params.yaml--------------------------全局路径规划算法的指定（A*、D*）
base_local_planner_params.yaml----------------------机器人的配置以及一些优化配置

