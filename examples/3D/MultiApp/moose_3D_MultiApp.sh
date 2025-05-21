#!/bin/bash
#SBATCH --job-name=VS3D
#SBATCH --output=%j_moose_run.out     # 临时 .out 文件名
#SBATCH --error=%j_moose_run.err      # 临时 .err 文件名
#SBATCH --nodes=1
#SBATCH --ntasks=2
#SBATCH --nodelist=node1
#SBATCH --mem=200G

# 加载 Conda 环境
source ~/.bashrc
conda activate moose

# 进入 MOOSE 项目目录
cd /home/qianbo/projects/viscosity_sintering

# 获取任务号并创建输出目录
TASK_ID=$SLURM_JOB_ID
OUTPUT_DIR="outputs/${TASK_ID}"
mkdir -p "$OUTPUT_DIR"

# 记录运行信息到标准输出（最终会被移动到目标目录）
echo "========== MOOSE 运行信息 ==========" | tee "${OUTPUT_DIR}/${TASK_ID}.log"
echo "提交时间: $(date)" | tee -a "${OUTPUT_DIR}/${TASK_ID}.log"
echo "任务号: $TASK_ID" | tee -a "${OUTPUT_DIR}/${TASK_ID}.log"
echo "运行节点数: $SLURM_NNODES" | tee -a "${OUTPUT_DIR}/${TASK_ID}.log"
echo "使用核数: $SLURM_NTASKS" | tee -a "${OUTPUT_DIR}/${TASK_ID}.log"

# 记录开始时间
start_time=$(date +%s) 
echo "开始时间: $(date)"  | tee -a "${OUTPUT_DIR}/${TASK_ID}.log"
echo "====================================" | tee -a "${OUTPUT_DIR}/${TASK_ID}.log"

# 运行 MOOSE
mpiexec -n $SLURM_NTASKS ./viscosity_sintering-opt -i ./examples/3D/MultiApp/viscosity_sintering_MultiApp_3D.i Outputs/file_base="${OUTPUT_DIR}/${TASK_ID}_VS3D" --color off 2>&1 | tee -a "${OUTPUT_DIR}/${TASK_ID}.log"

# 记录结束时间
end_time=$(date +%s) 
echo "====================================" | tee -a "${OUTPUT_DIR}/${TASK_ID}.log"
echo "结束时间: $(date)" | tee -a "${OUTPUT_DIR}/${TASK_ID}.log"

# 计算运行时间
runtime=$((end_time - start_time))
echo "总运行时间: $runtime 秒" | tee -a "${OUTPUT_DIR}/${TASK_ID}.log"