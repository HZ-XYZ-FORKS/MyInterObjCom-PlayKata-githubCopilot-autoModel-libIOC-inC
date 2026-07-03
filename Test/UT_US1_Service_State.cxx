#include "_UT_IOC_Common.h"
#include <gtest/gtest.h>

#include "IOC/IOC.h"
#include <chrono>
#include <thread>

///////////////////////////////////////////////////////////////////////////////////////////////////
// UT_US1_Service_State.cxx - CaTDD Implementation for IOC Link Establishment (State)
//
// PURPOSE:
//   Test-driven implementation of P1 Design / State category for US-1:
//   service lifecycle state transitions and pending-queue resolution behavior.
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
 *   [WHAT] This file verifies IOC service lifecycle and state-related behaviors.
 *   [WHERE] in the IOC Service API module.
 *   [WHY] to ensure state-query determinism and queue-resolution consistency across transitions.
 *
 * SCOPE:
 *   - In scope: offline->online->offline state-query determinism.
 *   - In scope: pending connect resolution when service transitions offline.
 *   - Out of scope: capacity-limit pressure and nominal happy-path establishment.
 *
 * RELATIONSHIPS:
 *   - Related tests: UT_US1_Service_Typical.cxx, UT_US1_Service_Edge.cxx,
 *     UT_US1_Service_Misuse.cxx, UT_US1_Service_Fault.cxx, UT_US1_Service_Capability.cxx.
 *   - Production code: Source/IOC_SrvAPI.c.
 */
//======>END OF OVERVIEW OF THIS UNIT TESTING FILE=================================================

///////////////////////////////////////////////////////////////////////////////////////////////////
//======>BEGIN OF USER STORY DESIGN================================================================
/**
 * [@US-1] Establish IOC Link Between Service and Client
 *
 * Coverage focus in this file:
 *   - Lifecycle state observability through IOC_getSrvState.
 *   - Pending-queue resolution semantics when service state changes to offline.
 */
//======>END OF USER STORY DESIGN==================================================================

///////////////////////////////////////////////////////////////////////////////////////////////////
//======>BEGIN OF ACCEPTANCE CRITERIA DESIGN=======================================================
/**
 * [@US-1] Establish IOC Link Between Service and Client
 *
 * AC-2: Connect timeout/pending interactions resolve deterministically.
 * AC-3: Service lifecycle transitions preserve deterministic state/availability semantics.
 */
//=======>END OF ACCEPTANCE CRITERIA DESIGN========================================================

namespace {
class US1_StateTest : public ::testing::Test {
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
 * SUT: IOC service lifecycle state behavior
 * APIs: IOC_onlineService, IOC_offlineService, IOC_getSrvState
 *
 * @[Class]: P1 Design
 * @[Category]: State
 * @[US]: US-1
 * @[AC]: AC-3
 * @[TC]: TC-P1-S1
 * verifyServiceState_byOfflineToOnlineToOffline_expectDeterministicTransitions
 * @[Status]: GREEN
 *
 * @[AC]: AC-2
 * @[TC]: TC-P1-S2
 * verifyPendingQueue_byAcceptTimeoutOffline_expectSingleResolution
 * @[Status]: GREEN
 */
//======>END OF TEST CASES DESIGN==================================================================

TEST_F(US1_StateTest,
     TC_P1_S1_verifyServiceState_byOfflineToOnlineToOffline_expectDeterministicTransitions) {
  //===>>> SETUP <<<===
  UT_PHASE_SETUP(
    "TC_P1_S1_verifyServiceState_byOfflineToOnlineToOffline_"
    "expectDeterministicTransitions");

  IOC_SrvArgs_T srvArgs;
  IOC_Helper_initSrvArgs(&srvArgs);

  srvArgs.SrvURI.pProtocol = IOC_SRV_PROTO_FIFO;
  srvArgs.SrvURI.pHost = IOC_SRV_HOST_LOCAL_PROCESS;
  srvArgs.SrvURI.pPath = "UT_Service_State_Lifecycle";
  srvArgs.SrvURI.Port = 0;
  srvArgs.Flags = IOC_SRVFLAG_AUTO_ACCEPT;
  srvArgs.UsageCapabilites = IOC_LinkUsageDatReceiver;
  srvArgs.UsageArgs.pDat = nullptr;

  IOC_SrvState_T srvState = IOC_SrvStateOffline;

  //===>>> BEHAVIOR <<<===
  UT_PHASE_BEHAVIOR(
    "query invalid/offline handle, online service, query online state, "
    "offline service, query removed handle");

  IOC_Result_T result = IOC_getSrvState(IOC_INVALID_SRV_ID, &srvState);

  IOC_Result_T onlineResult = IOC_onlineService(&srvID_, &srvArgs);
  IOC_Result_T onlineStateResult = IOC_getSrvState(srvID_, &srvState);

  IOC_SrvID_T offlinedSrvID = srvID_;
  IOC_Result_T offlineResult = IOC_offlineService(srvID_);
  srvID_ = IOC_INVALID_SRV_ID;

  IOC_Result_T removedStateResult = IOC_getSrvState(offlinedSrvID, &srvState);

  //===>>> VERIFY <<<===
  UT_PHASE_VERIFY(
    "state-query results are deterministic across offline/online/offline "
    "transition path");
  VERIFY_KEYPOINT_EQ(
    result, IOC_RESULT_INVALID_PARAM,
    "Querying state with IOC_INVALID_SRV_ID should be rejected as invalid "
    "parameter");
  VERIFY_KEYPOINT_EQ(onlineResult, IOC_RESULT_SUCCESS,
           "Service online should succeed in lifecycle transition "
           "test");
  VERIFY_KEYPOINT_EQ(onlineStateResult, IOC_RESULT_SUCCESS,
           "State query should succeed for an active service ID");
  VERIFY_KEYPOINT_EQ(srvState, IOC_SrvStateOnline,
           "Active service state should be IOC_SrvStateOnline");
  VERIFY_KEYPOINT_EQ(offlineResult, IOC_RESULT_SUCCESS,
           "Service offline should succeed for an active service");
  VERIFY_KEYPOINT_EQ(
    removedStateResult, IOC_RESULT_NOT_EXIST_SERVICE,
    "State query should report IOC_RESULT_NOT_EXIST_SERVICE after service "
    "is offlined");

  //===>>> CLEANUP <<<===
  UT_PHASE_CLEANUP("handled by fixture TearDown");
}

TEST_F(US1_StateTest,
	   TC_P1_S2_verifyPendingQueue_byAcceptTimeoutOffline_expectSingleResolution) {
  //===>>> SETUP <<<===
  UT_PHASE_SETUP(
	  "TC_P1_S2_verifyPendingQueue_byAcceptTimeoutOffline_"
	  "expectSingleResolution");

  IOC_SrvArgs_T srvArgs;
  IOC_Helper_initSrvArgs(&srvArgs);

  srvArgs.SrvURI.pProtocol = IOC_SRV_PROTO_FIFO;
  srvArgs.SrvURI.pHost = IOC_SRV_HOST_LOCAL_PROCESS;
  srvArgs.SrvURI.pPath = "UT_Service_State_PendingTimeoutOffline";
  srvArgs.SrvURI.Port = 0;
  srvArgs.Flags = IOC_SRVFLAG_NONE; // manual-accept flow
  srvArgs.UsageCapabilites = IOC_LinkUsageDatReceiver;
  srvArgs.UsageArgs.pDat = nullptr;

  IOC_Result_T result = IOC_onlineService(&srvID_, &srvArgs);
  ASSERT_EQ(result, IOC_RESULT_SUCCESS)
	  << "Service online should succeed before pending queue state test";
  ASSERT_NE(srvID_, IOC_INVALID_SRV_ID)
	  << "Service ID must be valid before pending connect behavior";

  IOC_ConnArgs_T connArgs;
  IOC_Helper_initConnArgs(&connArgs);
  connArgs.SrvURI.pProtocol = IOC_SRV_PROTO_FIFO;
  connArgs.SrvURI.pHost = IOC_SRV_HOST_LOCAL_PROCESS;
  connArgs.SrvURI.pPath = "UT_Service_State_PendingTimeoutOffline";
  connArgs.SrvURI.Port = 0;
  connArgs.Usage = IOC_LinkUsageDatSender;

  IOC_Option_defineSyncTimeout(connectOpt, 200000); // 200ms
  IOC_Result_T connectResult = IOC_RESULT_FAILURE;

  //===>>> BEHAVIOR <<<===
  UT_PHASE_BEHAVIOR("start pending connect, offline service, then attempt "
					"manual accept to confirm single final resolution");

  std::thread connectThread([&]() {
	connectResult = IOC_connectService(&clientConnLinkID_, &connArgs, &connectOpt);
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(5));

  IOC_SrvID_T offlineSrvID = srvID_;
  result = IOC_offlineService(srvID_);
  ASSERT_EQ(result, IOC_RESULT_SUCCESS)
	  << "Service offline should succeed while a client is pending";
  srvID_ = IOC_INVALID_SRV_ID;

  connectThread.join();

  IOC_LinkID_T acceptedLinkID = IOC_INVALID_LINK_ID;
  IOC_Result_T acceptResult = IOC_acceptClient(offlineSrvID, &acceptedLinkID, nullptr);

  //===>>> VERIFY <<<===
  UT_PHASE_VERIFY("pending connect resolves once as failure and later accept is rejected");
  VERIFY_KEYPOINT_NE(connectResult, IOC_RESULT_SUCCESS,
					 "Pending connect should not succeed after service offline transition");
  VERIFY_KEYPOINT_EQ(acceptResult, IOC_RESULT_NOT_EXIST_SERVICE,
					 "After offline, IOC_acceptClient should report "
					 "IOC_RESULT_NOT_EXIST_SERVICE");
  VERIFY_KEYPOINT_EQ(acceptedLinkID, IOC_INVALID_LINK_ID,
					 "Rejected accept must not produce a valid server link ID");

  IOC_Result_T closeResult = IOC_closeLink(clientConnLinkID_);
  VERIFY_KEYPOINT_EQ(closeResult, IOC_RESULT_NOT_EXIST_LINK,
					 "Pending link should be cleaned by offline resolution path");
  clientConnLinkID_ = IOC_INVALID_LINK_ID;

  //===>>> CLEANUP <<<===
  UT_PHASE_CLEANUP("handled by fixture TearDown");
}
