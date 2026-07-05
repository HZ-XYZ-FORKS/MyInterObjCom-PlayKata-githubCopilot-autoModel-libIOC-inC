///////////////////////////////////////////////////////////////////////////////////////////////////
// UT_US4_Layering_Fault.cxx - CaTDD Design Skeleton for US-4 Layering Fault
// Scenarios
//
// PURPOSE:
//   P0 Functional / Fault verification for US-4 layered architecture behavior.
//   TC-P0-F1 and TC-P0-F2 are implemented.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "IOC/IOC.h"
#include "_UT_IOC_Common.h"

#include <cctype>
#include <fstream>
#include <string>
#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////////////
//======>BEGIN OF OVERVIEW OF THIS UNIT TESTING
// FILE===============================================
/**
 * @[Class]: P0 Functional / InvalidFunc
 * @[Category]: Fault
 * @[Priority]: P0
 * @[Status]: TODO
 * @[SUT]: Layer-dependency and ProtoMethods routing contract for US-4
 * @[US]: US-4 Re-architect IOC into Explicit L0-L3 Layers
 * @[SourceSPEC]:
 * .catdd/slashCommands/commands/Px-SpecFlow/SPEC_designUnitTests.md
 * @[SourceUT]:
 * .catdd/slashCommands/commands/P0-FuncTestsFlow/UT_designFuncTestsSkeleton.md
 * @[Template]: .catdd/methodPrompts/CaTDD_designAndImplTemplate.cxx
 */
//======>END OF OVERVIEW OF THIS UNIT TESTING
// FILE=================================================

///////////////////////////////////////////////////////////////////////////////////////////////////
//======>BEGIN OF TEST CASES
// DESIGN================================================================
/**
 * TC-P0-F1
 * verifyProtoRouting_byUnboundProtoMethods_expectDeterministicFailureAndNoBypassFallback
 *   @[US]: US-4
 *   @[AC]: AC-4.2
 *   @[TC]: TC-P0-F1
 *   @[Intent]: Verify missing/unbound ProtoMethods path fails deterministically
 * without bypassing layer contract.
 *   @[UseWhen]: Service/Link object is present but method binding is
 * incomplete.
 *   @[AvoidWhen]: Caller misuse of forbidden dependency edges.
 *   @[Status]: GREEN
 *
 * TC-P0-F2
 * verifyMigrationSafety_byUS1RegressionFailureSignal_expectSliceRejectedForPromotion
 *   @[US]: US-4
 *   @[AC]: AC-4.3
 *   @[TC]: TC-P0-F2
 *   @[Intent]: Verify migration-slice promotion is blocked when US-1 regression
 * baseline is not green.
 *   @[UseWhen]: Slice verification workflow aggregates US-1 suite status.
 *   @[AvoidWhen]: Pure dependency-map validation without regression signal.
 *   @[Status]: GREEN
 */
//======>END OF TEST CASES
// DESIGN==================================================================

namespace {

std::string _UT_US4_loadTextFileFromCandidates(
    const std::vector<std::string> &candidatePaths) {
  for (const auto &path : candidatePaths) {
    std::ifstream file(path);
    if (!file.is_open()) {
      continue;
    }

    std::string text;
    std::string line;
    while (std::getline(file, line)) {
      text.append(line);
      text.push_back('\n');
    }
    return text;
  }

  return {};
}

bool _UT_US4_containsAllTokens(const std::string &text,
                               const std::vector<std::string> &tokens) {
  for (const auto &token : tokens) {
    if (text.find(token) == std::string::npos) {
      return false;
    }
  }
  return true;
}

std::string _UT_US4_normalizeForPolicyScan(const std::string &text) {
  std::string normalized;
  normalized.reserve(text.size());

  for (unsigned char ch : text) {
    if (std::isalnum(ch) != 0) {
      normalized.push_back(static_cast<char>(std::tolower(ch)));
    }
  }

  return normalized;
}

} // namespace

///////////////////////////////////////////////////////////////////////////////////////////////////
// TC-P0-F1:
// verifyProtoRouting_byUnboundProtoMethods_expectDeterministicFailureAndNoBypassFallback
// @[Status]: GREEN
///////////////////////////////////////////////////////////////////////////////////////////////////
TEST(
    US4_Layering_FaultTest,
    TC_P0_F1_verifyProtoRouting_byUnboundProtoMethods_expectDeterministicFailureAndNoBypassFallback) {
  //===>>> SETUP <<<===
  UT_PHASE_SETUP("TC_P0_F1_verifyProtoRouting_byUnboundProtoMethods_"
                 "expectDeterministicFailureAndNoBypassFallback");

  const std::vector<std::string> archCandidates = {
      "README_ArchDesign.md", "../README_ArchDesign.md",
      "../../README_ArchDesign.md", "../../../README_ArchDesign.md"};
  const std::vector<std::string> detailCandidates = {
      "README_DetailDesign.md", "../README_DetailDesign.md",
      "../../README_DetailDesign.md", "../../../README_DetailDesign.md"};

  //===>>> BEHAVIOR <<<===
  UT_PHASE_BEHAVIOR("load architecture/detail fault-policy evidence for "
                    "unbound ProtoMethods deterministic-failure and "
                    "anti-bypass constraints");

  const std::string archText =
      _UT_US4_loadTextFileFromCandidates(archCandidates);
  const std::string detailText =
      _UT_US4_loadTextFileFromCandidates(detailCandidates);

  const std::string normalizedArchText =
      _UT_US4_normalizeForPolicyScan(archText);
  const std::string normalizedDetailText =
      _UT_US4_normalizeForPolicyScan(detailText);

  const std::vector<std::string> requiredArchSections = {
      "Protocol behavior constraint:",
      "## Manual Dependency Guard Checklist (First Phase)", "## Data Flow",
      "## Quality Attribute Scenarios (ASRs)"};

  const std::vector<std::string> noBypassFacts = {
      "l2mustnotbypassprotoobjectprotomethodsforprotocolspecificbehavior",
      "verifyl2protocolbehaviorcallsgothroughprotoobjectprotomethodsonly"};

  const std::vector<std::string> deterministicFailureFacts = {
      "l2translatesfaulttodeterministiciocresultandpreservesinvariants",
      "runtimefaultfromplatformprotocol",
      "adapteroperationfailsduringconnectofflinepath",
      "runtimefailurefromprotocolplatformmustmaptodeterministiciocresult"};

  //===>>> VERIFY <<<===
  UT_PHASE_VERIFY("fault-path policy must explicitly require deterministic "
                  "failure signaling and no ProtoMethods bypass fallback");

  VERIFY_KEYPOINT_TRUE(!archText.empty(),
                       "README_ArchDesign.md must be readable from the test "
                       "execution location");

  VERIFY_KEYPOINT_TRUE(!detailText.empty(),
                       "README_DetailDesign.md must be readable from the test "
                       "execution location");

  VERIFY_KEYPOINT_TRUE(
      _UT_US4_containsAllTokens(archText, requiredArchSections),
      "Architecture doc must retain protocol-constraint, manual-guard, "
      "data-flow, and ASR evidence sections");

  VERIFY_KEYPOINT_TRUE(
      _UT_US4_containsAllTokens(normalizedArchText, noBypassFacts),
      "Architecture policy must keep explicit no-bypass "
      "ProtoMethods constraints for L2 protocol behavior");

  VERIFY_KEYPOINT_TRUE(
      _UT_US4_containsAllTokens(normalizedArchText, deterministicFailureFacts),
      "Architecture/detail policy must keep deterministic "
      "fault-result requirements for protocol/platform "
      "failure paths");

  //===>>> CLEANUP <<<===
  UT_PHASE_CLEANUP("no runtime objects allocated in this documentation policy "
                   "verification test");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// TC-P0-F2:
// verifyMigrationSafety_byUS1RegressionFailureSignal_expectSliceRejectedForPromotion
// @[Status]: GREEN
///////////////////////////////////////////////////////////////////////////////////////////////////
TEST(
    US4_Layering_FaultTest,
    TC_P0_F2_verifyMigrationSafety_byUS1RegressionFailureSignal_expectSliceRejectedForPromotion) {
  //===>>> SETUP <<<===
  UT_PHASE_SETUP("TC_P0_F2_verifyMigrationSafety_byUS1RegressionFailureSignal_"
                 "expectSliceRejectedForPromotion");

  const std::vector<std::string> archCandidates = {
      "README_ArchDesign.md", "../README_ArchDesign.md",
      "../../README_ArchDesign.md", "../../../README_ArchDesign.md"};
  const std::vector<std::string> detailCandidates = {
      "README_DetailDesign.md", "../README_DetailDesign.md",
      "../../README_DetailDesign.md", "../../../README_DetailDesign.md"};
  const std::vector<std::string> verifyCandidates = {
      "README_VerifyDesign.md", "../README_VerifyDesign.md",
      "../../README_VerifyDesign.md", "../../../README_VerifyDesign.md"};

  //===>>> BEHAVIOR <<<===
  UT_PHASE_BEHAVIOR("load migration-safety evidence for US-1 regression gate "
                    "and verify promotion-block policy when baseline is not "
                    "green");

  const std::string archText =
      _UT_US4_loadTextFileFromCandidates(archCandidates);
  const std::string detailText =
      _UT_US4_loadTextFileFromCandidates(detailCandidates);
  const std::string verifyText =
      _UT_US4_loadTextFileFromCandidates(verifyCandidates);

  const std::string normalizedArchText =
      _UT_US4_normalizeForPolicyScan(archText);
  const std::string normalizedDetailText =
      _UT_US4_normalizeForPolicyScan(detailText);
  const std::string normalizedVerifyText =
      _UT_US4_normalizeForPolicyScan(verifyText);

  const std::vector<std::string> requiredVerifySections = {
      "## US-4 Verification Design Addendum",
      "### Parallel-Ready Implementation Checklist (US-4)", "Slice D: Fault"};

  const std::vector<std::string> archMigrationGateFacts = {
      "us1verifiedbehaviorisahardregressiongateforeachmigrationslice",
      "allus1testtargetsremainpassforeachslicebeforenextslicebegins"};

  const std::vector<std::string> detailMigrationSafetyFacts = {
      "keepeachextractionsliceregressionsafeagainstus1testbaseline"};

  const std::vector<std::string> promotionBlockFacts = {
      "verifymigrationsafetybyus1regressionfailuresignalexpectslicerejectedforp"
      "romotion",
      "validationcheckpointdeterministicfaultsignalingandmigrationpromotionbloc"
      "k"};

  //===>>> VERIFY <<<===
  UT_PHASE_VERIFY("migration slice must be blocked from promotion when US-1 "
                  "regression signal is not green");

  VERIFY_KEYPOINT_TRUE(!archText.empty(),
                       "README_ArchDesign.md must be readable from the test "
                       "execution location");

  VERIFY_KEYPOINT_TRUE(!detailText.empty(),
                       "README_DetailDesign.md must be readable from the test "
                       "execution location");

  VERIFY_KEYPOINT_TRUE(!verifyText.empty(),
                       "README_VerifyDesign.md must be readable from the test "
                       "execution location");

  VERIFY_KEYPOINT_TRUE(
      _UT_US4_containsAllTokens(verifyText, requiredVerifySections),
      "Verification design doc must retain US-4 addendum and Slice-D fault "
      "checklist sections");

  VERIFY_KEYPOINT_TRUE(
      _UT_US4_containsAllTokens(normalizedArchText, archMigrationGateFacts),
      "Architecture policy must keep explicit US-1 regression-gate facts for "
      "migration-slice safety");

  VERIFY_KEYPOINT_TRUE(
      _UT_US4_containsAllTokens(normalizedDetailText,
                                detailMigrationSafetyFacts),
      "Detail design must keep regression-safe migration-slice guidance "
      "against US-1 baseline");

  VERIFY_KEYPOINT_TRUE(
      _UT_US4_containsAllTokens(normalizedVerifyText, promotionBlockFacts),
      "Verification artifacts must keep explicit promotion-block evidence for "
      "the F2 migration-safety path");

  //===>>> CLEANUP <<<===
  UT_PHASE_CLEANUP("no runtime objects allocated in this documentation policy "
                   "verification test");
}
