--- src/gallium/drivers/swr/rasterizer/common/os.h.orig	2020-12-05 17:15:49.783069000 +0100
+++ src/gallium/drivers/swr/rasterizer/common/os.h	2020-12-05 17:17:06.500333000 +0100
@@ -103,7 +103,7 @@
 #define _mm_popcount_sizeT _mm_popcnt_u32
 #endif
 
-#elif defined(__APPLE__) || defined(FORCE_LINUX) || defined(__linux__) || defined(__gnu_linux__)
+#elif defined(__APPLE__) || defined(FORCE_LINUX) || defined(__linux__) || defined(__gnu_linux__) || defined(__FreeBSD__)
 
 #define SWR_API
 #define SWR_VISIBLE __attribute__((visibility("default")))
