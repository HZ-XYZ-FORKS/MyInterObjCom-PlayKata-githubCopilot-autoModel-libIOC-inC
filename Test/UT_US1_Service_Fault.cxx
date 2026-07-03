#include "_UT_IOC_Common.h"
#include <gtest/gtest.h>

#include "IOC/IOC.h"
#include <chrono>
#include <thread>

///////////////////////////////////////////////////////////////////////////////////////////////////
// UT_US1_Service_Fault.cxx - CaTDD Implementation for IOC Link Establishment (Fault)
//
// PURPOSE:
//   Test-driven implementation of P0 Functional / Fault category for US-1:
//   runtime failure and offline-transition failure-path handling.
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
 *   [WHAT] This file verifies IOC link-establishment fault behaviors.
 *   [WHERE] in the IOC Service API module.
 *   [WHY] to ensure graceful failure when services transition offline or resources exhaust.
 *
 * SCOPE:
 *   - In scope: pending connect resolution during service offline transition.
 *   - In scope: connect behavior under finite link resource exhaustion.
 *   - Out of scope: misuse-only API abuse and nominal happy paths.
 *
 * RELATIONSHIPS:
 *   - Related tests: UT_US1_Service_Typical.cxx, UT_US1_Service_Edge.cxx,
 *     UT_US1_Service_Misuse.cxx, UT_US1_Service_State.cxx.
 *   - Production code: Source/IOC_SrvAPI.c.
 */
//======>END OF OVERVIEW OF THIS UNIT TESTING FILE=================================================

///////////////////////////////////////////////////////////////////////////////////////////////////
//======>BEGIN OF USER STORY DESIGN================================================================
/**
 * [@US-1] Establish IOC Link Between Service and Client
 *
 * Coverage focus in this file:
 *   - Fault-path determinism for pending connect during offline transition.
 *   - Fault-path determinism for runtime allocation/resource-pressure failures.
 */
//======>END OF USER STORY DESIGN==================================================================

///////////////////////////////////////////////////////////////////////////////////////////////////
//======>BEGIN OF ACCEPTANCE CRITERIA DESIGN=======================================================
/**
 * [@US-1] Establish IOC Link Between Service and Client
 *
 * AC-1: Auto-accept establishment succeeds with compatible usage.
 * AC-2: Failure conditions resolve deterministically without leaked usable links.
 */
//=======>END OF ACCEPTANCE CRITERIA DESIGN========================================================

namespace {
class US1_FaultTest : public ::testing::Test {
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
//======>BEGIN OF TEST CASES DESIGN================================================================
/**
 * SUT: IOC Service/Link establishment behavior
 * APIs: IOC_connectService, IOC_offlineService
 *
 * @[Class]: P0 Functional
 * @[Category]: Fault
 * @[US]: US-1
 * @[AC]: AC-2
 * @[TC]: TC-P0-F1
 * verifyPendingAccept_byServiceOfflineTransition_expectResolvedFailure
 * @[Status]: GREEN
 *
 * @[AC]: AC-1
 * @[TC]: TC-P0-F2
 * verifyConnect_byRuntimeFailureDuringConnect_expectGracefulFailure
 * @[Status]: GREEN
 */
//======>END OF TEST CASES DESIGN==================================================================
TEST_F(US1_FaultTest,
  TC_P0_F1_verifyPendingAccept_byServiceOfflineTransition_expectResolvedFailure) {
  //===>>> SETUP <<<===
  UT_PHASE_SETUP(
	  "TC_P0_F1_verifyPendingAccept_byServiceOfflineTransition_"
	  "expectResolvedFailure");

  IOC_SrvArgs_T srvArgs;
  IOC_Helper_initSrvArgs(&srvArgs);

  srvArgs.SrvURI.pProtocol = IOC_SRV_PROTO_FIFO;
  srvArgs.SrvURI.pHost = IOC_SRV_HOST_LOCAL_PROCESS;
  srvArgs.SrvURI.pPath = "UT_Service_Fault_PendingOffline";
  srvArgs.SrvURI.Port = 0;
  srvArgs.Flags = IOC_SRVFLAG_NONE; // Manual accept path.
  srvArgs.UsageCapabilites = IOC_LinkUsageDatReceiver;
  srvArgs.UsageArgs.pDat = nullptr;

  IOC_Result_T result = IOC_onlineService(&srvID_, &srvArgs);
  ASSERT_EQ(result, IOC_RESULT_SUCCESS)
	  << "Service online should succeed for fault pending-offline scenario";
  ASSERT_NE(srvID_, IOC_INVALID_SRV_ID)
	  << "Service ID must be valid before pending connect transition";

  IOC_ConnArgs_T connArgs;
  IOC_Helper_initConnArgs(&connArgs);
  connArgs.SrvURI.pProtocol = IOC_SRV_PROTO_FIFO;
  connArgs.SrvURI.pHost = IOC_SRV_HOST_LOCAL_PROCESS;
  connArgs.SrvURI.pPath = "UT_Service_Fault_PendingOffline";
  connArgs.SrvURI.Port = 0;
  connArgs.Usage = IOC_LinkUsageDatSender;

  IOC_Option_defineSyncTimeout(connectOpt, 200000); // 200ms
  IOC_Result_T connectResult = IOC_RESULT_FAILURE;

  //===>>> BEHAVIOR <<<===
  UT_PHASE_BEHAVIOR("start pending manual-accept connect, then offline service "
					"before accept");
  std::thread connectThread([&]() {
	connectResult = IOC_connectService(&clientConnLinkID_, &connArgs, &connectOpt);
  });

  // Give connect a short window to enter pending wait state.
  std::this_thread::sleep_for(std::chrono::milliseconds(5));

  result = IOC_offlineService(srvID_);
  ASSERT_EQ(result, IOC_RESULT_SUCCESS)
	  << "Service offline should succeed during pending manual-accept connect";
  srvID_ = IOC_INVALID_SRV_ID;

  connectThread.join();

  IOC_Result_T closeResult = IOC_closeLink(clientConnLinkID_);

  //===>>> VERIFY <<<===
  UT_PHASE_VERIFY(
	  "pending connect resolves as failure and link is cleaned after offline");
  VERIFY_KEYPOINT_NE(connectResult, IOC_RESULT_SUCCESS,
					 "Pending connect must not succeed after service offlines "
					 "before manual accept");
  VERIFY_KEYPOINT_EQ(closeResult, IOC_RESULT_NOT_EXIST_LINK,
					 "Pending link should be cleaned up by offline transition");
  clientConnLinkID_ = IOC_INVALID_LINK_ID;

  //===>>> CLEANUP <<<===
  UT_PHASE_CLEANUP("handled by fixture TearDown");
}

TEST_F(US1_FaultTest,
       TC_P0_F2_verifyConnect_byRuntimeFailureDuringConnect_expectGracefulFailure) {
  //===>>> SETUP <<<===
  UT_PHASE_SETUP(
      "TC_P0_F2_verifyConnect_byRuntimeFailureDuringConnect_"
      "expectGracefulFailure");

  IOC_SrvArgs_T srvArgs;
  IOC_Helper_initSrvArgs(&srvArgs);

  srvArgs.SrvURI.pProtocol = IOC_SRV_PROTO_FIFO;
  srvArgs.SrvURI.pHost = IOC_SRV_HOST_LOCAL_PROCESS;
  srvArgs.SrvURI.pPath = "UT_Service_Fault_RuntimeFailure";
  srvArgs.SrvURI.Port = 0;
  srvArgs.Flags = IOC_SRVFLAG_AUTO_ACCEPT;
  srvArgs.UsageCapabilites = IOC_LinkUsageDatReceiver;
  srvArgs.UsageArgs.pDat = nullptr;

  IOC_Result_T result = IOC_onlineService(&srvID_, &srvArgs);
  ASSERT_EQ(result, IOC_RESULT_SUCCESS)
      << "Service online should succeed before runtime-failure connect test";
  ASSERT_NE(srvID_, IOC_INVALID_SRV_ID)
      << "Service ID must be valid before resource-pressure connect loop";

  IOC_ConnArgs_T connArgs;
  IOC_Helper_initConnArgs(&connArgs);
  connArgs.SrvURI.pProtocol = IOC_SRV_PROTO_FIFO;
  connArgs.SrvURI.pHost = IOC_SRV_HOST_LOCAL_PROCESS;
  connArgs.SrvURI.pPath = "UT_Service_Fault_RuntimeFailure";
  connArgs.SrvURI.Port = 0;
  connArgs.Usage = IOC_LinkUsageDatSender;

  //===>>> BEHAVIOR <<<===
  UT_PHASE_BEHAVIOR(
      "repeated connect under resource pressure until runtime allocation "
      "fails");
  IOC_Result_T failureResult = IOC_RESULT_SUCCESS;
  IOC_LinkID_T failureLinkID = IOC_INVALID_LINK_ID;
  IOC_Bool_T observedFailure = IOC_FALSE;

  for (size_t i = 0; i < 256; ++i) {
    IOC_LinkID_T linkID = IOC_INVALID_LINK_ID;
    result = IOC_connectService(&linkID, &connArgs, nullptr);
    if (result != IOC_RESULT_SUCCESS) {
      failureResult = result;
      failureLinkID = linkID;
      observedFailure = IOC_TRUE;
      break;
    }
    clientConnLinkID_ = linkID;
  }

  //===>>> VERIFY <<<===
  UT_PHASE_VERIFY(
      "runtime failure is reported deterministically and no usable link is "
      "returned on failed connect");
  VERIFY_KEYPOINT_TRUE(observedFailure == IOC_TRUE,
                       "Runtime-failure scenario must eventually observe a "
                       "connect failure under finite link resources");
  VERIFY_KEYPOINT_EQ(failureResult, IOC_RESULT_TOO_MANY_LINKS,
                     "Resource-exhaustion connect failure should report "
                     "IOC_RESULT_TOO_MANY_LINKS");
  VERIFY_KEYPOINT_EQ(
      failureLinkID, IOC_INVALID_LINK_ID,
      "Failed connect must not return a usable link ID during runtime failure");

  //===>>> CLEANUP <<<===
  UT_PHASE_CLEANUP("handled by fixture TearDown");
}
