/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_example_ordering_system_MainActivity */

#ifndef _Included_com_example_ordering_system_MainActivity
#define _Included_com_example_ordering_system_MainActivity
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_example_ordering_system_MainActivity
 * Method:    Print7Segment
 * Signature: (ILjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_example_ordering_1system_MainActivity_Print7Segment
  (JNIEnv *, jobject, jint, jstring);

/*
 * Class:     com_example_ordering_system_MainActivity
 * Method:    PrintLed
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_example_ordering_1system_MainActivity_PrintLed
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_example_ordering_system_MainActivity
 * Method:    GetRes
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_example_ordering_1system_MainActivity_GetRes
  (JNIEnv *, jobject);

/*
 * Class:     com_example_ordering_system_MainActivity
 * Method:    GetTouch
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_example_ordering_1system_MainActivity_GetTouch
  (JNIEnv *, jobject);

/*
 * Class:     com_example_ordering_system_MainActivity
 * Method:    GetSwitch
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_example_ordering_1system_MainActivity_GetSwitch
  (JNIEnv *, jobject);

/*
 * Class:     com_example_ordering_system_MainActivity
 * Method:    Piezo
 * Signature: (I)V
 */
JNIEXPORT jint JNICALL Java_com_example_ordering_1system_MainActivity_Piezo
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_example_ordering_system_MainActivity
 * Method:    Vibrator
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_example_ordering_1system_MainActivity_Vibrator
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_example_ordering_system_MainActivity
 * Method:    PrintTextlcd
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT jint JNICALL Java_com_example_ordering_1system_MainActivity_PrintTextlcd
  (JNIEnv *, jobject, jstring);

#ifdef __cplusplus
}
#endif
#endif
/* Header for class com_example_ordering_system_MainActivity_ScheduleJob */

#ifndef _Included_com_example_ordering_system_MainActivity_ScheduleJob
#define _Included_com_example_ordering_system_MainActivity_ScheduleJob
#ifdef __cplusplus
extern "C" {
#endif
#undef com_example_ordering_system_MainActivity_ScheduleJob_VIRGIN
#define com_example_ordering_system_MainActivity_ScheduleJob_VIRGIN 0L
#undef com_example_ordering_system_MainActivity_ScheduleJob_SCHEDULED
#define com_example_ordering_system_MainActivity_ScheduleJob_SCHEDULED 1L
#undef com_example_ordering_system_MainActivity_ScheduleJob_EXECUTED
#define com_example_ordering_system_MainActivity_ScheduleJob_EXECUTED 2L
#undef com_example_ordering_system_MainActivity_ScheduleJob_CANCELLED
#define com_example_ordering_system_MainActivity_ScheduleJob_CANCELLED 3L
#ifdef __cplusplus
}
#endif
#endif
