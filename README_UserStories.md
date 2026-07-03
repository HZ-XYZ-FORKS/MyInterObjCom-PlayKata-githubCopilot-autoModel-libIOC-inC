# MyInterObjComBase User Stories

Project-level ledger for user-story lifecycle state (TODO/DOING/DONE) and acceptance-criteria trace/status.

## Story Ledger

### TODO / DOING Stories

| Story ID | Title | State | Source | Trace Link |
| --- | --- | --- | --- | --- |
| US-2 | Establish IOC Link Concurrency Safety | todo | `.catdd/spec/todoUS/20260618-EstablishedLink-UserStory.md` | `.catdd/spec/todoUS/20260621-EstablishedLink-ConcurrencyDesignStory.md` |

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
```

## Acceptance Criteria Ledger

| AC ID | Story | Given | When | Then | Status | Verification Trace |
| --- | --- | --- | --- | --- | --- | --- |
| AC-1 | US-1 | Service online with compatible usage | Client calls `IOC_connectService` in auto-accept path | IOC establishes valid link pair and returns success | reviewed | US-1 AS-1, TC seeds T1/E1/M1/M2/F2 |
| AC-2 | US-1 | Service online in manual-accept mode with pending client | Service calls `IOC_acceptClient` | IOC finalizes link pair and messaging APIs become usable | reviewed | US-1 AS-2, TC seeds T2/E2/F1 |
| AC-3 | US-1 | Service may have established links | Service calls `IOC_offlineService` | No new connect succeeds; keep/close behavior follows service flags | reviewed | US-1 AS-3, TC seeds E3/E4/F1 |

## Lifecycle Links

- Pending input: `.catdd/spec/pendingNews/`
- Active story: none (US-1 closed)
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
