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
  # [./c]
  #   order = FIRST
  #   family = LAGRANGE
    
  #   # Initial expression for phase-field c
  #   [./InitialCondition]
  #       type = MultiParticles_2D
  #       delta = 3
  #       radius = 25
  #       number_x = 2
  #       number_y = 2
  #       omega = 0.05
  #       domain = '200 200'
  #   [../]
  # [../]
  # [./mu]
  #   order = FIRST
  #   family = LAGRANGE
  #   [./InitialCondition]
  #       type = ConstantIC
  #       value = 0.0
  #   [../]
  # [../]
  [./u]
    order = SECOND
    family = LAGRANGE
    # [./InitialCondition]
    #     type = ConstantIC
    #     value = 0.0
    # [../]
  [../]
  [./v]
    order = SECOND
    family = LAGRANGE
    # [./InitialCondition]
    #     type = ConstantIC
    #     value = 0.0
    # [../]
  [../]
  [./p]
    order = FIRST
    family = LAGRANGE
    # [./InitialCondition]
    #     type = ConstantIC
    #     value = 0.0
    # [../]
  [../]
[]

[AuxVariables]
  [./c]
    order = FIRST
    family = MONOMIAL
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
  [./F_density]
    order = FIRST
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./TotalFreeEnergy]
    type = VSTotalFreeEnergy
    variable = F_density
    phase_field = c
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
    x_velocity = u
    y_velocity = v
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
    # petsc_options_iname = '-pc_type'
    # petsc_options_value = 'lu'
  [../]
[]

[Executioner]
  type = Steady
  solve_type = PJFNK
  # scheme = bdf2

  petsc_options_iname = '-pc_type -ksp_gmres_restart -pc_factor_mat_solver_type'
  petsc_options_value = 'lu 1500 superlu_dist'

  # l_max_its = 500
  # l_tol = 1e-6
  # nl_max_its = 30
  nl_rel_tol = 1e-15
  nl_abs_tol = 1e-6

  # dt = 0.001
  # start_time = 0.0
  # end_time = 0.01
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