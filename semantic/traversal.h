#ifndef _TRAVERSAL_H
#define _TRAVERSAL_H

#include <iostream>
#include "../ast/ast.h"
#include <map>

void traverse_declarations(std::vector<Decl *> *DeclList);

void traverse_gradient(std::vector<GradStmt *> *GradStmtList);

#endif