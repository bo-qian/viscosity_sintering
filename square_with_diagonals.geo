// Define points
Point(1) = {0, 0, 0, 1.0};
Point(2) = {1, 0, 0, 1.0};
Point(3) = {1, 1, 0, 1.0};
Point(4) = {0, 1, 0, 1.0};
Point(5) = {0.5, 0.5, 0, 1.0};

// Define lines (edges of the square)
Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 1};

// Define diagonals
Line(5) = {1, 3};
Line(6) = {2, 4};

// Define surface
Line Loop(7) = {1, 2, 3, 4};
Plane Surface(8) = {7};

// Mesh the surface with diagonals
Line {5, 6} In Surface {8};

// Generate the mesh
Mesh 2;
