# Establish IOC Link Between Service and Client

> **Story ID:** US-1 | **State:** todo | **Priority:** P1
> **Source:** `.catdd/spec/analyzedNews/20260617-EstablishedLink-Feature.md`
> **Created:** 2026-06-19

---

## Story Statement

<!-- Technique: write-user-story -->

**As a** developer integrating IOC Service and Client applications,
**I want** IOC to establish a bidirectional link pair (`AcptLinkID <-> ConnLinkID`) from service-online to client-connect, where accept mode is determined by `SrvArgs` during `IOC_onlineService`,
**So that** both endpoints can reliably start DAT/EVT/CMD messaging on valid link IDs.

---

## Priority

<!-- Technique: prioritize-requirements -->

| Dimension | Score (1-9) | Rationale |
|---|---|---|
| Business Value | 9 | Link establishment is the prerequisite capability for all IOC communication roles. |
| User Value | 8 | Integrators cannot exercise DAT/EVT/CMD APIs until link setup succeeds. |
| Cost / Effort | 4 | Core control-path behavior; moderate implementation and test effort. |
| Risk / Complexity | 4 | Remaining uncertainty is mainly timeout/retry and reconnection policy scope. |

**Priority Score:** (9 + 8) / (4 + 5) = **1.89** | **Priority:** **P1**

---

## Visual Model

<!-- Technique: elicit-requirements-models -->

```mermaid
flowchart TD
	A[Service calls IOC_onlineService] --> B{Client calls IOC_connectService with matching usage}
	B -->|No| E[Reject connection]
	B -->|Yes| C{Accept mode}
	C -->|Auto-accept| D1[IOC creates AcptLinkID and returns ConnLinkID]
	C -->|Manual-accept| D2[Service calls IOC_acceptClient to create AcptLinkID]
	D1 --> F[Link pair established: AcptLinkID <-> ConnLinkID]
	D2 --> F
	F --> G[DAT/EVT/CMD messaging can start]
	G --> H[Service calls IOC_offlineService]
	H --> I[Service transitions offline]
```

### Model Gap Analysis

| # | Gap Found | Question |
|---|---|---|
| 1 | Reject path behavior details are partially specified. | For usage mismatch, should tests assert specifically `IOC_RESULT_INCOMPATIBLE_USAGE`? |
| 2 | KEEP and CLOSE flag semantics need edge-condition detail. | For KEEP mode, are links preserved until peer disconnect only, or can service later force-close them without re-online? |
| 3 | Retry branch is still product-level policy. | Timeout is supported via `IOC_OPTID_TIMEOUT`; should retry remain out of scope for this story? |

---

## Acceptance Criteria

<!-- Techniques: write-user-story + facilitate-example-mapping -->

### Scenario 1: Auto-Accept Establishment

**Rule:** Accept mode is determined by `SrvArgs` when `IOC_onlineService` is called. If mode is auto-accept and usage is compatible, IOC must establish a link pair without explicit service accept call.
**Given** Service App calls `IOC_onlineService()` successfully with valid `SrvID` and compatible usage capability
**When** Client App calls `IOC_connectService()` with matching usage
**Then** IOC establishes valid link pair (`AcptLinkID <-> ConnLinkID`) and returns success (`ConnLinkID`) to client

| Concrete Examples | Counter-Examples |
|---|---|
| Service online as DAT receiver, client connects as DAT sender, callback configured, both IDs returned | Service online without compatible usage, client requests unsupported usage, connection rejected |

**Open Questions:** None.

### Scenario 2: Manual-Accept Establishment

**Rule:** In manual-accept mode, IOC must not finalize link pair until service explicitly accepts pending client.
**Given** Service App is online in manual-accept mode and client has requested connection
**When** Service App calls `IOC_acceptClient(SrvID, &AcptLinkID, pOption)` for that pending client
**Then** IOC establishes valid link pair (`AcptLinkID <-> ConnLinkID`) and both sides can invoke messaging APIs on their link IDs

| Concrete Examples | Counter-Examples |
|---|---|
| Pending client exists, service accepts once, link pair becomes active | Service never calls accept, client remains pending and cannot send DAT/EVT/CMD |

**Open Questions:** Is there a required max wait duration for pending clients before automatic rejection?

### Scenario 3: Service Offline Lifecycle Transition

**Rule:** `IOC_offlineService` flag determines whether established links are kept or closed, and service offline operation must stop new connection establishment for that service.
**Given** At least one service instance exists and service may have established link(s)
**When** Service App calls `IOC_offlineService(SrvID)`
**Then** no new `IOC_connectService()` should succeed for that service until it is online again

| Concrete Examples | Counter-Examples |
|---|---|
| Service offlines with KEEP flag, existing links stay valid and new connects fail | Service offlines but new connect still succeeds |
| Service offlines with CLOSE flag, existing links are closed and new connects fail | Service offlines but links stay open despite CLOSE flag |

**Open Questions:** None.

---

## Business Rules

<!-- Technique: extract-business-rules -->

| ID | Rule | Type | Implied Functional Requirement |
|---|---|---|---|
| BR-1 | Service must be online before client can establish link. | Constraint | IOC shall reject connect attempts to offline service. |
| BR-2 | Client usage must match service capability. | Constraint | IOC shall validate usage compatibility during connect. |
| BR-3 | Auto-accept mode creates accepted link automatically. | Action Enabler | IOC shall allocate `AcptLinkID` without explicit service accept call in auto mode. |
| BR-4 | Manual-accept mode requires service acceptance for finalization. | Constraint | IOC shall keep client pending until `IOC_acceptClient` succeeds. |
| BR-5 | Link IDs are paired for bidirectional communication. | Fact | IOC shall expose both endpoint IDs as correlated pair for message operations. |
| BR-6 | Service offline ends availability for new connections. | Constraint | IOC shall block new connection establishment once service is offline. |
| BR-7 | Service offline keep-or-close behavior is controlled by an offline flag. | Action Enabler | IOC shall preserve or close established links according to `IOC_offlineService` flag value. |
| BR-8 | Connect timeout is configurable through options. | Action Enabler | IOC shall support timeout-controlled connect attempts via `IOC_OPTID_TIMEOUT` and report timeout result. |
| BR-9 | Usage compatibility follows complementary mapping rules. | Constraint | IOC shall reject incompatible usage combinations according to documented service/client mapping. |

---

## Edge Cases & Error Paths

<!-- Techniques: write-user-story + elicit-requirements-models -->

| # | Condition | Expected Behavior | Status |
|---|---|---|---|
| 1 | Client requests usage not supported by service | Connection rejected with `IOC_RESULT_INCOMPATIBLE_USAGE` | draft |
| 2 | Client connects before service online | Connection rejected, no link ID allocated | draft |
| 3 | Service offlines while client pending manual accept | Pending request resolved by explicit failure/cleanup path | needs clarification |
| 4 | Auto-accept callback not configured | Link establishment still succeeds without callback delivery | draft |
| 5 | Duplicate connect requests from same client context | Requests are isolated or deduplicated by defined rule | needs clarification |
| 6 | Connect timeout option set and service response exceeds timeout | `IOC_connectService` returns `IOC_RESULT_TIMEOUT` | draft |

---

## Scope

**In scope:**

- `IOC_onlineService` + `IOC_connectService` path to establish paired link IDs.
- Auto-accept and manual-accept mode behavior for initial link establishment.
- Service offline effect on future connection establishment.

**Non-goals:**

- DAT/EVT/CMD payload semantics after link establishment.
- Retry/backoff design and timeout policy finalization.
- Reconnection behavior and strategy.
- Performance and multi-client fairness optimization.

---

## Risks & Assumptions

| # | Risk / Assumption | Severity | Mitigation / Clarification Needed |
|---|---|---|---|
| 1 | Accept mode selection must be implemented exactly from `SrvArgs` at service-online time. | Medium | Add tests for both mode configurations from `SrvArgs`. |
| 2 | Offline flag semantics must be implemented consistently across all established links. | Medium | Add tests for both KEEP and CLOSE paths in offline transition. |
| 3 | Retry policy after timeout/failure is not fixed. | Medium | Keep retry out of scope for this story unless explicitly requested. |
| 4 | Assume IOC header contracts stay stable in `Include/IOC/*.h`. | Medium | Revalidate story against header changes before implementation. |

---

## Initial Acceptance Questions

<!-- Gate: story is NOT ready for SPEC_openUserStory if any question is open -->

| # | Question | Raised By | Status |
|---|---|---|---|
| 1 | Which mode is default for first implementation target: auto-accept or manual-accept? Answer: mode is determined by `SrvArgs` during `IOC_onlineService`. | developer decision | answered |
| 2 | What is required behavior for already-established links after `IOC_offlineService()`? Answer: behavior is determined by offline flag (keep or close). | developer decision | answered |
| 3 | Must connection timeout and retry semantics be defined in this story? Answer: timeout semantics are in scope via `IOC_OPTID_TIMEOUT` and `IOC_RESULT_TIMEOUT`; retry policy remains deferred. | header evidence + scope decision | answered |
| 4 | Is reconnection behavior part of this story or a follow-up story? Answer: reconnection is out of scope for this story and deferred to a follow-up story. | developer decision | answered |

**Gate:** This story is **READY** for `SPEC_openUserStory` because all initial acceptance questions are answered.

---

## Ambiguity Warnings

<!-- Technique: validate-requirements-criteria -->

| # | Ambiguous Term | Found In Section | Clarifying Question |
|---|---|---|---|
| 1 | "valid SrvID" | Acceptance Criteria | What exact validity checks define valid `SrvID`? |
| 2 | "matching usage" | Acceptance Criteria | Use documented complementary capability mapping from `IOC_ConnArgs` contract; no strict-equality interpretation. |
| 3 | "success" | Acceptance Criteria | For connect path, success is `IOC_RESULT_SUCCESS`; for timeout path, expected is `IOC_RESULT_TIMEOUT`. |
| 4 | "pending" | Scenario 2 | Is there a timeout window and lifecycle state model for pending clients? |

---

## Traceability

| From → To | Link |
|---|---|
| This story → Raw input | `.catdd/spec/analyzedNews/20260617-EstablishedLink-Feature.md` |
| Project story index | `README_UserStories.md` |
| This story ID | `US-1` |
