#ifndef _TRAVERSAL_H
#define _TRAVERSAL_H

#include <iostream>
#include "ast.h"
#include <map>
#include "sym.h"
#include <algorithm>

extern char filename[50];

void traverse_gradient(Start *root);

void traverse_declarations(Start *root);

void traverse_operations(Start *root);

int semantic_error(std::string, int a = 0, int b = 0, bool print_line = false);

std::string get_line(int);

#endif