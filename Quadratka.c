#include <stdio.h>
#include <math.h>

enum REZ
{
    NOROOTS = 0,
    ONEROOT = 1,
    TWOROOTS = 2,
    ALL = 3,
    LINE = 4
};

int input (double*, double*, double*);
enum REZ roots (double, double, double, double*, double*);
void enter (double, double, enum REZ);

int main()
{
    double a = 0.0, b = 0.0, c = 0.0, x1 = 0.0, x2 = 0.0;
    if (input (&a, &b, &c) == 0){
     return 0;
    }
    enum REZ result = roots (a, b, c, &x1, &x2);
    enter (x1, x2, result);
}

int input (double *a, double *b, double *c)
{
    printf ("Input 3 real numbers separated by the space\n");
    int d;
    if ((d = scanf ("%lf %lf %lf", a, b, c)) != 3)
    {
        if (d != EOF)
        {
            printf("Incorrect input. Try again.\n");
            char ch;
            while (((ch = getc(stdin)) != '\n') && (ch != EOF))
            {
            }
            if (ch == '\n')
            {
                if (scanf ("%lf %lf %lf", a, b, c) != 3)
                {
                    printf("Incorrect input again\n");
                    return 0;
                }
                else
                {
                    return 3;
                }
            }
            else
            {
                printf("There is end of input\n");
                return 0;
            }
        }
        else
        {
            printf("There is end of input\n");
            return 0;
        }
    }
    else
    {
        return 3;
    }
}

enum REZ roots (double a, double b, double c, double *x1, double *x2)
{
    if (a == 0)
    {
        if (b == 0)
        {
            if (c == 0)
            {
                return ALL;
            }
            else
            {
                return NOROOTS;
            }
        }
        else
        {
            *x1 = -c / b;
            return LINE;
        }
    }
    double d = b * b - 4 * a * c;
    if (d < -0.00001)
    {
        return NOROOTS;
    }
    else
    {
        if (d < 0.00001)
        {
            *x1 = -b / 2;
            return ONEROOT;
        }
        else
        {
            *x1 = (-b + sqrt(d)) / (2 * a);
            *x2 = (-b - sqrt(d)) / (2 * a);
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
