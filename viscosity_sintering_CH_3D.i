[Mesh]
    type = GeneratedMesh
    dim = 3
    nx = 160
    ny = 80
    nz = 80
    xmin = 0
    xmax = 160
    ymin = 0
    ymax = 80
    zmin = 0
    zmax = 80
    elem_type = HEX8
    # uniform_refine = 1
  []
  
  [Variables]
    [./c]
        order = FIRST
        family = LAGRANGE
        
        # Initial expression for phase-field c
        [./InitialCondition]
            type = MultiParticles_3D
            delta = 3
            radius = 25
            number_x = 2
            number_y = 1
            number_z = 1
            omega = 0.05
            domain = '160 80 80'
        [../]
    [../]
    [./mu]
        order = FIRST
        family = LAGRANGE
    [../]
    [./u]
        order = FIRST
        family = LAGRANGE
    [../]
    [./v]
        order = FIRST
        family = LAGRANGE
    [../]
    [./w]
        order = FIRST
        family = LAGRANGE
    [../]
    [./p]
        order = FIRST
        family = LAGRANGE
    [../]
  []
  
  [Kernels]
    [./dt_mu]
      type = CoupledTimeDerivative
      variable = mu
      v = c
    [../]

    [./CH_wres]
      type = SplitCHWRes
      variable = mu
      mob_name = M
    [../]

    [./CH_Parsed]
      type = SplitCHParsed
      variable = c
      f_name = f_loc
      w = mu
      kappa_name = kappa_c
    [../]

    [./CoupledVelocity]
      type = CHCoupV
      variable = c
      v = 'u v w'
    [../]

    [./StokesX]
      type = StokesX
      variable = u
      v = v
      w = w
      p = p
    [../]

    [./StokesY]
      type = StokesY
      variable = v
      u = u
      w = w
      p = p
    [../]

    [./StokesZ]
      type = StokesZ
      variable = w
      u = u
      v = v
      p = p
    [../]
    
    [./Incompressibility]
      type = Incompressibility
      variable = p
      v = 'u v w'
    [../]
  []

  
  [BCs]
    [./bcs]
      #zero flux BC
      type = NeumannBC
      value = 0
      variable = c
      boundary = '0 1 2 3'
    [../]
  []
  
  [Materials]
    [./vaicosity_material]
      type = ViscositySinteringMaterial
      cvar = c
    [../]
    
    [./constants]
      type = GenericConstantMaterial
      prop_names = 'kappa_c M'
      prop_values = '135.00 0.005'
    [../]
  
    [./free_energy]
      type = DerivativeParsedMaterial
      property_name = f_loc
      constant_names = 'A'
      constant_expressions = '120.00'
      coupled_variables = 'c'
      expression = 'A*c^2*(1-c)^2'
      derivative_order = 2
    [../]
  []

  
  [Preconditioning]
    [./coupled]
      type = SMP
      full = true
    [../]
  []
  
  [Executioner]
    type = Transient
    scheme = bdf2
    solve_type = NEWTON
    petsc_options_iname = '-pc_type -ksp_gmres_restart -sub_ksp_type
                           -sub_pc_type -pc_asm_overlap'
    petsc_options_value = 'asm      31                  preonly
                           ilu          2'
    l_tol = 1e-05
    nl_max_its = 30
    l_max_its = 30
    nl_rel_tol = 1e-07
    nl_abs_tol = 1e-09
    start_time = 0.0
    end_time = 8000.0
    dt = 0.5
  []
  
  [Outputs]
    exodus = true
    perf_graph = true
    [./display]
      type = Console
      max_rows = 12
    [../]
  []
  