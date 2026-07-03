#ifndef TEST_UT_IOC_COMMON_H
#define TEST_UT_IOC_COMMON_H

#include <cstdio>
#include <gtest/gtest.h>

#ifndef VERIFY_KEYPOINT_EQ
#define VERIFY_KEYPOINT_EQ(actual, expected, message)                          \
  EXPECT_EQ((actual), (expected)) << (message)
#endif

#ifndef VERIFY_KEYPOINT_NE
#define VERIFY_KEYPOINT_NE(actual, expected, message)                          \
  EXPECT_NE((actual), (expected)) << (message)
#endif

#ifndef VERIFY_KEYPOINT_TRUE
#define VERIFY_KEYPOINT_TRUE(condition, message)                               \
  EXPECT_TRUE((condition)) << (message)
#endif

#ifndef UT_PHASE_PRINT
#define UT_PHASE_PRINT(phase, message)                                         \
  do {                                                                         \
    std::printf("%s: %s\\n", (phase), (message));                              \
  } while (0)
#endif

#ifndef UT_PHASE_SETUP
#define UT_PHASE_SETUP(message) UT_PHASE_PRINT("SETUP", (message))
#endif

#ifndef UT_PHASE_BEHAVIOR
#define UT_PHASE_BEHAVIOR(message) UT_PHASE_PRINT("BEHAVIOR", (message))
#endif

#ifndef UT_PHASE_VERIFY
#define UT_PHASE_VERIFY(message) UT_PHASE_PRINT("VERIFY", (message))
#endif

#ifndef UT_PHASE_CLEANUP
#define UT_PHASE_CLEANUP(message) UT_PHASE_PRINT("CLEANUP", (message))
#endif

#endif // TEST_UT_IOC_COMMON_H
