#ifndef _VM_GRAMMAR_H
#define _VM_GRAMMAR_H
#define VM_END_SENTENCE ";"
#define VM_VARIABLE_NAME "[a-zA-Z]{1}[a-zA-Z0-9]*"
#define VM_UINT_VALUE "[0-9]{1}[0-9]*"
#define VM_STRING_VALUE "\".{1}.*\""
#define VM_UINT_ASSIGNMENT "\\s*"VM_VARIABLE_NAME"\\s*=\\s*"VM_UINT_VALUE"\\s*"VM_END_SENTENCE
#define VM_STRING_ASSIGNMENT "\\s*"VM_VARIABLE_NAME"\\s*=\\s*"VM_STRING_VALUE"\\s*"VM_END_SENTENCE
#endif
