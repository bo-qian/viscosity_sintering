// GMSH script to generate a 200x100 grid with each quadrilateral split into four right triangles

// Define the dimensions of the grid
Lx = 200;
Ly = 100;

// Define the number of divisions along x and y
Nx = 200;  // Number of divisions in x direction
Ny = 100;  // Number of divisions in y direction

// Define points at the corners of the rectangle
Point(1) = {0, 0, 0, 1};
Point(2) = {Lx, 0, 0, 1};
Point(3) = {Lx, Ly, 0, 1};
Point(4) = {0, Ly, 0, 1};

// Create the lines to form the boundary
Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 1};

// Create a curve loop and plane surface
Curve Loop(1) = {1, 2, 3, 4};
Plane Surface(1) = {1};

// Transfinite settings for lines and surface
Transfinite Line {1, 3} = Nx + 1;
Transfinite Line {2, 4} = Ny + 1;
Transfinite Surface {1} = {1, 2, 3, 4};
Recombine Surface {1}; // Create quadrangles

// Create inner diagonal connections for each quadrilateral
For i In {0:Nx-1}
  For j In {0:Ny-1}
    x1 = i * Lx / Nx;
    y1 = j * Ly / Ny;
    x2 = (i+1) * Lx / Nx;
    y2 = (j+1) * Ly / Ny;

    // Define the corner points
    Point(i * Ny * 13 + j * 13 + 100 + 1) = {x1, y1, 0, 1};
    p1 = i * Ny * 13 + j * 13 + 100 + 1;

    Point(i * Ny * 13 + j * 13 + 100 + 2) = {x2, y1, 0, 1};
    p2 = i * Ny * 13 + j * 13 + 100 + 2;

    Point(i * Ny * 13 + j * 13 + 100 + 3) = {x2, y2, 0, 1};
    p3 = i * Ny * 13 + j * 13 + 100 + 3;

    Point(i * Ny * 13 + j * 13 + 100 + 4) = {x1, y2, 0, 1};
    p4 = i * Ny * 13 + j * 13 + 100 + 4;

    // Define the intersection point of diagonals
    x_int = (x1 + x2) / 2;
    y_int = (y1 + y2) / 2;
    Point(i * Ny * 13 + j * 13 + 100 + 5) = {x_int, y_int, 0, 1};
    p_int = i * Ny * 13 + j * 13 + 100 + 5;

    // Create triangles by connecting diagonals
    Line(i * Ny * 13 + j * 13 + 100 + 6) = {p1, p_int}; // Diagonal from p1 to intersection
    Line(i * Ny * 13 + j * 13 + 100 + 7) = {p2, p_int}; // Diagonal from p2 to intersection
    Line(i * Ny * 13 + j * 13 + 100 + 8) = {p3, p_int}; // Diagonal from p3 to intersection
    Line(i * Ny * 13 + j * 13 + 100 + 9) = {p4, p_int}; // Diagonal from p4 to intersection

    Curve Loop(i * Ny * 13 + j * 13 + 100 + 10) = {1, 2, 3, 4}

  EndFor
EndFor

// Apply meshing
Mesh 2;

// Add physical groups for boundary conditions
Physical Line("top") = {2};
Physical Line("bottom") = {4};
Physical Line("left") = {1};
Physical Line("right") = {3};
Physical Surface("Grid") = {1};
