# Lexical Analysis

[![Lexer Build Status](https://github.com/IITH-COMPILERS2/compilers-2-project-team-9-aug22/actions/workflows/lexer.yml/badge.svg)](https://github.com/IITH-COMPILERS2/compilers-2-project-team-9-aug22/actions/workflows/lexer.yml)

## Introduction

Lexical analysis is the process of converting a string of characters into a sequence of tokens. A token is a string with an assigned and thus identified meaning. The sequence of tokens is called a token stream. The token stream is used as input for the parser.


## Lexical Analysis

<!-- ### Regular Expressions -->

<!-- ### Finite Automata -->

### Lexical Analyzer
Recognizes all basic keywords such as cns,var,Tensor etc and operators used by our language 

### Error Handling
Our lexer can recognize unclosed comments ,strings and overflows in constant values
### Testing
go inside the Lexer folder and then run
```console
make 
make test 
```

This should run the test cases and report their results
## References
 C Lexer
