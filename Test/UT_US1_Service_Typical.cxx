///////////////////////////////////////////////////////////////////////////////////////////////////
// UT_US1_Service_Typical.cxx - CaTDD Implementation for IOC Link Establishment
//
// PURPOSE:
//   Test-driven implementation of P0 Functional / Typical category for US-1:
//   Service/Client link establishment with auto-accept and manual-accept modes.
//
// TDD WORKFLOW:
//   RED stage: Implement tests that fail (no product code yet)
//   GREEN stage: Minimal product code makes tests pass
//   REFACTOR stage: Clean up without behavior change
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "IOC/IOC.h"
#include <gtest/gtest.h>

///////////////////////////////////////////////////////////////////////////////////////////////////
//======>BEGIN OF OVERVIEW OF THIS UNIT TESTING
// FILE===============================================
/**
 * @brief
 *   [WHAT] This file verifies IOC Service/Client link establishment behavior
 *   [WHERE] in the IOC Service API module (IOC_SrvAPI.h)
 *   [WHY] to ensure reliable link creation and pairing under auto-accept and
 * manual-accept modes
 *
 * SCOPE:
 *   - In scope: IOC_onlineService, IOC_connectService, IOC_acceptClient
 * lifecycle
 *   - In scope: Link ID validity and paired link semantics
 *   - Out of scope: Edge cases, timeouts, offline modes (see
 * UT_US1_Service_Edge.cxx)
 *   - Out of scope: Error handling for incompatible usage (see
 * UT_US1_Service_Misuse.cxx)
 *   - Out of scope: Fault recovery and state transitions (see
 * UT_US1_Service_Fault.cxx, State.cxx)
 *
 * KEY CONCEPTS:
 *   - Auto-Accept Mode: Service accepts client implicitly during
 * IOC_connectService
 *   - Manual-Accept Mode: Service must explicitly call IOC_acceptClient after
 * client connects
 *   - Link Pairing: Client-side link and server-side link are complementary
 * endpoints
 *   - Usage Compatibility: DatSender ↔ DatReceiver, EvtConsumer ↔ EvtProducer,
 * etc.
 *
 * RELATIONSHIPS:
 *   - Depends on: IOC_SrvAPI.h, IOC_Types.h (service/link types)
 *   - Related tests: UT_US1_Service_Edge.cxx, UT_US1_Service_Misuse.cxx,
 * UT_US1_Service_Fault.cxx
 *   - Production code: Source/IOC_Service*.c (not yet implemented; graceful
 * when absent)
 *
 * @[Class]: P0 Functional / ValidFunc
 * @[Category]: Typical
 * @[US]: US-1 Establish IOC Link Between Service and Client
 * @[AC]: AC-1, AC-2 (see detailed acceptance criteria below)
 */
//======>END OF OVERVIEW OF THIS UNIT TESTING
// FILE=================================================

///////////////////////////////////////////////////////////////////////////////////////////////////
//======>BEGIN OF ACCEPTANCE CRITERIA
// DESIGN=======================================================
/**
 * [@US-1] Establish IOC Link Between Service and Client
 *
 *  AC-1: GIVEN service online with AUTO_ACCEPT capability and complementary
 * usage type, WHEN client calls IOC_connectService with matching opposite
 * usage, THEN IOC returns IOC_RESULT_SUCCESS, AND both endpoints receive valid
 * non-zero link IDs, AND link is ready for immediate data/event/command use.
 *
 *  AC-2: GIVEN service online with MANUAL_ACCEPT mode (no AUTO_ACCEPT flag),
 *         WHEN client calls IOC_connectService (connection pending),
 *         AND service calls IOC_acceptClient to finalize the pair,
 *         THEN both calls return IOC_RESULT_SUCCESS,
 *          AND both endpoints receive valid non-zero link IDs,
 *          AND paired links are now active.
 *
 * DESIGN RATIONALE:
 *   AC-1 proves the fast-path (auto-accept) works correctly.
 *   AC-2 proves the explicit-accept path works correctly.
 *   Together they cover the core happy-path link establishment logic.
 */
//======>END OF ACCEPTANCE CRITERIA
// DESIGN=======================================================

///////////////////////////////////////////////////////////////////////////////////////////////////
//======>BEGIN OF TEST CASES
// DESIGN================================================================
/**
 * TC-P0-T1: verifyConnect_byAutoAcceptCompatibleUsage_expectSuccess
 *   @[Purpose]: Prove auto-accept link establishment works for compatible usage
 *   @[Brief]:
 *      GIVEN service online with AUTO_ACCEPT and complementary usage
 * (DAT_RECEIVER) WHEN client calls IOC_connectService with matching DAT_SENDER
 * usage THEN IOC returns success and both endpoints receive valid paired link
 * IDs
 *   @[Expect]: IOC_RESULT_SUCCESS from both online and connect calls; non-zero
 * link IDs
 *   @[Status]: RED (test implemented, product code missing) → GREEN → REFACTOR
 *
 * TC-P0-T2: verifyConnect_byManualAcceptPendingClient_expectSuccess
 *   @[Purpose]: Prove manual-accept link establishment works with explicit
 * accept call
 *   @[Brief]:
 *      GIVEN service online in MANUAL_ACCEPT mode
 *      WHEN client calls IOC_connectService (pending) THEN service calls
 * IOC_acceptClient THEN IOC finalizes link pair and both endpoints hold valid
 * paired link IDs
 *   @[Expect]: Both online and connect and accept calls return success;
 * non-zero link IDs
 *   @[Status]: RED (test implemented, product code missing) → GREEN → REFACTOR
 */
//======>END OF TEST CASES
// DESIGN==================================================================

namespace {
class US1_TypicalTest : public ::testing::Test {
protected:
  IOC_SrvID_T srvID_;
  IOC_LinkID_T srvAcceptLinkID_;
  IOC_LinkID_T clientConnLinkID_;

  void SetUp() override {
    srvID_ = 0;
    srvAcceptLinkID_ = 0;
    clientConnLinkID_ = 0;
  }

  void TearDown() override {
    // CLEANUP: close links and offline service if still active
    if (srvAcceptLinkID_ != 0) {
      IOC_closeLink(srvAcceptLinkID_);
    }
    if (clientConnLinkID_ != 0) {
      IOC_closeLink(clientConnLinkID_);
    }
    if (srvID_ != 0) {
      IOC_offlineService(srvID_);
    }
  }
};
} // namespace

///////////////////////////////////////////////////////////////////////////////////////////////////
// TC-P0-T1: verifyConnect_byAutoAcceptCompatibleUsage_expectSuccess
// @[Status]: GREEN (test passes with minimal product code)
///////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * @[Class]: P0 Functional / ValidFunc
 * @[Category]: Typical
 * @[Intent]: Prove auto-accept link establishment works for compatible usage
 * @[UseWhen]: Service online with AUTO_ACCEPT and complementary usage; client
 * calls connect
 * @[AvoidWhen]: Testing Edge (limits), Misuse (wrong usage), Fault (failures),
 * State (transitions)
 * @[US]: US-1
 * @[AC]: AC-1
 * @[TC]: TC-P0-T1 verifyConnect_byAutoAcceptCompatibleUsage_expectSuccess
 *
 * @[Brief]: GIVEN service online with AUTO_ACCEPT capability and DAT_RECEIVER
 * usage, WHEN client calls IOC_connectService with complementary DAT_SENDER
 * usage, THEN IOC returns success and both endpoints hold valid non-zero link
 * IDs
 */
TEST_F(US1_TypicalTest,
       TC_P0_T1_verifyConnect_byAutoAcceptCompatibleUsage_expectSuccess) {
  // SETUP: Prepare service with AUTO_ACCEPT capability
  IOC_SrvArgs_T srvArgs;
  IOC_Helper_initSrvArgs(&srvArgs);

  // Configure service URI (local FIFO for testing)
  srvArgs.SrvURI.pProtocol = IOC_SRV_PROTO_FIFO;
  srvArgs.SrvURI.pHost = IOC_SRV_HOST_LOCAL_PROCESS;
  srvArgs.SrvURI.pPath = "UT_Service_Typical";
  srvArgs.SrvURI.Port = 0;

  // Configure service flags: AUTO_ACCEPT mode
  srvArgs.Flags = IOC_SRVFLAG_AUTO_ACCEPT;

  // Configure service capabilities: this service receives data (DAT_RECEIVER)
  srvArgs.UsageCapabilites = IOC_LinkUsageDatReceiver;

  // No special usage args needed for this typical case (no callbacks for now)
  srvArgs.UsageArgs.pDat = nullptr;

  // BEHAVIOR: Bring service online
  IOC_Result_T result = IOC_onlineService(&srvID_, &srvArgs);
  ASSERT_EQ(result, IOC_RESULT_SUCCESS)
      << "Service failed to come online: expected IOC_RESULT_SUCCESS";
  ASSERT_NE(srvID_, 0) << "Service ID must be non-zero after successful online";

  // BEHAVIOR: Client connects with complementary usage (DAT_SENDER)
  IOC_ConnArgs_T connArgs;
  IOC_Helper_initConnArgs(&connArgs);

  // Configure connection to same service URI
  connArgs.SrvURI.pProtocol = IOC_SRV_PROTO_FIFO;
  connArgs.SrvURI.pHost = IOC_SRV_HOST_LOCAL_PROCESS;
  connArgs.SrvURI.pPath = "UT_Service_Typical";
  connArgs.SrvURI.Port = 0;

  // Configure client link usage: this client sends data (DAT_SENDER)
  connArgs.Usage = IOC_LinkUsageDatSender;

  result = IOC_connectService(&clientConnLinkID_, &connArgs, nullptr);

  // VERIFY: Connection succeeded and both sides have valid link IDs
  EXPECT_EQ(result, IOC_RESULT_SUCCESS)
      << "Client connect should return IOC_RESULT_SUCCESS in auto-accept mode";
  EXPECT_NE(clientConnLinkID_, 0)
      << "Client link ID must be non-zero after successful connect";
  // Note: Server-side link ID is obtained implicitly by service in auto-accept
  // mode via IOC_getServiceLinkIDs() or OnAutoAccepted_F callback

  // CLEANUP: handled by TearDown()
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// TC-P0-T2: verifyConnect_byManualAcceptPendingClient_expectSuccess
// @[Status]: RED (expecting test to fail until manual-accept product code
// exists)
///////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * @[Class]: P0 Functional / ValidFunc
 * @[Category]: Typical
 * @[Intent]: Prove manual-accept link establishment works with explicit accept
 * call
 * @[UseWhen]: Service online without AUTO_ACCEPT; server must explicitly accept
 * pending client
 * @[AvoidWhen]: Testing auto-accept fast path, incompatible usage, or fault
 * handling
 * @[US]: US-1
 * @[AC]: AC-2
 * @[TC]: TC-P0-T2 verifyConnect_byManualAcceptPendingClient_expectSuccess
 *
 * @[Brief]: GIVEN service online in MANUAL_ACCEPT mode, WHEN client calls
 * IOC_connectService and service later calls IOC_acceptClient, THEN IOC returns
 * success and both endpoints hold valid non-zero link IDs
 */
TEST_F(US1_TypicalTest,
       TC_P0_T2_verifyConnect_byManualAcceptPendingClient_expectSuccess) {
  // SETUP: Prepare service without AUTO_ACCEPT so explicit accept is required.
  IOC_SrvArgs_T srvArgs;
  IOC_Helper_initSrvArgs(&srvArgs);

  srvArgs.SrvURI.pProtocol = IOC_SRV_PROTO_FIFO;
  srvArgs.SrvURI.pHost = IOC_SRV_HOST_LOCAL_PROCESS;
  srvArgs.SrvURI.pPath = "UT_Service_Typical_ManualAccept";
  srvArgs.SrvURI.Port = 0;
  srvArgs.Flags = IOC_SRVFLAG_NONE;
  srvArgs.UsageCapabilites = IOC_LinkUsageDatReceiver;
  srvArgs.UsageArgs.pDat = nullptr;

  IOC_Result_T result = IOC_onlineService(&srvID_, &srvArgs);
  ASSERT_EQ(result, IOC_RESULT_SUCCESS)
      << "Service failed to come online for manual-accept scenario";
  ASSERT_NE(srvID_, 0) << "Service ID must be valid before connect/accept";

  // BEHAVIOR: Client connects first; service should accept explicitly later.
  IOC_ConnArgs_T connArgs;
  IOC_Helper_initConnArgs(&connArgs);

  connArgs.SrvURI.pProtocol = IOC_SRV_PROTO_FIFO;
  connArgs.SrvURI.pHost = IOC_SRV_HOST_LOCAL_PROCESS;
  connArgs.SrvURI.pPath = "UT_Service_Typical_ManualAccept";
  connArgs.SrvURI.Port = 0;
  connArgs.Usage = IOC_LinkUsageDatSender;

  result = IOC_connectService(&clientConnLinkID_, &connArgs, nullptr);

  // VERIFY: RED expectation today is that this path is not implemented yet.
  EXPECT_EQ(result, IOC_RESULT_SUCCESS)
      << "Manual-accept client connect should succeed and remain pending until "
         "IOC_acceptClient finalizes it";
  EXPECT_NE(clientConnLinkID_, 0)
      << "Client-side link ID should be reserved for pending manual-accept "
         "connection";

  result = IOC_acceptClient(srvID_, &srvAcceptLinkID_, nullptr);

  EXPECT_EQ(result, IOC_RESULT_SUCCESS)
      << "Service accept should finalize the pending client in manual-accept "
         "mode";
  EXPECT_NE(srvAcceptLinkID_, 0)
      << "Server-side accepted link ID must be non-zero after explicit accept";

  // CLEANUP: handled by TearDown()
}
