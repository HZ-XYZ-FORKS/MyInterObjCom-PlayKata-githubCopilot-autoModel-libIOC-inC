#include <gtest/gtest.h>

/*
 * SUT: IOC Service/Link establishment behavior
 * APIs: IOC_connectService, IOC_offlineService
 *
 * @[Class]: P0 Functional
 * @[Category]: Fault
 * @[US]: US-1
 * @[AC]: AC-2
 * @[TC]: TC-P0-F1 verifyPendingAccept_byServiceOfflineTransition_expectResolvedFailure
 * @[Status]: TODO
 *
 * @[AC]: AC-1
 * @[TC]: TC-P0-F2 verifyConnect_byRuntimeFailureDuringConnect_expectGracefulFailure
 * @[Status]: TODO
 */
