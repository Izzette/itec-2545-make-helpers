// input.c

#include <stddef.h>
#include <stdbool.h>

#include <alloca.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <jni.h>

#include <input.h>

// Returns NULL on error and throws either OutOfMemoryError or IOException.
char *string_input (JNIEnv *jenv, const char *prompt, size_t *length) {
	jclass oomErrorClass = (*jenv)->FindClass (jenv, "Ljava/lang/OutOfMemoryError;");
	jclass ioExceptClass = (*jenv)->FindClass (jenv, "Ljava/io/IOException;");

	if (!oomErrorClass || !ioExceptClass)
		return NULL;

	char *ln = NULL;
	size_t ln_alloced = 0;
	ssize_t ln_length;

	printf ("%s: ", prompt);
	fflush (stdout);

	ln_length = getline (&ln, &ln_alloced, stdin);

	if (0 <= ln_length) {
		ln[ln_length - 1] = '\0';
		ln = realloc (ln, ln_length);
		if (length) *length = ln_length - 1;
		return ln;
	}

	if (feof (stdin)) {
		clearerr (stdin);
		(*jenv)->ThrowNew (
			jenv, ioExceptClass, "Received EOF reading user input");
	} else {
		errno = 0;
		(*jenv)->ThrowNew (
			jenv, oomErrorClass, "Failed to (re-)allocated getline buffer");
	}

	if (ln) free (ln);

	return NULL;
}

// Throws exception on error.
double double_input (JNIEnv *jenv, const char *prompt) {
	for (;;) {
		size_t inlength = 0;
		char *instr = string_input (jenv, prompt, &inlength);
		if (!instr)
			return 0.;

		char *endptr;
		double result = strtod (instr, &endptr);
		free (instr);

		if (inlength != (size_t)(endptr - instr)) {
			puts ("Not a valid double, try again!");
			continue;
		}

		return result;
	}
}

// Throws exception on error.
int int_input (JNIEnv *jenv, const char *prompt) {
	for (;;) {
		size_t inlength = 0;
		char *instr = string_input (jenv, prompt, &inlength);

		char *endptr;
		int result = (int)strtol (instr, &endptr, 10);
		free (instr);

		if (inlength != (size_t)(endptr - instr)) {
			puts ("Not a valid int, try again!");
			continue;
		}

		return result;
	}
}

// Throws exception on error.
bool yes_no_input (JNIEnv *jenv, const char *prompt) {
	char *new_prompt = alloca (strlen (prompt) + 6 + 1);
	strcpy (new_prompt, prompt);
	strcat (new_prompt, " [Y/n]");

	for (;;) {
		char *instr = string_input (jenv, new_prompt, NULL);
		if (!instr)
			return 0.;

		// Read the first character only.
		char inc = *instr;
		free (instr);

		switch (inc) {
			case 'y':
			case 'Y':
			case '\0':
				return true;
			case 'n':
			case 'N':
				return false;
			default:
				puts ("Not a valid response, try again!");
				continue;
		}
	}
}

// vim: set ts=4 sw=4 noet syn=c:
