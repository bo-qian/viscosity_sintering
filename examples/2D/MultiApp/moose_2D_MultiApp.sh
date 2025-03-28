#!/bin/bash
#SBATCH --job-name=VS2D
#SBATCH --output=%j_moose_run.out     # Temporary .out filename
#SBATCH --error=%j_moose_run.err      # Temporary .err filename
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --nodelist=node1

# Load Conda environment
source ~/.bashrc
conda activate moose

# Enter MOOSE project directory
cd /home/qianbo/projects/viscosity_sintering

# Get task ID and create output directory
TASK_ID=$SLURM_JOB_ID
OUTPUT_DIR="outputs/${TASK_ID}"
mkdir -p "$OUTPUT_DIR"

# Log run information to standard output (will be moved to target directory)
echo "========== MOOSE Run Information ==========" | tee "${OUTPUT_DIR}/${TASK_ID}.log"
echo "Submission Time: $(date)" | tee -a "${OUTPUT_DIR}/${TASK_ID}.log"
echo "Task ID: $TASK_ID" | tee -a "${OUTPUT_DIR}/${TASK_ID}.log"
echo "Number of Nodes: $SLURM_NNODES" | tee -a "${OUTPUT_DIR}/${TASK_ID}.log"
echo "Number of Tasks: $SLURM_NTASKS" | tee -a "${OUTPUT_DIR}/${TASK_ID}.log"

# Record start time
start_time=$(date +%s) 
echo "Start Time: $(date)"  | tee -a "${OUTPUT_DIR}/${TASK_ID}.log"
echo "===========================================" | tee -a "${OUTPUT_DIR}/${TASK_ID}.log"

# Run MOOSE
mpiexec -n $SLURM_NTASKS ./viscosity_sintering-opt -i ./examples/2D/MultiApp/viscosity_sintering_MultiApp_2D.i Outputs/file_base="${OUTPUT_DIR}/${TASK_ID}_VS2D" --color off 2>&1 | tee -a "${OUTPUT_DIR}/${TASK_ID}.log"

# Record end time
end_time=$(date +%s) 
echo "===========================================" | tee -a "${OUTPUT_DIR}/${TASK_ID}.log"
echo "End Time: $(date)" | tee -a "${OUTPUT_DIR}/${TASK_ID}.log"

# Calculate runtime
runtime=$((end_time - start_time))
echo "Total Runtime: $runtime seconds" | tee -a "${OUTPUT_DIR}/${TASK_ID}.log"