# SPEC_implUnitTests

## Purpose

**Lifecycle coordinator** for implementing CaTDD test cases across an active user story.

SPEC_implUnitTests orchestrates **cross-file, cross-category sequencing** (P0 Functional priority order).
It delegates **per-TC test implementation** to `UT_implTestCase` for strict RED/GREEN/REFACTOR discipline.

## CoT Pattern

**Linear Orchestration** — Direct execution. SPEC_implUnitTests:

1. Parses the active story's TC list from design verification artifacts
2. Selects TCs matching P0 Functional priority order (Typical → Edge → Misuse → Fault)
3. **For each selected TC**: invokes `UT_implTestCase` with:
   - Single TC ID
   - Target test file
   - Source files
   - Stage (RED → GREEN or REFACTOR)
4. Reports cumulative test status and recommends next SPEC command

If any TC fails RED or GREEN phase, SPEC_implUnitTests surfaces the failure before advancing.

## Responsibility Division

**SPEC_implUnitTests handles** (Px-SpecFlow lifecycle layer):

- Which TCs to implement (category priority order, P0-first)
- Cross-file coordination (which TC lives in which test file)
- Test framework selection and build validation
- Cumulative status reporting across multiple TCs
- Next SPEC command routing

**UT_implTestCase handles** (CaTDD category mechanics):

- Design skeleton preservation (@[US]/@[AC]/@[TC]/@[Status])
- RED state verification (test fails before product code)
- GREEN state verification (test passes after minimal product code)
- REFACTOR scope containment
- Single TC at a time

## Inputs

- `selected_category`: P0 Functional category to implement (e.g., "Typical", "Edge", "Misuse", "Fault")
  - If omitted: use verification design artifact to auto-select next P0 category in priority order
- `test_framework`: project test framework (e.g., GoogleTest, Catch2)
  - Usually auto-detected from CMakeLists.txt or build config

## Method References

- [Px-SpecFlow](../../flows/Px-SpecFlow.md) — Lifecycle context
- [P0-FuncTestsFlow](../../flows/P0-FuncTestsFlow.md) — P0 priority order
- [UT_implTestCase](../P0-FuncTestsFlow/UT_implTestCase.md) — Delegated TC implementation
- [CaTDD_methodPrompt](../../../methodPrompts/CaTDD_methodPrompt.md) — Category semantics

## Output Contract

- **For each TC in the selected category:**
  - `UT_implTestCase` output: test code with preserved skeleton, RED or GREEN status, status marker updated
  - Verification: test runs and shows actual result (RED/GREEN/ERROR)
- **Cumulative report:** X of Y TCs in category passing. Next category recommendation.
- **Next recommended command:**
  - If category TCs all GREEN: `SPEC_implUnitTests` with next P0 category (e.g., Edge → Misuse → Fault)
  - If category has RED or ERROR: `SPEC_implProductCodes` to implement product behavior for failing TC
  - After all P0 categories complete: `SPEC_implProductCodes` or promote to P1 (State/Capability)

## Execution Flow

```text
SPEC_implUnitTests [<category>]
  ├─ Locate <UT_<feature>_<category>.cxx> test file in project
  ├─ Extract all TC entries from file: TC-<id-1>, TC-<id-2>, ...
  │
  ├─ For each TC:
  │   └─ UT_implTestCase (TC-<id>, test_file, sources, stage=RED)
  │      ├─ Preserve @[US]/@[AC]/@[TC] skeleton
  │      ├─ Implement test code
  │      ├─ Run: ✗ FAIL (RED) ← Expected, skeleton ready for product code
  │      └─ Report TC status: RED
  │
  └─ Summary: <category> coverage: X/Y RED, Z/Y GREEN ✓
     Next: SPEC_implProductCodes for failing TCs, or next category

EXAMPLE (IOC project):
  SPEC_implUnitTests [Typical]
    ├─ Locate Test/UT_US1_Service_Typical.cxx
    ├─ Extract: TC-P0-T1, TC-P0-T2
    ├─ For TC-P0-T1: UT_implTestCase → RED ✓
    ├─ For TC-P0-T2: UT_implTestCase → RED ✓
    └─ Summary: Typical: 0/2 GREEN, 2/2 RED
       Next: SPEC_implProductCodes for P0 Typical
```

## Prompt Template

When invoked, ask the developer:

1. **Category focus?** (Default: next P0 category from verification design)
   - Options: Typical, Edge, Misuse, Fault (P0 Functional priority order)
   - Or: State, Capability, Concurrency (P1 Design, after P0 complete)
   - If ambiguous: "Verification design shows P0 categories in order: [list from project]. Which should I implement next?"

2. **Single TC or batch?** (Default: batch all TCs in selected category)
   - Single TC: `UT_implTestCase` for one specific TC with explicit RED/GREEN stage
   - Batch: `UT_implTestCase` invoked for each TC sequentially, status accumulated

3. **Stage?** (Default: RED → GREEN sequence)
   - RED: Implement test only, show failure (no product code yet)
   - GREEN: Product code exists and should now pass
   - REFACTOR: Product code works, clean it up (no behavior change)

## Conflict Guard

- **Do not batch TCs across categories.** Typical → Edge → Misuse → Fault in order. Enforce P0 Functional ordering.
- **Do not reimplement UT_implTestCase logic.** Delegate per-TC work to `UT_implTestCase` instead of duplicating skeleton preservation, RED/GREEN verification, or status tracking.
- **Do not advance to SPEC_implProductCodes until all TCs in current category are RED.** RED state proves tests are meaningful.
- If a TC cannot compile or run (build error, missing API), surface that as a blocker before proceeding.
- If test framework is unknown or build fails, ask developer for clarification.

ONE-MORE-THING: ask developer if something not sure
