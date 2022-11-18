#ifndef _TRAVERSAL_H
#define _TRAVERSAL_H

#include <iostream>
#include "ast.h"
#include <map>
#include "sym.h"
#include <algorithm>


void traverse_gradient(Start *root);

void traverse_declarations(Start *root);

void traverse_operations(Start *root);
#endif