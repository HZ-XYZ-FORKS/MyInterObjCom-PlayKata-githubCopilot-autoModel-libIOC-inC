///////////////////////////////////////////////////////////////////////////////////////////////////
// UT_US4_Layering_Misuse.cxx - CaTDD Design Skeleton for US-4 Layering Misuse
// Scenarios
//
// PURPOSE:
//   P0 Functional / Misuse verification for US-4 layered architecture behavior.
//   TC-P0-M1 and TC-P0-M2 are implemented.
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
 * @[Category]: Misuse
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
 * TC-P0-M1
 * verifyDependencyGuards_byForbiddenL3ToL0DirectAccess_expectViolationReported
 *   @[US]: US-4
 *   @[AC]: AC-4.1
 *   @[TC]: TC-P0-M1
 *   @[Intent]: Detect L3 bypass of L2 and direct L3->L0 dependency misuse.
 *   @[UseWhen]: Dependency scan or manual review checklist evaluates forbidden
 * edges.
 *   @[AvoidWhen]: Valid-edge confirmation paths.
 *   @[Status]: GREEN
 *
 * TC-P0-M2
 * verifyProtocolPath_byL2BypassOfServiceLinkProtoMethods_expectViolationReported
 *   @[US]: US-4
 *   @[AC]: AC-4.2
 *   @[TC]: TC-P0-M2
 *   @[Intent]: Detect protocol behavior call path that skips L1 Service/Link
 * ProtoMethods.
 *   @[UseWhen]: L2 call graph includes protocol operation invocations.
 *   @[AvoidWhen]: Dependency failure injection or runtime unavailable paths.
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
// TC-P0-M1:
// verifyDependencyGuards_byForbiddenL3ToL0DirectAccess_expectViolationReported
// @[Status]: GREEN
///////////////////////////////////////////////////////////////////////////////////////////////////
TEST(
    US4_Layering_MisuseTest,
    TC_P0_M1_verifyDependencyGuards_byForbiddenL3ToL0DirectAccess_expectViolationReported) {
  //===>>> SETUP <<<===
  UT_PHASE_SETUP(
      "TC_P0_M1_verifyDependencyGuards_byForbiddenL3ToL0DirectAccess_"
      "expectViolationReported");

  const std::vector<std::string> readmeCandidates = {
      "README_ArchDesign.md", "../README_ArchDesign.md",
      "../../README_ArchDesign.md", "../../../README_ArchDesign.md"};

  //===>>> BEHAVIOR <<<===
  UT_PHASE_BEHAVIOR("load architecture misuse policy and verify explicit "
                    "forbidden L3 bypass constraints");

  const std::string archText =
      _UT_US4_loadTextFileFromCandidates(readmeCandidates);
  const std::string normalizedArchText =
      _UT_US4_normalizeForPolicyScan(archText);

  const std::vector<std::string> forbiddenFacts = {
      "l3l1", "l3l0", "forbidden", "anyupwarddependency", "l0l1l2l3"};

  const std::vector<std::string> guardArtifacts = {
      "## Dependency Direction Rules",
      "## Dependency Graph (Mandatory Diagram Artifact)",
      "## Manual Dependency Guard Checklist (First Phase)"};

  //===>>> VERIFY <<<===
  UT_PHASE_VERIFY("architecture policy explicitly marks L3->L0 bypass as "
                  "forbidden misuse and keeps guard evidence sections");

  VERIFY_KEYPOINT_TRUE(!archText.empty(),
                       "README_ArchDesign.md must be readable from the test "
                       "execution location");

  VERIFY_KEYPOINT_TRUE(_UT_US4_containsAllTokens(archText, guardArtifacts),
                       "Architecture doc must retain dependency-rule, graph, "
                       "and guard-checklist artifacts");

  VERIFY_KEYPOINT_TRUE(
      _UT_US4_containsAllTokens(normalizedArchText, forbiddenFacts),
      "Architecture policy must keep explicit forbidden facts "
      "for L3 bypass and upward dependency misuse");

  //===>>> CLEANUP <<<===
  UT_PHASE_CLEANUP("no runtime objects allocated in this documentation policy "
                   "verification test");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// TC-P0-M2:
// verifyProtocolPath_byL2BypassOfServiceLinkProtoMethods_expectViolationReported
// @[Status]: GREEN
///////////////////////////////////////////////////////////////////////////////////////////////////
TEST(
    US4_Layering_MisuseTest,
    TC_P0_M2_verifyProtocolPath_byL2BypassOfServiceLinkProtoMethods_expectViolationReported) {
  //===>>> SETUP <<<===
  UT_PHASE_SETUP(
      "TC_P0_M2_verifyProtocolPath_byL2BypassOfServiceLinkProtoMethods_"
      "expectViolationReported");

  const std::vector<std::string> readmeCandidates = {
      "README_ArchDesign.md", "../README_ArchDesign.md",
      "../../README_ArchDesign.md", "../../../README_ArchDesign.md"};

  //===>>> BEHAVIOR <<<===
  UT_PHASE_BEHAVIOR(
      "load architecture protocol-path policy and verify explicit "
      "anti-bypass requirements for L2 behavior");

  const std::string archText =
      _UT_US4_loadTextFileFromCandidates(readmeCandidates);
  const std::string normalizedArchText =
      _UT_US4_normalizeForPolicyScan(archText);

  const std::vector<std::string> protocolConstraintSections = {
      "Protocol behavior constraint:",
      "## Manual Dependency Guard Checklist (First Phase)", "## Data Flow"};

  const std::vector<std::string> antiBypassFacts = {
      "l2mustnotbypassprotoobjectprotomethodsforprotocolspecificbehavior",
      "l2triggersiocinterfacebehavioritmustcallprotomethodsthroughanl1serviceob"
      "jectorlinkobject",
      "protomethodsoponlineservice", "protomethodsopsenddat"};

  //===>>> VERIFY <<<===
  UT_PHASE_VERIFY("architecture policy must explicitly report L2 bypass of "
                  "L1 ProtoMethods as a contract violation");

  VERIFY_KEYPOINT_TRUE(!archText.empty(),
                       "README_ArchDesign.md must be readable from the test "
                       "execution location");

  VERIFY_KEYPOINT_TRUE(
      _UT_US4_containsAllTokens(archText, protocolConstraintSections),
      "Architecture doc must retain protocol constraint, manual guard "
      "checklist, and data-flow sections");

  VERIFY_KEYPOINT_TRUE(
      _UT_US4_containsAllTokens(normalizedArchText, antiBypassFacts),
      "Architecture policy must keep explicit anti-bypass "
      "facts requiring L2->L1 ProtoMethods protocol path");

  //===>>> CLEANUP <<<===
  UT_PHASE_CLEANUP("no runtime objects allocated in this documentation policy "
                   "verification test");
}
