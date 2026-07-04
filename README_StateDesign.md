# MyInterObjComBase State Design

State design for US-1 link-establishment lifecycle.

## Story Context

- Story: US-1 Establish IOC Link Between Service and Client
- Source artifact: `.catdd/spec/doingUS/20260618-EstablishedLink-UserStory.md`
- Related detail design: [README_DetailDesign.md](README_DetailDesign.md)
- Related verification design: [README_VerifyDesign.md](README_VerifyDesign.md)

## State Model

| State | Meaning | Owner | Entry Condition | Exit Condition |
| --- | --- | --- | --- | --- |
| `IOC_SrvStateOffline` | Service not available for new clients | Service registry | Initial or after offline completes | `IOC_onlineService` accepted |
| `IOC_SrvStateOnlining` | Online transition in progress | Service registry | `IOC_onlineService` starts | Validation/bind done -> Online; failure -> Offline |
| `IOC_SrvStateOnline` | Service accepts/handles connect flow | Service registry | Online transition success | `IOC_offlineService` starts |
| `IOC_SrvStateOfflining` | Service shutting down admissions and handling links | Service registry | `IOC_offlineService` starts | Cleanup done -> Offline |

## State Transitions

| From | Event | Guard | Action | To | Failure Behavior |
| --- | --- | --- | --- | --- | --- |
| Offline | `IOC_onlineService` | Valid `SrvArgs` and no conflict | Register service, apply flags, init admission mode | Online | Return invalid/conflict/too-many result; remain Offline |
| Online | `IOC_connectService` (auto mode) | Compatible usage + capacity | Allocate pair and bind mapping | Online | Return incompatible/too-many/timeout/fault |
| Online | `IOC_connectService` (manual mode) | Compatible usage + queue available | Enqueue pending request | Online | Return queue/capacity/fault |
| Online | `IOC_acceptClient` | Pending exists and mode supports manual accept | Finalize pair for pending request | Online | Return no-pending/not-supported/fault |
| Online | `IOC_offlineService` | Valid SrvID | Close admission, apply keep/close policy | Offlining->Offline | Return invalid/busy transition conflict |
| Offlining | New `IOC_connectService` | Always | Reject new connect attempts | Offlining | Return deterministic rejection |

## Invariants

- New connects are rejected whenever service state is Offlining or Offline.
- A pending manual-connect request is resolved exactly once: accepted, timed out, or failed by offline cleanup.
- Link pair mapping remains one-to-one while active.
- Offline keep/close behavior is deterministic by service flags configured at online time.

## Concurrency and Ownership

| Shared State | Writer | Reader | Synchronization | Race/Fault Risk |
| --- | --- | --- | --- | --- |
| Service lifecycle state | Service control path | Connect/accept paths | Service-level mutex or atomic + ordered transitions | Connect/offline race |
| Pending connect queue | Connect path (enqueue), accept/offline path (dequeue) | Accept/connect status checks | Queue lock + condition signaling | Lost/duplicate pending request resolution |
| Link-pair table | Connect/accept/close/offline paths | Messaging and diagnostics | Link-table lock + ID validity checks | Half-initialized pair or stale reference |
| Admission mode flags | Set at online | Connect/offline paths | Immutable snapshot per online session | Mode drift during runtime |

## Embedded and Digital Media State Points

Embedded software points:

- Hardware lifecycle: Not hardware-state specific in US-1; software service lifecycle only.
- ISR/task handoff: Not ISR-driven; state transitions occur in API call context.
- DMA state: Not applicable in this scope.
- Peripheral fault state: Modeled via IOC fault/timeout result paths.

digital video/audio points:

- Pipeline state: Not applicable in this scope.
- Stream sync state: Not applicable in this scope.
- Buffer state: Pending connect queue only; media buffers are out of scope.
- Format state: Not applicable in this scope.

## Review Checklist

- Every transition has clear guard/action/failure behavior.
- Shared state ownership and synchronization are explicit.
- State design is sufficient to drive `SPEC_designUnitTests` after review.

---

# US-4 State Design

State design for US-4 layered-architecture migration and L1 Service/Link object ProtoMethods dispatch.

## Story Context

- Story: US-4 Re-architect IOC into Explicit L0-L3 Layers
- Source artifact: `.catdd/spec/doingUS/20260704-reArchDesign-LayeredArchitecture-UserStory.md`
- Related architecture: [README_ArchDesign.md](README_ArchDesign.md)
- Related detail design: [README_DetailDesign.md](README_DetailDesign.md)

## Migration State Model

| State | Meaning | Owner | Entry Condition | Exit Condition |
| --- | --- | --- | --- | --- |
| `LayerMapDraft` | Ownership map exists but extraction has not started | Design/migration workflow | US-4 detail design begins | First extraction slice is selected |
| `FacadeSplitInProgress` | L3 facade responsibilities are being separated from mixed file | Refactor workflow | Initial extraction patch starts | L3 entry wrappers are isolated |
| `CoreSplitInProgress` | L2 lifecycle/policy logic is being isolated | Refactor workflow | L3 split is stable | Core unit boundaries are explicit |
| `ProtoContractBound` | L1 Service/Link object ProtoMethods contract is introduced | Protocol boundary design | L2 protocol call sites are redirected through bound L1 objects | All targeted protocol-specific paths route through ProtoMethods |
| `RegressionVerified` | Current migration slice preserved US-1 behavior | Verification workflow | Slice implementation finishes | Next slice may begin |

## Runtime Ownership State Model

| State | Meaning | Owner | Entry Condition | Exit Condition |
| --- | --- | --- | --- | --- |
| `ProtoUnbound` | No L1 Service/Link object is associated with current core path | L2/L1 boundary | Before protocol selection | L1 object selected and bound |
| `ProtoBound` | L1 Service/Link object is available for L2 protocol behavior | L1 object exposing L0-mapped ProtoMethods | Protocol selection completed | Protocol action ends or context is destroyed |
| `ProtoMethodExecuting` | Protocol-specific method is running through ProtoMethods | L1 object dispatching to L0 mapping | L2 dispatches through the bound L1 object | Success/failure returned to L2 |
| `CoreRuntimeDirectL0` | L2 is using approved L0 runtime primitive directly | L2 with L0 primitive | Timeout/sync/queue runtime need occurs | Primitive returns and core resumes |

## State Transitions

| From | Event | Guard | Action | To | Failure Behavior |
| --- | --- | --- | --- | --- | --- |
| `LayerMapDraft` | start slice planning | Ownership map approved | choose first extraction target | `FacadeSplitInProgress` | stay in draft until slice is explicit |
| `FacadeSplitInProgress` | isolate public entry logic | L3 contract unchanged | move validation/dispatch shell to L3-owned boundary | `CoreSplitInProgress` | rollback slice if public behavior changes |
| `CoreSplitInProgress` | introduce protocol dispatch seam | core invariants remain explicit | bind Service/Link object ProtoMethods contract | `ProtoContractBound` | reject slice if protocol-specific code still bypasses contract |
| `ProtoUnbound` | protocol selection completes | target protocol known | create or bind L1 Service/Link object | `ProtoBound` | return deterministic error if protocol binding fails |
| `ProtoBound` | L2 requests protocol action | required ProtoMethods entry exists | invoke method through bound L1 object | `ProtoMethodExecuting` | report unsupported/incomplete protocol contract |
| `ProtoMethodExecuting` | method returns | success or failure captured | translate outcome into core result path | `ProtoBound` or terminal caller state | preserve cleanup invariants on failure |
| any migration slice state | run US-1 regression | slice code builds | execute existing baseline tests | `RegressionVerified` | block next slice on any regression |

## Invariants

- L3 never owns protocol-specific or POSIX-specific state directly.
- L2 IOC interface behavior is routed through ProtoMethods on bound L1 Service/Link objects whenever protocol behavior is involved, including future mappings such as `OpSendDAT(pLinkObj, pDataDesc, pOption)`.
- Direct L2->L0 usage is limited to approved runtime primitives, not protocol-specific bypass.
- No migration slice may continue after regression failure against US-1 baseline.
- Mixed ownership in `Source/IOC_SrvAPI.c` must monotonically decrease across slices.

## Concurrency and Ownership

| Shared State | Writer | Reader | Synchronization / Discipline | Race / Fault Risk |
| --- | --- | --- | --- | --- |
| Migration ownership map | Design/refactor workflow | Review and implementation workflow | Commit-reviewed documentation and scoped slice edits | stale ownership assumption |
| Service Object / Link Object binding | L1 object binder / L2 dispatcher | L2 core paths | explicit creation/bind/unbind contract | null binding or stale method table |
| POSIX/runtime primitive handles | L0-backed code paths | L1 and approved L2 runtime paths | existing runtime discipline plus future resource design | leaked handle or inconsistent timeout path |
| Regression approval state | verification workflow | next slice planner | must pass before next slice | slice stacking after failed baseline |

## Embedded and Digital Media State Points

Embedded software points:

- Hardware lifecycle: still abstracted at L0 and not surfaced to L3.
- ISR/task handoff: deferred until resource design or concurrency story expands runtime model.
- Timeout/sync primitives: allowed as L2 direct runtime usage only when not bypassing Service/Link object ProtoMethods for protocol behavior.
- Fault state: migration slices must keep deterministic failure translation into core/L3 result paths.

digital video/audio points:

- Not applicable for current US-4 scope.

## Review Checklist

- Migration states make slice order and re-verification gate explicit.
- Service/Link object binding and ProtoMethods execution states are explicit enough for later detail review and test design.
- Invariants clearly separate allowed L2->L0 runtime usage from forbidden protocol bypass.
