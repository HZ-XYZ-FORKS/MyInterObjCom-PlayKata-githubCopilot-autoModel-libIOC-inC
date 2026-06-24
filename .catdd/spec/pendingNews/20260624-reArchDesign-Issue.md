# Issue Import - reArchDesign Layering

## Meta

- imported_by: SPEC_importIssue
- imported_on: 2026-06-24
- source_type: developer-reported problem
- classification: issue, refactor, architecture
- status: pending-analysis

## Issue Summary

Request to re-architect IOC into explicit layered design:

- L0 = Platform (Posix/Linux/MacOS, RTOS, ...)
- L1 = IOC-Protocol (FIFO, TCP, UDP, HTTP, ...)
- L2 = IOC-Core
- L3 = IOC-Interface

## Preserved Source Intent (Raw)

"reArchDesign with: L0==Platform(Posix/Linux/MacOS, RTOS,...); L1=IOC-Protocol(FIFO,TCP,UDP,HTTP,...); L2=IOC-Core; L3=IOC-Interface"

## Observed Concern

Current repository evolution may mix responsibilities across platform, protocol, core behavior, and interface boundaries, creating architectural coupling risk and making future protocol/platform expansion harder.

## Expected Direction (High-Level)

Introduce clear module boundaries and dependency direction for L0/L1/L2/L3 so platform- and protocol-specific code is isolated below IOC-Core and IOC-Interface.

## Trace

- project_context: .catdd/spec/projectContext.md
- related_active_story: .catdd/spec/doingUS/20260618-EstablishedLink-UserStory.md
- notes: Imported as raw pending issue only; no user story or acceptance criteria generated in this step.
