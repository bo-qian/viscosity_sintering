[Mesh]
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
    # kappa_C = 33.75
    # mu_volume = 0.2
  [../]
[]

[Problem]
  type = FEProblem
  mass_matrix = 'Incompressibility'
  extra_tag_matrices = 'Incompressibility'
  use_pressure_mass_matrix = true
[]

[Preconditioning]
  [FSP]
    type = FSP
    topsplit = 'up'

    [up]
      splitting = 'u v p'
      vars = 'u:u v:v p:p'       # 显式绑定变量名到分块
      splitting_type = schur
      schur_precondition = selfp
      petsc_options_value = '-pc_type fieldsplit -pc_fieldsplit_type schur -fieldsplit_u_pc_type hypre -fieldsplit_v_pc_type hypre -fieldsplit_p_pc_type jacobi'
    []
  []
[]

# [Preconditioning]
#   [./CH_Stokes]
#     type = SMP
#     full = true
#     petsc_options_iname = '-pc_type -pc_factor_mat_solver_type -ksp_gmres_restart'
#     petsc_options_value = 'lu mumps 2500'
#   [../]
# []

[Executioner]
  type = Steady
  solve_type = NEWTON

  l_tol = 1e-7
  l_abs_tol = 1e-10
  l_max_its = 100
  
  nl_rel_tol = 1e-15
  nl_abs_tol = 1e-6
[]

[Outputs]
  # perf_graph = true
  # execute_on = 'TIMESTEP_END'
[]

[Debug]
  show_var_residual_norms = true
[]