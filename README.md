# iLinkView v1.0.0

嵌入式网络可视化设备软件，基于 CivetWeb + SNMPv3，提供网络拓扑可视化与设备信息采集。

## 功能
- HTTP REST API 提供拓扑数据
- SNMPv3 数据采集
- 并发 Worker Pool
- 请求 Rate-Limiting
- 模块化设计，底层 C，上层 Modern C++

## 构建
```bash
mkdir build && cd build
cmake ..
make -j
./app

