# Issue: sync ArchDesign2DetailDesign2SrcImpl

> Imported by: SPEC_importIssue
> Imported on: 2026-07-09
> analyzed_by: SPEC_analyzeIssue
> analyzed_on: 2026-08-02
> Import mode: inline
> Labels: refactor, architecture-sync, detail-design-sync, source-implementation-sync
> status: analyzed

## Source Type

Developer-reported problem.

## Raw Source

"sync ArchDesign2DetailDesign2SrcImpl"

## Lightweight Normalization

- Topic: synchronize architecture design, detail design, and source implementation layering.
- Referenced artifacts: `README_ArchDesign.md`, `README_DetailDesign.md`, and source implementation layout.
- Observed concern: architecture and detail-design layering intent may not fully match current implementation boundaries.
- Expected direction: produce a bounded, testable synchronization user story rather than a broad unscoped refactor.

## Trace Notes

- status: analyzed
- project_context: `.catdd/spec/projectContext.md`
- generated_todo_story: `.catdd/spec/todoUS/20260709-sync-ArchDesign2DetailDesign2SrcImpl-UserStory.md`
- project_story_ledger: `README_UserStories.md`
- related_closed_story: `.catdd/spec/doneUS/20260704-reArchDesign-LayeredArchitecture-UserStory.md`
- clarification_2026_08_02:
 	- interface-to-implementation pairing is required (for example `IOC_SrvAPI.h -> IOC_SrvAPI.c`)
 	- layer families are explicit: `_Core_*`, `_Proto_*`, `_Plat_*`
 	- object anchors are explicit: `_IOC_SrvObject_T`, `_IOC_LinkObject_T`, `_IOC_ProtoObject_T`
 	- merge direction confirmed for architecture, detail design, and source layering evidence
- notes: moved from `pendingNews` to `analyzedNews`, normalized for SPEC flow traceability, and clarified to support ready-for-open todo story state.
