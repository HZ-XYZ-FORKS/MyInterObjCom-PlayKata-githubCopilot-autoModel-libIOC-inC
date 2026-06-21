# MyInterObjComBase User Guide

Practical guide for developers integrating IOC service/client link establishment in this repository.

## Who

Use this guide if you are:

- Implementing IOC service/client integration behavior.
- Writing or reviewing CaTDD tests for US-1 link-establishment flow.
- Verifying runtime behavior for online/connect/accept/offline sequence.

## What

This guide explains how to use the current IOC requirement slice for service-client link establishment.

It covers:

- Service online setup and accept-mode behavior.
- Client connect behavior for compatible and incompatible usage.
- Offline transition behavior for KEEP/CLOSE accepted-link policy.

## When

Use this guide when:

- You are implementing or validating US-1 behavior.
- You are preparing detail design and unit-test design for link establishment.
- You need quick reference for accepted scope and non-goals.

## Where

Main repository paths:

```text
Doc/IOC_UsageScenarios.md
Include/IOC/
.catdd/spec/doingUS/20260618-EstablishedLink-UserStory.md
README_UserStories.md
```

## Why

This workflow matters because link establishment is the prerequisite for DAT/EVT/CMD operations.

It helps preserve:

- Requirement traceability from stories to AC and test seeds.
- Consistent lifecycle behavior across auto/manual accept paths.
- Clear scope boundaries before design and implementation.

## How

1. Bring a service online with `IOC_onlineService` and service flags/args that define accept mode.
2. Call `IOC_connectService` from client with compatible usage.
3. If manual accept mode is used, complete link establishment via `IOC_acceptClient`.
4. Validate offline behavior via `IOC_offlineService` for KEEP/CLOSE policy and blocked new connections.
5. Confirm behavior against US-1 acceptance criteria and AC ledger in `README_UserStories.md`.

## Current Scope Notes

- In scope: link establishment and offline transition behavior for US-1.
- Out of scope: reconnection strategy, concurrency design tests (tracked as US-2), and payload-level DAT/EVT/CMD semantics beyond link establishment.

## Quality Checklist

- Keep this guide aligned with `README_UserStories.md` and active lifecycle artifacts.
- Keep commands and API references consistent with `Include/IOC/*.h` contracts.
- Route architecture and detail decisions to architecture/detail design docs instead of expanding this guide.
