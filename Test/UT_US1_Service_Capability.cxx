#include <gtest/gtest.h>

/*
 * SUT: IOC service capability boundary behavior
 * APIs: IOC_onlineService, IOC_connectService, IOC_acceptClient
 *
 * @[Class]: P1 Design
 * @[Category]: Capability
 * @[US]: US-1
 * @[AC]: AC-1
 * @[TC]: TC-P1-C1 verifyCapabilityBoundary_byComplementaryUsage_expectAllowedOrRejected
 * @[Status]: TODO
 *
 * @[AC]: AC-2
 * @[TC]: TC-P1-C2 verifyCapabilityBoundary_byManualAcceptInAutoMode_expectNotSupportManualAccept
 * @[Status]: TODO
 */
