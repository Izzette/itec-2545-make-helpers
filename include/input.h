// input.h

#include <stddef.h>
#include <stdbool.h>

#include <jni.h>

char *string_input (JNIEnv *, const char *, size_t *);
double double_input (JNIEnv *, const char *);
int int_input (JNIEnv *, const char *);
bool yes_no_input (JNIEnv *, const char *);

// vim: set ts=4 sw=4 noet syn=c: