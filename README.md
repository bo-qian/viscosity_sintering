<!--
 * @Author: Bo Qian
 * @Date: 2025-02-11 17:10:06
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2025-03-18 20:05:44
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @Description: Readme
 * @FilePath: /viscosity_sintering/README.md
-->

# Viscosity Sintering Project

## 项目简介

本项目实现了粘性烧结过程的模拟，基于MOOSE框架开发。包含了Stokes方程和Cahn-Hilliard方程的求解。

## 环境配置

### 安装MOOSE

1. 克隆MOOSE仓库：
    ```bash
    git clone https://github.com/idaholab/moose.git
    cd moose
    ```

2. 安装依赖：
    ```bash
    ./scripts/update_and_rebuild_libmesh.sh
    ```

3. 配置环境变量：
    ```bash
    source moose/env.sh
    ```

### 配置项目

1. 克隆本项目仓库：
    ```bash
    git clone https://github.com/yourusername/viscosity_sintering.git
    cd viscosity_sintering
    ```

2. 编译项目：
    ```bash
    make -j4
    ```

## 运行程序

### 运行示例

1. 进入项目目录：
    ```bash
    cd /home/qianbo/projects/viscosity_sintering
    ```

2. 运行示例输入文件：
    ```bash
    ./viscosity_sintering-opt -i input_file.i
    ```

### 输入文件说明

输入文件包含了模拟所需的所有参数和设置。以下是一个简单的示例：

```text
[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 50
  ny = 50
[]

[Variables]
  [./c]
    initial_condition = 0.5
  [../]
  [./x_velocity]
    initial_condition = 0.0
  [../]
  [./y_velocity]
    initial_condition = 0.0
  [../]
  [./pressure]
    initial_condition = 0.0
  [../]
[]

[Kernels]
  [./stokes_x]
    type = StokesX
    dim = 2
    x_velocity = x_velocity
    y_velocity = y_velocity
    pressure = pressure
    phase_field = c
  [../]
  [./stokes_y]
    type = StokesY
    dim = 2
    x_velocity = x_velocity
    y_velocity = y_velocity
    pressure = pressure
    phase_field = c
  [../]
  [./stokes_z]
    type = StokesZ
    x_velocity = x_velocity
    y_velocity = y_velocity
    pressure = pressure
    phase_field = c
  [../]
[]

[Materials]
  [./stokes_material]
    type = StokesMaterial
    cvar = c
    x_velocity = x_velocity
    y_velocity = y_velocity
    pressure = pressure
  [../]
  [./viscosity_sintering_material]
    type = ViscositySinteringMaterial
    cvar = c
    x_velocity = x_velocity
    y_velocity = y_velocity
    pressure = pressure
  [../]
[]

[Executioner]
  type = Transient
  scheme = bdf2
  dt = 1e-3
  end_time = 1.0
[]

[Outputs]
  exodus = true
[]
```

## 贡献指南

欢迎对本项目进行贡献！请提交Pull Request或Issue。

## 联系方式

如有任何问题，请联系Bo Qian (bqian@shu.edu.cn)。

---

## 原有内容

viscosity_sintering
=====

Fork "viscosity_sintering" to create a new MOOSE-based application.

For more information see: [https://mooseframework.org/getting_started/new_users.html#create-an-app](https://mooseframework.org/getting_started/new_users.html#create-an-app)

Debug code by using valgrind, 使用 `tee` 同时输出到终端和文件：

```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./viscosity_sintering-opt -i viscosity_sintering_IC_2D.i 2>&1 | tee valgrind_output.txt
```

- `tee valgrind_output.txt` 会将输出写入到 `valgrind_output.txt` 文件，同时还会将它显示在终端上。