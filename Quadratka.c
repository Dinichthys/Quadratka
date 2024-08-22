#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <getopt.h>

const int MAX_TRIES = 50;
const double ACCURACY = 0.00001;

enum SOLVE_SQUARE_RESULT
{
    NO_ROOTS  = 0,
    ONE_ROOT  = 1,
    TWO_ROOTS = 2,
    ALL       = 3,
    LINE      = 4
};

void  standard                              (FILE* stream_in, FILE* stream_out);
bool  my_is_finite                          (double x);
bool  is_equal                              (double first_number, double second_number);
bool  is_null                               (double number);
char  clear_input                           (FILE* stream);
bool  is_end_of_input                       (FILE* stream);
bool  input_coefficients                    (FILE* stream_in, FILE* stream_out, double* a, double* b, double* c);
enum  SOLVE_SQUARE_RESULT solve_lineal      (double b, double c, double* x);
enum  SOLVE_SQUARE_RESULT solve_square      (double a, double b, double c, double* x1, double* x2);
void  output_roots                          (FILE* stream, enum SOLVE_SQUARE_RESULT result, double x1, double x2);
void  run_test_v                            (FILE* stream, int number_of_test, double a, double b, double c, enum SOLVE_SQUARE_RESULT expect_result, double expect_x1, double expect_x2);
void  run_test                              (FILE* stream, int number_of_test, double a, double b, double c, enum SOLVE_SQUARE_RESULT expect_result, double expect_x1, double expect_x2);
void  run_all_tests                         (FILE* stream, bool verbose);

// ./rofl {--tests, -t|--user, -u} [-o stdout] [-i stdin] [--verbose, -v]
// TODO: command line args
int main (int argc,char **argv)
{
    const struct option modifications [] =
    {
        {"tests", 0, 0, 't'},
        {"user", 0, 0, 'u'},
        {"verbose", 0, 0, 'v'},
        {0, 0, 0, 0}
    };

    int mode = 0;
    int long_index = 0;
    FILE* stream_in = stdin;
    FILE* stream_out = stdout;
    bool test_or_user = false;
    bool verbose = false;

    while (mode != -1)
    {
        mode = getopt_long (argc, argv, "tuo:i:v", modifications, &long_index);
        switch (mode)
        {
            case 't':
            {
                test_or_user = true;
                break;
            }
            case 'u':
            {
                break;
            }
            case 'o':
            {
                stream_out = fopen (optarg, "w");
                break;
            }
            case 'i':
            {
                stream_in = fopen (optarg, "r");
                break;
            }
            case 'v':
            {
                verbose = true;
                break;
            }
            default:
            break;
        }
    }
    if (test_or_user)
    {
        run_all_tests (stream_out, verbose);
    }
    else
    {
        standard (stream_in, stream_out);
    }
    fclose (stream_in);
    fclose (stream_out);
}

void standard (FILE * const stream_in, FILE * const stream_out)
{
    assert (stream_in != NULL);
    assert (stream_out != NULL);
    assert (stream_in != stream_out);

    double a  = 0.0;
    double b  = 0.0;
    double c  = 0.0;
    double x1 = 0.0;
    double x2 = 0.0;

    if (!(input_coefficients (stream_in, stream_out, &a, &b, &c)))
    {
        return;
    }
    enum SOLVE_SQUARE_RESULT number_of_roots = solve_square (a, b, c, &x1, &x2);
    output_roots (stream_out, number_of_roots, x1, x2);
}

bool my_is_finite (double x)
{
    return ((x == x) && (x != INFINITY) && (x != -INFINITY));
}

// TODO: is_equal
bool is_equal (const double first_num, const double second_num)
{
    assert (my_is_finite (first_num));
    assert (my_is_finite (second_num));

    return (fabs(first_num - second_num) < ACCURACY);
}

bool is_null (const double number)
{
    assert (my_is_finite (number));

    return is_equal (number, 0);
}

bool is_end_of_input (FILE * const stream)
{
    assert (stream != NULL);

    char ch = getc (stream);
    return ((isspace (ch)) || (ch == EOF));
}

char clear_input (FILE * stream)
{
    assert (stream != NULL);

    char ch = '\0';
    while (((ch = getc (stream)) != '\n') && (ch != EOF))
    {
        ;
    }
    return (ch);
}

bool input_coefficients (FILE * stream_in, FILE * stream_out,  \
                        double * const a, double * const b, double * const c)
{
    assert (stream_in != NULL);
    assert (stream_out != NULL);
    assert (stream_in != stream_out);
    assert (a != NULL);
    assert (b != NULL);
    assert (c != NULL);
    assert (a != b);
    assert (a != c);
    assert (b != c);

    fprintf (stream_out, "This is the solver of square equation."
                         " Input 3 coefficients of it by real numbers separated by the space to discover the roots.\n");
    int d = 0;
    char ch = '\0';
    int tries = 0;
    while (MAX_TRIES > tries)
    {
        if (((d = fscanf (stream_in, "%lf %lf %lf", a, b, c)) == 3) && (is_end_of_input (stream_in))) // TODO: add func
        {
           return true;
        }
        if (d == EOF)
        {
            fprintf (stream_out, "There is end of input\n");
            return false;
        }
        ch = clear_input (stream_in);
        if (ch == EOF)
        {
            fprintf (stream_out, "There is end of input\n");
            return false;
        }
        fprintf (stream_out, "Incorrect input. Try again.\n");
        tries++;
    }
    fprintf (stream_out, "Restart the program and try again.\n");
    return false;
}

enum SOLVE_SQUARE_RESULT solve_lineal (const double b, const double c, double * const x)
{
    assert (my_is_finite (b));
    assert (my_is_finite (c));

    assert (x != NULL);

    if (is_null (b))
    {
        return (is_null (c)) ? ALL : NO_ROOTS;
    }
    else
    {
        *x = -c / b;
        return LINE;
    }
}

enum SOLVE_SQUARE_RESULT solve_square   (const double a, const double b, const double c, \
                                        double * const x1, double * const x2)
{
    assert (my_is_finite (a));
    assert (my_is_finite (b));
    assert (my_is_finite (c));

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
        return ONE_ROOT;
    }
    else if (d < 0)
    {
        return NO_ROOTS;
    }
    else
    {
        double sqrt_of_d = sqrt (d);
        double x = 0.0;
        *x1 = (is_null (x = (-b + sqrt_of_d) / (2 * a))) ? 0.0 : x;
        *x2 = (is_null (x = (-b - sqrt_of_d) / (2 * a))) ? 0.0 : x;
        return (is_equal (*x1, *x2)) ? ONE_ROOT : TWO_ROOTS;
    }
}

void output_roots   (FILE * const stream, \
                    const enum SOLVE_SQUARE_RESULT number_of_roots, \
                    double x1, double x2)
{
    assert (stream != NULL);
    assert (my_is_finite (x1));
    assert (my_is_finite (x2));

    switch (number_of_roots)
    {
        case NO_ROOTS:
        {
            fprintf (stream, "There is no roots\n");
            break;
        }
        case ONE_ROOT:
        {
            fprintf (stream, "Roots are similar and equal %.5lf\n", x1);
            break;
        }
        case TWO_ROOTS:
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
            fprintf (stream, "Invalid value for \"number_of_roots\" with type SOLVE_SQUARE_RESULT.\n");
            break;
        }
    }
}

void run_test_v (FILE * const stream, \
                const int number_of_test, \
                const double a, const double b, const double c, \
                const enum SOLVE_SQUARE_RESULT expect_result, \
                const double expect_x1, const double expect_x2)
{
    assert (stream != NULL);
    assert (my_is_finite (a));
    assert (my_is_finite (b));
    assert (my_is_finite (c));
    assert (my_is_finite (expect_x1));
    assert (my_is_finite (expect_x2));

    double x1 = 0.0;
    double x2 = 0.0;

    enum SOLVE_SQUARE_RESULT result = solve_square (a, b, c, &x1, &x2);

    if ((result == TWO_ROOTS) && ((x1 - x2) < ACCURACY))
    {
        double change = x1;
        x1 = x2;
        x2 = change;
    }

    if (result != expect_result)
    {
        fprintf (stream, ":  Error in test № %d.     :\n"
                         " The coefficients:\n"
                         " a :\n"
                         " %.5lf\n"
                         " b :\n"
                         " %.5lf\n"
                         " c :\n"
                         " %.5lf\n"
                         " Expected result:\n"
                         " %d\n"
                         " Real result:\n"
                         " %d\n"
                         "____________________________\n"
                         ,number_of_test, a, b, c, expect_result, result);
        return;
    }
    if ((!(is_null (x1 - expect_x1))) || (!(is_null (x2 - expect_x2))))
    {
    fprintf (stream, ":  Error in test № %d      :\n"
                     " The coefficients:\n"
                     " a :\n"
                     " %.5lf\n"
                     " b :\n"
                     " %.5lf\n"
                     " c :\n"
                     " %.5lf\n"
                     " The result:\n"
                     " %d\n"
                     " The expected roots:\n"
                     " %.5lf %.5lf\n"
                     " The real roots:\n"
                     " %.5lf %.5lf\n"
                     "____________________________\n"
                     , number_of_test, a, b, c, expect_result, expect_x1, expect_x2, x1, x2);
        return;
    }
    fprintf (stream, ":  Test № %d is succeeded  :\n"
                     " The coefficients:\n"
                     " a :\n"
                     " %.5lf\n"
                     " b :\n"
                     " %.5lf\n"
                     " c :\n"
                     " %.5lf\n"
                     " The result:\n"
                     " %d\n"
                     " The roots:\n"
                     " %.5lf %.5lf\n"
                     "____________________________\n"
                     , number_of_test, a, b, c, expect_result, expect_x1, expect_x2);
}

void run_test   (FILE * const stream, \
                const int number_of_test, \
                const double a, const double b, const double c, \
                const enum SOLVE_SQUARE_RESULT expect_result, \
                double expect_x1, double expect_x2)
{
    assert (stream != NULL);
    assert (my_is_finite (a));
    assert (my_is_finite (b));
    assert (my_is_finite (c));
    assert (my_is_finite (expect_x1));
    assert (my_is_finite (expect_x2));

    double x1 = 0.0;
    double x2 = 0.0;

    enum SOLVE_SQUARE_RESULT result = solve_square (a, b, c, &x1, &x2);

    if ((result == TWO_ROOTS) && ((x1 - x2) < ACCURACY))
    {
        double change = x1;
        x1 = x2;
        x2 = change;
    }

    if (result != expect_result)
    {
        fprintf (stream, ":  Error in test № %d.     :\n"
                         " Expected result:\n"
                         " %d\n"
                         " Real result:\n"
                         " %d\n"
                         "____________________________\n",
                         number_of_test, expect_result, result);
        return;
    }

    if ((!(is_null (x1 - expect_x1))) || (!(is_null (x2 - expect_x2))))
    {
        fprintf (stream, ":  Error in test № %d.     :\n"
                         " Expected roots:\n"
                         " %.5lf %.5lf\n"
                         " Real roots:\n"
                         " %.5lf %.5lf\n"
                         "____________________________\n",
                         number_of_test, expect_x1, expect_x2, x1, x2);
        return;
    }

    fprintf (stream, ":  Test № %d is succeeded  :\n", number_of_test);
}

void run_all_tests (FILE * const stream, const bool verbose)
{
    assert (stream != NULL);

    int number_of_test = 1;

    if (verbose)
    {
        run_test_v (stream, number_of_test++, 0,   0,   0,   ALL,       0,    0  );
        run_test_v (stream, number_of_test++, 0,   0,   3.5, NO_ROOTS,  0,    0  );
        run_test_v (stream, number_of_test++, 0,   10,  4,   LINE,      -0.4, 0  );
        run_test_v (stream, number_of_test++, 0,   3,   0,   LINE,      0,    0  );
        run_test_v (stream, number_of_test++, 2.5, 5,   2.5, ONE_ROOT,  -1,   0  );
        run_test_v (stream, number_of_test++, 1,   -7,  12,  TWO_ROOTS, 4,    3  );
        run_test_v (stream, number_of_test++, 4,   -16, 15,  TWO_ROOTS, 2.5,  1.5);
        run_test_v (stream, number_of_test++, -2,   5,  0,   TWO_ROOTS, 2.5,  0  );
    }
    else
    {
        run_test (stream, number_of_test++, 0,   0,   0,   ALL,       0,    0  );
        run_test (stream, number_of_test++, 0,   0,   3.5, NO_ROOTS,  0,    0  );
        run_test (stream, number_of_test++, 0,   10,  4,   LINE,      -0.4, 0  );
        run_test (stream, number_of_test++, 0,   3,   0,   LINE,      0,    0  );
        run_test (stream, number_of_test++, 2.5, 5,   2.5, ONE_ROOT,  -1,   0  );
        run_test (stream, number_of_test++, 1,   -7,  12,  TWO_ROOTS, 4,    3  );
        run_test (stream, number_of_test++, 4,   -16, 15,  TWO_ROOTS, 2.5,  1.5);
        run_test (stream, number_of_test++, -2,   5,  0,   TWO_ROOTS, 2.5,  0  );
    }

    fprintf  (stream, "All tests are done\n");
}
