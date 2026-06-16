# Project Context

## Meta

- default_lang: US_EN
- initialized_by: SPEC_initProjectContext
- initialized_on: 2026-06-16
- project_root: /Users/enigmawu/HZ-XYZ-FORKS/MyInterObjCom-PlayKata-githubCopilot-autoModel-libIOC-inC

## Project Facts (Confirmed)

- Project name: MyInterObjComBase.
- Current target outcome: design, implement, and test IOC as libIOC.a in C.
- Existing IOC interface headers are located under Include/IOC.
- IOC domain model centers on Service and Link.
- Communication roles are split into DAT, EVT, and CMD.
- DAT semantics: asynchronous streaming with no-drop intent and ordered/completeness expectations.
- EVT semantics: asynchronous fire-and-forget with may-drop acceptance.
- CMD semantics: synchronous request-response with final result/failure certainty.
- Usage scenarios and acceptance-style behavior examples are documented in Doc/IOC_UsageScenarios.md.

## Reference Tech Stack (from MyIOC_inTDD_withGHC README)

- Development approach in this repository: Comment-alive Test-Driven Development (CaTDD).
- AI-assisted workflow reference: GitHub Copilot assisted development.
- IOC module reference shape: static library with C/C++ API.
- Deployment orientation reference: may target embedded systems in small memory footprints.

## Scope and Boundaries (Current)

- In scope now:
  - IOC library implementation path focused on static lib output (libIOC.a).
  - C language implementation target for this repository cycle.
  - DAT/EVT/CMD behavior and Service/Link lifecycle coverage.
- Out of scope now (until explicitly added by developer):
  - Shared library (.so) target.
  - OpenAPI generation in TypeScript/Python.
  - Non-C implementation tracks.

## Constraints

- Do not invent product requirements beyond README and scenario documents.
- Preserve IOC semantic contract in Include/IOC headers unless a future user story explicitly changes it.
- Maintain semantics compatibility with documented usage scenarios in Doc/IOC_UsageScenarios.md.

## Code Conventions (Current Baseline)

- Primary implementation language target: C.
- Public API contract source: Include/IOC/*.h.
- Keep design and implementation traceable to DAT/EVT/CMD semantics and Service/Link lifecycle.

## Test Conventions (Current Baseline)

- Testing approach: CaTDD, test-first style.
- Behavior expectations should be traceable to scenario-level acceptance criteria in Doc/IOC_UsageScenarios.md.
- CaTDD method semantics remain governed by .catdd/methodPrompts/ (not duplicated here).

## Constitution-Level Guardrails

- Project context is a stable decision baseline for later SPEC_* commands.
- Separate stable facts from assumptions and open questions.
- Avoid lifecycle-step skipping: use Px-SpecFlow review and gate commands before downstream implementation steps.
- Do not use this file to redefine CaTDD category rules or priority order.

## Assumptions To Confirm

- Build toolchain (e.g., Make/CMake/Ninja) is not yet fixed in this repository.
- Target platform matrix (Linux/macOS/embedded RTOS variants) is not yet fixed.
- CI policy and required checks are not yet defined.
- Naming/packaging conventions for test files are not yet fixed.

## Open Questions

- Which build system should be canonical for libIOC.a in C (Make, CMake, or other)?
- Which test framework should be standard for this repo (Unity, CMocka, Criterion, custom, etc.)?
- Which compilers and C standard should be mandatory (e.g., gcc/clang, C11/C17)?
- Which target environments must be supported first (desktop, embedded, both)?
- Should API compatibility policy be strict source compatibility, ABI compatibility, or both?
