--- src/gallium/drivers/swr/rasterizer/core/threads.cpp.orig	2020-11-23 20:04:03.077138400 +0100
+++ src/gallium/drivers/swr/rasterizer/core/threads.cpp	2020-12-05 18:51:04.532698000 +0100
@@ -30,16 +30,23 @@
 #include <fstream>
 #include <string>
 
-#if defined(__linux__) || defined(__gnu_linux__) || defined(__APPLE__)
+#if defined(__linux__) || defined(__gnu_linux__) || defined(__APPLE__) || defined(__FreeBSD__)
 #include <pthread.h>
 #include <sched.h>
 #include <unistd.h>
 #endif
 
-#ifdef __APPLE__
+#if defined(__APPLE__) || defined(__FreeBSD__)
 #include <sys/types.h>
 #include <sys/sysctl.h>
+#ifdef __FreeBSD__
+#include <pthread_np.h>
+#include <sys/cpuset.h>
+#ifndef cpu_set_t
+#define cpu_set_t cpuset_t
 #endif
+#endif
+#endif
 
 #include "common/os.h"
 #include "core/api.h"
@@ -218,7 +225,7 @@
         }
     }
 
-#elif defined(__APPLE__)
+#elif defined(__APPLE__) || defined(__FreeBSD__)
 
     auto numProcessors  = 0;
     auto numCores       = 0;
@@ -227,6 +234,7 @@
     int    value;
     size_t size = sizeof(value);
 
+#ifdef __APPLE__ 
     int result = sysctlbyname("hw.packages", &value, &size, NULL, 0);
     SWR_ASSERT(result == 0);
     numPhysicalIds = value;
@@ -238,7 +246,20 @@
     result = sysctlbyname("hw.physicalcpu", &value, &size, NULL, 0);
     SWR_ASSERT(result == 0);
     numCores = value;
+#else
+    int result = sysctlbyname("kern.smp.cores", &value, &size, NULL, 0);
+    SWR_ASSERT(result == 0);
+    numProcessors = value;
 
+    result = sysctlbyname("kern.smp.threads_per_core", &value, &size, NULL, 0);
+    SWR_ASSERT(result == 0);
+    numCores = value*numProcessors;
+
+    result = sysctlbyname("kern.smp.cpus", &value, &size, NULL, 0);
+    SWR_ASSERT(result == 0);
+    numPhysicalIds = value / numCores;
+ #endif
+
     out_nodes.resize(numPhysicalIds);
 
     for (auto physId = 0; physId < numPhysicalIds; ++physId)
@@ -353,7 +374,7 @@
         SWR_INVALID("Failed to set Thread Affinity");
     }
 
-#elif defined(__linux__) || defined(__gnu_linux__)
+#elif defined(__linux__) || defined(__gnu_linux__) || defined(__FreeBSD__)
 
     cpu_set_t cpuset;
     pthread_t thread = pthread_self();
