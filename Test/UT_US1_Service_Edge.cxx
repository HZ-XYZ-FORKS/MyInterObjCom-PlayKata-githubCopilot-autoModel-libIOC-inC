#include <gtest/gtest.h>

#include "IOC/IOC.h"
#include <cstdio>

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

///////////////////////////////////////////////////////////////////////////////////////////////////
// UT_US1_Service_Edge.cxx - CaTDD Implementation for IOC Link Establishment
// (Edge)
//
// PURPOSE:
//   Test-driven implementation of P0 Functional / Edge category for US-1:
//   edge behaviors around callback absence, timeout, and offline/keep-link
//   semantics.
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//======>BEGIN OF OVERVIEW OF THIS UNIT TESTING
//FILE===============================================
/**
 * @brief
 *   [WHAT] This file verifies IOC Service/Client link-establishment edge
 * behaviors. [WHERE] in the IOC Service API module. [WHY] to ensure
 * edge-condition correctness for timeout and lifecycle handling.
 *
 * SCOPE:
 *   - In scope: auto-accept without callback, explicit connect timeout
 * behavior, and KEEP_ACCEPTED_LINK behavior across service offline.
 *   - Out of scope: misuse/fault categories (covered by dedicated files).
 *
 * RELATIONSHIPS:
 *   - Related tests: UT_US1_Service_Typical.cxx, UT_US1_Service_Misuse.cxx,
 *     UT_US1_Service_Fault.cxx, UT_US1_Service_State.cxx.
 *   - Production code: Source/IOC_SrvAPI.c.
 */
//======>END OF OVERVIEW OF THIS UNIT TESTING
//FILE=================================================

///////////////////////////////////////////////////////////////////////////////////////////////////
//======>BEGIN OF ACCEPTANCE CRITERIA
//DESIGN=======================================================
/**
 * [@US-1] Establish IOC Link Between Service and Client
 *
 * AC-1: Auto-accept establishment succeeds with compatible usage.
 * AC-2: Connect timeout returns IOC_RESULT_TIMEOUT exactly when timeout is
 * exceeded. AC-3: Service offline honors keep/close semantics and rejects new
 * connects.
 */
//=======>END OF ACCEPTANCE CRITERIA
//DESIGN========================================================

///////////////////////////////////////////////////////////////////////////////////////////////////
//======>BEGIN OF TEST CASES
//DESIGN================================================================
/**
 * SUT: IOC Service/Link establishment behavior
 * APIs: IOC_connectService, IOC_offlineService
 *
 * @[Class]: P0 Functional
 * @[Category]: Edge
 * @[US]: US-1
 * @[AC]: AC-1
 * @[TC]: TC-P0-E1 verifyConnect_byAutoAcceptWithoutCallback_expectSuccess
 * @[Status]: REFACTOR
 *
 * @[AC]: AC-2
 * @[TC]: TC-P0-E2 verifyConnect_byTimeoutOptionExceeded_expectTimeout
 * @[Status]: REFACTOR
 *
 * @[AC]: AC-3
 * @[TC]: TC-P0-E3
 * verifyOffline_byKeepAcceptedLinkFlag_expectKeepExistingRejectNew
 * @[Status]: GREEN
 *
 * @[AC]: AC-3
 * @[TC]: TC-P0-E4
 * verifyOffline_byCloseAcceptedLinkDefault_expectCloseExistingRejectNew
 * @[Status]: TODO
 */
//======>END OF TEST CASES
//DESIGN==================================================================

namespace {
class US1_EdgeTest : public ::testing::Test {
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

/*
 * @[Class]: P0 Functional / ValidFunc
 * @[Category]: Edge
 * @[Intent]: Auto-accept path should still succeed when DAT callback is not
 * configured
 * @[US]: US-1
 * @[AC]: AC-1
 * @[TC]: TC-P0-E1 verifyConnect_byAutoAcceptWithoutCallback_expectSuccess
 */
TEST_F(US1_EdgeTest,
       TC_P0_E1_verifyConnect_byAutoAcceptWithoutCallback_expectSuccess) {
  //===>>> SETUP <<<===
  printf("SETUP: "
         "TC_P0_E1_verifyConnect_byAutoAcceptWithoutCallback_expectSuccess\n");

  IOC_SrvArgs_T srvArgs;
  IOC_Helper_initSrvArgs(&srvArgs);

  srvArgs.SrvURI.pProtocol = IOC_SRV_PROTO_FIFO;
  srvArgs.SrvURI.pHost = IOC_SRV_HOST_LOCAL_PROCESS;
  srvArgs.SrvURI.pPath = "UT_Service_Edge_NoCallback";
  srvArgs.SrvURI.Port = 0;

  srvArgs.Flags = IOC_SRVFLAG_AUTO_ACCEPT;
  srvArgs.UsageCapabilites = IOC_LinkUsageDatReceiver;

  IOC_DatUsageArgs_T datArgs = {};
  datArgs.CbRecvDat_F = nullptr;
  datArgs.pCbPrivData = nullptr;
  srvArgs.UsageArgs.pDat = &datArgs;

  //===>>> BEHAVIOR <<<===
  printf("BEHAVIOR: online service + connect client in auto-accept mode\n");

  IOC_Result_T result = IOC_onlineService(&srvID_, &srvArgs);
  ASSERT_EQ(result, IOC_RESULT_SUCCESS)
      << "Service online should succeed even without DAT callback";
  ASSERT_NE(srvID_, IOC_INVALID_SRV_ID)
      << "Service ID must be valid after online";

  IOC_ConnArgs_T connArgs;
  IOC_Helper_initConnArgs(&connArgs);
  connArgs.SrvURI.pProtocol = IOC_SRV_PROTO_FIFO;
  connArgs.SrvURI.pHost = IOC_SRV_HOST_LOCAL_PROCESS;
  connArgs.SrvURI.pPath = "UT_Service_Edge_NoCallback";
  connArgs.SrvURI.Port = 0;
  connArgs.Usage = IOC_LinkUsageDatSender;

  result = IOC_connectService(&clientConnLinkID_, &connArgs, nullptr);

  //===>>> VERIFY <<<===
  printf("VERIFY: connect succeeds and link is valid\n");
  VERIFY_KEYPOINT_EQ(
      result, IOC_RESULT_SUCCESS,
      "Auto-accept connect should succeed when callback is not configured");
  VERIFY_KEYPOINT_NE(clientConnLinkID_, IOC_INVALID_LINK_ID,
                     "Client link ID must be valid after successful connect");

  //===>>> CLEANUP <<<===
  printf("CLEANUP: handled by fixture TearDown\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//======>BEGIN OF TODO/IMPLEMENTATION TRACKING
//SECTION============================================
// P0 Edge status summary for this file:
// - E1: REFACTOR
// - E2: REFACTOR
// - E3: RED (KEEP_ACCEPTED_LINK behavior not yet satisfied by product code)
// - E4: TODO
//======>END OF TODO/IMPLEMENTATION TRACKING
//SECTION===============================================

/*
 * @[Class]: P0 Functional / ValidFunc
 * @[Category]: Edge
 * @[Intent]: Manual-accept connect with explicit timeout should return timeout
 * when no accept occurs
 * @[US]: US-1
 * @[AC]: AC-2
 * @[TC]: TC-P0-E2 verifyConnect_byTimeoutOptionExceeded_expectTimeout
 */
TEST_F(US1_EdgeTest,
       TC_P0_E2_verifyConnect_byTimeoutOptionExceeded_expectTimeout) {
  //===>>> SETUP <<<===
  printf(
      "SETUP: TC_P0_E2_verifyConnect_byTimeoutOptionExceeded_expectTimeout\n");

  IOC_SrvArgs_T srvArgs;
  IOC_Helper_initSrvArgs(&srvArgs);

  srvArgs.SrvURI.pProtocol = IOC_SRV_PROTO_FIFO;
  srvArgs.SrvURI.pHost = IOC_SRV_HOST_LOCAL_PROCESS;
  srvArgs.SrvURI.pPath = "UT_Service_Edge_Timeout";
  srvArgs.SrvURI.Port = 0;

  // No AUTO_ACCEPT means connect waits for IOC_acceptClient.
  srvArgs.Flags = IOC_SRVFLAG_NONE;
  srvArgs.UsageCapabilites = IOC_LinkUsageDatReceiver;
  srvArgs.UsageArgs.pDat = nullptr;

  IOC_Result_T result = IOC_onlineService(&srvID_, &srvArgs);
  ASSERT_EQ(result, IOC_RESULT_SUCCESS)
      << "Service online should succeed for timeout scenario";
  ASSERT_NE(srvID_, IOC_INVALID_SRV_ID)
      << "Service ID must be valid before timeout connect";

  IOC_ConnArgs_T connArgs;
  IOC_Helper_initConnArgs(&connArgs);
  connArgs.SrvURI.pProtocol = IOC_SRV_PROTO_FIFO;
  connArgs.SrvURI.pHost = IOC_SRV_HOST_LOCAL_PROCESS;
  connArgs.SrvURI.pPath = "UT_Service_Edge_Timeout";
  connArgs.SrvURI.Port = 0;
  connArgs.Usage = IOC_LinkUsageDatSender;

  IOC_Option_defineSyncTimeout(connectOpt, IOC_TIMEOUT_IMMEDIATE);

  //===>>> BEHAVIOR <<<===
  printf("BEHAVIOR: connect with explicit sync-timeout and no manual accept\n");
  result = IOC_connectService(&clientConnLinkID_, &connArgs, &connectOpt);

  //===>>> VERIFY <<<===
  printf("VERIFY: timeout result is exact IOC_RESULT_TIMEOUT\n");
  VERIFY_KEYPOINT_EQ(result, IOC_RESULT_TIMEOUT,
                     "Connect must return IOC_RESULT_TIMEOUT exactly when "
                     "timeout is exceeded");

  //===>>> CLEANUP <<<===
  printf("CLEANUP: handled by fixture TearDown\n");
}

/*
 * @[Class]: P0 Functional / ValidFunc
 * @[Category]: Edge
 * @[Intent]: KEEP_ACCEPTED_LINK keeps established links alive after offline and
 * rejects new connections
 * @[US]: US-1
 * @[AC]: AC-3
 * @[TC]: TC-P0-E3
 * verifyOffline_byKeepAcceptedLinkFlag_expectKeepExistingRejectNew
 */
TEST_F(
    US1_EdgeTest,
    TC_P0_E3_verifyOffline_byKeepAcceptedLinkFlag_expectKeepExistingRejectNew) {
  //===>>> SETUP <<<===
  printf("SETUP: "
         "TC_P0_E3_verifyOffline_byKeepAcceptedLinkFlag_"
         "expectKeepExistingRejectNew\n");

  IOC_SrvArgs_T srvArgs;
  IOC_Helper_initSrvArgs(&srvArgs);

  srvArgs.SrvURI.pProtocol = IOC_SRV_PROTO_FIFO;
  srvArgs.SrvURI.pHost = IOC_SRV_HOST_LOCAL_PROCESS;
  srvArgs.SrvURI.pPath = "UT_Service_Edge_KeepAccepted";
  srvArgs.SrvURI.Port = 0;
  srvArgs.Flags = (IOC_SrvFlags_T)(IOC_SRVFLAG_AUTO_ACCEPT |
                                   IOC_SRVFLAG_KEEP_ACCEPTED_LINK);
  srvArgs.UsageCapabilites = IOC_LinkUsageDatReceiver;
  srvArgs.UsageArgs.pDat = nullptr;

  IOC_Result_T result = IOC_onlineService(&srvID_, &srvArgs);
  ASSERT_EQ(result, IOC_RESULT_SUCCESS)
      << "Service online should succeed for KEEP_ACCEPTED_LINK scenario";
  ASSERT_NE(srvID_, IOC_INVALID_SRV_ID)
      << "Service ID must be valid before creating the initial link";

  IOC_ConnArgs_T connArgs;
  IOC_Helper_initConnArgs(&connArgs);
  connArgs.SrvURI.pProtocol = IOC_SRV_PROTO_FIFO;
  connArgs.SrvURI.pHost = IOC_SRV_HOST_LOCAL_PROCESS;
  connArgs.SrvURI.pPath = "UT_Service_Edge_KeepAccepted";
  connArgs.SrvURI.Port = 0;
  connArgs.Usage = IOC_LinkUsageDatSender;

  //===>>> BEHAVIOR <<<===
  printf("BEHAVIOR: establish link, offline service, then attempt reconnect\n");

  result = IOC_connectService(&clientConnLinkID_, &connArgs, nullptr);
  ASSERT_EQ(result, IOC_RESULT_SUCCESS)
      << "Initial connect should succeed before service offline";
  ASSERT_NE(clientConnLinkID_, IOC_INVALID_LINK_ID)
      << "Initial client link must be valid before service offline";

  result = IOC_offlineService(srvID_);
  ASSERT_EQ(result, IOC_RESULT_SUCCESS)
      << "Service offline should succeed with KEEP_ACCEPTED_LINK enabled";
  srvID_ = IOC_INVALID_SRV_ID;

  result = IOC_closeLink(clientConnLinkID_);

  //===>>> VERIFY <<<===
  printf("VERIFY: kept link remains valid and reconnect is rejected\n");
  VERIFY_KEYPOINT_EQ(result, IOC_RESULT_SUCCESS,
                     "Existing link should remain valid after offline when "
                     "KEEP_ACCEPTED_LINK is set");
  clientConnLinkID_ = IOC_INVALID_LINK_ID;

  IOC_Option_defineSyncTimeout(connectOpt, IOC_TIMEOUT_IMMEDIATE);
  IOC_LinkID_T newConnLinkID = IOC_INVALID_LINK_ID;
  result = IOC_connectService(&newConnLinkID, &connArgs, &connectOpt);
  VERIFY_KEYPOINT_NE(result, IOC_RESULT_SUCCESS,
                     "New connect must fail after service is offline");
  VERIFY_KEYPOINT_EQ(
      newConnLinkID, IOC_INVALID_LINK_ID,
      "Failed reconnect after offline must not produce a usable link ID");

  //===>>> CLEANUP <<<===
  printf("CLEANUP: close unexpected link if produced; otherwise fixture "
         "handles cleanup\n");
  if (newConnLinkID != IOC_INVALID_LINK_ID) {
    IOC_closeLink(newConnLinkID);
  }
}
