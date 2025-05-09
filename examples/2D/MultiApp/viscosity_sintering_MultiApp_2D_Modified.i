dimension = 2
mesh_ratio = 1.5
domain_x = 160
domain_y = 120

[Mesh]
  type = GeneratedMesh
  dim = ${dimension}
  nx = ${fparse domain_x * mesh_ratio}
  ny = ${fparse domain_y * mesh_ratio}
  xmin = 0
  xmax = ${domain_x}
  ymin = 0
  ymax = ${domain_y}
  elem_type = TRI6
[]

[Variables]
  [./c]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = MultiParticlesIC
      dim = ${dimension}
      delta = 3
      radius = 20
      number_x = 2
      number_y = 1
      omega = 0.05
      domain = '${domain_x} ${domain_y}'
    [../]
    # [./InitialCondition]
    #   type = RandomParticle2DIC
    #   domain = '${domain_x} ${domain_y}'
    #   num_particles = 15
    #   radius_min = 15
    #   radius_max = 25
    #   max_attempts = 30
    #   edge_factor = 0.05
    #   delta =3
    #   omega = 0.05
    # [../]
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
    dim = ${dimension}
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
    rank_two_tensor = stress_modified
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
    rank_two_tensor = stress_modified
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
    pressure = p
    # kappa_C = 33.75
    # mu_volume = 0.2
  [../]
[]

[UserObjects/study]
  type = RepeatableRayStudy
  names = 'neck_length shrinkage_length'
  start_points = '${fparse 0.5*domain_x} 0 0
                  0 ${fparse 0.5*domain_y} 0'
  end_points = '${fparse 0.5*domain_x} ${fparse domain_y} 0
                ${fparse domain_x} ${fparse 0.5*domain_y} 0'
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

[MultiApps]
  [./Stokes]
    type = FullSolveMultiApp
    input_files = "viscosity_sintering_Stokes_2D_Modified.i"
    execute_on = 'INITIAL TIMESTEP_END'
    clone_parent_mesh = true
  [../]
[]

[Transfers]
  [./CHToStokes]
    type = MultiAppCopyTransfer
    to_multi_app = Stokes
    source_variable = 'c mu'
    variable = 'c mu'
  [../]
  [./StokesToCH]
    type = MultiAppCopyTransfer
    from_multi_app = Stokes
    source_variable = 'u v p' 
    variable = 'u v p'
  [../]
[]

[Preconditioning]
  [./CH_Stokes]
    type = SMP
    full = true
    petsc_options_iname = '-ksp_type -pc_type'
    petsc_options_value = 'tfqmr ilu'
  [../]
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  # verbose = true

  # petsc_options_iname = '-pc_type -ksp_gmres_restart -pc_factor_mat_solver_type'
  # petsc_options_value = 'lu 2500 mumps'

  l_tol = 1e-7
  l_abs_tol = 1e-10
  l_max_its = 100

  nl_rel_tol = 1e-15
  nl_abs_tol = 1e-6

  dt = 0.01
  start_time = 0.0
  end_time = 0.15
[]

[Outputs]
  exodus = true
  time_step_interval = 1
  perf_graph = true
  checkpoint = true
  csv = true
[]

[Debug]
  # show_var_residual_norms = true
[]