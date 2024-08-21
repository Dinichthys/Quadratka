#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define MODE 5

enum SOLVE_SQUARE_RESULT
{
    NOROOTS  = 0,
    ONEROOT  = 1,
    TWOROOTS = 2,
    ALL      = 3,
    LINE     = 4
};

bool  is_smaller                            (double number_1, double number_2);
bool  is_null                               (double number);
char  white_screen                          (FILE* stream);
bool  input_coefficients                    (FILE* stream_in, FILE* stream_out, double* a, double* b, double* c);
enum  SOLVE_SQUARE_RESULT solve_lineal      (double b, double c, double* x);
enum  SOLVE_SQUARE_RESULT solve_square      (double a, double b, double c, double* x1, double* x2);
void  output_roots                          (FILE* stream, enum SOLVE_SQUARE_RESULT result, double x1, double x2);
void  run_test                              (FILE* stream, int number_of_test, double a, double b, double c, enum SOLVE_SQUARE_RESULT expect_result, double expect_x1, double expect_x2);
void  run_all_test                          (FILE* stream);

int main ()
{
    double a  = 0.0;
    double b  = 0.0;
    double c  = 0.0;
    double x1 = 0.0;
    double x2 = 0.0;
    char word [MODE];
    fprintf (stdout, "Choose the mode of testing the program. Write \"Test\" or \"User\".\n");
    fgets (word, MODE, stdin);
    if (strcmp (word, "Test") == 0)
    {
        run_all_test (stdout);
    }
    else if (strcmp (word, "User") == 0)
    {
        if (!(input_coefficients (stdin, stdout, &a, &b, &c)))
        {
            return 0;
        }
        enum SOLVE_SQUARE_RESULT number_of_roots = solve_square (a, b, c, &x1, &x2);
        output_roots (stdout, number_of_roots, x1, x2);
    }
    else
    {
        printf ("Invalid name for mode. Restart the program.\n");
    }
}

bool is_smaller (const double number_1, const double number_2)
{
    assert (isfinite (number_1));
    assert (isfinite (number_2));

    return (number_1 < number_2);
}

bool is_null (const double number)
{
    assert (isfinite (number));

    if (is_smaller (fabs (number), 0.00001))
    {
        return true;
    }
    return false;
}

char white_screen (FILE* stream)
{
    char ch = '\0';
    while (((ch = getc (stream)) != '\n') && (ch != EOF))
    {
    }
    ungetc (ch, stream);
    return (ch);
}

bool input_coefficients (FILE* stream_in, FILE* stream_out,  double * const a, double * const b, double * const c)
{
    assert (a != NULL);
    assert (b != NULL);
    assert (c != NULL);
    assert (a != b);
    assert (a != c);
    assert (b != c);

    fprintf (stream_out, "This is the solver of square equation. Input 3 coefficients of it by real numbers separated by the space to discover the roots.\n");
    int d = 0;
    char ch = '\0';
    if (((d = fscanf (stream_in, "%lf %lf %lf", a, b, c)) == 3) && ((isspace (ch = getc (stream_in))) || (ch == EOF)))
    {
        return true;
    }
    if (d == EOF)
    {
        fprintf (stream_out, "There is end of input\n");
        return false;
    }
    int tries = 50;
    while (tries > 0)
    {
        fprintf (stream_out, "Incorrect input. Try again.\n");
        ch = white_screen (stream_in);
        if (ch == EOF)
        {
            fprintf (stream_out, "There is end of input\n");
            return false;
        }
        if (((d = fscanf (stream_in, "%lf %lf %lf", a, b, c)) == 3) && ((isspace (ch = getc (stream_in))) || (ch == EOF)))
        {
           return true;
        }
        if (d == EOF)
        {
            fprintf (stream_out, "There is end of input\n");
            return false;
        }
        tries--;
    }
    fprintf (stream_out, "Restart the program and try again.\n");
    return false;
}

enum SOLVE_SQUARE_RESULT solve_lineal (const double b, const double c, double * const x)
{
    assert (isfinite (b));
    assert (isfinite (c));

    assert (x != NULL);

    if (is_null (b))
    {
        if (is_null (c))
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
        *x = -c / b;
        return LINE;
    }
}

enum SOLVE_SQUARE_RESULT solve_square (const double a, const double b, const double c, double * const x1, double * const x2)
{
    assert (isfinite (a));
    assert (isfinite (b));
    assert (isfinite (c));

    assert (x1 != NULL);
    assert (x2 != NULL);
    assert (x1 != x2);

    if (is_null (a))
    {
        return solve_lineal (b, c, x1);
    }
    double d = b * b - 4 * a * c;
    if (is_null (d))
    {
        *x1 = -b / (2 * a);
        if (is_null (*x1))
        {
            *x1 = 0.0;
        }
        return ONEROOT;
    }
    else if (is_smaller (d, 0))
    {
        return NOROOTS;
    }
    else
    {
        double sqrt_of_d = sqrt (d);
        *x1 = (-b + sqrt_of_d) / (2 * a);
        *x2 = (-b - sqrt_of_d) / (2 * a);
        if (is_null (*x1))
        {
            if (is_null (*x2))
            {
                return ONEROOT;
            }
            *x1 = 0.0;
        }
        else if (is_null (*x2))
        {
            *x2 = 0.0;
        }
        return TWOROOTS;
    }
}

void output_roots (FILE* stream, const enum SOLVE_SQUARE_RESULT number_of_roots, const double x1, const double x2)
{
    assert (isfinite (x1));
    assert (isfinite (x2));

    switch (number_of_roots)
    {
        case NOROOTS:
        {
            fprintf (stream, "There is no roots\n");
            break;
        }
        case ONEROOT:
        {
            fprintf (stream, "Roots are similar and equal %.5lf\n", x1);
            break;
        }
        case TWOROOTS:
        {
            fprintf (stream, "There are 2 roots\nx1 = %.5lf\nx2 = %.5lf\n", x1, x2);
            break;
        }
        case ALL:
        {
            fprintf (stream, "Roots are all real numbers\n");
            break;
        }
        case LINE:
        {
            fprintf (stream, "There is only one root because it is lineal equation\nx = %.5lf\n", x1);
            break;
        }
        default:
        {
            assert(0 && "Invalid SOLVE_SQUARE_RESULT value");
            fprintf (stream, "Something went wrong\n");
            break;
        }
    }
}

void  run_test (FILE* stream, const int number_of_test, const double a, const double b, const double c, const enum SOLVE_SQUARE_RESULT expect_result, const double expect_x1, const double expect_x2)
{
    assert (isfinite (a));
    assert (isfinite (b));
    assert (isfinite (c));
    assert (isfinite (expect_x1));
    assert (isfinite (expect_x2));

    double x1 = 0.0;
    double x2 = 0.0;
    enum SOLVE_SQUARE_RESULT result = solve_square (a, b, c, &x1, &x2);
    if ((result != expect_result) || (!(is_null (x1 - expect_x1))) || (!(is_null (x2 - expect_x2))))
    {
        fprintf (stream, ":  Error in test № %d.     :\n Expected result:\n %d\n Expected roots:\n %.5lf %.5lf\n Real result:\n %d\n Real roots:\n %.5lf %.5lf\n", number_of_test, expect_result, expect_x1, expect_x2, result, x1, x2);
    }
    else
    {
        fprintf (stream, ":  Test № %d is succeeded  :\n", number_of_test);
    }
}

void run_all_test (FILE* stream)
{
    int number_of_test = 1;
    run_test (stream, number_of_test++, 0,   0,   0,   ALL,      0,    0  );
    run_test (stream, number_of_test++, 0,   0,   3.5, NOROOTS,  0,    0  );
    run_test (stream, number_of_test++, 0,   10,  4,   LINE,     -0.4, 0  );
    run_test (stream, number_of_test++, 0,   3,   0,   LINE,     0,    0  );
    run_test (stream, number_of_test++, 2.5, 5,   2.5, ONEROOT,  -1,   0  );
    run_test (stream, number_of_test++, 1,   -7,  12,  TWOROOTS, 4,    3  );
    run_test (stream, number_of_test++, 4,   -16, 15,  TWOROOTS, 2.5,  1.5);
    fprintf  (stream, "All test are done\n");
}
