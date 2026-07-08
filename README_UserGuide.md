# MyInterObjComBase User Guide

Practical guide for application developers who want to build new IOC-based apps without reading implementation source files.

## Who

Use this guide if you are:

- Building a new application that integrates IOC service/client communication.
- Onboarding to IOC public APIs and role semantics (Service, Link, DAT, EVT, CMD).
- Reviewing requirement-level usage behavior before architecture/detail design deep dives.

## What

This guide provides a usage-first path for:

- Build/run onboarding from existing project docs.
- Service/link API integration flow using public headers.
- DAT/EVT/CMD role semantics and where to find detailed scenarios.

It does not redefine architecture or implementation internals.

## When

Use this guide when:

- You need a first IOC integration path for a new app.
- You want to understand IOC usage sequence before touching source code.
- You need requirement-traceable usage behavior aligned with project story ledgers.

## Where

Primary references in this repository:

```text
README_BuildRunTest.md
README_UserStories.md
Doc/IOC_UsageScenarios.md
Include/IOC/
README_ArchDesign.md
README_DetailDesign.md
README_StateDesign.md
```

## Why

This guide exists to reduce onboarding friction and keep usage behavior aligned with requirement artifacts.

It helps preserve:

- Clear user-level integration flow before deep technical design work.
- Stable traceability from story acceptance criteria to usage guidance.
- Correct boundary between user guidance and design-internal documentation.

## How

### 1) Build and Run Onboarding

1. Start with project build/run entry points in `README_BuildRunTest.md`.
2. Confirm current active/todo/done requirement context in `README_UserStories.md`.
3. Use this guide as the API-and-scenarios bridge from build/run commands to IOC usage semantics.

### Quickstart Snippet (Compact)

Use this minimal sequence as a first integration sketch:

```c
// Service side
IOC_Result_T r1 = IOC_onlineService(serviceArgs, serviceFlags, &serviceID);

// Client side
IOC_Result_T r2 = IOC_connectService(serviceID, connectArgs, &linkID);

// Optional in manual-accept mode
IOC_Result_T r3 = IOC_acceptClient(serviceID, linkID, acceptArgs);

// Later, service shutdown
IOC_Result_T r4 = IOC_offlineService(serviceID, offlineFlags);
```

Adjust argument structures and option flags according to `Include/IOC/*.h`.

### 2) IOC API Integration Flow (Service/Link)

Use the following usage-level sequence for first integration:

1. Service side publishes readiness with `IOC_onlineService`.
2. Client side requests linkage with `IOC_connectService`.
3. If manual accept mode is configured, service finalizes via `IOC_acceptClient`.
4. Service transitions offline through `IOC_offlineService` when needed.

Public API contracts are defined under `Include/IOC/`. Keep integration behavior consistent with those headers.

### 3) DAT / EVT / CMD Usage Semantics

- DAT: asynchronous streaming semantics with ordered/completeness intent.
- EVT: asynchronous fire-and-forget semantics with may-drop acceptance.
- CMD: synchronous request-response semantics with definite success/failure result.

For scenario-level behavior and examples, use `Doc/IOC_UsageScenarios.md`.

### 4) Cross-Document Boundary Rule

This guide is usage-facing.

- If content needs architecture boundaries or dependency direction, use `README_ArchDesign.md`.
- If content needs class/API internals or object responsibilities, use `README_DetailDesign.md`.
- If content needs lifecycle/substate transition internals, use `README_StateDesign.md`.

## Current Scope Notes

- In scope: app-developer onboarding, usage-level service/link API flow, and DAT/EVT/CMD semantic orientation with trace links.
- Out of scope: changing IOC public API contracts, defining new canonical build policy, or duplicating design-internal decisions.

## Requirement Trace

- Story alignment: US-7 in `README_UserStories.md`.
- Acceptance trace: AC-7.1 to AC-7.4 in `README_UserStories.md`.
- Source scenario references: `Doc/IOC_UsageScenarios.md`.
- Public API source of truth: `Include/IOC/*.h`.

## Quality Checklist

- Keep this guide aligned with `README_UserStories.md` lifecycle and AC status.
- Keep API behavior descriptions consistent with `Include/IOC/*.h` and scenario docs.
- Keep architecture/detail/state internals in their dedicated README design docs.
