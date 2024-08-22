#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>

const int MAX_TRIES = 50;
const double ACCURACY = 0.00001;

struct settings_of_command
{
    FILE* stream_in;
    FILE* stream_out;
    bool test_or_user;
    bool verbose;
} set;


enum SOLVE_SQUARE_RESULT
{
    NO_ROOTS  = 0,
    ONE_ROOT  = 1,
    TWO_ROOTS = 2,
    ALL       = 3,
    LINE      = 4
};

struct pattern_for_tests
{
    double a;
    double b;
    double c;
    enum SOLVE_SQUARE_RESULT expect_result;
    double expect_x1;
    double expect_x2;
};

void  settings                              (int arg_count, char **arg_v);
void  user_version                          (FILE* stream_in, FILE* stream_out);
bool  my_is_finite                          (double x);
bool  is_equal                              (double first_number, double second_number);
bool  is_null                               (double number);
char  clear_input                           (FILE* stream);
bool  is_end_of_input                       (FILE* stream);
bool  input_coefficients                    (FILE* stream_in, FILE* stream_out, double* a, double* b, double* c);
enum  SOLVE_SQUARE_RESULT solve_lineal      (double b, double c, double* x);
enum  SOLVE_SQUARE_RESULT solve_square      (double a, double b, double c, double* x1, double* x2);
void  output_roots                          (FILE* stream, enum SOLVE_SQUARE_RESULT result, double x1, double x2);
void  swap                                  (enum SOLVE_SQUARE_RESULT result, double* x1, double* x2);
void  run_test_v                            (FILE * const stream, const int number_of_test, struct pattern_for_tests pattern);
void  run_test                              (FILE * const stream, const int number_of_test, struct pattern_for_tests pattern);
void  run_all_tests                         (FILE* stream, bool verbose);

// ./rofl {--tests, -t|--user, -u} [-o stdout] [-i stdin] [--verbose, -v]
int main (int argc, char **argv)
{
    set.stream_in = stdin;
    set.stream_out = stdout;
    set.test_or_user = false;
    set.verbose = false;

    settings (argc, argv);

    if (set.test_or_user)
    {
        run_all_tests (set.stream_out, set.verbose);
    }
    else
    {
        user_version (set.stream_in, set.stream_out);
    }
    fclose (set.stream_in);
    fclose (set.stream_out);
}

void settings (int argc, char **argv)
{
    assert (argv != NULL);

    const struct option modifications [] =
    {
        {"tests",   0, 0, 't'},
        {"user",    0, 0, 'u'},
        {"verbose", 0, 0, 'v'},
        {"help",    0, 0, 'h'},
        {0,         0, 0,  0 }
    };

    int mode = 0;
    int long_index = 0;

    while (mode != -1)
    {
        mode = getopt_long (argc, argv, "tuo:i:vh", modifications, &long_index);
        switch (mode)
        {
            case 't':
            {
                set.test_or_user = true;
                break;
            }
            case 'u':
            {
                break;
            }
            case 'o':
            {
                set.stream_out = fopen (optarg, "w");
                break;
            }
            case 'i':
            {
                set.stream_in = fopen (optarg, "r");
                break;
            }
            case 'v':
            {
                set.verbose = true;
                break;
            }
            case 'h':
            {
                printf (" There is list of parameters of that program.\n"
                        " \"--test\" or \"-t\"    :"
                        " the program is starting testing by the tests.\n"
                        " \"--user\" or \"-u\"    :"
                        " the program is starting working with user.\n"
                        " \"-i\"                :"
                        " you can change the stream of input and write the name of place of input after it.\n"
                        " \"-o\"                :"
                        " you can change the stream of output and write the name of place of output after it.\n"
                        " \"--verbose\" or \"-v\" :"
                        " makes the analytic investigation verbose.\n"); //fixed
                exit(0);
            }
            case -1:
            {
                break;
            }
            default:
                assert (0 && "Invalid name of command in terminal.\n");
                break;
        }
    }
}

void user_version (FILE * const stream_in, FILE * const stream_out)
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
//fixed
bool my_is_finite (double x)
{
    return ((x == x) && (x * 0 == x * 0));
}

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
//fixed
    char ch = '\0';
    while ((isspace (ch = getc (stream))) && (ch != '\n'))
    {
        ;
    }
    return ((ch == '\n') || (ch == EOF));
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
                         " Input 3 coefficients of it by real numbers"
                         " separated by the space to discover the roots.\n");
    int d = 0;
    char ch = '\0';
    int tries = 0;
    while (tries < MAX_TRIES)
    {
        if (((d = fscanf (stream_in, "%lf %lf %lf", a, b, c)) == 3) && (is_end_of_input (stream_in)))
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

    *x = -c / b;
    return LINE;
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

void swap (const enum SOLVE_SQUARE_RESULT result, double * const x1, double * const x2)
{
    assert (x1 != NULL);
    assert (x2 != NULL);
    assert (x1 != x2);

    if ((result == TWO_ROOTS) && ((*x1 - *x2) < ACCURACY))
    {
        double change = *x1;
        *x1 = *x2;
        *x2 = change;
    }
    return;
}

void run_test_v (FILE * const stream, const int number_of_test, struct pattern_for_tests pattern) //fixed
{
    assert (stream != NULL);
    assert (my_is_finite (pattern.a));
    assert (my_is_finite (pattern.b));
    assert (my_is_finite (pattern.c));
    assert (my_is_finite (pattern.expect_x1));
    assert (my_is_finite (pattern.expect_x2));

    double x1 = 0.0;
    double x2 = 0.0;

    enum SOLVE_SQUARE_RESULT result = solve_square (pattern.a, pattern.b, pattern.c, &x1, &x2);

    swap (result, &x1, &x2);

    if ((result != pattern.expect_result) ||
       (!(is_null (x1 - pattern.expect_x1))) ||
       (!(is_null (x2 - pattern.expect_x2))))
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
                         ,number_of_test, pattern.a, pattern.b, pattern.c);
        output_roots (stream, pattern.expect_result, pattern.expect_x1, pattern.expect_x2);
        fprintf (stream, " Real result:\n");
        output_roots (stream, result, x1, x2);
        fprintf (stream, "____________________________\n");
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
                     ,number_of_test, pattern.a, pattern.b, pattern.c);
    output_roots (stream, pattern.expect_result, pattern.expect_x1, pattern.expect_x2);
    fprintf (stream, "____________________________\n");
}

void run_test (FILE * const stream, const int number_of_test, struct pattern_for_tests pattern)
{
    assert (stream != NULL);
    assert (my_is_finite (pattern.a));
    assert (my_is_finite (pattern.b));
    assert (my_is_finite (pattern.c));
    assert (my_is_finite (pattern.expect_x1));
    assert (my_is_finite (pattern.expect_x2));

    double x1 = 0.0;
    double x2 = 0.0;

    enum SOLVE_SQUARE_RESULT result = solve_square (pattern.a, pattern.b, pattern.c, &x1, &x2);

    swap (result, &x1, &x2);

    if ((result != pattern.expect_result) ||
       (!(is_null (x1 - pattern.expect_x1))) ||
       (!(is_null (x2 - pattern.expect_x2))))
    {
        fprintf (stream, ":  Error in test № %d.     :\n"
                         " Expected result:\n"
                         ,number_of_test);
        output_roots (stream, pattern.expect_result, pattern.expect_x1, pattern.expect_x2);
        fprintf (stream, " Real result:\n");
        output_roots (stream, result, x1, x2);
        return;
    }
    fprintf (stream, ":  Test № %d is succeeded  :\n", number_of_test);
}
//fixed
void run_all_tests (FILE * const stream, const bool verbose)
{
    assert (stream != NULL);

    int number_of_test = 1;

    void (*print) (FILE*, int, struct pattern_for_tests) = run_test;

    if (verbose){
        print = run_test_v;
    }

    print (stream, number_of_test++, (struct pattern_for_tests) { 0,   0,   0,   ALL,       0,    0  });
    print (stream, number_of_test++, (struct pattern_for_tests) { 0,   0,   3.5, NO_ROOTS,  0,    0  });
    print (stream, number_of_test++, (struct pattern_for_tests) { 0,   10,  4,   LINE,      -0.4, 0  });
    print (stream, number_of_test++, (struct pattern_for_tests) { 0,   3,   0,   LINE,      0,    0  });
    print (stream, number_of_test++, (struct pattern_for_tests) { 2.5, 5,   2.5, ONE_ROOT,  -1,   0  });
    print (stream, number_of_test++, (struct pattern_for_tests) { 1,   -7,  12,  TWO_ROOTS, 4,    3  });
    print (stream, number_of_test++, (struct pattern_for_tests) { 4,   -16, 15,  TWO_ROOTS, 2.5,  1.5});
    print (stream, number_of_test++, (struct pattern_for_tests) { -2,   5,  0,   TWO_ROOTS, 2.5,  0  });
    fprintf (stream, "All tests are done\n");
}
