# 动态协议
主要用作网络通信协议。

## 设计思想
动态协议乃以数据解析数据，理论上可以兼容所有数据类型。  
动态协议旨在自动生成代码，序列化与反序列化不同数据类型，包括基础类型与复合类型，甚至支持复合类型相互嵌套。  
例如C++容器之间能够相互嵌套，再如lua的table也可以层层嵌套，进而推导出任意复合类型相互嵌套。

## 实现方法
自定义类Table解决以下问题：
1. 不同容器之间接口未完全统一。对于模板而言，关键在于统一隐式接口，一种隐式接口可以实例化为多种显式接口。  
2. 不同数据类型的格式和长度不一致。就序列化与反序列化而言，关键在于定义一套数据格式，以不同解析方法兼容不同数据类型。  
3. 从语言特性层面出发，借助模板实例化与类型推导，实现自动生成代码。

## 数据类型
### 基础类型
在反序列化数据之时可以转换基础类型。  
已支持基础类型如下：
* uint8_t
* uint16_t
* uint32_t
* uint64_t
* int8_t
* int16_t
* int32_t
* int64_t
* bool
* float
* double

## 复合类型
自定义类Table与复合类型可以相互嵌套。  
已支持复合类型如下：
* vector
* deque
* list
* set
* multiset
* unordered_set
* unordered_multiset
* map
* multimap
* unordered_map
* unordered_multimap

# 作者
name: 许聪  
mailbox: solifree@qq.com  
CSDN：https://blog.csdn.net/xucongyoushan  
gitee: https://gitee.com/solifree  
github: https://github.com/SolifreeXu
