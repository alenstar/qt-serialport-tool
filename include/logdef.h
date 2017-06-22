#ifndef LOGDEF_H_
#define LOGDEF_H_
#include <stdio.h>
#include <string.h>
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#if 1 //def DEBUG
#if 1
#define LOGD(fmt, ...) do{printf("%s(%3d) [D]: " fmt "\n", __FILENAME__, __LINE__, ##__VA_ARGS__);}while(0)
#define LOGE(fmt, ...) do{printf("%s(%3d) [E]: " fmt "\n", __FILENAME__, __LINE__, ##__VA_ARGS__);}while(0)
#define LOGW(fmt, ...) do{printf("%s(%3d) [W]: " fmt "\n", __FILENAME__, __LINE__, ##__VA_ARGS__);}while(0)
#define LOGI(fmt, ...) do{printf("%s(%3d) [I]: " fmt "\n", __FILENAME__, __LINE__, ##__VA_ARGS__);}while(0)
#else
#define LOGD(...)
#define LOGE(fmt, ...) do{printf("%s(%3d) [E]: " fmt "\n", __FILENAME__, __LINE__, ##__VA_ARGS__);}while(0)
#define LOGW(fmt, ...) do{printf("%s(%3d) [W]: " fmt "\n", __FILENAME__, __LINE__, ##__VA_ARGS__);}while(0)
#define LOGI(fmt, ...) do{printf("%s(%3d) [I]: " fmt "\n", __FILENAME__, __LINE__, ##__VA_ARGS__);}while(0)
#endif 

#ifdef ANDROID
#if 0
#define LOG_TAG  "clog"
#include <android/log.h>
#define  LOGD(fmt, ...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##__VA_ARGS__)
#define  LOGE(fmt, ...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##__VA_ARGS__)
#define  LOGW(fmt, ...)  __android_log_print(ANDROID_LOG_WARN, LOG_TAG, fmt, ##args)
#define  LOGI(fmt, ...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##__VA_ARGS__)
#else
#define LOGD(...)  ((void)0)
#define LOGE(...)  ((void)0)
#define LOGW(...)  ((void)0)
#define LOGI(...)  ((void)0)
#endif
#endif
#endif

#endif // LOGDEF_H_
