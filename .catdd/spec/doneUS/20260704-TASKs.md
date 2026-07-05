# TASKs for US-4 Re-architect IOC into Explicit L0-L3 Layers

> **Active Story (Closed):** `.catdd/spec/doneUS/20260704-reArchDesign-LayeredArchitecture-UserStory.md`
> **Story ID:** `US-4`
> **Planning Command:** `SPEC_makePlan`
> **Plan Date:** 2026-07-04

---

## Current Readiness

- [x] Active story is now closed under `.catdd/spec/doneUS/`.
- [x] Mutual intent is cleared (`Review Result: CLEARED`).
- [x] Requirement source is traceable to `.catdd/spec/analyzedNews/20260624-reArchDesign-Issue.md`.
- [x] Project-level story ledger is consistent (`README_UserStories.md` shows US-4 as `done` with doneUS trace link).
- [x] Story scope is design-oriented (layer boundaries, dependency rules, migration strategy).
- [x] Architecture design artifact for US-4 is created in project-root README SPEC docs (`README_ArchDesign.md`).
- [x] Architecture review gate is passed after re-review.
- [x] Detail design artifacts for US-4 are available in `README_DetailDesign.md` and `README_StateDesign.md`.
- [x] Detail design review gate is passed after re-review.
- [x] Unit-test design readiness is satisfied after requirement-ledger sync.

---

## Work Orientation

- [x] Intent-clearing work is already complete.
- [ ] Requirement-oriented update is required now.
- [x] Design-oriented work is required next.
- [ ] Implementation-oriented work is ready now.

Reasoning:

- The active story and mutual-intent contract are aligned and stable enough to start design planning.
- The story objective is architecture-first (L0-L3 layering and dependency direction), so implementation is premature.
- No new requirement-surface delta is currently identified for `README_UserStories.md` or `README_UserGuide.md`.

---

## Skipped Or Satisfied Prerequisites

- [x] `SPEC_clearStoryIntent` is satisfied.
- [x] Requirement-update path is skipped for now because no requirement-surface delta is currently identified.
- [x] `SPEC_reviewUserStory` is not required at this step because requirement docs are not being changed in this planning round.
- [x] `SPEC_takeArchDesign` is completed.

---

## Candidate Next Steps

- [ ] `SPEC_updateUserStory`
  - Rejected now: no explicit requirement-surface changes are pending in project requirement docs.

- [ ] `SPEC_reviewUserStory`
  - Rejected now: requirement-update branch is not active in this planning round.

- [x] `SPEC_takeArchDesign`
  - Selected: this is initial architecture design for explicit L0-L3 ownership and dependency-direction constraints.

- [ ] `SPEC_updateArchDesign`
  - Rejected now: no prior US-4 architecture artifact exists yet; follow-up revision is not the first step.

- [ ] `SPEC_takeDetailDesign`
  - Rejected now: detail design should follow architecture baseline and architecture review.

- [ ] `SPEC_designUnitTests`
  - Rejected now: requirement/design readiness for implementation-oriented entry is not yet complete.

---

## Selected Next Step

- [x] Executed command: `SPEC_takeArchDesign`
- [x] Executed command: `SPEC_reviewArchDesign`
- [x] Executed command: `SPEC_updateArchDesign`
- [x] Re-executed command: `SPEC_reviewArchDesign`
- [x] Executed command: `SPEC_takeDetailDesign`
- [x] Executed command: `SPEC_reviewDetailDesign`
- [x] Executed command: `SPEC_updateDetailDesign`
- [x] Re-executed command: `SPEC_reviewDetailDesign`
- [x] Executed command: `SPEC_reviewUserStory`
- [x] Executed command: `SPEC_updateUserStory`
- [x] Re-executed command: `SPEC_reviewUserStory`
- [x] Executed command: `SPEC_designUnitTests`
- [x] Next command: `UT_reviewFuncTestsSkeleton`

Rationale:

- US-4 is a P1 design story centered on architecture boundaries and dependency rules.
- Architecture baseline now exists with mandatory ownership table + dependency diagram and explicit layer rules.
- Architecture review result is `REVISE` due to cross-section rule inconsistency in `README_ArchDesign.md`.
- The smallest correct next lifecycle step is architecture revision before any detail design.

## Architecture Review Result (SPEC_reviewArchDesign)

- Finding: `REVISE`
- Evidence:
  - `README_ArchDesign.md` states strict adjacency-only direction in goals (`L3 -> L2 -> L1 -> L0` only), but dependency rules also allow `L2 -> L0`.
  - `README_ArchDesign.md` requires ProtoObject ProtoMethods for L2 protocol behavior; this rule must be reflected consistently across goals/views/checks.
- Required revision focus for `SPEC_updateArchDesign`:
  - Normalize architecture goals, views, and dependency text to the same rule set (`L3->L2`, `L2->L1/L0`, `L1->L0`, ProtoObject ProtoMethods constraint for protocol behavior).
  - Reconfirm review checklist/rules so no contradictory statements remain.

## Architecture Update Result (SPEC_updateArchDesign)

- Update status: completed
- Addressed in `README_ArchDesign.md`:
  - Normalized architecture goals to match approved dependency policy.
  - Preserved ProtoObject ProtoMethods constraint and dependency-rule consistency.
  - Added architecture-feedback checklist and remaining-risks section for re-review traceability.
- Re-gate requirement: run `SPEC_reviewArchDesign` again before `SPEC_takeDetailDesign`.

## Architecture Re-Review Result (SPEC_reviewArchDesign)

- Finding: `PASS`
- Evidence:
  - Architecture goal statement now matches approved dependency policy and no longer conflicts with dependency rules.
  - ProtoObject ProtoMethods constraint remains explicit in dependency rules and manual guard checklist.
  - Required C4 views, module/consuming-system context, and architecture-oriented surface coverage are present.
- Decision:
  - Architecture is approved to enter detail-design phase.

## Detail Design Result (SPEC_takeDetailDesign)

- Update status: completed
- Design artifacts updated:
  - `README_DetailDesign.md`
  - `README_StateDesign.md`
- Coverage added:
  - L3/L2/L1/L0 structure ownership at implementation-detail level
  - ProtoObject and ProtoMethods contract responsibilities
  - migration slice plan and regression gating
  - migration/runtime state models for refactor and protocol dispatch
- Next gate:
  - run `SPEC_reviewDetailDesign` before any unit-test design or code changes

## Detail Design Review Result (SPEC_reviewDetailDesign)

- Finding: `REVISE`
- Evidence:
  - `README_DetailDesign.md` defines `ProtoObject::ProtoMethods` as `connect/send/recv/accept/poll/close as needed`, which leaves the minimum required method set ambiguous for AC-4.2.
  - `README_DetailDesign.md` open question still asks which exact minimum ProtoMethods set is needed for the first migration slice.
- Required revision focus for `SPEC_updateDetailDesign`:
  - Define the concrete minimum ProtoMethods set for the first migration slice.
  - Tie each method explicitly to current lifecycle use in the migration slice so AC-4.2 is directly convertible to CaTDD skeleton design.

## Detail Design Update Result (SPEC_updateDetailDesign)

- Update status: completed
- Addressed in `README_DetailDesign.md`:
  - Defined the first migration slice ProtoMethods set explicitly as `connect`, `accept`, and `close`.
  - Added a per-method table mapping first-slice use versus deferred methods.
  - Removed the ambiguous open question about minimum ProtoMethods scope.
- Re-gate requirement:
  - run `SPEC_reviewDetailDesign` again before `SPEC_designUnitTests`.

## Detail Design Re-Review Result (SPEC_reviewDetailDesign)

- Finding: `PASS`
- Evidence:
  - ProtoMethods dispatch is now explicit through L1 Service/Link objects with concrete first-slice methods `OpOnlineService`, `OpAcceptClient`, and `OpCloseLink`.
  - AC-4.2 is now directly convertible to CaTDD skeleton design because the minimum method set and lifecycle call usage are explicit.
  - State design aligns with the updated object-plus-method-table dispatch model.
- Decision:
  - Detail design is approved for final story/design readiness review.

## User Story Review Result (SPEC_reviewUserStory)

- Finding: `REVISE`
- Evidence:
  - `README_UserStories.md` shows US-4 in the story ledger, but the Acceptance Criteria Ledger does not yet contain trace/status rows for US-4 acceptance criteria.
  - The active US-4 story defines three acceptance scenarios (layer contract, adapter isolation, migration safety), but those are not yet reflected in project-level AC trace/status.
- Required revision focus for `SPEC_updateUserStory`:
  - Add US-4 acceptance-criteria ledger rows to `README_UserStories.md` with status and verification trace seeds aligned to the active story/design artifacts.
  - Reconfirm project-level requirement traceability so story-level ACs and lifecycle docs are synchronized.

## User Story Update Result (SPEC_updateUserStory)

- Update status: completed
- Addressed in `README_UserStories.md`:
  - Added US-4 AC ledger rows `AC-4.1`, `AC-4.2`, and `AC-4.3`.
  - Added status and verification trace fields aligned to active story and design artifacts.

## User Story Re-Review Result (SPEC_reviewUserStory)

- Finding: `PASS`
- Evidence:
  - `README_UserStories.md` story ledger and lifecycle links are consistent with the active US-4 doing story.
  - `README_UserStories.md` Acceptance Criteria Ledger now contains traceable US-4 rows aligned to the active story scenarios and design artifacts.
  - Story, architecture, and detail design are all synchronized enough to enter CaTDD unit-test design.
- Decision:
  - Requirement/design readiness review is complete; transfer to unit-test design.

## User Story PASS Reaffirmation (SPEC_reviewUserStory)

- Finding: `PASS`
- Evidence:
  - The later clarification that IOC interface behavior maps into ProtoMethods on L1 Service/Link objects remains consistent with the active US-4 acceptance criteria and design artifacts.
  - No project-ledger, lifecycle-state, or requirement-trace mismatch was introduced by the wording update.
- Decision:
  - Keep routing to `SPEC_designUnitTests`.

## Unit Test Design Result (SPEC_designUnitTests)

- Finding: `DONE` for P0 Functional skeleton design scope.
- P0 route decision:
  - Applied full P0 category set through `UT_designFuncTestsSkeleton` contract (Typical, Edge, Misuse, Fault).
- Artifacts created:
  - `Test/UT_US4_Layering_Typical.cxx`
  - `Test/UT_US4_Layering_Edge.cxx`
  - `Test/UT_US4_Layering_Misuse.cxx`
  - `Test/UT_US4_Layering_Fault.cxx`
- Verification design updated:
  - `README_VerifyDesign.md` appended with US-4 addendum including US/AC/TC trace and implementation slices.
- Traceability gate:
  - US-4 AC-4.1/4.2/4.3 are mapped to at least one TC.
  - Designed files include SUT, source-command provenance, and initial status markers.
- Decision:
  - Run `UT_reviewFuncTestsSkeleton` before any executable test implementation in `SPEC_implUnitTests`.

## Functional Skeleton Review Result (UT_reviewFuncTestsSkeleton)

- Finding: `PASS` with non-blocking clarifications.
- Coverage summary:
  - Typical: present (`TC-P0-T1`, `TC-P0-T2`) in `Test/UT_US4_Layering_Typical.cxx`.
  - Edge: present (`TC-P0-E1`, `TC-P0-E2`) in `Test/UT_US4_Layering_Edge.cxx`.
  - Misuse: present (`TC-P0-M1`, `TC-P0-M2`) in `Test/UT_US4_Layering_Misuse.cxx`.
  - Fault: present (`TC-P0-F1`, `TC-P0-F2`) in `Test/UT_US4_Layering_Fault.cxx`.
- Traceability check:
  - Every reviewed skeleton declares class/category and includes US/AC/TC links.
  - US-4 AC coverage is complete for P0 scope: AC-4.1, AC-4.2, and AC-4.3 each map to at least one TC.
- Conflicts/duplication:
  - No category conflicts or duplicated TC intent found.
- Non-blocking assumptions to clarify before implementation:
  - Decide the concrete evidence source for dependency-edge and call-path checks (manual checklist parser, static analyzer output, or hybrid).
  - Confirm expected failure signaling contract for unbound ProtoMethods in `TC-P0-F1`.
- Decision:
  - Skeleton set is coherent enough to proceed to next-TC selection.
  - Next command: `UT_tellMeNextImplTest`.

## Next Implementation TC Selection Result (UT_tellMeNextImplTest)

- Finding: `SELECTED`
- Selected TC:
  - `TC-P0-T1 verifyLayerContract_byValidOwnershipAndDependencyMap_expectAllowedEdgesOnly`
  - Source file: `Test/UT_US4_Layering_Typical.cxx`
  - Trace: `US-4` / `AC-4.1` / `P0 Functional / Typical`
- Selection reason:
  - Follows CaTDD P0 priority (`Typical -> Edge -> Misuse -> Fault`) and picks the highest-priority unimplemented TC.
  - Establishes baseline evidence for allowed/forbidden dependency edges that downstream Edge/Misuse/Fault checks can reuse.
  - Current status marker is `TODO` with no blockers recorded.
- Preconditions before implementation:
  - Confirm the concrete edge-evidence input source for the first implementation slice (manual checklist parser, static analyzer output, or hybrid).
  - Keep implementation scope to one TC only in the next step.
- Decision:
  - Next command: `UT_implTestCase` for `TC-P0-T1`.

## Test Case Implementation Result (UT_implTestCase)

- Finding: `DONE` for selected scope (`TC-P0-T1` only).
- Implemented TC:
  - `TC-P0-T1 verifyLayerContract_byValidOwnershipAndDependencyMap_expectAllowedEdgesOnly`
  - File: `Test/UT_US4_Layering_Typical.cxx`
- Implementation notes:
  - Preserved CaTDD US/AC/TC skeleton comments.
  - Updated selected TC status marker from `TODO` to `GREEN`.
  - Added strict 4-phase implementation layout: `SETUP -> BEHAVIOR -> VERIFY -> CLEANUP`.
  - Used `VERIFY_KEYPOINT_TRUE` assertions for key checks.
  - Kept `TC-P0-T2` unchanged (`TODO`) to satisfy one-TC-at-a-time guard.
- Validation notes:
  - Focused build command attempted: `cmake --build CMakeBuilt --target UT_US4_Layering_Typical`.
  - Result: target not found in current generated build system (`No rule to make target 'UT_US4_Layering_Typical'`).
  - Action needed before execution validation: refresh CMake generation to include the new test file target, then run the focused target.
- Decision:
  - Next command: `UT_reviewImplTestCase` for implemented `TC-P0-T1`.

## Implementation Review Result (UT_reviewImplTestCase)

- Finding: `REVISE`
- Alignment checks:
  - Skeleton and implementation still map to `US-4 / AC-4.1 / TC-P0-T1` in `Test/UT_US4_Layering_Typical.cxx`.
  - Required 4-phase test structure is present (`SETUP -> BEHAVIOR -> VERIFY -> CLEANUP`).
  - Key assertions use `VERIFY_KEYPOINT_TRUE` as required by command contract.
- Drift / risks found:
  - TC intent says "ownership and dependency map satisfies allowed edges only", but implementation currently performs markdown token presence checks only; it does not verify ownership-map evidence is actually parsed/validated.
  - Forbidden-edge verification depends on specific prose/token formatting (`"`L3 -> L1` and `L3 -> L0`"`, mermaid marker text), making the test brittle to wording/layout updates that keep the same semantics.
  - Validation status is incomplete because the focused target is not in current build generation yet.
- Required revision focus for next `UT_implTestCase` loop:
  - Keep one-TC scope (`TC-P0-T1`).
  - Refine verification to check stable policy evidence with lower formatting coupling and explicit ownership-map presence criteria.
  - Re-run focused build/test after CMake regeneration includes the US-4 test target.
- Decision:
  - Next command: `UT_implTestCase` (revision loop) for `TC-P0-T1`.

## Test Case Revision Result (UT_implTestCase Retry)

- Finding: `DONE` for requested doc-verification intent.
- Clarified intent (confirmed by developer):
  - Verify architecture document correctness and internal consistency only.
  - Do not validate repository source-code dependency edges in this TC.
- Revision applied to `TC-P0-T1` (`Test/UT_US4_Layering_Typical.cxx`):
  - Kept one-TC scope and preserved `US/AC/TC` trace comments.
  - Added normalized policy-fact checks to reduce coupling to markdown punctuation/layout.
  - Added explicit mandatory-artifact checks for ownership table and dependency sections/diagram.
- Decision:
  - Next command: `UT_reviewImplTestCase` to confirm revised implementation aligns with skeleton intent.

## Implementation Re-Review Result (UT_reviewImplTestCase)

- Finding: `PASS`
- Alignment checks:
  - Implemented `TC-P0-T1` remains aligned with `US-4 / AC-4.1` intent in one-TC scope.
  - `SETUP -> BEHAVIOR -> VERIFY -> CLEANUP` phase layout is explicit and readable.
  - Key checks remain in VERIFY using `VERIFY_KEYPOINT_TRUE` macros.
- Review notes:
  - The TC now verifies architecture document internal consistency and mandatory artifacts, which matches the clarified doc-only verification intent.
  - This TC intentionally does not validate source-code dependency edges.
  - Build-target regeneration is still needed later for executable validation, but this does not block implementation-design alignment review.
- Decision:
  - Keep current implementation of `TC-P0-T1`.
  - Next command: `UT_tellMeNextImplTest`.

## Unit Test Implementation Result (SPEC_implUnitTests)

- Finding: `DONE` for one P0 slice in this pass.
- ReACT summary:
  - Reason: P0-first ordering is active and the next ready Typical TC after `TC-P0-T1` is `TC-P0-T2`.
  - Act: selected and implemented `TC-P0-T2` in `Test/UT_US4_Layering_Typical.cxx`.
  - Observe: implementation keeps one-TC scope, preserves CaTDD comments, and uses strict `SETUP/BEHAVIOR/VERIFY/CLEANUP` with `VERIFY_KEYPOINT_TRUE` assertions.
  - Decide: continue with next P0 TC selection.
- Implemented TC:
  - `TC-P0-T2 verifyProtoDispatch_byBoundServiceLinkObject_expectMappedL0Operation`
  - File: `Test/UT_US4_Layering_Typical.cxx`
  - Status marker updated: `TODO -> GREEN`
- Review outcome (UT_reviewImplTestCase criteria):
  - Alignment with skeleton intent: `PASS`
  - No setup/cleanup gap found.
  - No drift requiring skeleton redesign in this pass.
- Decision:
  - Next command: `UT_tellMeNextImplTest` to pick the next P0 TC.

## Next Implementation TC Selection Result (UT_tellMeNextImplTest After TC-P0-T2)

- Finding: `SELECTED`
- Selected TC:
  - `TC-P0-E1 verifyDependencyPolicy_byL2ToL0AndL2ToL1Transitions_expectExplicitlyAllowed`
  - Source file: `Test/UT_US4_Layering_Edge.cxx`
  - Trace: `US-4` / `AC-4.1` / `P0 Functional / Edge`
- Selection reason:
  - P0-first order is preserved and Typical (`TC-P0-T1`, `TC-P0-T2`) is already `GREEN`.
  - `TC-P0-E1` is the highest-priority remaining unimplemented (`TODO`) P0 TC.
  - This TC extends AC-4.1 coverage to valid L2 fan-out boundaries before entering Misuse/Fault negative paths.
- Preconditions:
  - Keep one-TC implementation scope in the next step.
  - Maintain doc-consistency verification intent (not source-code dependency correctness) unless developer overrides.
- Decision:
  - Next command: `UT_implTestCase` for `TC-P0-E1`.

## Test Case Implementation Result (UT_implTestCase for TC-P0-E1)

- Finding: `DONE` for selected scope (`TC-P0-E1` only).
- Implemented TC:
  - `TC-P0-E1 verifyDependencyPolicy_byL2ToL0AndL2ToL1Transitions_expectExplicitlyAllowed`
  - File: `Test/UT_US4_Layering_Edge.cxx`
- Implementation notes:
  - Preserved CaTDD US/AC/TC skeleton comments.
  - Updated selected TC status marker from `TODO` to `GREEN`.
  - Added strict 4-phase implementation layout: `SETUP -> BEHAVIOR -> VERIFY -> CLEANUP`.
  - Used `VERIFY_KEYPOINT_TRUE` assertions for key checks.
  - Kept `TC-P0-E2` unchanged (`TODO`) to satisfy one-TC-at-a-time guard.
- Validation notes:
  - Focused executable validation remains pending CMake target regeneration for US-4 tests.
- Decision:
  - Next command: `UT_reviewImplTestCase` for implemented `TC-P0-E1`.

## Implementation Review Result (UT_reviewImplTestCase for TC-P0-E1)

- Finding: `PASS`
- Alignment checks:
  - `TC-P0-E1` implementation remains aligned with `US-4 / AC-4.1` edge-intent in one-TC scope.
  - Required 4-phase layout is explicit and readable (`SETUP -> BEHAVIOR -> VERIFY -> CLEANUP`).
  - Key checks stay in VERIFY and use `VERIFY_KEYPOINT_TRUE` macros.
- Review notes:
  - Edge coverage now includes explicit L2 fan-out allowance checks for both `L2->L1` and `L2->L0` with doc-consistency intent.
  - `TC-P0-E2` remains untouched (`TODO`), preserving one-TC-at-a-time flow.
  - Focused executable validation still depends on CMake regeneration for US-4 targets; this does not block implementation-skeleton alignment review.
- Decision:
  - Keep current implementation of `TC-P0-E1`.
  - Next command: `UT_tellMeNextImplTest`.

## Next Implementation TC Selection Result (UT_tellMeNextImplTest After TC-P0-E1)

- Finding: `SELECTED`
- Selected TC:
  - `TC-P0-E2 verifyProtoMethodSet_byFirstMigrationSliceMinimum_expectOnlineAcceptCloseReady`
  - Source file: `Test/UT_US4_Layering_Edge.cxx`
  - Trace: `US-4` / `AC-4.2` / `P0 Functional / Edge`
- Selection reason:
  - P0-first order is preserved; Typical and first Edge TC (`TC-P0-E1`) are already `GREEN`.
  - `TC-P0-E2` is now the highest-priority remaining unimplemented (`TODO`) P0 TC.
  - This TC completes Edge coverage for AC-4.2 before moving into Misuse/Fault categories.
- Preconditions:
  - Keep one-TC implementation scope in the next step.
  - Maintain doc-consistency verification intent unless developer explicitly switches to source-code dependency validation.
- Decision:
  - Next command: `UT_implTestCase` for `TC-P0-E2`.

## Test Case Implementation Result (UT_implTestCase for TC-P0-E2)

- Finding: `DONE` for selected scope (`TC-P0-E2` only).
- Implemented TC:
  - `TC-P0-E2 verifyProtoMethodSet_byFirstMigrationSliceMinimum_expectOnlineAcceptCloseReady`
  - File: `Test/UT_US4_Layering_Edge.cxx`
- Implementation notes:
  - Preserved CaTDD US/AC/TC skeleton comments.
  - Updated selected TC status marker from `TODO` to `GREEN`.
  - Added strict 4-phase implementation layout: `SETUP -> BEHAVIOR -> VERIFY -> CLEANUP`.
  - Used `VERIFY_KEYPOINT_TRUE` assertions for key checks.
  - Kept Misuse/Fault TCs unchanged (`TODO`) to preserve one-TC-at-a-time progress.
- Validation notes:
  - Focused executable validation remains pending CMake target regeneration for US-4 tests.
- Decision:
  - Next command: `UT_reviewImplTestCase` for implemented `TC-P0-E2`.

## Implementation Review Result (UT_reviewImplTestCase for TC-P0-E2)

- Finding: `PASS`
- Alignment checks:
  - `TC-P0-E2` implementation remains aligned with `US-4 / AC-4.2` edge-intent in one-TC scope.
  - Required 4-phase layout is explicit (`SETUP -> BEHAVIOR -> VERIFY -> CLEANUP`).
  - Key checks stay in VERIFY and use `VERIFY_KEYPOINT_TRUE` macros.
- Review notes:
  - Edge coverage now includes first migration-slice ProtoMethods minimum-set verification (`OpOnlineService`, `OpAcceptClient`, `OpCloseLink`) from detail design.
  - Misuse/Fault TCs remain untouched (`TODO`), preserving one-TC-at-a-time flow.
  - Focused executable validation still depends on CMake regeneration for US-4 targets; this does not block implementation-skeleton alignment review.
- Decision:
  - Keep current implementation of `TC-P0-E2`.
  - Next command: `UT_tellMeNextImplTest`.

## Next Implementation TC Selection Result (UT_tellMeNextImplTest After TC-P0-E2)

- Finding: `SELECTED`
- Selected TC:
  - `TC-P0-M1 verifyDependencyGuards_byForbiddenL3ToL0DirectAccess_expectViolationReported`
  - Source file: `Test/UT_US4_Layering_Misuse.cxx`
  - Trace: `US-4` / `AC-4.1` / `P0 Functional / Misuse`
- Selection reason:
  - P0-first order is preserved; Typical and Edge TCs are already `GREEN`.
  - `TC-P0-M1` is the highest-priority remaining unimplemented (`TODO`) P0 TC.
  - This TC starts invalid-path verification before fault-path verification.
- Preconditions:
  - Keep one-TC implementation scope in the next step.
  - Maintain doc-consistency verification intent unless developer explicitly changes the evidence model.
- Decision:
  - Next command: `UT_implTestCase` for `TC-P0-M1`.

## Unit Test Implementation Result (UT_implTestCase for TC-P0-M1)

- Finding: `DONE`
- Scope:
  - Implemented only `TC-P0-M1 verifyDependencyGuards_byForbiddenL3ToL0DirectAccess_expectViolationReported` in `Test/UT_US4_Layering_Misuse.cxx`.
  - Kept `TC-P0-M2` in `TODO` state (no implementation in this step).
- Implemented behavior summary:
  - Added file-read helper and normalization utilities for architecture-policy scanning.
  - Added one executable test case with strict `SETUP/BEHAVIOR/VERIFY/CLEANUP` phase blocks.
  - Verified artifact presence for dependency rules, mandatory dependency graph, and manual guard checklist sections.
  - Verified explicit forbidden misuse facts for `L3 -> L0`, `L3 -> L1`, and upward-dependency prohibition.
- Constraint check:
  - One-TC-only implementation scope preserved.
  - Evidence model follows doc-consistency verification intent.
- Decision:
  - Next command: `UT_reviewImplTestCase` for implemented `TC-P0-M1`.

## Implementation Review Result (UT_reviewImplTestCase for TC-P0-M1)

- Finding: `PASS`
- Alignment summary:
  - US/AC/TC mapping is explicit and consistent (`US-4` / `AC-4.1` / `TC-P0-M1`).
  - `SETUP/BEHAVIOR/VERIFY/CLEANUP` phases are present and readable.
  - Assertions verify the promised misuse expectation by requiring explicit forbidden-edge policy facts in architecture documentation artifacts.
- Review checks:
  - No one-TC scope violation (`TC-P0-M2` remains `TODO`).
  - No drift detected between skeleton intent and implementation evidence model.
  - Key assertion density is sufficient for this doc-consistency misuse case.
- Decision:
  - Keep current `TC-P0-M1` implementation.
  - Next command: `UT_tellMeNextImplTest`.

## Next Implementation TC Selection Result (UT_tellMeNextImplTest After TC-P0-M1)

- Finding: `SELECTED`
- Selected TC:
  - `TC-P0-M2 verifyProtocolPath_byL2BypassOfServiceLinkProtoMethods_expectViolationReported`
  - Source file: `Test/UT_US4_Layering_Misuse.cxx`
  - Trace: `US-4` / `AC-4.2` / `P0 Functional / Misuse`
- Selection reason:
  - P0-first order remains satisfied.
  - `TC-P0-M2` is the highest-priority remaining `TODO` after `TC-P0-M1` review pass.
  - Misuse category should complete before moving into Fault category.
- Preconditions:
  - Keep one-TC implementation scope in the next step.
  - Preserve the current doc-consistency evidence model unless developer changes intent.
- Decision:
  - Next command: `UT_implTestCase` for `TC-P0-M2`.

## Unit Test Implementation Result (UT_implTestCase for TC-P0-M2)

- Finding: `DONE`
- Scope:
  - Implemented only `TC-P0-M2 verifyProtocolPath_byL2BypassOfServiceLinkProtoMethods_expectViolationReported` in `Test/UT_US4_Layering_Misuse.cxx`.
  - Kept Fault TCs (`TC-P0-F1`, `TC-P0-F2`) untouched in `TODO` state.
- Implemented behavior summary:
  - Added one executable misuse TC with strict `SETUP/BEHAVIOR/VERIFY/CLEANUP` phase blocks.
  - Verified architecture sections covering protocol constraint, manual guard checklist, and data flow are present.
  - Verified explicit anti-bypass policy facts that require L2 protocol behavior to route through L1 Service/Link ProtoMethods and disallow direct bypass.
- Constraint check:
  - One-TC-only implementation scope preserved.
  - Evidence model remains doc-consistency validation, aligned with developer intent.
- Decision:
  - Next command: `UT_reviewImplTestCase` for implemented `TC-P0-M2`.

## Implementation Review Result (UT_reviewImplTestCase for TC-P0-M2)

- Finding: `PASS`
- Alignment summary:
  - US/AC/TC trace is explicit and consistent (`US-4` / `AC-4.2` / `TC-P0-M2`).
  - `SETUP/BEHAVIOR/VERIFY/CLEANUP` phase structure is clear and compliant.
  - Assertions verify the promised misuse expectation by enforcing explicit anti-bypass protocol-path facts in architecture documentation.
- Review checks:
  - No one-TC scope drift was introduced in this step.
  - `TC-P0-M2` implementation remains consistent with the current doc-consistency evidence model.
  - Key assertions are present and focused on AC-4.2 intent.
- Decision:
  - Keep current `TC-P0-M2` implementation.
  - Next command: `UT_tellMeNextImplTest`.

## Next Implementation TC Selection Result (UT_tellMeNextImplTest After TC-P0-M2)

- Finding: `SELECTED`
- Selected TC:
  - `TC-P0-F1 verifyProtoRouting_byUnboundProtoMethods_expectDeterministicFailureAndNoBypassFallback`
  - Source file: `Test/UT_US4_Layering_Fault.cxx`
  - Trace: `US-4` / `AC-4.2` / `P0 Functional / Fault`
- Selection reason:
  - P0-first category progression is maintained.
  - Typical, Edge, and Misuse categories are already review-passed.
  - `TC-P0-F1` is the highest-priority remaining `TODO` before `TC-P0-F2`.
- Preconditions:
  - Keep one-TC implementation scope in the next step.
  - Continue the current doc-consistency evidence model unless developer changes intent.
- Decision:
  - Next command: `UT_implTestCase` for `TC-P0-F1`.

## Unit Test Implementation Result (UT_implTestCase for TC-P0-F1)

- Finding: `DONE`
- Scope:
  - Implemented only `TC-P0-F1 verifyProtoRouting_byUnboundProtoMethods_expectDeterministicFailureAndNoBypassFallback` in `Test/UT_US4_Layering_Fault.cxx`.
  - Kept `TC-P0-F2` in `TODO` state (no implementation in this step).
- Implemented behavior summary:
  - Added one executable fault TC with strict `SETUP/BEHAVIOR/VERIFY/CLEANUP` phase blocks.
  - Verified architecture/detail evidence sections for protocol constraints, manual guard checklist, data flow, and ASR reliability context.
  - Verified explicit no-bypass ProtoMethods constraints and deterministic-failure policy facts for protocol/platform fault paths.
- Constraint check:
  - One-TC-only implementation scope preserved.
  - Evidence model remains doc-consistency verification, aligned with current developer intent.
- Decision:
  - Next command: `UT_reviewImplTestCase` for implemented `TC-P0-F1`.

## Implementation Review Result (UT_reviewImplTestCase for TC-P0-F1)

- Finding: `PASS`
- Alignment summary:
  - US/AC/TC trace is explicit and consistent (`US-4` / `AC-4.2` / `TC-P0-F1`).
  - `SETUP/BEHAVIOR/VERIFY/CLEANUP` phase structure is complete and readable.
  - Assertions verify deterministic-failure and no-bypass expectations through architecture/detail policy evidence.
- Review checks:
  - No one-TC scope drift detected (`TC-P0-F2` remains `TODO`).
  - No skeleton-intent drift detected for fault-path semantics.
  - Minor note: `normalizedDetailText` is currently computed but not directly used; non-blocking for this review step.
- Decision:
  - Keep current `TC-P0-F1` implementation.
  - Next command: `UT_tellMeNextImplTest`.

## Next Implementation TC Selection Result (UT_tellMeNextImplTest After TC-P0-F1)

- Finding: `SELECTED`
- Selected TC:
  - `TC-P0-F2 verifyMigrationSafety_byUS1RegressionFailureSignal_expectSliceRejectedForPromotion`
  - Source file: `Test/UT_US4_Layering_Fault.cxx`
  - Trace: `US-4` / `AC-4.3` / `P0 Functional / Fault`
- Selection reason:
  - P0-first progression is preserved and all earlier categories/cases are review-passed.
  - `TC-P0-F2` is the only remaining `TODO` P0 TC.
  - This completes the fault category and final pending P0 path for US-4.
- Preconditions:
  - Keep one-TC implementation scope in the next step.
  - Maintain current doc-consistency evidence model unless developer changes intent.
- Decision:
  - Next command: `UT_implTestCase` for `TC-P0-F2`.

## Unit Test Implementation Result (UT_implTestCase for TC-P0-F2)

- Finding: `DONE`
- Scope:
  - Implemented only `TC-P0-F2 verifyMigrationSafety_byUS1RegressionFailureSignal_expectSliceRejectedForPromotion` in `Test/UT_US4_Layering_Fault.cxx`.
  - No additional TC or category expansion was introduced in this step.
- Implemented behavior summary:
  - Added one executable fault TC with strict `SETUP/BEHAVIOR/VERIFY/CLEANUP` phase blocks.
  - Verified US-1 regression-gate policy facts across architecture/detail artifacts.
  - Verified verification-design evidence for Slice-D fault flow and explicit promotion-block traceability for the F2 path.
- Constraint check:
  - One-TC-only implementation scope preserved.
  - Evidence model remains doc-consistency verification, aligned with current developer intent.
- Decision:
  - Next command: `UT_reviewImplTestCase` for implemented `TC-P0-F2`.

## Implementation Review Result (UT_reviewImplTestCase for TC-P0-F2)

- Finding: `PASS`
- Alignment summary:
  - US/AC/TC trace is explicit and consistent (`US-4` / `AC-4.3` / `TC-P0-F2`).
  - `SETUP/BEHAVIOR/VERIFY/CLEANUP` phase structure is complete and readable.
  - Assertions verify migration-safety promotion-block intent via US-1 regression-gate and verification-design evidence.
- Review checks:
  - No one-TC scope drift detected in this step.
  - No mismatch detected between F2 skeleton intent and implementation evidence model.
  - Key assertions are sufficient for the documented AC-4.3 path.
- Decision:
  - Keep current `TC-P0-F2` implementation.
  - Next command: `UT_tellMeNextImplTest`.

## Next Implementation TC Selection Result (UT_tellMeNextImplTest After TC-P0-F2)

- Finding: `NO_READY_TC`
- Selection outcome:
  - No remaining `TODO` or `RED` test case in US-4 P0 categories (`Typical`, `Edge`, `Misuse`, `Fault`).
  - Current P0 status: `TC-P0-T1/T2`, `TC-P0-E1/E2`, `TC-P0-M1/M2`, `TC-P0-F1/F2` are all `GREEN`.
- Selection reason:
  - Command contract prefers highest-priority unimplemented/RED TC; none remain.
  - Preconditions for additional `UT_implTestCase` in P0 are not met.
- Decision:
  - P0 implementation selection loop is complete for US-4.
  - Next command: `SPEC_whatsNextTask`.

## Next Lifecycle Decision Result (SPEC_whatsNextTask After P0 Completion)

- Finding: `SELECTED`
- Recommended next command:
  - `SPEC_implProductCodes`
- Rationale:
  - US-4 P0 unit-test implementation/review loop is complete (`NO_READY_TC` from `UT_tellMeNextImplTest`).
  - Px-SpecFlow implementation-oriented sequence advances from unit-test implementation to product-code implementation before product-code review and close.
  - Selecting `SPEC_implProductCodes` avoids skipping required downstream lifecycle gates.
- Required input artifacts:
  - Active story and task artifacts in `.catdd/spec/doingUS/`.
  - Current design/test evidence in `README_ArchDesign.md`, `README_DetailDesign.md`, and `README_VerifyDesign.md`.
- Decision:
  - Next command: `SPEC_implProductCodes`.

## Product Code Implementation Result (SPEC_implProductCodes for US-4)

- Finding: `DONE_NO_CODE_CHANGE`
- Scope evaluation:
  - Active US-4 verification model in current loop is documentation-consistency based.
  - Completed P0 tests verify architecture/detail/verification artifacts and do not introduce failing runtime behavior gaps that require immediate `Source/*` changes in this step.
- Product code decision:
  - No product-code modification is applied in this command.
  - Existing production files remain unchanged to avoid widening scope beyond active evidence requirements.
- Traceability:
  - Inputs validated from active story/task artifacts and `README_ArchDesign.md`, `README_DetailDesign.md`, `README_VerifyDesign.md`.
  - Decision preserves one-story scope and defers any structural code migration to explicit follow-up story scope.
- Verification note:
  - No additional runtime execution was required for this no-change implementation step.
- Decision:
  - Next command: `SPEC_reviewProductCodes`.

## Product Code Review Result (SPEC_reviewProductCodes for US-4)

- Finding: `PASS`
- Review scope:
  - Changed files are limited to US-4 test/design lifecycle artifacts and US-4 test files.
  - No unintended production-file expansion was introduced in this loop.
- Quality findings:
  - P0 Functional categories are fully implemented and marked `GREEN` across `Typical`, `Edge`, `Misuse`, and `Fault` test files.
  - Traceability between active story, task artifact, and verification-design surfaces is preserved.
  - Product-code step outcome `DONE_NO_CODE_CHANGE` remains consistent with the active documentation-consistency verification model.
- Verification evidence note:
  - Runtime build/test execution was not rerun in this review step; decision is based on artifact consistency and prior UT review trail.
- Decision:
  - Review passes for current story scope.
  - Next command: `SPEC_commitWorks`.

## Commit Result (SPEC_commitWorks for US-4)

- Finding: `DONE`
- Commit scope summary:
  - Included active story lifecycle artifacts under `.catdd/spec/doingUS/`.
  - Included US-4 test artifacts in `Test/UT_US4_Layering_*.cxx` carrying final P0 implementation/review status.
  - Excluded unrelated files and local-only artifacts.
- Commit message (WHAT/HOW/WHY):
  - WHAT: finalize US-4 P0 test implementation/review lifecycle artifacts and test files.
  - HOW: preserve the completed CaTDD P0 loop (Typical/Edge/Misuse/Fault) and SPEC decision trail through product-code review.
  - WHY: keep a coherent, traceable checkpoint before story-close routing.
- Decision:
  - Next command: `SPEC_closeUserStory`.

## Close Result (SPEC_closeUserStory for US-4)

- Finding: `DONE`
- Lifecycle move:
  - Moved story/task artifacts from `.catdd/spec/doingUS/` to `.catdd/spec/doneUS/`.
  - Removed duplicate active copy from `.catdd/spec/doingUS/`.
- Ledger/link synchronization:
  - `README_UserStories.md` synchronized from `doing` to `done` for US-4.
  - Story-specific design/verification trace links normalized to `.catdd/spec/doneUS/`.
  - `SPEC_updateProjectContext` applied in-flow for major lifecycle impact.
- CI/verification summary used for close gate:
  - Accepted equivalent: artifact-consistency verification trail with commit `8d34906` and `SPEC_reviewProductCodes` `PASS`.
- Close checkpoint:
  - `close_commit_required = yes`
  - `next_command = SPEC_commitWorks`

---

## Task Checklist

- [x] Open US-4 into `.catdd/spec/doingUS/`.
- [x] Clear mutual intent for US-4.
- [x] Create paired planning artifact for US-4.
- [x] Run `SPEC_takeArchDesign` for US-4 layered architecture baseline.
- [x] Run `SPEC_reviewArchDesign` after architecture design is produced.
- [x] Run `SPEC_updateArchDesign` to address review findings.
- [x] Re-run `SPEC_reviewArchDesign` after architecture update.
- [x] Run `SPEC_takeDetailDesign` after architecture review passes.
- [x] Run `SPEC_reviewDetailDesign` after detail design is produced.
- [x] Run `SPEC_updateDetailDesign` to address detail-review findings.
- [x] Re-run `SPEC_reviewDetailDesign` after detail design update.
- [x] Run `SPEC_reviewUserStory` for final story/design readiness.
- [x] Run `SPEC_updateUserStory` to synchronize project-level requirement ledger.
- [x] Run `SPEC_designUnitTests` now that design readiness is confirmed.
- [x] Run `UT_reviewFuncTestsSkeleton` for US-4 P0 skeletons before implementation.
- [x] Run `UT_tellMeNextImplTest` to select the first US-4 implementation TC.
- [x] Run `UT_implTestCase` for `TC-P0-T1 verifyLayerContract_byValidOwnershipAndDependencyMap_expectAllowedEdgesOnly`.
- [x] Run `UT_reviewImplTestCase` for implemented `TC-P0-T1`.
- [x] Re-run `UT_implTestCase` to revise `TC-P0-T1` per review findings.
- [x] Re-run `UT_reviewImplTestCase` for `TC-P0-T1` after revision.
- [x] Run `UT_tellMeNextImplTest` to select the next US-4 TC after `TC-P0-T1`.
- [x] Run `SPEC_implUnitTests` pass to implement selected next TC (`TC-P0-T2`).
- [x] Run `UT_tellMeNextImplTest` for the next remaining P0 TC (Edge/Misuse/Fault).
- [x] Run `UT_implTestCase` for `TC-P0-E1 verifyDependencyPolicy_byL2ToL0AndL2ToL1Transitions_expectExplicitlyAllowed`.
- [x] Run `UT_reviewImplTestCase` for implemented `TC-P0-E1`.
- [x] Run `UT_tellMeNextImplTest` to select the next remaining P0 TC after `TC-P0-E1`.
- [x] Run `UT_implTestCase` for `TC-P0-E2 verifyProtoMethodSet_byFirstMigrationSliceMinimum_expectOnlineAcceptCloseReady`.
- [x] Run `UT_reviewImplTestCase` for implemented `TC-P0-E2`.
- [x] Run `UT_tellMeNextImplTest` to select the next remaining P0 TC after `TC-P0-E2`.
- [x] Run `UT_implTestCase` for `TC-P0-M1 verifyDependencyGuards_byForbiddenL3ToL0DirectAccess_expectViolationReported`.
- [x] Run `UT_reviewImplTestCase` for implemented `TC-P0-M1`.
- [x] Run `UT_tellMeNextImplTest` to select the next remaining P0 TC after `TC-P0-M1`.
- [x] Run `UT_implTestCase` for `TC-P0-M2 verifyProtocolPath_byL2BypassOfServiceLinkProtoMethods_expectViolationReported`.
- [x] Run `UT_reviewImplTestCase` for implemented `TC-P0-M2`.
- [x] Run `UT_tellMeNextImplTest` to select the next remaining P0 TC after `TC-P0-M2`.
- [x] Run `UT_implTestCase` for `TC-P0-F1 verifyProtoRouting_byUnboundProtoMethods_expectDeterministicFailureAndNoBypassFallback`.
- [x] Run `UT_reviewImplTestCase` for implemented `TC-P0-F1`.
- [x] Run `UT_tellMeNextImplTest` to select the next remaining P0 TC after `TC-P0-F1`.
- [x] Run `UT_implTestCase` for `TC-P0-F2 verifyMigrationSafety_byUS1RegressionFailureSignal_expectSliceRejectedForPromotion`.
- [x] Run `UT_reviewImplTestCase` for implemented `TC-P0-F2`.
- [x] Run `UT_tellMeNextImplTest` to confirm whether any P0 TC remains after `TC-P0-F2`.
- [x] Run `SPEC_whatsNextTask` to decide the next lifecycle command after P0 completion.
- [x] Run `SPEC_implProductCodes` for US-4 implementation-oriented lifecycle continuation.
- [x] Run `SPEC_reviewProductCodes` to review product-code readiness before commit/close gates.
- [x] Run `SPEC_commitWorks` for current US-4 scoped changes.
- [x] Run `SPEC_closeUserStory` to move active story artifacts to done state after commit.
- [x] Run `SPEC_updateProjectContext` in-flow for major lifecycle impact after closure.
- [ ] Continue implementation/review/commit/close commands in lifecycle order when entry gates are satisfied.

---

## Open Questions Or Blockers

- [x] Artifact policy confirmed: require both ownership table and dependency diagram for initial layer mapping evidence.
- [x] Dependency-guard sequence confirmed: manual review checklist first, then scripted dependency scan hardening.
- [x] Decision recorded before `SPEC_takeArchDesign` to reduce architecture-review churn.

---

## Requirement Trace

- Active story: `.catdd/spec/doneUS/20260704-reArchDesign-LayeredArchitecture-UserStory.md`
- Requirement source: `.catdd/spec/analyzedNews/20260624-reArchDesign-Issue.md`
- Project story ledger: `README_UserStories.md`
- Project context: `.catdd/spec/projectContext.md`
