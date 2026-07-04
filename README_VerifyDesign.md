# MyInterObjComBase Verification Design

This document is created by SPEC_designUnitTests for US-1.

## Story and Design Inputs

- Story: US-1 Establish IOC Link Between Service and Client
- Detail design: [README_DetailDesign.md](README_DetailDesign.md)
- State design: [README_StateDesign.md](README_StateDesign.md)
- Active story artifact: [.catdd/spec/doingUS/20260618-EstablishedLink-UserStory.md](.catdd/spec/doingUS/20260618-EstablishedLink-UserStory.md)
- Target test files:
  - [Test/UT_US1_Service_Typical.cxx](Test/UT_US1_Service_Typical.cxx)
  - [Test/UT_US1_Service_Edge.cxx](Test/UT_US1_Service_Edge.cxx)
  - [Test/UT_US1_Service_Misuse.cxx](Test/UT_US1_Service_Misuse.cxx)
  - [Test/UT_US1_Service_Fault.cxx](Test/UT_US1_Service_Fault.cxx)
  - [Test/UT_US1_Service_State.cxx](Test/UT_US1_Service_State.cxx)
  - [Test/UT_US1_Service_Capability.cxx](Test/UT_US1_Service_Capability.cxx)

## Source-Command Provenance

- Orchestrator: SPEC_designUnitTests
- P0 contract inspected: .catdd/slashCommands/commands/P0-FuncTestsFlow/UT_designFuncTestsSkeleton.md
- P1 contracts inspected:
  - .catdd/slashCommands/commands/P1-DesignTestsFlow/UT_designStateSkeleton.md
  - .catdd/slashCommands/commands/P1-DesignTestsFlow/UT_designCapabilitySkeleton.md
- P1 contract skipped intentionally:
  - .catdd/slashCommands/commands/P1-DesignTestsFlow/UT_designConcurrencySkeleton.md
  - Reason: project-root README_ResourceDesign.md is not present and concurrency scope is tracked separately by US-2.
- P2 contract inspected for gating:
  - .catdd/slashCommands/commands/P2-QualityTestsFlow/UT_designRobustSkeleton.md
  - Reason for defer: project-root README_ErrorDesign.md is not present.

## Language-Template Provenance

- Target test language: C++ (.cxx, GoogleTest)
- Available method template in methodPrompts: CaTDD_designAndImplTemplate.cxx (C++)
- Gap note: no C-specific CaTDD design template exists yet; this design follows the comment-alive US/AC/TC skeleton structure in split C++ skeleton files.

## Testing Definition

- UnitTesting: verifies function and API behavior for IOC service/link establishment.
- ModuleTesting: verifies IOC service lifecycle behavior across API calls for one module boundary.
- UserTesting: end-to-end workflow verification outside this unit-test skeleton scope.

For US-1, UnitTesting and ModuleTesting are both needed but start from UnitTesting skeletons first.

## Test Strategy

- Build P0 Functional coverage first in strict order: Typical -> Edge -> Misuse -> Fault.
- Promote selective P1 categories only after P0 exists:
  - P1 State required now (README_StateDesign.md exists).
  - P1 Capability required now (README_DetailDesign.md exists).
  - P1 Concurrency deferred (no README_ResourceDesign.md and US-2 owns concurrency slice).
- Defer P2 categories until corresponding design surfaces exist.

## CaTDD Category Coverage

| Priority | Category | Scope | Required Now | Notes |
| --- | --- | --- | --- | --- |
| P0 | Functional: Typical | IOC_onlineService + IOC_connectService + pair creation | Yes | Core AC-1 path |
| P0 | Functional: Edge | Timeout and KEEP/CLOSE offline edge behavior | Yes | AC-1/AC-3 linked |
| P0 | Functional: Misuse | Incompatible usage and offline connect misuse | Yes | AC-1 misuse protection |
| P0 | Functional: Fault | Offline-during-pending and runtime fault cleanup | Yes | AC-2/AC-3 fault outcomes |
| P1 | Design: State | Service lifecycle and pending resolution | Yes | README_StateDesign.md source |
| P1 | Design: Capability | Supported/unsupported accept and usage boundaries | Yes | README_DetailDesign.md source |
| P1 | Design: Concurrency | Shared ownership and interleaving | No | Deferred to US-2 plus resource design |
| P2 | Performance | Timeout budget and latency targets | No | README_PerfDesign.md missing |
| P2 | Robust | Degradation and recovery taxonomy | No | README_ErrorDesign.md missing |
| P2 | Compatibility | Platform/version constraints | No | README_CompatDesign.md missing |
| P2 | Configuration | Mode/config matrix | No | deferred until config design expands |

## US/AC/TC Traceability

| US | AC | TC | Test File | Status |
| --- | --- | --- | --- | --- |
| US-1 | AC-1 | TC-P0-T1 verifyConnect_byAutoAcceptCompatibleUsage_expectSuccess | Test/UT_US1_Service_Typical.cxx | TODO |
| US-1 | AC-2 | TC-P0-T2 verifyConnect_byManualAcceptPendingClient_expectSuccess | Test/UT_US1_Service_Typical.cxx | TODO |
| US-1 | AC-1 | TC-P0-E1 verifyConnect_byAutoAcceptWithoutCallback_expectSuccess | Test/UT_US1_Service_Edge.cxx | TODO |
| US-1 | AC-2 | TC-P0-E2 verifyConnect_byTimeoutOptionExceeded_expectTimeout | Test/UT_US1_Service_Edge.cxx | TODO |
| US-1 | AC-3 | TC-P0-E3 verifyOffline_byKeepAcceptedLinkFlag_expectKeepExistingRejectNew | Test/UT_US1_Service_Edge.cxx | TODO |
| US-1 | AC-3 | TC-P0-E4 verifyOffline_byCloseAcceptedLinkDefault_expectCloseExistingRejectNew | Test/UT_US1_Service_Edge.cxx | TODO |
| US-1 | AC-1 | TC-P0-M1 verifyConnect_byIncompatibleUsage_expectIncompatibleUsage | Test/UT_US1_Service_Misuse.cxx | TODO |
| US-1 | AC-2 | TC-P0-F1 verifyPendingAccept_byServiceOfflineTransition_expectResolvedFailure | Test/UT_US1_Service_Fault.cxx | TODO |
| US-1 | AC-1 | TC-P0-F2 verifyConnect_byRuntimeFailureDuringConnect_expectGracefulFailure | Test/UT_US1_Service_Fault.cxx | TODO |
| US-1 | AC-3 | TC-P1-S1 verifyServiceState_byOfflineToOnlineToOffline_expectDeterministicTransitions | Test/UT_US1_Service_State.cxx | TODO |
| US-1 | AC-2 | TC-P1-S2 verifyPendingQueue_byAcceptTimeoutOffline_expectSingleResolution | Test/UT_US1_Service_State.cxx | TODO |
| US-1 | AC-1 | TC-P1-C1 verifyCapabilityBoundary_byComplementaryUsage_expectAllowedOrRejected | Test/UT_US1_Service_Capability.cxx | TODO |
| US-1 | AC-2 | TC-P1-C2 verifyCapabilityBoundary_byManualAcceptInAutoMode_expectNotSupportManualAccept | Test/UT_US1_Service_Capability.cxx | TODO |

## Parallel-Ready Implementation Checklist

- Slice A (P0 Typical)
  - TC-P0-T1, TC-P0-T2
  - Dependency: service/link fixture helpers
  - Validation checkpoint: both paths create paired link IDs and success result
- Slice B (P0 Edge)
  - TC-P0-E1, TC-P0-E2, TC-P0-E3, TC-P0-E4
  - Dependency: timeout option helper and controllable service flags
  - Validation checkpoint: expected timeout and keep/close semantics observed
- Slice C (P0 Misuse and Fault)
  - TC-P0-M1, TC-P0-F1, TC-P0-F2
  - Dependency: deterministic failure injection hooks
  - Validation checkpoint: no half-initialized links after failure
- Slice D (P1 State and Capability)
  - TC-P1-S1, TC-P1-S2, TC-P1-C1, TC-P1-C2
  - Dependency: state query hooks and mode/capability setup helpers
  - Validation checkpoint: lifecycle transitions and capability boundaries are explicit and stable

## Risks and Deferred Coverage

- Concurrency race and ordering tests are deferred to US-2 and resource-design completion.
- Robust/performance/compatibility quality categories are deferred until corresponding design docs exist.

## Review Checklist

- P0 Functional coverage is defined before P1/P2 promotion.
- Every AC maps to at least one TC.
- Every listed TC has file target and initial status marker.
- SUT is explicitly declared in the target test-file overview.

---

## US-4 Verification Design Addendum

This addendum is created by SPEC_designUnitTests for US-4.

### Story and Design Inputs

- Story: US-4 Re-architect IOC into Explicit L0-L3 Layers
- Active story artifact: [.catdd/spec/doingUS/20260704-reArchDesign-LayeredArchitecture-UserStory.md](.catdd/spec/doingUS/20260704-reArchDesign-LayeredArchitecture-UserStory.md)
- Architecture design: [README_ArchDesign.md](README_ArchDesign.md)
- Detail design: [README_DetailDesign.md](README_DetailDesign.md)
- State design: [README_StateDesign.md](README_StateDesign.md)
- Target test files:
  - [Test/UT_US4_Layering_Typical.cxx](Test/UT_US4_Layering_Typical.cxx)
  - [Test/UT_US4_Layering_Edge.cxx](Test/UT_US4_Layering_Edge.cxx)
  - [Test/UT_US4_Layering_Misuse.cxx](Test/UT_US4_Layering_Misuse.cxx)
  - [Test/UT_US4_Layering_Fault.cxx](Test/UT_US4_Layering_Fault.cxx)

### Flow-Coupling Decision Record

- Selected route: P0-FuncTestsFlow only for this step.
- P0 category decision:
  - Typical: required.
  - Edge: required.
  - Misuse: required.
  - Fault: required.
- P1 promotion decision: not entered in this command.
- P2 promotion decision: not entered in this command.
- UT command contracts inspected:
  - `.catdd/slashCommands/commands/P0-FuncTestsFlow/UT_designFuncTestsSkeleton.md`
- Recommended review command before implementation:
  - `UT_reviewFuncTestsSkeleton`

### Source-Command and Template Provenance

- Orchestrator: `SPEC_designUnitTests`
- P0 design source command: `UT_designFuncTestsSkeleton`
- Language template source: `.catdd/methodPrompts/CaTDD_designAndImplTemplate.cxx`

### US/AC/TC Traceability (US-4 P0)

| US | AC | TC | Category | Test File | Status |
| --- | --- | --- | --- | --- | --- |
| US-4 | AC-4.1 | TC-P0-T1 verifyLayerContract_byValidOwnershipAndDependencyMap_expectAllowedEdgesOnly | Typical | Test/UT_US4_Layering_Typical.cxx | TODO |
| US-4 | AC-4.2 | TC-P0-T2 verifyProtoDispatch_byBoundServiceLinkObject_expectMappedL0Operation | Typical | Test/UT_US4_Layering_Typical.cxx | TODO |
| US-4 | AC-4.1 | TC-P0-E1 verifyDependencyPolicy_byL2ToL0AndL2ToL1Transitions_expectExplicitlyAllowed | Edge | Test/UT_US4_Layering_Edge.cxx | TODO |
| US-4 | AC-4.2 | TC-P0-E2 verifyProtoMethodSet_byFirstMigrationSliceMinimum_expectOnlineAcceptCloseReady | Edge | Test/UT_US4_Layering_Edge.cxx | TODO |
| US-4 | AC-4.1 | TC-P0-M1 verifyDependencyGuards_byForbiddenL3ToL0DirectAccess_expectViolationReported | Misuse | Test/UT_US4_Layering_Misuse.cxx | TODO |
| US-4 | AC-4.2 | TC-P0-M2 verifyProtocolPath_byL2BypassOfServiceLinkProtoMethods_expectViolationReported | Misuse | Test/UT_US4_Layering_Misuse.cxx | TODO |
| US-4 | AC-4.2 | TC-P0-F1 verifyProtoRouting_byUnboundProtoMethods_expectDeterministicFailureAndNoBypassFallback | Fault | Test/UT_US4_Layering_Fault.cxx | TODO |
| US-4 | AC-4.3 | TC-P0-F2 verifyMigrationSafety_byUS1RegressionFailureSignal_expectSliceRejectedForPromotion | Fault | Test/UT_US4_Layering_Fault.cxx | TODO |

### Parallel-Ready Implementation Checklist (US-4)

- Slice A: Typical
  - TCs: TC-P0-T1, TC-P0-T2
  - Dependencies: ownership/dependency extraction helper; ProtoMethods binding probe hook
  - Validation checkpoint: allowed edges only; no L2 protocol bypass path
- Slice B: Edge
  - TCs: TC-P0-E1, TC-P0-E2
  - Dependencies: static-rule parser for dependency policy and first-slice ProtoMethods list
  - Validation checkpoint: L2 boundary fan-out accepted and first-slice methods recognized
- Slice C: Misuse
  - TCs: TC-P0-M1, TC-P0-M2
  - Dependencies: forbidden-edge detector and call-path inspection hook
  - Validation checkpoint: violations are deterministic and traceable
- Slice D: Fault
  - TCs: TC-P0-F1, TC-P0-F2
  - Dependencies: unbound-method simulation and regression-suite status bridge
  - Validation checkpoint: deterministic fault signaling and migration promotion block

### Handoff Decision

- `SPEC_implUnitTests` is not recommended yet.
- Blocking next step: run `UT_reviewFuncTestsSkeleton` for US-4 P0 skeleton review.
