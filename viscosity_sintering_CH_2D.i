
[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 200
  ny = 200
  xmin = 0
  xmax = 200
  ymin = 0
  ymax = 200
  elem_type = QUAD9
  # uniform_refine = 1
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
  # [./u]
  #   order = SECOND
  #   family = LAGRANGE
  # [../]
  # [./v]
  #   order = SECOND
  #   family = LAGRANGE
  # [../]
  # [./p]
  #   order = FIRST
  #   family = LAGRANGE
  # [../]
[]

[Kernels]
  # Cahn Hilliard kernels
  # [./StokesX]
  #   type = StokesX
  #   variable = u
  #   dim = 2
  #   phase_field = c
  #   pressure = p
  #   y_velocity = v
  # [../]
  # [./StokesY]
  #   type = StokesY
  #   variable = v
  #   dim = 2
  #   phase_field = c
  #   pressure = p
  #   x_velocity = u
  # [../]
  # [./Incompressibility]
  #   type = Incompressibility
  #   variable = p
  #   x_velocity = u
  #   y_velocity = v
  # [../]


  [./dt_C]
    type = TimeDerivative
    variable = c
  [../]
  
  # [./CH_CoupleV]
  #   type = CHCoupV
  #   variable = c
  #   x_velocity = u
  #   y_velocity = v
  # [../]

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


  # [./dt_mu]
  #   type = CoupledTimeDerivative
  #   variable = mu
  #   v = c
  # [../]
  # [./CH_wres]
  #   type = SplitCHWRes
  #   variable = mu
  #   mob_name = M
  # [../]
  # [./CH_Parsed]
  #   type = SplitCHParsed
  #   variable = c
  #   f_name = f_loc
  #   w = mu
  #   kappa_name = kappa_c
  # [../]

[]



# [BCs]
#   [./bcs_u]
#     type = DirichletBC
#     variable = u
#     boundary = '0 1 2 3'
#     value = 0
#   [../]
#   [./bcs_v]
#     type = DirichletBC
#     variable = v
#     boundary = '0 1 2 3'
#     value = 0
#   [../]
# []

[Materials]
  [./ViscosityMaterial]
    type = ViscositySinteringMaterial
    cvar = c
  [../]


  # [./constants]
  #   type = GenericConstantMaterial
  #   block = 0
  #   prop_names = 'kappa_c M'
  #   prop_values = '135.00 0.005'
  # [../]

  # [./free_energy]
  #   type = DerivativeParsedMaterial
  #   property_name = f_loc
  #   constant_names = 'A'
  #   constant_expressions = '120.00'
  #   coupled_variables = 'c'
  #   expression = 'A*c^2*(1-c)^2'
  #   derivative_order = 2
  # [../]
[]



[Preconditioning]
  [./cw_coupling]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient
  solve_type = JFNK
  # scheme = bdf2

  petsc_options_iname = '-pc_type -sub_pc_type'
  petsc_options_value = 'lu      lu          '

  # l_max_its = 100
  # l_tol = 1e-6
  # nl_max_its = 30
  nl_rel_tol = 1e-7
  nl_abs_tol = 1e-6

  dt = 0.5
  start_time = 0.0
  end_time = 500
[]


[Outputs]
  exodus = true
  time_step_interval = 2
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