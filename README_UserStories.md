# MyInterObjComBase User Stories

Project-level ledger for user-story lifecycle state (TODO/DOING/DONE) and acceptance-criteria trace/status.

## Story Ledger

### TODO / DOING Stories

| Story ID | Title | State | Source | Trace Link |
| --- | --- | --- | --- | --- |
| US-2 | Establish IOC Link Concurrency Safety | todo | `.catdd/spec/todoUS/20260618-EstablishedLink-UserStory.md` | `.catdd/spec/todoUS/20260621-EstablishedLink-ConcurrencyDesignStory.md` |
| US-3 | Refine Interface Behavior Edge Coverage for UT_US1_Service_Edge | todo | `.catdd/spec/analyzedNews/20260703-UT_US1_Service_Edge-interfaceBehaviorEdge-Issue.md` | `.catdd/spec/todoUS/20260704-UT_US1_Service_Edge-InterfaceBehaviorEdge-UserStory.md` |
| US-4 | Re-architect IOC into Explicit L0-L3 Layers | doing | `.catdd/spec/analyzedNews/20260624-reArchDesign-Issue.md` | `.catdd/spec/doingUS/20260704-reArchDesign-LayeredArchitecture-UserStory.md` |

### DONE Stories

| Story ID | Title | Closed On | Evidence | Trace Link |
| --- | --- | --- | --- | --- |
| US-1 | Establish IOC Link Between Service and Client | 2026-07-03 | commit `512e010`; CTest UT_US1_Service_* PASS | `.catdd/spec/doneUS/20260618-EstablishedLink-UserStory.md` |

## Active Story

```text
US-2: As a concurrent IOC API consumer,
      I want duplicate or interleaved IOC_connectService attempts against the same context
      to behave deterministically under contention,
      So that link establishment avoids duplicate links, lost requests, and deadlocks.

US-3: As a developer maintaining IOC link-establishment tests,
      I want UT_US1_Service_Edge to prioritize interface behavior edge scenarios while excluding argument-edge permutations,
      So that edge verification remains focused and behavior-traceable.

US-4: As a maintainer of IOC architecture,
      I want IOC to be restructured into explicit L0-L3 layers with clear dependency direction,
      So that platform/protocol growth remains isolated from core and interface contracts.
```

## Acceptance Criteria Ledger

| AC ID | Story | Given | When | Then | Status | Verification Trace |
| --- | --- | --- | --- | --- | --- | --- |
| AC-1 | US-1 | Service online with compatible usage | Client calls `IOC_connectService` in auto-accept path | IOC establishes valid link pair and returns success | reviewed | US-1 AS-1, TC seeds T1/E1/M1/M2/F2 |
| AC-2 | US-1 | Service online in manual-accept mode with pending client | Service calls `IOC_acceptClient` | IOC finalizes link pair and messaging APIs become usable | reviewed | US-1 AS-2, TC seeds T2/E2/F1 |
| AC-3 | US-1 | Service may have established links | Service calls `IOC_offlineService` | No new connect succeeds; keep/close behavior follows service flags | reviewed | US-1 AS-3, TC seeds E3/E4/F1 |
| AC-4.1 | US-4 | IOC architecture documentation and source ownership map | Each module/file is classified into one of L0-L3 | Allowed and forbidden dependency directions are explicit and reviewable | drafted-reviewed | US-4 Scenario 1, README_ArchDesign layer ownership map + dependency graph, TC seed C1/C2 |
| AC-4.2 | US-4 | L2 needs protocol behavior | Core calls protocol operation through bound L1 Service/Link object | ProtoMethods dispatch maps to L0 implementations without bypass | drafted-reviewed | US-4 Scenario 1, README_DetailDesign ProtoMethods minimum set, TC seed C2 |
| AC-4.3 | US-4 | Incremental refactor slice is applied | US-1 regression suite runs after the slice | Existing US-1 behavior remains passing and traceable | drafted-reviewed | US-4 Scenario 3, README_DetailDesign lightweight implementation plan + README_StateDesign migration state model, TC seed C3 |

## Lifecycle Links

- Pending input: `.catdd/spec/pendingNews/`
- Active story: `.catdd/spec/doingUS/20260704-reArchDesign-LayeredArchitecture-UserStory.md`
- Todo story: `.catdd/spec/todoUS/20260621-EstablishedLink-ConcurrencyDesignStory.md`
- Done story: `.catdd/spec/doneUS/`

## Sync Rules

- Keep opened stories in TODO/DOING until closure is complete.
- Move stories to DONE only when closure evidence exists (review + commit + CI or accepted equivalent).
- Keep AC status aligned with story lifecycle and verification evidence.
- Do not leave story-state mismatches between this ledger and `.catdd/spec/*US/` artifacts.

## Traceability Notes

- Related detail design: `README_DetailDesign.md` (to be created by design commands)
- Related verification design: `README_VerifyDesign.md` (to be created as needed)
- Related user guide: `README_UserGuide.md`
