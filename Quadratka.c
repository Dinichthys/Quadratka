#include <stdio.h>
#include <math.h>

int roots (double, double, double, double*, double*);
void enter (double, double, int);

int main()
{
    double a, b, c, x1, x2;
    scanf ("%lf %lf %lf", &a, &b, &c);
    int result = roots (a, b, c, &x1, &x2);
    enter (x1, x2, result);
}

int roots (double a, double b, double c, double *x1, double *x2)
{
    if (a == 0){
        if (b == 0){
            if (c == 0){
                return 3;
            }else{
                return 0;
            }
        }else{
            *x1 = -c / b;
            return 4;
        }
    }
    double d = b * b - 4 * a * c;
    if (d < -0.00001){
        return 0;
    }else{
        if (d < 0.00001){
            *x1 = -b / 2;
            return 1;
        }else{
            *x1 = (-b + sqrt(d)) / 2;
            *x2 = (-b - sqrt(d)) / 2;
            return 2;
        }
    }
}

void enter (double x1, double x2, int rez)
{
    switch (rez)
    {
        case 0:
            printf ("There is no roots\n");
            break;
        case 1:
            printf ("Roots are similar and equal %.5lf\n", x1);
            break;
        case 2:
            printf ("x1 = %.5lf\nx2 = %.5lf\n", x1, x2);
            break;
        case 3:
            printf ("Roots are all real numbers\n");
            break;
        case 4:
            printf ("There is only one root because it is linear equation\nx = %.5lf\n", x1);
            break;
        default:
            printf ("Something went wrong\n");
            break;
    }
}
