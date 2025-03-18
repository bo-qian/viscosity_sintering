[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 240
  ny = 180
  xmin = 0
  xmax = 160
  ymin = 0
  ymax = 120
  elem_type = TRI6
[]

[Variables]
  [./u]
    order = SECOND
    family = LAGRANGE
  [../]
  [./v]
    order = SECOND
    family = LAGRANGE
  [../]
  [./p]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[AuxVariables]
  [./c]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = MultiParticles_2D
      delta = 3
      radius = 20
      number_x = 2
      number_y = 1
      omega = 0.05
      domain = '160 120'
    [../]
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
  [Stress_xx]
    order = FIRST
    family = MONOMIAL
  [../]
  [Stress_xy]
    order = FIRST
    family = MONOMIAL
  [../]
  [Stress_yy]
    order = FIRST
    family = MONOMIAL
  [../]
  [Stress_yx]
    order = FIRST
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./TotalFreeEnergy]
    type = VSTotalFreeEnergy
    variable = F_density
    phase_field = c
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
  [./VelocityMagnitude]
    type = VelocityMagnitude
    variable = V_Magnitude
    dim = 2
    x_velocity = u
    y_velocity = v
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
[]

[Kernels]
  # Stokes kernels
  [./StokesX]
    type = StokesX
    variable = u
    dim = 2
    phase_field = c
    pressure = p
    y_velocity = v

  [../]
  [./StokesY]
    type = StokesY
    variable = v
    dim = 2
    phase_field = c
    pressure = p
    x_velocity = u
  [../]
  [./Incompressibility]
    type = Incompressibility
    variable = p
    dim = 2
    x_velocity = u
    y_velocity = v
  [../]
[]

[BCs]
  [./bcs_u]
    type = DirichletBC
    variable = u
    boundary = '0 1 2 3'
    value = 0
  [../]
  [./bcs_v]
    type = DirichletBC
    variable = v
    boundary = '0 1 2 3'
    value = 0
  [../]
[]

[Materials]
  [./ViscosityMaterial]
    type = StokesMaterial
    cvar = c
    x_velocity = u
    y_velocity = v
    pressure = p
    # alpha = 120.0
    # kappa_C = 60.0
    # mu_volume = 35.17
    # mu_ratio = 0.005
  [../]
[]

[UserObjects/study]
  type = RepeatableRayStudy
  names = 'neck_length shrinkage_length'
  start_points = '80 0 0
                   0 60 0'
  end_points = '80 120 0
                 160 60 0'
  execute_on = 'INITIAL TIMESTEP_END'
[]

[RayKernels/c_integral]
  type = VariableIntegralRayKernel
  variable = c
[]

[Postprocessors]
  [./total_energy]
    type = ElementIntegralVariablePostprocessor
    variable = F_density
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
  [./neck_length]
    type = RayIntegralValue
    ray_kernel = c_integral
    ray = neck_length
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
  [./shrinkage_length]
    type = RayIntegralValue
    ray_kernel = c_integral
    ray = shrinkage_length
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
[]

[Preconditioning]
  [./cw_coupling]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Steady
  solve_type = NEWTON

  petsc_options_iname = '-pc_type -ksp_gmres_restart -pc_factor_mat_solver_type'
  petsc_options_value = 'lu 2500 superlu_dist'

  nl_rel_tol = 1e-15
  nl_abs_tol = 1e-6
[]

[Outputs]
  exodus = true
  csv = true
  perf_graph = true
  # [exodus]
  #   type = Exodus
  #   # file_name = output.e
  #   # output_material_properties = true  # 允许输出材料属性
  # []
  [./display]
    type = Console
    max_rows = 12
  [../]
[]

[Debug]
  show_material_props = true
  show_var_residual_norms = true
[]