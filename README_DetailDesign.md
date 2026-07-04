# MyInterObjComBase Detail Design

Detailed design for US-1 (Established Link) before test/code implementation.

## Story Context

- Story: US-1 Establish IOC Link Between Service and Client
- Source artifact: `.catdd/spec/doingUS/20260618-EstablishedLink-UserStory.md`
- Related overview: [README.md](README.md)
- Related user stories: [README_UserStories.md](README_UserStories.md)

## Requirements

| Requirement | Source | Notes |
| --- | --- | --- |
| Service must come online and expose connectable capability via SrvArgs | US-1 / AC-1 | `IOC_onlineService` assigns SrvID and service usage capability |
| Client connect must establish paired LinkIDs for compatible usage | US-1 / AC-1 | `IOC_connectService` compatible mapping enforced |
| Manual accept path must finalize pending client only after service acceptance | US-1 / AC-2 | `IOC_acceptClient` required in manual mode |
| Service offline must block new connects and apply keep/close policy | US-1 / AC-3 | `IOC_offlineService` + `IOC_SRVFLAG_KEEP_ACCEPTED_LINK` |
| Timeout option on connect must be honored | US-1 / Edge E2 | `IOC_OPTID_TIMEOUT` with `IOC_RESULT_TIMEOUT` |

## Acceptance Criteria

| AC ID | Given | When | Then | Design Impact |
| --- | --- | --- | --- | --- |
| AC-1 | Service online with compatible usage | Client calls `IOC_connectService` | Link pair established and connect succeeds | Define connect validation and link-pair allocation path |
| AC-2 | Service in manual-accept mode with pending client | Service calls `IOC_acceptClient` | Link pair finalized and usable | Define pending-client queue and accept finalization path |
| AC-3 | Service has possible established links | Service calls `IOC_offlineService` | New connects fail; existing links keep/close by flag | Define offline transition behavior and admission gate |

## Interface Design

| Interface | Input | Output | Error Behavior |
| --- | --- | --- | --- |
| `IOC_onlineService` | `IOC_SrvArgs_T` (URI, usage, flags) | `SrvID` | invalid args/conflict/too many services |
| `IOC_connectService` | `IOC_ConnArgs_T`, optional `IOC_Options_T` | `ConnLinkID` | incompatible usage, timeout, connection failed |
| `IOC_acceptClient` | `SrvID`, optional options | `AcptLinkID` | no pending client, invalid service, unsupported mode |
| `IOC_offlineService` | `SrvID` | success/failure | invalid service, busy/offline transition conflict |
| `IOC_closeLink` | `LinkID` | success/failure | not-exist/broken link handling |

## Behavior Design

```text
1. online path
   1.1 validate SrvArgs (URI + capabilities + flags)
   1.2 allocate/register service record and set SrvState=Online
   1.3 initialize admission policy (auto-accept or manual)

2. connect path
   2.1 validate ConnArgs and compatibility with target service
   2.2 if service is Offline/Offlining, reject
   2.3 if AUTO_ACCEPT, allocate AcptLinkID + ConnLinkID and bind pair
   2.4 if manual mode, enqueue pending request and await IOC_acceptClient
   2.5 apply timeout option when waiting (manual path)

3. manual accept path
   3.1 service fetches oldest/target pending client request
   3.2 allocate AcptLinkID and finalize pair
   3.3 return AcptLinkID to service and ConnLinkID success to client

4. offline path
   4.1 mark service Offlining and close admission gate
   4.2 reject all new IOC_connectService for this SrvID
   4.3 apply keep/close policy to existing accepted links
   4.4 transition service to Offline
```

## State and Data

| State/Data | Owner | Lifecycle | Invariant |
| --- | --- | --- | --- |
| `IOC_SrvState_T` | Service registry | Offline->Onlining->Online->Offlining->Offline | No new admissions when Offlining/Offline |
| Service admission mode (AUTO_ACCEPT/manual) | Service record | set at online, immutable during online session | Acceptance behavior must match `SrvArgs.Flags` |
| Pending connect queue (manual mode) | Service record | enqueue on connect, dequeue on accept/timeout/offline | A pending request is resolved exactly once |
| Link pair mapping (`AcptLinkID` <-> `ConnLinkID`) | Link manager | create on accept, remove on close/offline cleanup | Pair relation is one-to-one while active |
| Offline keep/close policy | Service record | read at offline transition | Existing link handling must be deterministic |

## Embedded and Digital Media Detail Points

Embedded software points:

- Interrupt behavior: Not ISR-driven in this story scope; use thread-safe state transitions and bounded waits.
- Driver interaction: Protocol adapters are abstracted by IOC URI/protocol layer.
- Resource lifetime: Link/service records are created on online/connect and released on close/offline.
- Fault handling: Timeout and broken-link paths must return deterministic `IOC_Result_T`.

digital video/audio points:

- Not applicable for US-1 detail scope (no media pipeline behavior defined here).

## Error and Edge Handling

- Incompatible usage connect -> return `IOC_RESULT_INCOMPATIBLE_USAGE`, no link allocated.
- Connect while service offline -> deterministic failure, no link allocated.
- Connect timeout in manual path -> return `IOC_RESULT_TIMEOUT` and clear pending request.
- Offline during pending manual request -> resolve pending request by cleanup/failure path.
- Runtime/dependency fault during connect -> graceful failure with no half-initialized link pair.

## Open Questions

- For KEEP policy, should service have a follow-up API to force-close preserved links while offline?
- In manual mode, should pending requests use FIFO strictly, or allow policy-based selection?

## Review Checklist

- Every acceptance criterion has an explicit design impact and error-handling path.
- Interface and state ownership are clear enough to drive CaTDD unit-test design.
- Open questions are explicit and non-blocking for detail-design review gate.

---

# US-4 Detail Design

Detailed design for US-4 (Re-architect IOC into Explicit L0-L3 Layers) after architecture review pass and before implementation-oriented work.

## Story Context

- Story: US-4 Re-architect IOC into Explicit L0-L3 Layers
- Source artifact: `.catdd/spec/doingUS/20260704-reArchDesign-LayeredArchitecture-UserStory.md`
- Related architecture: [README_ArchDesign.md](README_ArchDesign.md)
- Related user stories: [README_UserStories.md](README_UserStories.md)

## Requirements

| Requirement | Source | Notes |
| --- | --- | --- |
| Public IOC interface remains stable while internal layering is made explicit | US-4 / Scenario 2 / BR-3 | `Include/IOC/*.h` signatures are preserved during migration unless future story changes contract |
| L3 depends only on L2; L2 may use L1 and L0; L1 may use L0 | US-4 / Scenario 1 / BR-2 | POSIX is treated as L0 and direct L2->L0 is allowed for approved core behavior |
| L2 IOC interface behavior must call ProtoMethods through bound L1 Service/Link objects | US-4 / Scenario 1 / BR-2 | Interface behaviors such as online, accept, close, and future DAT transfer map through ProtoMethods and cannot bypass object-based dispatch |
| Migration slices must preserve existing US-1 behavior | US-4 / Scenario 3 / BR-4 | Each slice must be regression-checked before next slice |

## Acceptance Criteria

| AC ID | Given | When | Then | Design Impact |
| --- | --- | --- | --- | --- |
| AC-4.1 | IOC source ownership map is being defined | Files/modules are assigned to L0-L3 | Allowed/forbidden dependencies are explicit and reviewable | Define concrete file split targets and ownership matrix |
| AC-4.2 | L2 needs protocol behavior | Core calls protocol operation | Call path goes through ProtoObject ProtoMethods backed by L0 behavior | Define ProtoObject, ProtoMethods table, and call sequence |
| AC-4.3 | Incremental refactor slice is applied | Regression suite runs | US-1 baseline remains passing | Define slice order, completion criteria, and rollback-safe boundaries |

## Structure Design

| Design Element | Owner Layer | Responsibility | Implementation Direction |
| --- | --- | --- | --- |
| `IOC_SrvAPI` facade | L3 | Preserve public API entry points and argument/result contract | Keep current API names; reduce logic to validation/dispatch/orchestration |
| Service lifecycle core | L2 | Online/offline state, admission policy, link-creation orchestration | Extract from `Source/IOC_SrvAPI.c` into core-owned internal unit |
| Link pairing core | L2 | Pair allocation, invariants, failure cleanup, policy enforcement | Internal core unit with explicit one-to-one link mapping responsibilities |
| `Service Object` / `Link Object` | L1 protocol-facing objects | Carry the ProtoMethods used by L2 for protocol behavior | Internal protocol-facing objects with method-table ownership |
| ProtoMethods table | L1 object-exposed mapping to L0 | Function pointers or equivalent dispatch mapping for protocol operations onto L0 implementations | Keep method set minimal and behavior-specific |
| POSIX/platform primitives | L0 | Time, queue, sync, allocation, socket/descriptor or OS-backed operations | May be called by L1 and approved L2 runtime logic |

## Interface Design

| Interface | Layer | Input | Output | Error/Constraint |
| --- | --- | --- | --- | --- |
| `IOC_*` public APIs | L3 | Existing IOC public structs and options | Existing `IOC_Result_T` and IDs | Must remain source-compatible |
| `Core_*` internal lifecycle entry points | L2 | Sanitized service/link arguments and context handles | Core status + state updates | Must not expose POSIX types to L3 |
| `ProtoMethods` on L1 Service/Link objects | L1-exposed mapping invoked by L2 | First migration slice minimum set: `OpOnlineService`, `OpAcceptClient`, `OpCloseLink`; future IOC behavior examples include `OpSendDAT` | Protocol operation result + adapter state effects | IOC interface behavior must go through this mapping; DAT/EVT/CMD transfer methods are deferred beyond the first slice |
| `Plat_*` or POSIX-backed primitive calls | L0 | Timeouts, queue ops, sync, descriptors, low-level runtime ops | Primitive result/failure | L2 direct use allowed only for core runtime concerns, not protocol-specific bypass |

## ProtoMethods Minimum Set For First Migration Slice

| Method | Used By | First-Slice Purpose | Deferred? |
| --- | --- | --- | --- |
| `OpOnlineService` | L2 service-online flow | Map service-online behavior from L2 onto the L0-backed protocol/platform implementation using `ProtoMethods.OpOnlineService(pSrvObj, pSrvArgs, pOptions)` | No |
| `OpAcceptClient` | L2 manual-accept flow | Finalize protocol-specific accept behavior for pending client path using the bound L1 Service Object | No |
| `OpCloseLink` | L2 offline/cleanup flow | Close protocol-specific link state during cleanup and offline-driven shutdown using the bound L1 Link Object | No |
| `OpSendDAT` | future DAT flow | Map `IOC_sendDAT`-style behavior from L2 onto the bound L1 Link Object using `ProtoMethods.OpSendDAT(pLinkObj, pDataDesc, pOption)` | Yes |
| `OpRecvDAT` | future DAT flow | Map receive-side DAT behavior outside current migration slice | Yes |
| `OpPostEVT` / `OpPullEVT` | future EVT flow | Map EVT delivery behaviors outside current migration slice | Yes |
| `OpExecCMD` / `OpWaitCMD` | future CMD flow | Map synchronous command behaviors outside current migration slice | Yes |

## Behavior Design

```text
1. L3 facade path
   1.1 receive existing IOC API call without changing public signature
   1.2 validate public arguments and normalize into core-facing inputs
   1.3 dispatch to L2 core entry point

2. L2 core path
   2.1 enforce service/link invariants and policy checks
   2.2 if protocol behavior is needed, obtain the bound L1 Service Object or Link Object for the target protocol path
   2.3 call first-slice ProtoMethods on that L1 object for IOC lifecycle actions: `OpOnlineService`, `OpAcceptClient`, or `OpCloseLink`
   2.4 future IOC interface behaviors such as DAT transfer should map similarly, for example: `ProtoMethods.OpSendDAT(pLinkObj, pDataDesc, pOption)`
   2.5 call L0/POSIX directly only for approved core runtime concerns (timeout/sync/queue/etc.)
   2.6 translate failures into deterministic IOC result and preserve cleanup invariants

3. L1 protocol path
   3.1 implement Service Object / Link Object ownership and ProtoMethods binding
   3.2 map ProtoMethods invocation onto concrete L0-backed behavior
   3.3 keep protocol-specific branching out of L3 and out of ad-hoc L2 direct paths

4. migration path
   4.1 extract ownership map from current `Source/IOC_SrvAPI.c`
   4.2 isolate L3 facade logic first
   4.3 extract L2 lifecycle/policy units next
   4.4 introduce Service Object / Link Object + ProtoMethods seam before adding real alternate protocols
   4.5 rerun US-1 regression after each slice
```

## Data and Ownership

| Data / Structure | Owner | Lifecycle | Invariant |
| --- | --- | --- | --- |
| Public IOC argument/result structs | L3 | Stable across current story | No L0-specific leakage into public headers |
| Service and link records | L2 | Created/updated across lifecycle APIs | Core owns invariants and cleanup |
| Service Object / Link Object binding | L1 | Bound per protocol selection / service-link context | L2 protocol actions use ProtoMethods through these objects |
| ProtoMethods table | L1 object-exposed mapping to L0 | Created with Service Object / Link Object binding | All protocol-specific operations are routed through it |
| POSIX/platform handles | L0 | Created and destroyed by L0-backed behavior | L3 never owns or exposes them directly |

## Constraints and Error Handling

- Do not change `Include/IOC/*.h` public API semantics in US-4.
- Do not allow L3 to call protocol or POSIX details directly.
- Do not allow L2 IOC interface behavior to bypass Service Object / Link Object ProtoMethods.
- Preserve deterministic cleanup when migration slices partially fail.
- Treat mixed ownership in `Source/IOC_SrvAPI.c` as transitional only; new detail work must reduce, not deepen, that mixing.

## Lightweight Implementation Plan

1. Create an internal ownership map for current `Source/IOC_SrvAPI.c` symbols and blocks.
2. Extract L3 facade-only responsibilities from L2 lifecycle responsibilities.
3. Define internal Service Object / Link Object ProtoMethods contract for the first migration slice with exactly `OpOnlineService`, `OpAcceptClient`, and `OpCloseLink`.
4. Move approved timeout/sync/queue runtime logic into explicit L0-backed primitives.
5. Keep each extraction slice regression-safe against US-1 test baseline.

## Open Questions

- Should the first physical split create new internal headers for L2/L1/L0 boundaries immediately, or first annotate ownership inside the existing file before extraction?

## Review Checklist

- Each US-4 acceptance criterion maps to a concrete design impact.
- Service Object / Link Object ProtoMethods contract is explicit enough to drive later unit-test design.
- Allowed direct L2->L0 runtime usage is separated from forbidden protocol-specific bypass.
- Migration slices are localized enough to preserve US-1 regression safety.
