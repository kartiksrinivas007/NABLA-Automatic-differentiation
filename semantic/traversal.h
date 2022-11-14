#ifndef _TRAVERSAL_H
#define _TRAVERSAL_H

#include <iostream>
#include "../ast/ast.h"
#include <map>
#include "../Symbol_table/sym.h"

void traverse_declarations(std::vector<Decl *> *DeclList);

void traverse_gradient(std::vector<GradStmt *> *GradStmtList);

void traverse_gradient2(Start *root);

void traverse_declarations2(Start *root);

#endif