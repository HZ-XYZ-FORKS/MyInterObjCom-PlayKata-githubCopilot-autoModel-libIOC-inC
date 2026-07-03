#include "_UT_IOC_Common.h"
#include <gtest/gtest.h>

#include "IOC/IOC.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// UT_US1_Service_Capability.cxx - CaTDD Implementation for IOC Link Establishment (Capability)
//
// PURPOSE:
//   Test-driven implementation of P1 Design / Capability category for US-1:
//   capability boundary behaviors around usage compatibility and auto-accept constraints.
//
// TDD WORKFLOW:
//   RED stage: Implement tests that fail (no product code yet)
//   GREEN stage: Minimal product code makes tests pass
//   REFACTOR stage: Clean up without behavior change
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//======>BEGIN OF OVERVIEW OF THIS UNIT TESTING FILE===============================================
/**
 * @brief
 *   [WHAT] This file verifies IOC Service/Client capability-boundary behavior.
 *   [WHERE] in the IOC Service API module.
 *   [WHY] to ensure capability constraints are deterministic and enforceable.
 *
 * SCOPE:
 *   - In scope: complementary vs incompatible usage pairing behavior.
 *   - In scope: manual-accept API behavior when service is in auto-accept mode.
 *   - Out of scope: lifecycle/offline transitions (covered by State/Edge/Fault files).
 *
 * RELATIONSHIPS:
 *   - Related tests: UT_US1_Service_Typical.cxx, UT_US1_Service_Edge.cxx,
 *     UT_US1_Service_Misuse.cxx, UT_US1_Service_Fault.cxx, UT_US1_Service_State.cxx.
 *   - Production code: Source/IOC_SrvAPI.c.
 */
//======>END OF OVERVIEW OF THIS UNIT TESTING FILE=================================================

///////////////////////////////////////////////////////////////////////////////////////////////////
//======>BEGIN OF USER STORY DESIGN================================================================
/**
 * [@US-1] Establish IOC Link Between Service and Client
 *
 * Coverage focus in this file:
 *   - Service capability declaration vs client requested usage pairing.
 *   - Auto-accept mode boundary for IOC_acceptClient behavior.
 */
//======>END OF USER STORY DESIGN==================================================================

///////////////////////////////////////////////////////////////////////////////////////////////////
//======>BEGIN OF ACCEPTANCE CRITERIA DESIGN=======================================================
/**
 * [@US-1] Establish IOC Link Between Service and Client
 *
 * AC-1: Auto-accept establishment succeeds with compatible usage.
 * AC-2: Service mode constraints reject unsupported manual-accept operations.
 */
//=======>END OF ACCEPTANCE CRITERIA DESIGN========================================================

namespace {
class US1_CapabilityTest : public ::testing::Test {
protected:
	IOC_SrvID_T srvID_ = IOC_INVALID_SRV_ID;

	void TearDown() override {
		if (srvID_ != IOC_INVALID_SRV_ID) {
			IOC_offlineService(srvID_);
		}
	}
};
} // namespace

///////////////////////////////////////////////////////////////////////////////////////////////////
//======>BEGIN OF TEST CASES DESIGN================================================================
/**
 * SUT: IOC service capability boundary behavior
 * APIs: IOC_onlineService, IOC_connectService, IOC_acceptClient
 *
 * @[Class]: P1 Design
 * @[Category]: Capability
 * @[US]: US-1
 * @[AC]: AC-1
 * @[TC]: TC-P1-C1
 * verifyCapabilityBoundary_byComplementaryUsage_expectAllowedOrRejected
 * @[Status]: GREEN
 *
 * @[AC]: AC-2
 * @[TC]: TC-P1-C2
 * verifyCapabilityBoundary_byManualAcceptInAutoMode_expectNotSupportManualAccept
 * @[Status]: GREEN
 */
//======>END OF TEST CASES DESIGN==================================================================

TEST_F(
  US1_CapabilityTest,
  TC_P1_C1_verifyCapabilityBoundary_byComplementaryUsage_expectAllowedOrRejected) {
  //===>>> SETUP <<<===
  UT_PHASE_SETUP(
    "TC_P1_C1_verifyCapabilityBoundary_byComplementaryUsage_"
    "expectAllowedOrRejected");

  IOC_SrvArgs_T srvArgs;
  IOC_Helper_initSrvArgs(&srvArgs);

  srvArgs.SrvURI.pProtocol = IOC_SRV_PROTO_FIFO;
  srvArgs.SrvURI.pHost = IOC_SRV_HOST_LOCAL_PROCESS;
  srvArgs.SrvURI.pPath = "UT_Service_Capability_Complementary";
  srvArgs.SrvURI.Port = 0;
  srvArgs.Flags = IOC_SRVFLAG_AUTO_ACCEPT;
  srvArgs.UsageCapabilites = IOC_LinkUsageDatReceiver;
  srvArgs.UsageArgs.pDat = nullptr;

  IOC_Result_T result = IOC_onlineService(&srvID_, &srvArgs);
  ASSERT_EQ(result, IOC_RESULT_SUCCESS)
    << "Service online should succeed for capability-boundary test";
  ASSERT_NE(srvID_, IOC_INVALID_SRV_ID)
    << "Service ID must be valid before capability connect checks";

  IOC_ConnArgs_T connArgs;
  IOC_Helper_initConnArgs(&connArgs);
  connArgs.SrvURI.pProtocol = IOC_SRV_PROTO_FIFO;
  connArgs.SrvURI.pHost = IOC_SRV_HOST_LOCAL_PROCESS;
  connArgs.SrvURI.pPath = "UT_Service_Capability_Complementary";
  connArgs.SrvURI.Port = 0;

  //===>>> BEHAVIOR <<<===
  UT_PHASE_BEHAVIOR(
    "connect with complementary usage then incompatible usage against same "
    "service capability");

  IOC_LinkID_T allowedLinkID = IOC_INVALID_LINK_ID;
  connArgs.Usage = IOC_LinkUsageDatSender; // Complementary to DAT_RECEIVER.
  IOC_Result_T allowedResult = IOC_connectService(&allowedLinkID, &connArgs, nullptr);

  IOC_LinkID_T rejectedLinkID = IOC_INVALID_LINK_ID;
  connArgs.Usage = IOC_LinkUsageEvtConsumer; // Incompatible with DAT_RECEIVER.
  IOC_Result_T rejectedResult =
    IOC_connectService(&rejectedLinkID, &connArgs, nullptr);

  //===>>> VERIFY <<<===
  UT_PHASE_VERIFY(
    "complementary usage is allowed while incompatible usage is rejected");
  VERIFY_KEYPOINT_EQ(allowedResult, IOC_RESULT_SUCCESS,
           "Complementary usage connect should be accepted");
  VERIFY_KEYPOINT_NE(allowedLinkID, IOC_INVALID_LINK_ID,
           "Accepted complementary usage should produce valid link "
           "ID");
  VERIFY_KEYPOINT_EQ(rejectedResult, IOC_RESULT_INCOMPATIBLE_USAGE,
           "Incompatible usage connect should return exact "
           "IOC_RESULT_INCOMPATIBLE_USAGE");
  VERIFY_KEYPOINT_EQ(
    rejectedLinkID, IOC_INVALID_LINK_ID,
    "Rejected incompatible usage should not produce usable link ID");

  //===>>> CLEANUP <<<===
  UT_PHASE_CLEANUP("close accepted link; fixture handles service offline");
  if (allowedLinkID != IOC_INVALID_LINK_ID) {
  IOC_closeLink(allowedLinkID);
  }
}

TEST_F(
	US1_CapabilityTest,
	TC_P1_C2_verifyCapabilityBoundary_byManualAcceptInAutoMode_expectNotSupportManualAccept) {
  //===>>> SETUP <<<===
  UT_PHASE_SETUP(
	  "TC_P1_C2_verifyCapabilityBoundary_byManualAcceptInAutoMode_"
	  "expectNotSupportManualAccept");

  IOC_SrvArgs_T srvArgs;
  IOC_Helper_initSrvArgs(&srvArgs);

  srvArgs.SrvURI.pProtocol = IOC_SRV_PROTO_FIFO;
  srvArgs.SrvURI.pHost = IOC_SRV_HOST_LOCAL_PROCESS;
  srvArgs.SrvURI.pPath = "UT_Service_Capability_AutoMode";
  srvArgs.SrvURI.Port = 0;
  srvArgs.Flags = IOC_SRVFLAG_AUTO_ACCEPT;
  srvArgs.UsageCapabilites = IOC_LinkUsageDatReceiver;
  srvArgs.UsageArgs.pDat = nullptr;

  IOC_Result_T result = IOC_onlineService(&srvID_, &srvArgs);
  ASSERT_EQ(result, IOC_RESULT_SUCCESS)
	  << "Service online should succeed before capability boundary check";
  ASSERT_NE(srvID_, IOC_INVALID_SRV_ID)
	  << "Service ID must be valid before calling IOC_acceptClient";

  IOC_LinkID_T acceptedLinkID = IOC_INVALID_LINK_ID;

  //===>>> BEHAVIOR <<<===
  UT_PHASE_BEHAVIOR(
	  "call IOC_acceptClient while service is configured in AUTO_ACCEPT mode");
  result = IOC_acceptClient(srvID_, &acceptedLinkID, nullptr);

  //===>>> VERIFY <<<===
  UT_PHASE_VERIFY(
	  "manual accept is rejected in auto-accept mode with no link output");
  VERIFY_KEYPOINT_EQ(result, IOC_RESULT_NOT_SUPPORT_MANUAL_ACCEPT,
					 "IOC_acceptClient should report "
					 "IOC_RESULT_NOT_SUPPORT_MANUAL_ACCEPT in AUTO_ACCEPT "
					 "mode");
  VERIFY_KEYPOINT_EQ(acceptedLinkID, IOC_INVALID_LINK_ID,
					 "Rejected manual accept should not produce a valid link "
					 "ID");

  //===>>> CLEANUP <<<===
  UT_PHASE_CLEANUP("handled by fixture TearDown");
}
