///////////////////////////////////////////////////////////////////////////////////////////////////
// UT_US4_Layering_Edge.cxx - CaTDD Design Skeleton for US-4 Layering Edge
// Scenarios
//
// PURPOSE:
//   P0 Functional / Edge verification for US-4 layered architecture behavior.
//   TC-P0-E1 and TC-P0-E2 are implemented.
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
 * @[Class]: P0 Functional / ValidFunc
 * @[Category]: Edge
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
 * TC-P0-E1
 * verifyDependencyPolicy_byL2ToL0AndL2ToL1Transitions_expectExplicitlyAllowed
 *   @[US]: US-4
 *   @[AC]: AC-4.1
 *   @[TC]: TC-P0-E1
 *   @[Intent]: Cover valid adjacency edges around L2 fan-out boundary.
 *   @[UseWhen]: L2 accesses both L1 and approved L0 concerns.
 *   @[AvoidWhen]: Forbidden bypass checks.
 *   @[Status]: GREEN
 *
 * TC-P0-E2
 * verifyProtoMethodSet_byFirstMigrationSliceMinimum_expectOnlineAcceptCloseReady
 *   @[US]: US-4
 *   @[AC]: AC-4.2
 *   @[TC]: TC-P0-E2
 *   @[Intent]: Cover first migration-slice method boundary (`OpOnlineService`,
 * `OpAcceptClient`, `OpCloseLink`).
 *   @[UseWhen]: First-slice ProtoMethods set is bound and documented.
 *   @[AvoidWhen]: Missing-method or fault-injection paths.
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
// TC-P0-E1:
// verifyDependencyPolicy_byL2ToL0AndL2ToL1Transitions_expectExplicitlyAllowed
// @[Status]: GREEN
///////////////////////////////////////////////////////////////////////////////////////////////////
TEST(
    US4_Layering_EdgeTest,
    TC_P0_E1_verifyDependencyPolicy_byL2ToL0AndL2ToL1Transitions_expectExplicitlyAllowed) {
  //===>>> SETUP <<<===
  UT_PHASE_SETUP("TC_P0_E1_verifyDependencyPolicy_byL2ToL0AndL2ToL1Transitions_"
                 "expectExplicitlyAllowed");

  const std::vector<std::string> readmeCandidates = {
      "README_ArchDesign.md", "../README_ArchDesign.md",
      "../../README_ArchDesign.md", "../../../README_ArchDesign.md"};

  //===>>> BEHAVIOR <<<===
  UT_PHASE_BEHAVIOR("load architecture policy and evaluate explicit L2 fan-out "
                    "allowance to both L1 and L0");

  const std::string archText =
      _UT_US4_loadTextFileFromCandidates(readmeCandidates);
  const std::string normalizedArchText =
      _UT_US4_normalizeForPolicyScan(archText);

  const std::vector<std::string> l2FanoutEvidence = {
      "l2l1", "l2l0",
      "posixplatformprimitivesareallowedforcoreruntimebehavior"};

  const std::vector<std::string> boundarySections = {
      "## Dependency Direction Rules",
      "## Manual Dependency Guard Checklist (First Phase)"};

  //===>>> VERIFY <<<===
  UT_PHASE_VERIFY("architecture policy keeps explicit and valid L2->L1 plus "
                  "L2->L0 edge allowance without boundary ambiguity");

  VERIFY_KEYPOINT_TRUE(!archText.empty(),
                       "README_ArchDesign.md must be readable from the test "
                       "execution location");

  VERIFY_KEYPOINT_TRUE(_UT_US4_containsAllTokens(archText, boundarySections),
                       "Architecture doc must retain dependency rules and "
                       "manual guard checklist sections");

  VERIFY_KEYPOINT_TRUE(
      _UT_US4_containsAllTokens(normalizedArchText, l2FanoutEvidence),
      "Architecture policy must explicitly allow both L2->L1 "
      "and L2->L0 fan-out with approved core-runtime context");

  //===>>> CLEANUP <<<===
  UT_PHASE_CLEANUP("no runtime objects allocated in this documentation policy "
                   "verification test");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// TC-P0-E2:
// verifyProtoMethodSet_byFirstMigrationSliceMinimum_expectOnlineAcceptCloseReady
// @[Status]: GREEN
///////////////////////////////////////////////////////////////////////////////////////////////////
TEST(
    US4_Layering_EdgeTest,
    TC_P0_E2_verifyProtoMethodSet_byFirstMigrationSliceMinimum_expectOnlineAcceptCloseReady) {
  //===>>> SETUP <<<===
  UT_PHASE_SETUP("TC_P0_E2_verifyProtoMethodSet_byFirstMigrationSliceMinimum_"
                 "expectOnlineAcceptCloseReady");

  const std::vector<std::string> detailCandidates = {
      "README_DetailDesign.md", "../README_DetailDesign.md",
      "../../README_DetailDesign.md", "../../../README_DetailDesign.md"};

  //===>>> BEHAVIOR <<<===
  UT_PHASE_BEHAVIOR("load detail design and verify first migration-slice "
                    "ProtoMethods minimum set");

  const std::string detailText =
      _UT_US4_loadTextFileFromCandidates(detailCandidates);
  const std::string normalizedDetailText =
      _UT_US4_normalizeForPolicyScan(detailText);

  const std::vector<std::string> requiredDesignSections = {
      "## Lightweight Implementation Plan", "first migration slice",
      "ProtoMethods"};

  const std::vector<std::string> requiredFirstSliceMethods = {
      "oponlineservice", "opacceptclient", "opcloselink"};

  //===>>> VERIFY <<<===
  UT_PHASE_VERIFY("detail design explicitly documents online/accept/close as "
                  "the first-slice ProtoMethods set");

  VERIFY_KEYPOINT_TRUE(!detailText.empty(),
                       "README_DetailDesign.md must be readable from the test "
                       "execution location");

  VERIFY_KEYPOINT_TRUE(
      _UT_US4_containsAllTokens(detailText, requiredDesignSections),
      "Detail design must retain migration-plan and "
      "ProtoMethods boundary sections");

  VERIFY_KEYPOINT_TRUE(_UT_US4_containsAllTokens(normalizedDetailText,
                                                 requiredFirstSliceMethods),
                       "First migration-slice ProtoMethods must include "
                       "OpOnlineService, OpAcceptClient, and OpCloseLink");

  //===>>> CLEANUP <<<===
  UT_PHASE_CLEANUP("no runtime objects allocated in this documentation policy "
                   "verification test");
}
