// jni_helpers.h

#include <stddef.h>


#ifndef JNI_LOCAL_REFS
#define JNI_LOCAL_REFS 16
#endif

#define JNI_PUSH(jenv, oomAction) \
	if (0 > (*jenv)->PushLocalFrame (jenv, JNI_LOCAL_REFS)) oomAction

#define JNI_POP(jenv) \
	(*jenv)->PopLocalFrame (jenv, NULL)

// vim: set ts=4 sw=4 noet syn=c: