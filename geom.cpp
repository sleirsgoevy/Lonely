#include "geom.hpp"
#include <cmath>

using namespace std;

int solve_square_equation(double a, double b, double c, double& z1, double& z2)
{
    double discrim = b * b - 4 * a * c;
    if(discrim < 0)
        return 0;
    else if(discrim == 0)
    {
        z1 = -b / 2 / a;
        return 1;
    }
    else
    {
        discrim = sqrt(discrim);
        z1 = (-b - discrim) / 2 / a;
        z2 = (-b + discrim) / 2 / a;
        return 2;
    }
}

coord ray_x_sphere(coord ray, coord sph_c, double sph_r)
{
    double square_part = ray.x * ray.x + ray.y * ray.y + 1;
    double linear_part = -2 * (ray.x * sph_c.x + ray.y * sph_c.y + ray.z * sph_c.z);
    double free_part = sph_c.x * sph_c.x + sph_c.y * sph_c.y + sph_c.z * sph_c.z - sph_r * sph_r;
    double z1, z2;
    int ans = solve_square_equation(square_part, linear_part, free_part, z1, z2);
    if(ans == 0)
        return {0, 0, -1};
    else
        return {ray.x * z1, ray.y * z1, z1};
}

matrix matrix::operator*(const matrix& other)
{
    matrix ans;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
        {
            ans.data[i][j] = 0;
            for(int k = 0; k < 3; k++)
                ans.data[i][j] += data[i][k] * other.data[k][j];
        }
    return ans;
}

double get(double* data, const coord& pos)
{
    return data[0] * pos.x + data[1] * pos.y + data[2] * pos.z;
}

coord matrix::operator*(const coord& pos)
{
    return {get(data[0], pos), get(data[1], pos), get(data[2], pos)};
}
