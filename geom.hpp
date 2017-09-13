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

struct matrix
{
    double data[3][3];
    inline matrix()
    {
        for(int i = 0; i < 3; i++)
            for(int j = 0; j < 3; j++)
                data[i][j] = (i == j)?1:0;
    }
    inline matrix(double a, double b, double c, double d, double e, double f, double g, double h, double i)
    {
        data[0][0] = a;
        data[0][1] = b;
        data[0][2] = c;
        data[1][0] = d;
        data[1][1] = e;
        data[1][2] = f;
        data[2][0] = g;
        data[2][1] = h;
        data[2][2] = i;
    }
    matrix operator*(const matrix& other);
    coord operator*(const coord& pos);
};
#endif
