#ifndef _Base64_H__
#define _Base64_H__
 
#include <stdio.h>
 
char *base64_encode(const char *input, const size_t length,char *output);
char *base64_decode(const char *input,char *output);
 
#endif
