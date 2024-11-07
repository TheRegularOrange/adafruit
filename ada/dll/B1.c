#include <jni.h>
#include <stdio.h>
#include "B1.h"

// Add function
JNIEXPORT jint JNICALL Java_B1_add(JNIEnv *env, jobject obj, jint a, jint b)
{
    printf("\n%d + %d = %d\n", a, b, (a + b));
    return (a + b);
}

// Subtract function
JNIEXPORT jint JNICALL Java_B1_sub(JNIEnv *env, jobject obj, jint a, jint b)
{
    printf("\n%d - %d = %d\n", a, b, (a - b));
    return (a - b);
}

// Multiply function
JNIEXPORT jint JNICALL Java_B1_mult(JNIEnv *env, jobject obj, jint a, jint b)
{
    printf("\n%d * %d = %d\n", a, b, (a * b));
    return (a * b);
}

// Divide function
JNIEXPORT jint JNICALL Java_B1_div(JNIEnv *env, jobject obj, jint a, jint b)
{
    if (b == 0) {
        printf("\nDivision by zero error\n");
        return 0;  // or handle error as desired
    }
    printf("\n%d / %d = %d\n", a, b, (a / b));
    return (a / b);
}
