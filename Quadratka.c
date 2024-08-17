#include <stdio.h>
#include <math.h>

enum REZ
{
    NOROOTS, ONEROOT, TWOROOTS, ALL, LINE
};

enum REZ roots (double, double, double, double*, double*);
void enter (double, double, enum REZ);

int main()
{
    double a, b, c, x1, x2;
    scanf ("%lf %lf %lf", &a, &b, &c);
    enum REZ result = roots (a, b, c, &x1, &x2);
    enter (x1, x2, result);
}

enum REZ roots (double a, double b, double c, double *x1, double *x2)
{
    if (a == 0){
        if (b == 0){
            if (c == 0){
                return ALL;
            }else{
                return NOROOTS;
            }
        }else{
            *x1 = -c / b;
            return LINE;
        }
    }
    double d = b * b - 4 * a * c;
    if (d < -0.00001){
        return NOROOTS;
    }else{
        if (d < 0.00001){
            *x1 = -b / 2;
            return ONEROOT;
        }else{
            *x1 = (-b + sqrt(d)) / 2;
            *x2 = (-b - sqrt(d)) / 2;
            return TWOROOTS;
        }
    }
}

void enter (double x1, double x2, enum REZ result)
{
    switch (result)
    {
        case NOROOTS:
            printf ("There is no roots\n");
            break;
        case ONEROOT:
            printf ("Roots are similar and equal %.5lf\n", x1);
            break;
        case TWOROOTS:
            printf ("x1 = %.5lf\nx2 = %.5lf\n", x1, x2);
            break;
        case ALL:
            printf ("Roots are all real numbers\n");
            break;
        case LINE:
            printf ("There is only one root because it is linear equation\nx = %.5lf\n", x1);
            break;
        default:
            printf ("Something went wrong\n");
            break;
    }
}
