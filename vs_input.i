[Mesh]
    type = GeneratedMesh
    dim = 2
    nx = 300
    ny = 100
    xmin = 0
    xmax = 300
    ymin = 0
    ymax = 100
    elem_type = QUAD4
[]

[Variables]
    [./c]
        order = FIRST
        family = LAGRANGE
        
        # Initial expression for phase-field c
        [./InitialCondition]
            type = MultiParticles
            delta = 3
            radius = 25
            number_x = 2
            number_y = 1
            omega = 0.9
            domain = '300 100'
        [../]
    [../]
[]

[Kernels]
    [./c_dot]
      type = TimeDerivative
      variable = c
    [../]
    [./CHbulk]
      type = CHMath
      variable = c
    [../]
    [./CHint]
      type = CHInterface
      variable = c
      mob_name = M
      kappa_name = kappa_c
    [../]
  []
  
  [BCs]
    [./Periodic]
      [./all]
        auto_direction = 'x y'
      [../]
    [../]
  []
  
  [Materials]
    [./mat]
      type = GenericConstantMaterial
      prop_names  = 'M   kappa_c'
      prop_values = '1.0 0.5'
    [../]
  []
  
  [Postprocessors]
    [./top]
      type = SideIntegralVariablePostprocessor
      variable = c
      boundary = top
    [../]
  []
  
  [Executioner]
    type = Transient
    solve_type = 'NEWTON'
    scheme = bdf2
  
    # Preconditioning using the additive Schwartz method and LU decomposition
    petsc_options_iname = '-pc_type -sub_ksp_type -sub_pc_type'
    petsc_options_value = 'asm      preonly       lu          '
  
    # Alternative preconditioning options using Hypre (algebraic multi-grid)
    #petsc_options_iname = '-pc_type -pc_hypre_type'
    #petsc_options_value = 'hypre    boomeramg'
  
    l_tol = 1e-4
    l_max_its = 30
  
    dt = 0.5
    end_time = 80.0
  []
  
  [Outputs]
    exodus = true
    perf_graph = true
  []