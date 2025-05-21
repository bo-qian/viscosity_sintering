dimension = 3
mesh_ratio = 1.0
domain_x = 100
domain_y = 60
domain_z = 60

[Mesh]
  type = GeneratedMesh
  dim = ${dimension}
  nx = ${fparse domain_x * mesh_ratio}
  ny = ${fparse domain_y * mesh_ratio}
  nz = ${fparse domain_z * mesh_ratio}
  xmin = 0
  xmax = ${domain_x}
  ymin = 0
  ymax = ${domain_y}
  zmin = 0
  zmax = ${domain_z}
  elem_type = TET10
[]

[Variables]
[]

[Kernels]
[]

[Materials]
[]

[Executioner]
[]

[Outputs]
  exodus = true
[]