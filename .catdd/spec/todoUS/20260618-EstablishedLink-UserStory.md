# User Story: Establish IOC Link Between Service and Client

## Source Trace
- Raw input artifact: `.catdd/spec/analyzedNews/20260617-EstablishedLink-Feature.md`
- Imported from: `Doc/IOC_UsageScenarios.md` section `Established Link`

## ReACT Evidence
- Reason:
	- Read pending feature input and extracted actor, value, outcome, and prerequisite position for DAT/EVT/CMD flows.
	- Cross-checked constraints and scope boundaries against project context.
- Act:
	- Converted raw feature input into this independently testable user story slice.
	- Preserved lifecycle traceability from this story back to the archived raw feature artifact.
	- Captured acceptance scenarios, scope, non-goals, edge cases, risks, assumptions, and open questions.
- Verify:
	- Kept unresolved product decisions as open questions instead of inventing requirements.
	- Ensured the story remains value-focused and implementation-agnostic at analysis stage.
	- Ensured this slice is suitable for next lifecycle step (`SPEC_openUserStory`).

## Story Type
- Classification: Core feature slice
- Priority: High (prerequisite capability for DAT/EVT/CMD flows)
- Independent test intent: This slice is complete when link establishment works in both auto-accept and manual-accept paths and returns usable link IDs.

## User Story
As an IOC integrator developing Service and Client applications,
I want IOC to establish a bidirectional link pair between a valid online service and a connecting client,
So that both sides can exchange DAT/EVT/CMD messages through their own link IDs.

## User Value
- Enables the foundational communication channel required by all higher-level IOC messaging semantics.
- Makes service-client session setup deterministic and testable.
- Supports two operational modes (auto-accept and manual-accept) for deployment flexibility.

## In Scope
- Service online flow via `IOC_onlineService()` with valid `SrvID` and usage capability.
- Client connect flow via `IOC_connectService()` with matching usage.
- Link pair establishment: `AcptLinkID <-> ConnLinkID`.
- Auto-accept path including optional `OnAutoAccepted_F` callback behavior.
- Manual-accept path via `IOC_acceptClient()`.
- Basic lifecycle close path via `IOC_offlineService(SrvID)`.

## Out of Scope / Non-Goals
- DAT/EVT/CMD payload semantics after link establishment (covered by separate stories).
- Retry/backoff policy design for connection failure handling.
- Performance tuning and timeout budget finalization.
- Multi-client fairness/scheduling policy.

## Acceptance Scenarios (Initial)
### AS-1 Auto-Accept Establishment
- Given Service App successfully calls `IOC_onlineService()` with valid `SrvID` and complementary usage capability.
- When Client App calls `IOC_connectService()` with matching usage.
- Then IOC establishes a valid link pair (`AcptLinkID <-> ConnLinkID`).
- And Client receives success with `ConnLinkID`.
- And Service receives `OnAutoAccepted_F(SrvID, AcptLinkID, pPriv)` when callback is configured.

### AS-2 Manual-Accept Establishment
- Given Service App is online in manual-accept mode.
- And Client App calls `IOC_connectService()` with matching usage.
- When Service App calls `IOC_acceptClient(SrvID, &AcptLinkID, pOption)` for the pending client.
- Then IOC establishes a valid link pair (`AcptLinkID <-> ConnLinkID`).
- And both sides can use their respective link IDs for messaging APIs.

### AS-3 Lifecycle Close
- Given at least one established service-client link pair.
- When Service App calls `IOC_offlineService(SrvID)`.
- Then service-side link lifecycle transitions to offline/closed state.
- And no new link establishment should succeed for that service until it is online again.

## Edge Cases (Initial)
- Client usage does not match service capability.
- Client connects before service is online.
- Service goes offline while a client is pending manual acceptance.
- Callback not configured in auto-accept mode.
- Duplicate connect requests from same client context.

## Risks
- Ambiguity in default mode selection (auto-accept vs manual-accept) may cause integration mismatch.
- Unclear offline behavior for existing links could create resource leaks or stale handles.
- Undefined timeout behavior could produce flaky integration tests.

## Assumptions
- `IOC_onlineService()`, `IOC_connectService()`, `IOC_acceptClient()`, and `IOC_offlineService()` API contracts remain stable from current `Include/IOC` headers.
- Link pair identity mapping remains one accepted link ID to one connected link ID in default P2P behavior.
- This story targets C implementation in `libIOC.a` per project context.

## Open Questions (Need Developer Confirmation)
- Which mode is the product default: auto-accept or manual-accept?
- What is the required behavior for already-established links after `IOC_offlineService()`?
- Are connection-establishment timeouts and retry semantics required in this story or deferred?
- Must reconnection behavior be defined now or handled in a follow-up story?

## Initial Acceptance Questions Checklist
- [ ] Is actor/value/outcome aligned with product intent?
- [ ] Is this slice independent enough to test and deliver before DAT/EVT/CMD stories?
- [ ] Are AS-1/AS-2/AS-3 sufficient for first-pass implementation readiness?
- [ ] Should offline behavior details be promoted from open question to explicit acceptance criteria now?
