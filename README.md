-   [cloud-blockchain-book](#cloud-blockchain-book)
    -   [[01\_编程语言](01_编程语言/index.md)](#编程语言)
    -   [[02\_API设计](https://raw.githubusercontent.com/heidsoft/cloud-blockchain-book/master/02_API%E8%AE%BE%E8%AE%A1/OAuth%E5%BC%80%E5%8F%91%E4%B8%8E%E9%9B%86%E6%88%90.md)](#api设计)
-   [复习记录](#复习记录)
    -   [基础层](#基础层)
        -   [网络协议](#网络协议)
            -   [dns](#dns)
            -   [http](#http)
            -   [tcp/ip/udp](#tcpipudp)
        -   [数据结构算法](#数据结构算法)
            -   [B树](#b树)
            -   [动态划归](#动态划归)
                -   [数据库管理](#数据库管理)
                -   [io与nio](#io与nio)
                -   [线程](#线程)
                -   [异常处理](#异常处理)
                -   [设计模式](#设计模式)
                -   [jvm](#jvm)
                -   [重构](#重构)
                -   [领域驱动](#领域驱动)
                -   [并发控制](#并发控制)
                -   [异步与响应式](#异步与响应式)
                -   [系统架构](#系统架构)
-   [应用层](#应用层)
    -   [日志监控](#日志监控)
    -   [链路监控](#链路监控)
    -   [性能监控](#性能监控)
-   [平台层](#平台层)
    -   [service mesh](#service-mesh)
    -   [k8s](#k8s)
        -   [应用部署与架构](#应用部署与架构)
        -   [网络组件](#网络组件)
        -   [存储组件](#存储组件)

cloud-blockchain-book
=====================

[01\_编程语言](01_编程语言/index.md)
------------------------------------

[02\_API设计](https://raw.githubusercontent.com/heidsoft/cloud-blockchain-book/master/02_API%E8%AE%BE%E8%AE%A1/OAuth%E5%BC%80%E5%8F%91%E4%B8%8E%E9%9B%86%E6%88%90.md)
---------------------------------------------------------------------------------------------------------------------------------------------------------------------

复习记录
========

基础层
------

### 网络协议
```
四次挥手
滑动窗口
```

#### dns
```
dns 类型
```

* [dns原理入门]（http://www.ruanyifeng.com/blog/2016/06/dns.html） 

#### http

#### tcp
* [How to Adjust the TCP Window Size Limit on Linux](https://netbeez.net/blog/tcp-window-size/)

### 数据结构算法

#### 平衡二叉树
```
平衡二叉树是基于二分法的策略提高数据的查找速度的二叉树的数据结构；
```
#### B树
```

```

#### 动态划归


##### io与nio

##### 线程

* [Java 8 Concurrency Tutorial: Synchronization and Locks](https://winterbe.com/posts/2015/04/30/java8-concurrency-tutorial-synchronized-locks-examples/)
* [Java 8 Concurrency Tutorial: Threads and Executors](https://winterbe.com/posts/2015/04/07/java8-concurrency-tutorial-thread-executor-examples/)
* [Java 8 Concurrency Tutorial: Atomic Variables and ConcurrentMap](https://winterbe.com/posts/2015/05/22/java8-concurrency-tutorial-atomic-concurrent-map-examples/)

##### 异常处理

##### 设计模式

##### jvm

###### 栈与堆

    栈是运行时的单位，而堆是存储的单位。
    栈解决程序的运行问题，即程序如何执行，或者说如何处理数据；堆解决的是数据存储的问题，即数据怎么放、放在哪儿。

##### 重构

##### 领域驱动

##### 并发控制

##### 异步与响应式

##### 系统架构

###### spring boot

###### spring cloud

###### elasticsearch

###### kafka

###### redis

###### flink

###### cloudera

###### hive

###### flume

###### cassandra

###### hbase

###### airflow

应用层
======

日志监控
--------

链路监控
--------

性能监控
--------

平台层
======

service mesh
------------

k8s
---

### 应用部署与架构

### 网络组件

### 存储组件
