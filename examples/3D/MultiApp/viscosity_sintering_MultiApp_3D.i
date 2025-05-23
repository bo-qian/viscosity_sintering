dimension = 3
mesh_ratio = 1.0
domain_x = 100
domain_y = 60
domain_z = 60

# [Mesh]
#   type = GeneratedMesh
#   dim = ${dimension}
#   nx = ${fparse domain_x * mesh_ratio}
#   ny = ${fparse domain_y * mesh_ratio}
#   nz = ${fparse domain_z * mesh_ratio}
#   xmin = 0
#   xmax = ${domain_x}
#   ymin = 0
#   ymax = ${domain_y}
#   zmin = 0
#   zmax = ${domain_z}
#   elem_type = TET10
# []

[Mesh]
  [gen]
    type = DistributedRectilinearMeshGenerator
    dim = ${dimension}
    xmin = 0
    xmax = ${domain_x}
    ymin = 0
    ymax = ${domain_y}
    zmin = 0
    zmax = ${domain_z}
    nx = ${fparse domain_x * mesh_ratio}
    ny = ${fparse domain_y * mesh_ratio}
    nz = ${fparse domain_z * mesh_ratio}
    elem_type = HEX8
  []
  second_order = true
  parallel_type = distributed
[]

[Variables]
  [./c]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = MultiParticlesIC
      dim = ${dimension}
      delta = 3
      radius = 15
      number_x = 2
      number_y = 1
      number_z = 1
      omega = 0.05
      domain = '${domain_x} ${domain_y} ${domain_z}'
    [../]
  [../]
  [./mu]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
        type = ConstantIC
        value = 0.0
    [../]
  [../]
[]

[AuxVariables]
  [./u]
    order = SECOND
    family = LAGRANGE
  [../]
  [./v]
    order = SECOND
    family = LAGRANGE
  [../]
  [./w]
    order = SECOND
    family = LAGRANGE
  [../]
  [./p]
    order = FIRST
    family = LAGRANGE
  [../]
  [./F_density]
    order = FIRST
    family = MONOMIAL
  [../]
  [./V_Magnitude]
    order = FIRST
    family = MONOMIAL
  [../]
  [./Stress_Magnitude]
    order = FIRST
    family = MONOMIAL
  [../]
  [./Stress_xx]
    order = FIRST
    family = MONOMIAL
  [../]
  [./Stress_xy]
    order = FIRST
    family = MONOMIAL
  [../]
  [./Stress_xz]
    order = FIRST
    family = MONOMIAL
  [../]
  [./Stress_yy]
    order = FIRST
    family = MONOMIAL
  [../]
  [./Stress_yx]
    order = FIRST
    family = MONOMIAL
  [../]
  [./Stress_yz]
    order = FIRST
    family = MONOMIAL
  [../]
  [./Stress_zz]
    order = FIRST
    family = MONOMIAL
  [../]
  [./Stress_zx]
    order = FIRST
    family = MONOMIAL
  [../]
  [./Stress_zy]
    order = FIRST
    family = MONOMIAL
  [../]
  [./Real_Pressure]
    order = FIRST
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./TotalFreeEnergy]
    type = VSTotalFreeEnergy
    variable = F_density
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
  [./VelocityMagnitude]
    type = VelocityMagnitude
    variable = V_Magnitude
    dim = ${dimension}
    x_velocity = u
    y_velocity = v
    z_velocity = w
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
  [./StressMagnitude]
    type = StressMagnitude
    variable = Stress_Magnitude
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
  [./stress_xx]
    type = RankTwoAux
    variable = Stress_xx
    rank_two_tensor = stress
    index_i = 0
    index_j = 0
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
  [./stress_xy]
    type = RankTwoAux
    variable = Stress_xy
    rank_two_tensor = stress
    index_i = 0
    index_j = 1
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
  [./stress_xz]
    type = RankTwoAux
    variable = Stress_xz
    rank_two_tensor = stress
    index_i = 0
    index_j = 2
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
  [./stress_yy]
    type = RankTwoAux
    variable = Stress_yy
    rank_two_tensor = stress
    index_i = 1
    index_j = 1
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
  [./stress_yx]
    type = RankTwoAux
    variable = Stress_yx
    rank_two_tensor = stress
    index_i = 1
    index_j = 0
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
  [./stress_yz]
    type = RankTwoAux
    variable = Stress_yz
    rank_two_tensor = stress
    index_i = 1
    index_j = 2
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
  [./stress_zx]
    type = RankTwoAux
    variable = Stress_zx
    rank_two_tensor = stress
    index_i = 2
    index_j = 0
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
  [./stress_zy]
    type = RankTwoAux
    variable = Stress_zy
    rank_two_tensor = stress
    index_i = 2
    index_j = 1
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
  [./stress_zz]
    type = RankTwoAux
    variable = Stress_zz
    rank_two_tensor = stress
    index_i = 2
    index_j = 2
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
  [./Real_Pressure]
    type = RealPressure
    variable = Real_Pressure
    pressure = p
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
[]

[Kernels]
  # Cahn Hilliard kernels
  [./dt_C]
    type = TimeDerivative
    variable = c
  [../]
  
  [./CH_CoupleV]
    type = CHCoupV
    variable = c
    x_velocity = u
    y_velocity = v
  [../]

  [./CHMob]
    type = CHMob
    variable = c
    coupledvar = mu
  [../]

  [./CHMuFloc]
    type = CHMuFloc
    variable = mu
    coupledvar = c
  [../]

  [./CHMuKap]
    type = CHMuKap
    variable = mu
    coupledvar = c
  [../]
[]

[Materials]
  [./ViscosityMaterial]
    type = ViscositySinteringMaterial
    cvar = c
    x_velocity = u
    y_velocity = v
    z_velocity = w
    pressure = p
  [../]
[]

# [UserObjects/study]
#   type = RepeatableRayStudy
#   names = 'neck_length shrinkage_length'
#   start_points = '80 0 0
#                    0 60 0'
#   end_points = '80 120 0
#                  160 60 0'
#   execute_on = 'INITIAL TIMESTEP_END'
# []

# [RayKernels/c_integral]
#   type = VariableIntegralRayKernel
#   variable = c
# []

[Postprocessors]
  [./total_energy]
    type = ElementIntegralVariablePostprocessor
    variable = F_density
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
  # [./neck_length]
  #   type = RayIntegralValue
  #   ray_kernel = c_integral
  #   ray = neck_length
  #   execute_on = 'INITIAL TIMESTEP_END'
  # [../]
  # [./shrinkage_length]
  #   type = RayIntegralValue
  #   ray_kernel = c_integral
  #   ray = shrinkage_length
  #   execute_on = 'INITIAL TIMESTEP_END'
  # [../]
  [total]
    type = MemoryUsage
    execute_on = 'INITIAL TIMESTEP_END'
  []
  [per_proc]
    type = MemoryUsage
    value_type = "average"
    execute_on = 'INITIAL TIMESTEP_END'
  []
  [max_proc]
    type = MemoryUsage
    value_type = "max_process"
    execute_on = 'INITIAL TIMESTEP_END'
  []
[]

[Preconditioning]
  [./cw_coupling]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient
  solve_type = NEWTON

  petsc_options_iname = '-pc_type -ksp_gmres_restart -pc_factor_mat_solver_type'
  petsc_options_value = 'lu 2500 mumps'

  nl_rel_tol = 1e-15
  nl_abs_tol = 1e-6

  dt = 0.01
  start_time = 0.0
  end_time = 0.1

[]

[MultiApps]
  [./Stokes]
    type = FullSolveMultiApp
    input_files = "viscosity_sintering_Stokes_3D.i"
    execute_on = 'INITIAL TIMESTEP_END'
    clone_parent_mesh = true
  [../]
[]

[Transfers]
  [./CHToStokes]
    type = MultiAppCopyTransfer
    to_multi_app = Stokes
    source_variable = c
    variable = c
  [../]
  [./StokesToCH]
    type = MultiAppCopyTransfer
    from_multi_app = Stokes
    source_variable = 'u v w p' 
    variable = 'u v w p'
  [../]
[]

[Outputs]
  exodus = true
  time_step_interval = 1
  # perf_graph = true
  checkpoint = true
  csv = true
[]