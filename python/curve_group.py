'''
Author: Bo Qian
Date: 2025-02-26 13:53:09
Email: bqian@shu.edu.cn
Location: Shanghai University
LastEditTime: 2025-03-03 19:23:46
LastEditors: Bo Qian
Description: plotting program of viscosity sintering MOOSE program
FilePath: /viscosity_sintering/python/curve_group.py
'''

import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec
import numpy as np
import os
import pandas as pd
from matplotlib.gridspec import GridSpec
from matplotlib.pyplot import title
from matplotlib.tri import Triangulation
from mpl_toolkits.axes_grid1 import make_axes_locatable, axes_size
import matplotlib.cm as cm

plt.rcParams.update({
    'font.family': 'serif',
    'font.serif': ['Times New Roman'],
    'font.size': 32,
    'mathtext.fontset': 'custom',
    'mathtext.rm': 'Times New Roman',
    'mathtext.it': 'Times New Roman:italic',
    'mathtext.bf': 'Times New Roman:bold',
    'axes.unicode_minus': False
})

# plt.rcParams.update({
#     'text.usetex': True,  # 启用 LaTeX
#     'font.family': 'serif',  # 使用衬线字体
#     'font.serif': 'Times New Roman',  # 指定 Times New Roman
#     'font.size': 32,  # 全局字体大小
#     'legend.fontsize': 28,  # 图例字体大小
#     'legend.fontfamily': 'serif',  # 图例字体族
#     'legend.fontstyle': 'normal',  # 图例字体样式
#     'axes.linewidth': 3,  # 坐标轴线宽
#     'xtick.major.width': 3,  # X轴主刻度线宽
#     'ytick.major.width': 3  # Y轴主刻度线宽
# })

base_path = "/home/qianbo/projects/viscosity_sintering"
base_path_wsl = "/mnt/d/OneDrive/Science_Research/2.Secondstage_CodeAndData_Python/Viscosity Sintering/viscosity_sintering"
time_step = 360


def curve_comparison_two(path, label, x_1, y_1, time_step):
    # Function to plot curve group ********************************************************************************
    file_directory = os.getcwd() + "/python/Comparison of Viscosity Sintering"
    line_width = 4
    if not os.path.exists(file_directory):
        os.makedirs(file_directory)

    with plt.rc_context(
            {'font.family': 'serif', 'font.serif': ['Times New Roman'], 'font.weight': 'bold', 'font.size': 32}):
        fig, ax = plt.subplots(figsize=(12, 9), dpi=100)
        ax.spines['top'].set_linewidth(3)
        ax.spines['bottom'].set_linewidth(3)
        ax.spines['left'].set_linewidth(3)
        ax.spines['right'].set_linewidth(3)
        plt.tick_params(axis='both', direction='in', width=3, which='both', pad=10)  # 设置刻度朝内，边框厚度为 2

        data_csv = [None for _ in path]
        for i in range(len(path)):
            data_csv[i] = pd.read_csv(path[i])
            # 获取标题行
            title = data_csv[i].columns
            # 提取数据列
            data_x_1 = data_csv[i].iloc[:time_step, x_1]
            data_y_1 = data_csv[i].iloc[:time_step, y_1]
            ax.plot(data_x_1, data_y_1, label=fr'{label[i]}', linewidth=3)
            print(f"Data_x: {data_x_1}, Data_y: {data_y_1}")

        offset = ax.yaxis.get_offset_text()
        transform = offset.get_transform()
        offset.set_transform(transform + plt.matplotlib.transforms.ScaledTranslation(0, 5 / 72., fig.dpi_scale_trans))
        plt.title(f'Comparison of {title[y_1]}', pad=20, fontweight='bold')
        plt.xlabel(f'{title[x_1]}', fontweight='bold')
        plt.ylabel(f'{title[y_1]}', fontweight='bold')
        plt.tight_layout()
        plt.legend(fontsize='small')
        plt.savefig(file_directory + f'/Comparison of {title[y_1]}.png', dpi=100, bbox_inches='tight')
        plt.close()

# curve_comparison_two(
#     [
#         base_path + "/viscosity_sintering_2D_out.csv",
#         base_path + "/various_data_without stablizer.csv"
#     ],
#     ["MOOSE", "FEniCS"],
#     0,
#     1,
#     time_step+1
# )


def plot_curve_group_comparison(end_curve, path, label, name):
    file_directory = os.getcwd() + "/python/Curve Group Comparison of Viscosity Sintering"
    line_width = 4
    if not os.path.exists(file_directory):
        os.makedirs(file_directory)

    markers = ['o', 's', '^', 'D', 'v']
    data_csv = [None for _ in path]
    with plt.rc_context(
            {'font.family': 'serif', 'font.serif': ['Times New Roman'], 'font.weight': 'bold', 'font.size': 32}):
        fig = plt.figure(figsize=(24, 18), dpi=100)
        gs = GridSpec(2, 2, figure=fig)
        axes = [fig.add_subplot(gs[i, j]) for i in range(2) for j in range(2)]

        for i, ax in enumerate(axes):
            ax.spines['top'].set_linewidth(line_width)
            ax.spines['bottom'].set_linewidth(line_width)
            ax.spines['left'].set_linewidth(line_width)
            ax.spines['right'].set_linewidth(line_width)
            ax.tick_params(axis='both', direction='in', length=10, width=line_width, which='both', pad=10)

            for j in range(len(path)):
                data_csv[j] = pd.read_csv(path[j], skiprows=1)
                header = pd.read_csv(path[j], nrows=2, header=None)
                data_curve = data_csv[j].values
                name_unit = header.values

                ax.plot(data_curve[:end_curve, 0], data_curve[:end_curve, i + 1], label=rf"{label[j]}",
                        linewidth=3)  # """marker=markers[j % len(markers)], markevery=45 + 15 * j, markersize=8"""
                ax.ticklabel_format(style='sci', axis='y', scilimits=(0, 0))

            ax.set_title(f'{name_unit[0, i + 1]}', pad=20, fontweight='bold')
            ax.set_xlabel(rf'{name_unit[0, 0]} ($\mathrm{{{name_unit[1, 0]}}}$)', fontweight='bold')
            ax.set_ylabel(rf'{name_unit[0, i + 1]} ($\mathrm{{{name_unit[1, i + 1]}}}$)', fontweight='bold')

            offset = ax.yaxis.get_offset_text()
            transform = offset.get_transform()
            offset.set_transform(
                transform + plt.matplotlib.transforms.ScaledTranslation(0, 5 / 72., fig.dpi_scale_trans))
            ax.legend(fontsize='small')

        plt.tight_layout()
        plt.savefig(file_directory + f'/{name}.png', dpi=100, bbox_inches='tight')
        plt.close()

# plot_curve_group_comparison(
#     454,
#     [
#         base_path + "/viscosity_sintering_2D_out.csv",
#         base_path + "/various_data.csv"
#     ],
#     ["MOOSE", "FEniCS"],
#     "Two Particle Viscosity Sintering"
# )


# Function to plot curve group ********************************************************************************
def plot_curve_group(name, Data_directory):
    file_directory = os.getcwd() + "/Curve Group of Solid-state Sintering"
    line_width = 4
    if not os.path.exists(file_directory):
        os.makedirs(file_directory)

    data = pd.read_csv(Data_directory, skiprows=2)
    header = pd.read_csv(Data_directory, nrows=2, header=None)
    data_curve = data.iloc
    name_unit = header.iloc

    with plt.rc_context(
            {'font.family': 'serif', 'font.serif': ['Times New Roman'], 'font.weight': 'bold', 'font.size': 32}):
        fig = plt.figure(figsize=(24, 18), dpi=100)
        gs = GridSpec(2, 2, figure=fig)
        axes = [fig.add_subplot(gs[i, j]) for i in range(2) for j in range(2)]
        for i, ax in enumerate(axes):
            ax.spines['top'].set_linewidth(line_width)
            ax.spines['bottom'].set_linewidth(line_width)
            ax.spines['left'].set_linewidth(line_width)
            ax.spines['right'].set_linewidth(line_width)

            ax.tick_params(axis='both', direction='in', length=10, width=line_width, which='both', pad=10)
            ax.plot(data_curve[:, 0], data_curve[:, i + 1], label=f"{name_unit[0, i + 1]}", linewidth=line_width,
                    color='black')
            ax.ticklabel_format(style='sci', axis='y', scilimits=(0, 0))

            ax.set_title(name_unit[0, i + 1], pad=20, fontweight='bold')
            ax.set_xlabel(rf'{name_unit[0, 0]} ($\mathrm{{{name_unit[1, 0]}}}$)', fontweight='bold')
            ax.set_ylabel(rf'{name_unit[0, i + 1]} ($\mathrm{{{name_unit[1, i + 1]}}}$)', fontweight='bold')

            offset = ax.yaxis.get_offset_text()
            transform = offset.get_transform()
            offset.set_transform(
                transform + plt.matplotlib.transforms.ScaledTranslation(0, 5 / 72., fig.dpi_scale_trans))

        plt.tight_layout()
        plt.savefig(file_directory + f'/{name}.png', dpi=100, bbox_inches='tight')
        plt.close()


def plot_curve(x, y, name, Data_directory, time_step):
    file_directory = os.getcwd() + f"/python/Curve of {name}"
    print(f"file directory: {file_directory}")
    if not os.path.exists(file_directory):
        os.makedirs(file_directory)
    with plt.rc_context(
            {'font.family': 'serif', 'font.serif': ['Times New Roman'], 'font.weight': 'bold', 'font.size': 32}):
        fig, ax = plt.subplots(figsize=(12, 9), dpi=100)
        ax.spines['top'].set_linewidth(3)
        ax.spines['bottom'].set_linewidth(3)
        ax.spines['left'].set_linewidth(3)
        ax.spines['right'].set_linewidth(3)
        plt.tick_params(axis='both', direction='in', width=3, which='both', pad=10)  # 设置刻度朝内，边框厚度为 2

        data = pd.read_csv(Data_directory, skiprows=1, header=None)
        header = pd.read_csv(Data_directory, nrows=1, header=None)
        data_curve = data.iloc
        name_unit = header.iloc

        print(f"Data_x: {data_curve[:, x]}")
        print(f"Data_y: {data_curve[:, y]}")

        name_custom = ['Time', 'Total Free Energy']
        # 提取数据列

        plt.plot(data_curve[:time_step, x], data_curve[:time_step, y], label=fr'MOOSE', linewidth=3, color='black')

        offset = ax.yaxis.get_offset_text()
        transform = offset.get_transform()
        offset.set_transform(transform + plt.matplotlib.transforms.ScaledTranslation(0, 5 / 72., fig.dpi_scale_trans))
        # plt.title(f"{name_unit[0, y]}", pad=20, fontweight='bold')
        # plt.xlabel(f'{name_unit[0, x]} ', fontweight='bold')
        # plt.ylabel(f'{name_unit[0, y]} ', fontweight='bold')
        plt.title(f"{name_custom[1]}", pad=20, fontweight='bold')
        plt.xlabel(f'{name_custom[0]} ', fontweight='bold')
        plt.ylabel(f'{name_custom[1]} ', fontweight='bold')
        plt.tight_layout()
        plt.legend(fontsize='small')
        plt.savefig(file_directory + f'/{name} - {name_custom[1]}.png', dpi=100, bbox_inches='tight')
        plt.close()


plot_curve(0, 1, "Viscosity Sintering of ABS", base_path_wsl + "/outputs/4135/4135_VS2D.csv", 19+1)


def plot_relative_error(path, data_file_FEniCS, data_file_MOOSE, name, time_step):

    data_FEniCS = pd.read_csv(path[0], skiprows=0, header=0)  # 跳过第一行
    data_MOOSE = pd.read_csv(path[1], skiprows=1, header=None)  # 跳过第一行

    # 假设数据文件中，第一列是时间，第二列是相应的数据值
    time = data_FEniCS.iloc[:time_step, data_file_FEniCS[0]]  # 取出时间列
    values_FEniCS = data_FEniCS.iloc[:time_step, data_file_FEniCS[1]]  # 取出FEniCS的数据
    values_MOOSE = data_MOOSE.iloc[:time_step, data_file_MOOSE[1]]  # 取出MOOSE的数据

    print(f"time: {time}")
    print(f"data_FEniCS: {values_FEniCS}")
    print(f"data_MOOSE: {values_MOOSE}")

    # 计算相对误差
    relative_error = np.abs(values_FEniCS - values_MOOSE) / np.abs(values_FEniCS) * 100

    # 创建保存结果的文件夹
    file_directory = os.getcwd() + f"/python/Relative_Error_{name}"
    print(f"file directory: {file_directory}")
    if not os.path.exists(file_directory):
        os.makedirs(file_directory)

    # 使用matplotlib绘图
    with plt.rc_context(
            {'font.family': 'serif', 'font.serif': ['Times New Roman'], 'font.weight': 'bold', 'font.size': 32}):
        fig, ax = plt.subplots(figsize=(12, 9), dpi=100)

        ax.spines['top'].set_linewidth(3)
        ax.spines['bottom'].set_linewidth(3)
        ax.spines['left'].set_linewidth(3)
        ax.spines['right'].set_linewidth(3)
        plt.tick_params(axis='both', direction='in', width=3, which='both', pad=10)  # 设置刻度朝内，边框厚度为 2

        # 绘制相对误差曲线
        ax.plot(time, relative_error, label=f'Relative Error ({name})', linewidth=3, color='black')

        # 设置标题和标签
        ax.set_title(f'Relative Error for {name}', pad=20, fontweight='bold')
        ax.set_xlabel('Time', fontweight='bold')
        ax.set_ylabel('Relative Error (%)', fontweight='bold')

        # ax.yaxis.set_major_formatter(plt.FuncFormatter(lambda x, _: f'{x:.2f}%'))

        # 保存图像
        plt.tight_layout()
        plt.savefig(file_directory + f'/{name}_Relative_Error.png', dpi=100, bbox_inches='tight')
        plt.close()

# plot_relative_error(
#     [
#         base_path + "/various_data.csv",
#         base_path + "/viscosity_sintering_2D_out.csv"

#     ],
#     [0, 1],
#     [0, 1],
#     "MOOSE_FEniCS",
#     time_step+1
# )

# 定义函数 c_0
def c_0(x, y):
    term1 = 1 / 2 * (1 - np.tanh(
        (np.sqrt((x - 45) ** 2 + (y - 75) ** 2) - 15) * (2 * np.arctanh(0.9) / 3)))
    term2 = 1 / 2 * (1 - np.tanh(
        (np.sqrt((x - 75) ** 2 + (y - 75) ** 2) - 15) * (2 * np.arctanh(0.9) / 3)))
    term3 = 1 / 2 * (1 - np.tanh(
        (np.sqrt((x - 75) ** 2 + (y - 45) ** 2) - 15) * (2 * np.arctanh(0.9) / 3)))
    term4 = 1 / 2 * (1 - np.tanh(
        (np.sqrt((x - 45) ** 2 + (y - 45) ** 2) - 15) * (2 * np.arctanh(0.9) / 3)))
    return term1 + term2 + term3 + term4

def voronoi(dx, dy, dpx, dpy):
    dp = np.sqrt(dpx ** 2 + dpy ** 2)
    exper = (1 + np.tanh((0.5 * pow(dp, 2) - dx * dpx - dy * dpy) / (np.sqrt(2) * 1 * dp))) / 2
    return exper

def voronoi_1(center, x, y):
    dx = x - center[0]
    dy = y - center[1]

    # 定义分段函数的各部分
    dpx1, dpy1 = 45 - center[0], 45 - center[1]
    dpx2, dpy2 = 75 - center[0], 75 - center[1]
    dpx3, dpy3 = 75 - center[0], 45 - center[1]

    dp1 = np.sqrt(dpx1 ** 2 + dpy1 ** 2)
    dp2 = np.sqrt(dpx2 ** 2 + dpy2 ** 2)
    dp3 = np.sqrt(dpx3 ** 2 + dpy3 ** 2)

    expr1 = (1 + np.tanh((0.5 * dp1 ** 2 - dx * dpx1 - dy * dpy1) * (2 * np.arctanh(0.9) / (3 * dp1)))) / 2
    expr2 = (1 + np.tanh((0.5 * dp2 ** 2 - dx * dpx2 - dy * dpy2) * (2 * np.arctanh(0.9) / (3 * dp2)))) / 2
    expr3 = (1 + np.tanh((0.5 * dp3 ** 2 - dx * dpx3 - dy * dpy3) * (2 * np.arctanh(0.9) / (3 * dp3)))) / 2

    # 使用 np.where 实现分段函数
    exper = np.where((x < 60) & (x + y < 120), expr1, np.where((y > 60) & (x + y > 120), expr2, expr3))

    return exper


def voronoi_2(center, x, y):
    dx = x - center[0]
    dy = y - center[1]

    # 定义分段函数的各部分
    dpx1, dpy1 = 45 - center[0], 45 - center[1]
    dpx2, dpy2 = 45 - center[0], 75 - center[1]
    dpx3, dpy3 = 75 - center[0], 45 - center[1]

    dp1 = np.sqrt(dpx1 ** 2 + dpy1 ** 2)
    dp2 = np.sqrt(dpx2 ** 2 + dpy2 ** 2)
    dp3 = np.sqrt(dpx3 ** 2 + dpy3 ** 2)

    expr1 = (1 + np.tanh((0.5 * dp1 ** 2 - dx * dpx1 - dy * dpy1) * (2 * np.arctanh(0.9) / (3 * dp1)))) / 2
    expr2 = (1 + np.tanh((0.5 * dp2 ** 2 - dx * dpx2 - dy * dpy2) * (2 * np.arctanh(0.9) / (3 * dp2)))) / 2
    expr3 = (1 + np.tanh((0.5 * dp3 ** 2 - dx * dpx3 - dy * dpy3) * (2 * np.arctanh(0.9) / (3 * dp3)))) / 2

    # 使用 np.where 实现分段函数
    exper = np.where((x < 60) & (y < 60), expr1, np.where((y > 60) & (y > x), expr2, expr3))

    return exper


def voronoi_3(center, x, y):
    dx = x - center[0]
    dy = y - center[1]

    # 定义分段函数的各部分
    dpx1, dpy1 = 45 - center[0], 45 - center[1]
    dpx2, dpy2 = 75 - center[0], 75 - center[1]
    dpx3, dpy3 = 45 - center[0], 75 - center[1]

    dp1 = np.sqrt(dpx1 ** 2 + dpy1 ** 2)
    dp2 = np.sqrt(dpx2 ** 2 + dpy2 ** 2)
    dp3 = np.sqrt(dpx3 ** 2 + dpy3 ** 2)

    expr1 = (1 + np.tanh((0.5 * dp1 ** 2 - dx * dpx1 - dy * dpy1) * (2 * np.arctanh(0.9) / (3 * dp1)))) / 2
    expr2 = (1 + np.tanh((0.5 * dp2 ** 2 - dx * dpx2 - dy * dpy2) * (2 * np.arctanh(0.9) / (3 * dp2)))) / 2
    expr3 = (1 + np.tanh((0.5 * dp3 ** 2 - dx * dpx3 - dy * dpy3) * (2 * np.arctanh(0.9) / (3 * dp3)))) / 2

    # 使用 np.where 实现分段函数
    exper = np.where((y < 60) & (x + y < 120), expr1, np.where((x > 60) & (x + y > 120), expr2, expr3))

    return exper

def voronoi_4(center, x, y):
    dx = x - center[0]
    dy = y - center[1]

    # 定义分段函数的各部分
    dpx1, dpy1 = 45 - center[0], 75 - center[1]
    dpx2, dpy2 = 75 - center[0], 75 - center[1]
    dpx3, dpy3 = 75 - center[0], 45 - center[1]

    dp1 = np.sqrt(dpx1 ** 2 + dpy1 ** 2)
    dp2 = np.sqrt(dpx2 ** 2 + dpy2 ** 2)
    dp3 = np.sqrt(dpx3 ** 2 + dpy3 ** 2)

    expr1 = (1 + np.tanh((0.5 * dp1 ** 2 - dx * dpx1 - dy * dpy1) * (2 * np.arctanh(0.9) / (3 * dp1)))) / 2
    expr2 = (1 + np.tanh((0.5 * dp2 ** 2 - dx * dpx2 - dy * dpy2) * (2 * np.arctanh(0.9) / (3 * dp2)))) / 2
    expr3 = (1 + np.tanh((0.5 * dp3 ** 2 - dx * dpx3 - dy * dpy3) * (2 * np.arctanh(0.9) / (3 * dp3)))) / 2

    # 使用 np.where 实现分段函数
    exper = np.where((x < 60) & (y > x), expr1, np.where((y > 60) & (x > 60), expr2, expr3))

    return exper

def plot_three_dimension(z_value, name):
    file_directory = os.getcwd() + "/Figures of Three Dimension"
    if not os.path.exists(file_directory):
        os.makedirs(file_directory)
    with plt.rc_context(
            {'font.family': 'serif', 'font.serif': ['Times New Roman'], 'font.weight': 'bold', 'font.size': 32}):
        domain = [120, 120]
        fig, ax = plt.subplots(figsize=(12, 9), dpi=100)
        x = np.linspace(0, domain[0], domain[0] * 20)
        y = np.linspace(0, domain[1], domain[1] * 20)
        x, y = np.meshgrid(x, y)

        # 计算 z 值
        # z = (1 - np.tanh((np.sqrt((x - center[0]) ** 2 + (y - center[1]) ** 2) - r) / (np.sqrt(2) * epsilon))) / 2
        # z = c_0(x, y)
        # z = voronoi(x - 45, y - 75, dpxy[0], dpxy[1])
        z_1 = voronoi_1([45, 75], x, y)
        z_2 = voronoi_2([75, 75], x, y)
        z_3 = voronoi_3([75, 45], x, y)
        z_4 = voronoi_4([45, 45], x, y)

        text_coordinate = [[30, 90], [90, 90], [90, 30], [30, 30]]

        if z_value == 1:
            z = z_1
            text_coord = text_coordinate[0]
        elif z_value == 2:
            z = z_2
            text_coord = text_coordinate[1]
        elif z_value == 3:
            z = z_3
            text_coord = text_coordinate[2]
        elif z_value == 4:
            z = z_4
            text_coord = text_coordinate[3]
        elif z_value == 5:
            z0 = z_1 ** 2 + z_2 ** 2 + z_3 ** 2 + z_4 ** 2
            c0 = c_0(x, y)
            z = z0 * c0

        else:
            z = z_1 ** 2 + z_2 ** 2 + z_3 ** 2 + z_4 ** 2
        # z = z_value
        # 创建 3D 图


        # 使用 imshow 函数绘制二维颜色图
        tric = ax.imshow(z, extent=[0, domain[0], 0, domain[1]], origin='lower', cmap='coolwarm', aspect='auto')
        if z_value is None:
            ax.set_title(rf"Visualization of $\eta$", pad=20, fontweight='bold', fontsize=32)
        elif z_value == 5:
            ax.set_title(rf"Visualization of $\eta*$", pad=20, fontweight='bold', fontsize=32)
        else:
            ax.set_title(rf"Visualization of $\eta_{{{z_value}}}$", pad=20, fontweight='bold', fontsize=32)



        coordinates = [[45, 75], [75, 75], [75, 45], [45, 45]]
        radii = [15, 15, 15, 15]
        circle = []

        # print(text_coord)
        if z_value is None:
            for i in range(len(text_coordinate)):
                plt.text(text_coordinate[i][0], text_coordinate[i][1], rf"$\eta_{{{i + 1}}}$", fontsize=32,
                         ha='center', va='center', color='white', zorder=3)
        elif z_value == 5:
            for i in range(len(coordinates)):
                plt.text(coordinates[i][0], coordinates[i][1], rf"$\eta_{{{i + 1}}}$", fontsize=32, ha='center',
                         va = 'center', color = 'white', zorder = 3)
        else:
            plt.text(text_coord[0], text_coord[1], rf"$\eta_{{{z_value}}}$", fontsize=32, ha='center', va='center',
                     color='white', zorder=3)

        # for i in range(len(coordinates)):
        #     circle.append(plt.Circle((coordinates[i][0], coordinates[i][1]), radii[i], edgecolor='white',fill=False, facecolor='none', linewidth=2, linestyle='--', zorder=2))
        #     ax.add_artist(circle[i])
        # if z_value <= 4:
        #     plt.text(coordinates[z_value - 1][0], coordinates[z_value - 1][1], rf"$\eta_{{{z_value}}}$", fontsize=32, ha='center', va='center', color='white', zorder=3)

        # 添加颜色条
        # cbar = fig.colorbar(cax, ax=ax, orientation='vertical', shrink=0.76)
        # cbar.set_label(r'$c_0$')
        ax.set_aspect('equal', adjustable='box')
        divider = make_axes_locatable(plt.gca())
        width = axes_size.AxesY(ax, aspect=1. / 20)
        pad = axes_size.Fraction(0.5, width)
        cax = divider.append_axes("right", size=width, pad=pad)
        cbar = plt.colorbar(tric, cax=cax)
        # 设置颜色条的刻度
        # ticks = np.linspace(vmin, vmax, 6)  # 在vmin和vmax之间生成6个均匀分布的值
        # cbar.set_ticks(ticks)

        # 设置轴标签
        ax.set_xlabel('X-axis', fontweight='bold')
        ax.set_ylabel('Y-axis', fontweight='bold')

        # 调整边距
        plt.tight_layout()

        # 保存图形

        plt.savefig(file_directory + f'/{name}_3D.png', dpi=100, bbox_inches='tight')
        plt.close()

# plot_three_dimension(1, f"eta_1")
# plot_three_dimension(2, f"eta_2")
# plot_three_dimension(3, f"eta_3")
# plot_three_dimension(4, f"eta_4")
# plot_three_dimension(None, f"eta")
# plot_three_dimension(5, f"eta^*")


def plot_expression(r_epsilon, name):
    file_directory = os.getcwd() + "/Figures of Two Dimension"
    if not os.path.exists(file_directory):
        os.makedirs(file_directory)

    with plt.rc_context(
            {'font.family': 'serif', 'font.serif': ['Times New Roman'], 'font.weight': 'bold', 'font.size': 32}):
        fig, ax = plt.subplots(figsize=(12, 9), dpi=100)
        ax.spines['top'].set_linewidth(3)
        ax.spines['bottom'].set_linewidth(3)
        ax.spines['left'].set_linewidth(3)
        ax.spines['right'].set_linewidth(3)

        plt.tick_params(axis='both', direction='in', width=3, which='both', pad=10)

        print(f"当d=3，w=0.05时，\ne={3 / (2 * np.sqrt(2) * np.arctanh(1 - 0.1))}\n")

        print(f"当d=3，e=1时，\nw={(1 - np.tanh(3 / (2 * np.sqrt(2)))) / 2}\n")

        print(f"当e=1，w=1e-3时，\nd={2 * np.sqrt(2) * np.arctanh(1 - 2e-3)}\n")

        for i in range(len(r_epsilon)):
            r = r_epsilon[i][0]
            e = r_epsilon[i][1]
            d = r_epsilon[i][2]
            w = r_epsilon[i][3]
            w1 = 1e-3

            x = np.linspace(r - d / 2 - 2, r + d / 2 + 2, 400)
            x_1 = np.linspace(-15, 15, 4000)
            y = (1 - np.tanh((x - r) / (np.sqrt(2) * e))) / 2
            y_1 = (1 - np.tanh(x_1 * (2 * np.arctanh(1 - 2 * w1))/ d)) / 2
            y_2 = (1 - np.tanh(x_1 / (np.sqrt(2) * e))) / 2



            # plt.plot(x, y, linewidth=3,
            #          label=rf"$y=\frac{{1}}{{2}}\left[1 - \tan\left(\frac{{x - {r}}}{{\sqrt{{2}} \cdot {e}}}\right)\right]$")
            # plt.plot(x_1, y_1, linewidth=3, label=f"$r_0={r}$   $\\epsilon={e}$   $\\delta={d}$")
            plt.plot(x_1, y_2, linewidth=3, label=f"$r_0={r}$   $\\epsilon={e}$   $\\delta={d}$")
            plt.fill_between([np.sqrt(2) * e * np.arctanh(1 - 2 * (1 - w1)), np.sqrt(2) * e * np.arctanh(1 - 2 * w1)], 0, 1, color='lightgreen', alpha=0.5)
            plt.axvline(x=np.sqrt(2) * e * np.arctanh(1 - 2 * (1 - w1)), color='b', linestyle='--', linewidth=3)
            plt.text(np.sqrt(2) * e * np.arctanh(1 - 2 * (1 - w1)) - 9, 0.5, f"$x_1=\\sqrt{{2}}\\epsilon\\cdot\\mathrm{{arctanh}}(1-2(1-\\omega))$", fontsize=20, color='b', ha='left', va='center')
            plt.axvline(x=np.sqrt(2) * e * np.arctanh(1 - 2 * w1), color='r', linestyle='--', linewidth=3)
            plt.text(np.sqrt(2) * e * np.arctanh(1 - 2 * w1) + 0.5, 0.5, f"$x_2=\\sqrt{{2}}\\epsilon\\cdot\\mathrm{{arctanh}}(1-2\\omega)$", fontsize=20,
                     color='r', ha='left', va='center')

        plt.legend(fontsize='small')
        plt.xlabel(r'$x$', fontweight='bold')
        plt.ylabel(r'$y$', fontweight='bold')
        plt.grid(True)
        # plt.title(rf"$y=\frac{{1}}{{2}}\left[1 - \tanh\left(\frac{{r - r_0}}{{\sqrt{{2}} \cdot \epsilon}}\right)\right]$", pad=40, fontweight='normal')
        plt.tight_layout()
    plt.savefig(file_directory + f'/{name}.png', dpi=100, bbox_inches='tight')
    plt.close()


# plot_expression([(25, 0.7, 3),(25, 0.5, 2),(25, 0.2, 1)], "expression_2D")
# plot_expression([ (25, 1, 3, 1e-3)], "expression_2D")



def initial_schematic(coordinates, radii, domain):
    file_directory = os.getcwd() + "/Initial Schematic"
    if not os.path.exists(file_directory):
        os.makedirs(file_directory)
    with plt.rc_context(
            {'font.family': 'serif',
             'font.serif': ['Times New Roman'],
             'font.weight': 'bold',
             'font.size': 32}
    ):
        fig, ax = plt.subplots(figsize=(12, 9), dpi=100)
        ax.spines['top'].set_linewidth(3)
        ax.spines['bottom'].set_linewidth(3)
        ax.spines['left'].set_linewidth(3)
        ax.spines['right'].set_linewidth(3)

        ax.set_aspect('equal', 'box')
        ax.set_xlim(0, domain[0])
        ax.set_ylim(0, domain[1])


        circle = []
        for i in range(len(coordinates)):
            circle.append(plt.Circle((coordinates[i][0], coordinates[i][1]), radii[i], edgecolor='black',fill=True, facecolor='white', linewidth=3, zorder=2))
            plt.text(coordinates[i][0], coordinates[i][1], rf"$\eta_{{{i + 1}}}$", fontsize=32, ha='center', va='center', zorder=3)
            ax.add_artist(circle[i])

        ax.grid(True, linestyle='--', linewidth=3, zorder=1)
        plt.tick_params(axis='both', direction='in', width=3, which='both', pad=10)  # 设置刻度朝内，边框厚度为 2

        plt.xlabel('X-axis', fontweight='bold')
        plt.ylabel('Y-axis', fontweight='bold')
        plt.title('Four Particles', pad=20, fontweight='bold')
        plt.savefig(file_directory + f'/Initial Schematic.png', dpi=100, bbox_inches='tight')
        plt.close()

# coordinates_part = [[75, 75], [125, 75], [75, 125], [125, 125]]
# radii_part = [25, 25, 25, 25]
# initial_schematic(coordinates_part, radii_part, [200, 200])

def initial_schematic(coordinates, radii, domain):
    file_directory = os.getcwd() + "/Initial Schematic"
    if not os.path.exists(file_directory):
        os.makedirs(file_directory)
    with plt.rc_context(
            {'font.family': 'serif',
             'font.serif': ['Times New Roman'],
             'font.weight': 'bold',
             'font.size': 32}
    ):
        fig, ax = plt.subplots(figsize=(12, 9), dpi=100)
        ax.spines['top'].set_linewidth(3)
        ax.spines['bottom'].set_linewidth(3)
        ax.spines['left'].set_linewidth(3)
        ax.spines['right'].set_linewidth(3)

        ax.set_aspect('equal', 'box')
        ax.set_xlim(0, domain[0])
        ax.set_ylim(0, domain[1])


        circle = []
        for i in range(len(coordinates)):
            if i == 0:  # 跳过 i=0 的情况
                continue
            circle.append(plt.Circle((coordinates[i][0], coordinates[i][1]), radii[i], edgecolor='black', fill=True,
                                     facecolor='white', linewidth=3, zorder=2))
            plt.text(coordinates[i][0], coordinates[i][1], rf"$\eta_{{{i + 1}}}$", fontsize=32, ha='center',
                     va='center', zorder=3)
        # 确保在添加圆之前先绘制文本
        for circ in circle:
            ax.add_artist(circ)

        # 绘制三条线
        # 1. x = 60, 0 < y < 60
        ax.plot([60, 60], [0, 60], color='black', linewidth=3, label='x = 60')

        # 绘制水平线 y = 60, x 从 60 到 120
        ax.plot([60, 120], [60, 60], color='black', linewidth=3, label='y = 60')

        # 3. x + y = 120, 0 < x < 60
        x_values_1 = np.linspace(0, 60, 100)
        y_values_1 = 120 - x_values_1
        ax.plot(x_values_1, y_values_1, color='black', linewidth=3, label='x + y = 120')



        # 填充区域
        # 区域1：左下角 (x < 60, y < 60)
        ax.fill_between([60, 120], 0, 60, color='red', alpha=0.4)
        ax.text(95, 25, "III", fontsize=32, ha='center', va='center', zorder=3)
        # x + y = 120 和 y = 60 以上的公共区域
        x_values_2 = np.linspace(0, 120, 200)  # 定义 x 从 60 到 120 的取值范围
        y_values_2 = 120 - x_values_2  # 对应的斜线 y 值
        # 填充满足 x + y > 120 且 y > 60 的区域
        ax.fill_between(x_values_2, y_values_2, 120, where=(y_values_2 > 60), color='yellow', alpha=0.4)
        ax.fill_between([60, 120], 60, 120, color='yellow', alpha=0.4)
        ax.text(75, 105, "II", fontsize=32, ha='center', va='center', zorder=3)

        # 区域3：斜线和 x = 60, y = 60 的包围区域
        ax.fill_between(x_values_1, 0, y_values_1, where=(y_values_1 > 60), color='blue', alpha=0.4)
        ax.text(15, 45, "IV", fontsize=32, ha='center', va='center', zorder=3)

        # ax.grid(True, linestyle='--', linewidth=3, zorder=1)
        plt.tick_params(axis='both', direction='in', width=3, which='both', pad=10)  # 设置刻度朝内，边框厚度为 2

        plt.xlabel('X-axis', fontweight='bold')
        plt.ylabel('Y-axis', fontweight='bold')
        plt.title('Three Parts', pad=20, fontweight='bold')
        plt.savefig(file_directory + f'/Initial Schematic part.png', dpi=100, bbox_inches='tight')
        plt.close()

# initial_schematic(coordinates_part, radii_part, [120, 120])


def plot_histogram(cores, time, name):
    # 创建保存路径
    file_directory = os.getcwd() + f"/python/Histogram of {name}"
    print(f"File directory: {file_directory}")
    if not os.path.exists(file_directory):
        os.makedirs(file_directory)

    # 转换时间单位（秒 -> 小时）
    time_hours = [t / 3600 for t in time]

    # 选择颜色映射方案
    cmap = plt.get_cmap("viridis", len(cores))  # 兼容旧版本的 matplotlib
    colors = [cmap(i / (len(cores) - 1)) for i in range(len(cores))]

    # 使用相同的格式上下文
    with plt.rc_context({'font.family': 'serif',
                        'font.serif': ['Times New Roman'],
                        'font.weight': 'bold',
                        'font.size': 32}):
        # 创建图形和坐标轴
        fig, ax = plt.subplots(figsize=(12, 9), dpi=100)
        
        # 设置轴线宽度
        for spine in ax.spines.values():
            spine.set_linewidth(3)
        
        # 设置刻度参数
        ax.tick_params(axis='both', direction='in', 
                      width=3, which='both', pad=10)

        # 绘制条形图
        bars = ax.bar(cores, time_hours, 
                      width=0.6, 
                      color=colors,  # 使用渐变颜色
                      edgecolor='black',
                      alpha=0.85)

        # 添加数据标签
        for bar in bars:
            height = bar.get_height()
            ax.text(bar.get_x() + bar.get_width()/2., 
                    height,
                    f'{height:.2f}',  # 显示两位小数
                    ha='center', 
                    va='bottom',
                    fontsize=24)

        # 设置坐标轴标签
        ax.set_xlabel('Parallel Cores', fontweight='bold')
        ax.set_ylabel('Execution Time (h)', fontweight='bold')
        ax.set_title(f'{name} Scaling Analysis', 
                    fontweight='bold', pad=20)

        # 设置刻度范围
        ax.set_xticks(cores)
        ax.set_ylim(0, max(time_hours) * 1.2)

        # 网格线设置
        ax.grid(axis='y', linestyle='--', alpha=0.6)

        # 调整布局并保存
        plt.tight_layout()
        plt.savefig(f"{file_directory}/{name}_parallel_performance.png", 
                   dpi=100, 
                   bbox_inches='tight')
        plt.close()

parallel_cores = [1, 2, 4, 6, 8, 10, 12, 14, 16]
execution_time = [38468, 31047, 22431, 17719, 13897, 14029, 12218, 10018, 9940]  # 单位: 秒

plot_histogram(parallel_cores, execution_time, name="MOOSE")