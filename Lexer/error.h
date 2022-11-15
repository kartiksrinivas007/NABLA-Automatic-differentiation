#ifndef ERROR_H
#define ERROR_H

#include <string.h>
#include <stdio.h>

void yyerror(const char * s){
    fprintf(stderr, "Error: %s\n", s);
}

void warning(const char * s){
    fprintf(stderr, "Warning: %s\n", s);
}

void check_integer_overflow(const char * yytext)
{
    int is_neg = 0, l1, l2, overflow_detected = 0, n = 0;
    char int_max[] = "2147483647", int_min[] = "-2147483648";
    if (yytext[0] == '-')
        is_neg = 1;

    l1 = strlen(yytext);
    if (is_neg)
        l2 = strlen(int_min);
    else
        l2 = strlen(int_max);

    // If number is negative
    if (is_neg)
    {
        if (l2 < l1)
            overflow_detected = 1;
        else if (l2 == l1)
        {
            for (n = 1; n < l1; n++)
            {
                if (int_min[n] < yytext[n])
                    overflow_detected = 1;
            }
        }
    }

    if (l1 > l2)
        overflow_detected = 1;
    else if (l2 == l1)
    {
        for (n = 0; n < l1; n++)
        {
            if (yytext[n] > int_max[n])
                overflow_detected = 1;
        }
    }

    if (overflow_detected)
    {
        warning("Integer Overflow, number will not fit into 32-bit integer constant");
    }
}

#endif // ERROR_H