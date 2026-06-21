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
