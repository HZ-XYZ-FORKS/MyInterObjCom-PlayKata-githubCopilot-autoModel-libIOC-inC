#include "_UT_IOC_Common.h"
#include <gtest/gtest.h>

#include "IOC/IOC.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// UT_US1_Service_Misuse.cxx - CaTDD Implementation for IOC Link Establishment
// (Misuse)
//
// PURPOSE:
//   Test-driven implementation of P0 Functional / Misuse category for US-1:
//   API misuse behavior when client usage is incompatible with service
//   capability.
//
// TDD WORKFLOW:
//   RED stage: Implement tests that fail (no product code yet)
//   GREEN stage: Minimal product code makes tests pass
//   REFACTOR stage: Clean up without behavior change
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//======>BEGIN OF OVERVIEW OF THIS UNIT TESTING
//FILE===============================================
/**
 * @brief
 *   [WHAT] This file verifies IOC API misuse behavior for incompatible usage.
 *   [WHERE] in the IOC Service API module.
 *   [WHY] to ensure misuse is rejected with exact and deterministic result
 * codes.
 *
 * SCOPE:
 *   - In scope: connect rejection for incompatible client usage.
 *   - Out of scope: edge timeout/offline behavior and fault recovery.
 *
 * RELATIONSHIPS:
 *   - Related tests: UT_US1_Service_Typical.cxx, UT_US1_Service_Edge.cxx,
 *     UT_US1_Service_Fault.cxx, UT_US1_Service_State.cxx,
 * UT_US1_Service_Capability.cxx.
 *   - Production code: Source/IOC_SrvAPI.c.
 */
//======>END OF OVERVIEW OF THIS UNIT TESTING
//FILE=================================================

///////////////////////////////////////////////////////////////////////////////////////////////////
//======>BEGIN OF USER STORY
//DESIGN================================================================
/**
 * [@US-1] Establish IOC Link Between Service and Client
 *
 * Coverage focus in this file:
 *   - Invalid usage pairing is rejected without creating usable links.
 */
//======>END OF USER STORY
//DESIGN==================================================================

///////////////////////////////////////////////////////////////////////////////////////////////////
//======>BEGIN OF ACCEPTANCE CRITERIA
//DESIGN=======================================================
/**
 * [@US-1] Establish IOC Link Between Service and Client
 *
 * AC-1: Auto-accept establishment succeeds with compatible usage.
 * AC-1 (negative path): incompatible usage connect is rejected
 * deterministically.
 */
//=======>END OF ACCEPTANCE CRITERIA
//DESIGN========================================================

namespace {
class US1_MisuseTest : public ::testing::Test {
protected:
  IOC_SrvID_T srvID_ = IOC_INVALID_SRV_ID;
  IOC_LinkID_T clientConnLinkID_ = IOC_INVALID_LINK_ID;

  void TearDown() override {
    if (clientConnLinkID_ != IOC_INVALID_LINK_ID) {
      IOC_closeLink(clientConnLinkID_);
    }
    if (srvID_ != IOC_INVALID_SRV_ID) {
      IOC_offlineService(srvID_);
    }
  }
};
} // namespace

///////////////////////////////////////////////////////////////////////////////////////////////////
//======>BEGIN OF TEST CASES
//DESIGN================================================================
/**
 * SUT: IOC Service/Link establishment behavior
 * APIs: IOC_connectService
 *
 * @[Class]: P0 Functional
 * @[Category]: Misuse
 * @[US]: US-1
 * @[AC]: AC-1
 * @[TC]: TC-P0-M1 verifyConnect_byIncompatibleUsage_expectIncompatibleUsage
 * @[Status]: GREEN
 */
//======>END OF TEST CASES
//DESIGN==================================================================
TEST_F(US1_MisuseTest,
       TC_P0_M1_verifyConnect_byIncompatibleUsage_expectIncompatibleUsage) {
  //===>>> SETUP <<<===
  UT_PHASE_SETUP("TC_P0_M1_verifyConnect_byIncompatibleUsage_"
                 "expectIncompatibleUsage");

  IOC_SrvArgs_T srvArgs;
  IOC_Helper_initSrvArgs(&srvArgs);

  srvArgs.SrvURI.pProtocol = IOC_SRV_PROTO_FIFO;
  srvArgs.SrvURI.pHost = IOC_SRV_HOST_LOCAL_PROCESS;
  srvArgs.SrvURI.pPath = "UT_Service_Misuse_Incompatible";
  srvArgs.SrvURI.Port = 0;
  srvArgs.Flags = IOC_SRVFLAG_AUTO_ACCEPT;
  srvArgs.UsageCapabilites = IOC_LinkUsageDatReceiver;
  srvArgs.UsageArgs.pDat = nullptr;

  IOC_Result_T result = IOC_onlineService(&srvID_, &srvArgs);
  ASSERT_EQ(result, IOC_RESULT_SUCCESS)
      << "Service online should succeed for misuse incompatible-usage test";
  ASSERT_NE(srvID_, IOC_INVALID_SRV_ID)
      << "Service ID must be valid before client connect misuse action";

  IOC_ConnArgs_T connArgs;
  IOC_Helper_initConnArgs(&connArgs);
  connArgs.SrvURI.pProtocol = IOC_SRV_PROTO_FIFO;
  connArgs.SrvURI.pHost = IOC_SRV_HOST_LOCAL_PROCESS;
  connArgs.SrvURI.pPath = "UT_Service_Misuse_Incompatible";
  connArgs.SrvURI.Port = 0;
  // Service provides DAT_RECEIVER; EVT_CONSUMER is incompatible for this pair.
  connArgs.Usage = IOC_LinkUsageEvtConsumer;

  //===>>> BEHAVIOR <<<===
  UT_PHASE_BEHAVIOR("connect with incompatible client usage against service "
                    "capability");
  result = IOC_connectService(&clientConnLinkID_, &connArgs, nullptr);

  //===>>> VERIFY <<<===
  UT_PHASE_VERIFY("connect is rejected with IOC_RESULT_INCOMPATIBLE_USAGE");
  VERIFY_KEYPOINT_EQ(result, IOC_RESULT_INCOMPATIBLE_USAGE,
                     "Incompatible usage connect must be rejected with exact "
                     "IOC_RESULT_INCOMPATIBLE_USAGE");
  VERIFY_KEYPOINT_EQ(
      clientConnLinkID_, IOC_INVALID_LINK_ID,
      "Rejected connect must not produce a usable client link ID");

  //===>>> CLEANUP <<<===
  UT_PHASE_CLEANUP("handled by fixture TearDown");
}
