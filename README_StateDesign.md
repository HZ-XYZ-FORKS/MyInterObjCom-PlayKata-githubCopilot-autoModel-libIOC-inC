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
