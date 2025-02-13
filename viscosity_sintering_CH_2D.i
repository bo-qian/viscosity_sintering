
[Mesh]
  # type = GeneratedMesh
  # dim = 2
  # nx = 200
  # ny = 150
  # xmin = 0
  # xmax = 200
  # ymin = 0
  # ymax = 150
  # elem_type = TRI6
  # uniform_refine = 1
  file = 'viscosity_sintering_IC_2D_out.e'
[]

[Variables]
  [./c]
    order = FIRST
    family = LAGRANGE
    
    # Initial expression for phase-field c
    [./InitialCondition]
        type = MultiParticles_2D
        delta = 3
        radius = 25
        number_x = 2
        number_y = 2
        omega = 0.05
        domain = '200 200'
    [../]
  [../]
  [./mu]
    order = FIRST
    family = LAGRANGE
  [../]
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
  [./F_density]
    order = FIRST
    family = MONOMIAL
  [../]
[]

[Kernels]
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

[AuxKernels]
  [./TotalFreeEnergy]
    type = VSTotalFreeEnergy
    variable = F_density
    phase_field = c
  [../]
[]

[Materials]
  [./ViscosityMaterial]
    type = ViscositySinteringMaterial
    cvar = c
  [../]
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
  # scheme = bdf2

  petsc_options_iname = '-pc_type -ksp_gmres_restart -pc_factor_mat_solver_type'
  petsc_options_value = 'lu 1500 superlu_dist'

  # l_max_its = 500
  # l_tol = 1e-6
  # nl_max_its = 30
  nl_rel_tol = 1e-15
  nl_abs_tol = 1e-6

  dt = 0.5
  start_time = 0.0
  end_time = 500
[]


[Outputs]
  exodus = true
  # time_step_interval = 1
  perf_graph = true
  [./display]
    type = Console
    max_rows = 12
  [../]
[]

[Debug]
  show_material_props = true
  # show_execution_order = true
[]