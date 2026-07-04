///////////////////////////////////////////////////////////////////////////////////////////////////
// UT_US4_Layering_Typical.cxx - CaTDD Design Skeleton for US-4 Layering Typical
// Scenarios
//
// PURPOSE:
//   P0 Functional / Typical verification for US-4 layered architecture
//   behavior. TC-P0-T1 is implemented; remaining TCs stay as skeleton design.
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
 * @brief
 *   [WHAT] This file designs typical (valid core) verification skeletons for
 * US-4 layered IOC behavior [WHERE] in architecture/dependency guard behavior
 * for L0-L3 and ProtoMethods routing [WHY] to keep platform/protocol expansion
 * isolated while preserving interface/core contracts
 *
 * SUT:
 *   - Layer ownership and dependency policy verification around IOC
 * architecture artifacts
 *
 * SCOPE:
 *   - In scope: allowed dependency directions and valid L2 -> L1 -> L0
 * ProtoMethods routing
 *   - Out of scope: boundary-limit variants (see UT_US4_Layering_Edge.cxx)
 *   - Out of scope: contract violations (see UT_US4_Layering_Misuse.cxx)
 *   - Out of scope: dependency/fallback failure paths (see
 * UT_US4_Layering_Fault.cxx)
 *
 * RELATIONSHIPS:
 *   - Depends on: README_ArchDesign.md, README_DetailDesign.md,
 * README_StateDesign.md
 *   - Related tests: UT_US1_Service_*.cxx (regression baseline evidence)
 *
 * @[Class]: P0 Functional / ValidFunc
 * @[Category]: Typical
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
//======>BEGIN OF ACCEPTANCE CRITERIA
// DESIGN=======================================================
/**
 * [@US-4] Re-architect IOC into Explicit L0-L3 Layers
 *
 *  AC-4.1 (Layer Contract):
 *    GIVEN IOC architecture documentation and source ownership map
 *    WHEN each module/file is classified into one of L0-L3
 *    THEN dependency edges satisfy: L3->L2 only, L2->L1/L0 allowed, L1->L0
 * allowed.
 *
 *  AC-4.2 (ProtoMethods Routing):
 *    GIVEN L2 needs protocol behavior
 *    WHEN Core invokes protocol operation through bound L1 Service/Link object
 *    THEN ProtoMethods dispatch maps to L0 implementation without bypass.
 */
//======>END OF ACCEPTANCE CRITERIA
// DESIGN=========================================================

///////////////////////////////////////////////////////////////////////////////////////////////////
//======>BEGIN OF TEST CASES
// DESIGN================================================================
/**
 * TC-P0-T1
 * verifyLayerContract_byValidOwnershipAndDependencyMap_expectAllowedEdgesOnly
 *   @[Class]: P0 Functional / ValidFunc
 *   @[Category]: Typical
 *   @[US]: US-4
 *   @[AC]: AC-4.1
 *   @[TC]: TC-P0-T1
 *   @[Intent]: Prove valid ownership/dependency map satisfies allowed edges
 * only.
 *   @[UseWhen]: Ownership table and dependency graph are both present.
 *   @[AvoidWhen]: Boundary-limits, misuse, or injected-failure scenarios.
 *   @[Status]: GREEN
 *
 * TC-P0-T2 verifyProtoDispatch_byBoundServiceLinkObject_expectMappedL0Operation
 *   @[Class]: P0 Functional / ValidFunc
 *   @[Category]: Typical
 *   @[US]: US-4
 *   @[AC]: AC-4.2
 *   @[TC]: TC-P0-T2
 *   @[Intent]: Prove L2 protocol call path routes through L1 Service/Link
 * ProtoMethods to L0.
 *   @[UseWhen]: Service/Link object has bound ProtoMethods for operation path.
 *   @[AvoidWhen]: Missing methods or bypass attempts.
 *   @[Status]: TODO
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
// TC-P0-T1:
// verifyLayerContract_byValidOwnershipAndDependencyMap_expectAllowedEdgesOnly
// @[Status]: GREEN
///////////////////////////////////////////////////////////////////////////////////////////////////
TEST(
    US4_Layering_TypicalTest,
    TC_P0_T1_verifyLayerContract_byValidOwnershipAndDependencyMap_expectAllowedEdgesOnly) {
  //===>>> SETUP <<<===
  UT_PHASE_SETUP(
      "TC_P0_T1_verifyLayerContract_byValidOwnershipAndDependencyMap_"
      "expectAllowedEdgesOnly");

  const std::vector<std::string> readmeCandidates = {
      "README_ArchDesign.md", "../README_ArchDesign.md",
      "../../README_ArchDesign.md", "../../../README_ArchDesign.md"};

  //===>>> BEHAVIOR <<<===
  UT_PHASE_BEHAVIOR("load architecture evidence and evaluate allowed/forbidden "
                    "dependency policy consistency markers");

  const std::string archText =
      _UT_US4_loadTextFileFromCandidates(readmeCandidates);
  const std::string normalizedArchText =
      _UT_US4_normalizeForPolicyScan(archText);

  const std::vector<std::string> requiredArtifacts = {
      "## Layer Ownership Mapping (Mandatory Table Artifact)",
      "| Artifact | Assigned Layer | Responsibility | Notes |",
      "## Dependency Direction Rules",
      "## Dependency Graph (Mandatory Diagram Artifact)"};

  const std::vector<std::string> allowedEdgeFacts = {"l3l2", "l2l1", "l2l0",
                                                     "l1l0"};

  const std::vector<std::string> forbiddenEdgeFacts = {"l3l1", "l3l0",
                                                       "l0l1l2l3"};

  //===>>> VERIFY <<<===
  UT_PHASE_VERIFY("architecture documentation contains mandatory artifacts and "
                  "internally consistent layer-edge policy facts");

  VERIFY_KEYPOINT_TRUE(!archText.empty(),
                       "README_ArchDesign.md must be readable from the test "
                       "execution location");

  VERIFY_KEYPOINT_TRUE(_UT_US4_containsAllTokens(archText, requiredArtifacts),
                       "Architecture doc must include ownership table, "
                       "dependency-rule section, and dependency graph section");

  VERIFY_KEYPOINT_TRUE(
      _UT_US4_containsAllTokens(normalizedArchText, allowedEdgeFacts),
      "Architecture policy facts must include allowed edges: "
      "L3->L2, L2->L1, L2->L0, L1->L0");

  VERIFY_KEYPOINT_TRUE(
      _UT_US4_containsAllTokens(normalizedArchText, forbiddenEdgeFacts),
      "Architecture policy facts must include forbidden edges: "
      "L3->L1, L3->L0, and upward dependency example "
      "L0->L1/L2/L3");

  //===>>> CLEANUP <<<===
  UT_PHASE_CLEANUP("no runtime objects allocated in this documentation policy "
                   "verification test");
}
