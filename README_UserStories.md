# MyInterObjComBase User Stories

Project-level ledger for user-story lifecycle state (TODO/DOING/DONE) and acceptance-criteria trace/status.

## Story Ledger

### TODO / DOING Stories

| Story ID | Title | State | Source | Trace Link |
| --- | --- | --- | --- | --- |
| US-2 | Establish IOC Link Concurrency Safety | todo | `.catdd/spec/todoUS/20260618-EstablishedLink-UserStory.md` | `.catdd/spec/todoUS/20260621-EstablishedLink-ConcurrencyDesignStory.md` |
| US-3 | Refine Interface Behavior Edge Coverage for UT_US1_Service_Edge | todo | `.catdd/spec/analyzedNews/20260703-UT_US1_Service_Edge-interfaceBehaviorEdge-Issue.md` | `.catdd/spec/todoUS/20260704-UT_US1_Service_Edge-InterfaceBehaviorEdge-UserStory.md` |
| US-7 | Rewrite README_UserGuide for Developers Building IOC-Based Apps | doing | `.catdd/spec/analyzedNews/20260707-reWrite-README_UserGuide-fromUser_whoWillUseIOC_toDevNewApps-Issue.md` | `.catdd/spec/doingUS/20260707-RewriteREADMEUserGuideForAppDevelopers-UserStory.md` |

### ABORTED Stories

| Story ID | Title | State | Source | Trace Link |
| --- | --- | --- | --- | --- |
| US-6 | Implement Layered Architecture Slice After US-4 Design Closure | aborted | `.catdd/spec/analyzedNews/20260706-reArchImpl-LayeredArch-Issue.md` | `.catdd/spec/abortUS/20260706-reArchImpl-LayeredArch-UserStory.md` |

### DONE Stories

| Story ID | Title | Closed On | Evidence | Trace Link |
| --- | --- | --- | --- | --- |
| US-1 | Establish IOC Link Between Service and Client | 2026-07-03 | commit `512e010`; CTest UT_US1_Service_* PASS | `.catdd/spec/doneUS/20260618-EstablishedLink-UserStory.md` |
| US-4 | Re-architect IOC into Explicit L0-L3 Layers | 2026-07-05 | commit `8d34906`; accepted artifact-consistency verification summary for close gate | `.catdd/spec/doneUS/20260704-reArchDesign-LayeredArchitecture-UserStory.md` |

## Active Story

```text
US-7: As a developer building a new application with IOC,
      I want README_UserGuide.md rewritten around build/run onboarding,
      IOC API integration, and usage scenarios,
      So that I can use IOC correctly without reading implementation source.
```

## Acceptance Criteria Ledger

| AC ID | Story | Given | When | Then | Status | Verification Trace |
| --- | --- | --- | --- | --- | --- | --- |
| AC-1 | US-1 | Service online with compatible usage | Client calls `IOC_connectService` in auto-accept path | IOC establishes valid link pair and returns success | reviewed | US-1 AS-1, TC seeds T1/E1/M1/M2/F2 |
| AC-2 | US-1 | Service online in manual-accept mode with pending client | Service calls `IOC_acceptClient` | IOC finalizes link pair and messaging APIs become usable | reviewed | US-1 AS-2, TC seeds T2/E2/F1 |
| AC-3 | US-1 | Service may have established links | Service calls `IOC_offlineService` | No new connect succeeds; keep/close behavior follows service flags | reviewed | US-1 AS-3, TC seeds E3/E4/F1 |
| AC-4.1 | US-4 | IOC architecture documentation and source ownership map | Each module/file is classified into one of L0-L3 | Allowed and forbidden dependency directions are explicit and reviewable | reviewed | US-4 Scenario 1, README_ArchDesign layer ownership map + dependency graph, TC seed C1/C2 |
| AC-4.2 | US-4 | L2 needs protocol behavior | Core calls protocol operation through bound L1 Service/Link object | ProtoMethods dispatch maps to L0 implementations without bypass | reviewed | US-4 Scenario 1, README_DetailDesign ProtoMethods minimum set, TC seed C2 |
| AC-4.3 | US-4 | Incremental refactor slice is applied | US-1 regression suite runs after the slice | Existing US-1 behavior remains passing and traceable | reviewed | US-4 Scenario 3, README_DetailDesign lightweight implementation plan + README_StateDesign migration state model, TC seed C3 |
| AC-7.1 | US-7 | Developer opens the user guide without reading source | They follow setup/first-use guidance | They can identify build/run entry, public headers, and scenario references | reviewed | US-7 Scenario 1, README_UserGuide onboarding section |
| AC-7.2 | US-7 | Developer wants IOC service/link integration steps | They read API integration guidance | They understand online/connect/accept/offline sequence at usage level | reviewed | US-7 Scenario 2, README_UserGuide API integration section |
| AC-7.3 | US-7 | Developer has service/link usage context | They read DAT/EVT/CMD guidance | They can distinguish semantics and find deeper scenario references | reviewed | US-7 Scenario 3, README_UserGuide usage scenarios section |
| AC-7.4 | US-7 | Guide content touches lifecycle or design constraints | Internal details would be needed | Guide links to design docs instead of redefining internals | reviewed | US-7 Scenario 4, README_UserGuide cross-doc boundary section |

## Lifecycle Links

- Pending input: `.catdd/spec/pendingNews/`
- Active story: `.catdd/spec/doingUS/20260707-RewriteREADMEUserGuideForAppDevelopers-UserStory.md`
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
