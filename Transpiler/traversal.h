#ifndef _TRAVERSAL_H
#define _TRAVERSAL_H

#include <iostream>
#include "ast.h"
#include <map>
#include "sym.h"
#include <algorithm>

void traverse_declarations(std::vector<Decl *> *DeclList);

void traverse_gradient(std::vector<GradStmt *> *GradStmtList);

void traverse_gradient2(Start *root);

void traverse_declarations2(Start *root);

void traverse_operations(Start *root);
#endif