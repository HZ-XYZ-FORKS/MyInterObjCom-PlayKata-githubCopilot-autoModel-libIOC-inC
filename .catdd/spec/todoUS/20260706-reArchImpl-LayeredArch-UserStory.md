# Implement Layered Architecture Slice After US-4 Design Closure

> **Story ID:** US-6 | **State:** todo | **Priority:** P1
> **Source:** `.catdd/spec/analyzedNews/20260706-reArchImpl-LayeredArch-Issue.md`
> **Parent Story:** `.catdd/spec/doneUS/20260704-reArchDesign-LayeredArchitecture-UserStory.md`
> **CaTDD Class:** P1 Design
> **Primary Category:** Capability
> **Created:** 2026-07-06

---

## Story Statement

<!-- Technique: write-user-story -->

**As a** maintainer implementing IOC layering changes,
**I want** a concrete implementation slice plan and acceptance boundary derived from the closed US-4 design,
**So that** layered architecture can move from design intent into controlled implementation without breaking existing behavior guarantees.

---

## Priority

<!-- Technique: prioritize-requirements -->

| Dimension | Score (1-9) | Rationale |
|---|---|---|
| Business Value | 8 | Converts completed architecture design into executable implementation progress. |
| User Value | 7 | Maintainers gain clear, enforceable implementation boundaries and regression expectations. |
| Cost / Effort | 6 | Requires implementation slicing, dependency-rule enforcement, and regression safeguards. |
| Risk / Complexity | 7 | High coupling risk if implementation scope is ambiguous across layers and migration order. |

**Priority Score:** (8 + 7) / (6 + 7) = **1.15** | **Priority:** **P1**

---

## Visual Model

<!-- Technique: elicit-requirements-models -->

```mermaid
flowchart TD
 A[US-4 Done Design Baseline] --> B[Select first implementation slice]
 B --> C{Scope explicit?}
 C -->|No| D[Clarify boundaries and acceptance first]
 C -->|Yes| E[Implement slice in source modules]
 E --> F[Run regression guardrail checks]
 F --> G{Pass?}
 G -->|Yes| H[Promote slice and plan next one]
 G -->|No| I[Reject slice and refine implementation plan]
```

### Model Gap Analysis

| # | Gap Found | Question |
|---|---|---|
| 1 | First implementation slice boundaries were initially implicit. | Resolved: first slice is IOC service/link lifecycle APIs in `Include/IOC/IOC_SrvAPI.h` and their implementation path in `Source/IOC_SrvAPI.c`. |
| 2 | Regression gate was initially conceptual. | Resolved: run US-1 service regression executables (`UT_US1_Service_Typical`, `UT_US1_Service_Edge`, `UT_US1_Service_Misuse`, `UT_US1_Service_Fault`, `UT_US1_Service_State`, `UT_US1_Service_Capability`) with all tests passing before slice promotion. |

---

## Acceptance Criteria

<!-- Techniques: write-user-story + facilitate-example-mapping -->

### Scenario 1: First Implementation Slice Scope Is Explicit

**Rule:** Implementation must start with a bounded slice that is traceable to closed US-4 design artifacts.
**Given** the closed US-4 design story and related architecture/detail/state artifacts
**When** defining the first reArchImpl slice
**Then** in-scope modules, out-of-scope modules, and acceptance boundaries are explicit and reviewable

| Concrete Examples | Counter-Examples |
|---|---|
| First slice targets `IOC_onlineService`, `IOC_offlineService`, `IOC_acceptClient`, `IOC_connectService`, `IOC_closeLink` API path in `IOC_SrvAPI` | "Implement layered architecture" with no API/module boundary |

**Open Questions:** None.

### Scenario 2: Implementation Slice Includes Deterministic Regression Gate

**Rule:** Slice promotion requires explicit regression evidence preserving prior behavior contracts.
**Given** an implementation slice candidate
**When** regression gates are executed
**Then** promotion decisions are deterministic and traceable to named verification evidence

| Concrete Examples | Counter-Examples |
|---|---|
| Named UT suites and expected outcomes documented before merge/promotion | Promotion based on ad-hoc manual confidence without declared gate criteria |

**Open Questions:** None.

---

## Business Rules

<!-- Technique: extract-business-rules -->

| ID | Rule | Type | Implied Functional Requirement |
|---|---|---|---|
| BR-1 | US-4 design outputs are the baseline contract for reArch implementation slices. | Constraint | Slice definitions must trace to closed design artifacts before code changes. |
| BR-2 | Every implementation slice must declare explicit in-scope/out-of-scope boundaries. | Constraint | Implementation planning artifacts must include deterministic module/file boundary declarations. |
| BR-3 | Slice promotion must be gated by declared regression evidence. | Action Enabler | Verification plan must map required tests and expected pass criteria per slice. |

---

## Scope

**In scope:**

- Implement-first slice is constrained to IOC service/link lifecycle API path:
 	- Public API surface: `Include/IOC/IOC_SrvAPI.h`
 	- Primary implementation path: `Source/IOC_SrvAPI.c`
 	- First API set: `IOC_onlineService`, `IOC_offlineService`, `IOC_acceptClient`, `IOC_connectService`, `IOC_closeLink`
- Apply and verify deterministic regression gates before slice promotion:
 	- `UT_US1_Service_Typical`
 	- `UT_US1_Service_Edge`
 	- `UT_US1_Service_Misuse`
 	- `UT_US1_Service_Fault`
 	- `UT_US1_Service_State`
 	- `UT_US1_Service_Capability`

**Non-goals:**

- Full multi-slice rearchitecture implementation in one story.
- Contract-breaking API changes beyond the closed US-4 design baseline.

---

## Risks & Assumptions

| # | Risk / Assumption | Severity | Mitigation / Clarification Needed |
|---|---|---|---|
| 1 | Ambiguous slice boundaries can cause uncontrolled implementation spread. | High | Require explicit module/file inclusion and exclusion lists before opening execution. |
| 2 | Regression gate ambiguity can allow unsafe promotions. | High | Declare concrete required tests and expected outcomes before slice implementation starts. |

---

## Initial Acceptance Questions

| # | Question | Raised By | Status |
|---|---|---|---|
| 1 | Which exact first-slice modules/files are mandatory for `reArchImpl-LayeredArch`? | model gap | answered: `Include/IOC/IOC_SrvAPI.h` + `Source/IOC_SrvAPI.c` for service/link lifecycle API path |
| 2 | Which exact regression gate set is mandatory for first-slice promotion? | model gap | answered: all US-1 service regression suites listed in Scope must pass |
| 3 | Should this story first run a planning command to convert US-4 design outputs into an implementation task artifact before opening execution? | lifecycle guard | answered: yes, follow `SPEC_openUserStory` then `SPEC_makePlan` before implementation commands |

**Gate:** This story is **READY** for `SPEC_openUserStory`; next run `SPEC_makePlan` immediately after opening.

---

## Ambiguity Warnings

<!-- Technique: validate-requirements-criteria -->

| # | Ambiguous Term | Found In Section | Clarifying Question |
|---|---|---|---|
| 1 | "reArchImpl-LayeredArch" | Raw issue wording | Does this refer to only first implementation slice or the full end-to-end rearchitecture rollout? |
| 2 | "as designed" | Raw issue wording | Which exact US-4 design sections are normative for implementation acceptance? |

---

## Traceability

| From → To | Link |
|---|---|
| This story → Raw input | `.catdd/spec/analyzedNews/20260706-reArchImpl-LayeredArch-Issue.md` |
| Project story index | `README_UserStories.md` |
| Parent story | `.catdd/spec/doneUS/20260704-reArchDesign-LayeredArchitecture-UserStory.md` |
| This story ID | US-6 |
