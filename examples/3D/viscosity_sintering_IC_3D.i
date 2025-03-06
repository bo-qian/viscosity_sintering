[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 160
  ny = 120
  nz = 120
  xmin = 0
  xmax = 160
  ymin = 0
  ymax = 120
  zmin = 0
  zmax = 120
  elem_type = TET10
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
  [./w]
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
    family = MONOMIAL
    [./InitialCondition]
      type = MultiParticles_3D
      delta = 3
      radius = 20
      number_x = 2
      number_y = 1
      number_z = 1
      omega = 0.05
      domain = '160 120 120'
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
  [./Stress_xz]
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
    dim = 3
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
[]

[Kernels]
  # Stokes kernels
  [./StokesX]
    type = StokesX
    variable = u
    dim = 3
    phase_field = c
    pressure = p
    y_velocity = v
    z_velocity = w
  [../]
  [./StokesY]
    type = StokesY
    variable = v
    dim = 3
    phase_field = c
    pressure = p
    x_velocity = u
    z_velocity = w
  [../]
  [./StokesZ]
    type = StokesZ
    variable = w
    dim = 3
    phase_field = c
    pressure = p
    x_velocity = u
    y_velocity = v
  [../]
  [./Incompressibility]
    type = Incompressibility
    variable = p
    dim = 3
    x_velocity = u
    y_velocity = v
    z_velocity = w
  [../]
[]

[BCs]
  [./bcs_u]
    type = DirichletBC
    variable = u
    boundary = '0 1 2 3 4 5'
    value = 0
  [../]
  [./bcs_v]
    type = DirichletBC
    variable = v
    boundary = '0 1 2 3 4 5'
    value = 0
  [../]
  [./bcs_w]
    type = DirichletBC
    variable = w
    boundary = '0 1 2 3 4 5'
    value = 0
  [../]
[]

[Materials]
  [./ViscosityMaterial]
    type = StokesMaterial
    cvar = c
    x_velocity = u
    y_velocity = v
    z_velocity = w
    pressure = p
  [../]
[]

[Postprocessors]
  [./total_energy]
    type = ElementIntegralVariablePostprocessor
    variable = F_density
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
  checkpoint = true
  perf_graph = true
  [./display]
    type = Console
    max_rows = 12
  [../]
[]

[Debug]
  show_material_props = true
  show_var_residual_norms = true
[]