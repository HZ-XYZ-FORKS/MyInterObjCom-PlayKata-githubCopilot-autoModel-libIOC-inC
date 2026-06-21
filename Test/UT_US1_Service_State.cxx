#include <gtest/gtest.h>

/*
 * SUT: IOC service lifecycle state behavior
 * APIs: IOC_onlineService, IOC_offlineService, IOC_getSrvState
 *
 * @[Class]: P1 Design
 * @[Category]: State
 * @[US]: US-1
 * @[AC]: AC-3
 * @[TC]: TC-P1-S1 verifyServiceState_byOfflineToOnlineToOffline_expectDeterministicTransitions
 * @[Status]: TODO
 *
 * @[AC]: AC-2
 * @[TC]: TC-P1-S2 verifyPendingQueue_byAcceptTimeoutOffline_expectSingleResolution
 * @[Status]: TODO
 */
