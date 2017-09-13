#ifndef _GEOM_H
#define _GEOM_H
struct coord
{
    double x;
    double y;
    double z;
};

int solve_square_equation(double a, double b, double c, double& z1, double& z2);

coord ray_x_sphere(coord ray, coord sph_c, double sph_r);
#endif
