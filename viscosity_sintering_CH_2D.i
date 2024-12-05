# Tests the rigid body motion due to applied force of multiple particles.

# ***COPY AND PASTE THESE AS NEEDED***
# 'gr0 gr1 gr2 gr3 gr4 gr5 gr6 gr7 gr8 gr9 gr10 gr11 gr12 gr13 gr14 gr15 gr16 gr17 gr18 gr19'
# (gr0^2+gr1^2+gr2^2+gr3^2+gr4^2+gr5^2+gr6^2+gr7^2+gr8^2+gr9^2+gr10^2+gr11^2+gr12^2+gr13^2+gr14^2+gr15^2+gr16^2+gr17^2+gr18^2+gr19^2)
# (gr0^3+gr1^3+gr2^3+gr3^3+gr4^3+gr5^3+gr6^3+gr7^3+gr8^3+gr9^3+gr10^3+gr11^3+gr12^3+gr13^3+gr14^3+gr15^3+gr16^3+gr17^3+gr18^3+gr19^3)

# [GlobalParams]
#   op_num = 4
#   var_name_base = gr
# []

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 200
  ny = 200
  xmin = 0
  xmax = 200
  ymin = 0
  ymax = 200
  elem_type = QUAD4
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
  #   order = FIRST
  #   family = LAGRANGE
  # [../]
  # [./v]
  #   order = FIRST
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

  # [./dt_mu]
  #   type = CoupledTimeDerivative
  #   variable = mu
  #   v = c
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
  # [./constants]
  #   type = GenericConstantMaterial
  #   prop_names = 'kappa_gr kappa_c M L'
  #   prop_values = '250 4000 4.5 60'
  # [../]
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

# [Executioner]
#   type = Transient
#   solve_type = NEWTON
#   scheme = implicit-euler

#   petsc_options_iname = '-pc_type -sub_pc_type'
#   petsc_options_value = 'asm      lu          '

#   l_max_its = 30
#   l_tol = 1e-6
#   nl_max_its = 20
#   nl_rel_tol = 1e-9

#   dt = 0.001
#   start_time = 0.0
#   end_time = 0.001
# []

[Executioner]
  # petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  # petsc_options_value = 'hypre boomeramg 101'
  type = Transient
  dt = 0.001
  l_max_its = 30
  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-9
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  l_tol = 1e-4
  end_time = 80.0
  scheme = bdf2
[]


[Outputs]
  exodus = true
  time_step_interval = 1
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