#include <gtest/gtest.h>

/*
 * SUT: IOC Service/Link establishment behavior
 * APIs: IOC_connectService, IOC_offlineService
 *
 * @[Class]: P0 Functional
 * @[Category]: Edge
 * @[US]: US-1
 * @[AC]: AC-1
 * @[TC]: TC-P0-E1 verifyConnect_byAutoAcceptWithoutCallback_expectSuccess
 * @[Status]: TODO
 *
 * @[AC]: AC-2
 * @[TC]: TC-P0-E2 verifyConnect_byTimeoutOptionExceeded_expectTimeout
 * @[Status]: TODO
 *
 * @[AC]: AC-3
 * @[TC]: TC-P0-E3
 * verifyOffline_byKeepAcceptedLinkFlag_expectKeepExistingRejectNew
 * @[Status]: TODO
 *
 * @[AC]: AC-3
 * @[TC]: TC-P0-E4
 * verifyOffline_byCloseAcceptedLinkDefault_expectCloseExistingRejectNew
 * @[Status]: TODO
 */
