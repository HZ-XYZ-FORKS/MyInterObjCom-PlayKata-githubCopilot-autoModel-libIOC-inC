# Issue: Clarify IOC StateDesign Meaning

> Imported by: `SPEC_importIssue`
> Imported on: 2026-07-04
> analyzed_by: `SPEC_analyzeIssue`
> analyzed_on: 2026-07-05
> Import mode: `inline`
> Labels: `issue`, `clarification`, `design`, `state-model`
> status: `analyzed`

## Source Type

Developer-reported issue / clarification note.

## Raw Source

"StateDesign" in IOC means state diagram of SrvID and LinkID, and Cmd/Evt/Dat sub state over LinkID.

## Lightweight Normalization

- Topic: IOC state-design meaning
- Scope mentioned: `SrvID`, `LinkID`, and `Cmd/Evt/Dat` sub-state over `LinkID`
- Observed gap: current design discussion may not yet express StateDesign with the intended object/state-diagram focus
- Expected clarification: StateDesign should model `SrvID` and `LinkID` lifecycle/state plus `Cmd/Evt/Dat` sub-state layered over `LinkID`

## Trace Notes

- project_context: `.catdd/spec/projectContext.md`
- generated_todo_story: `.catdd/spec/todoUS/20260705-StateDesign-SrvID-LinkID-SubState-UserStory.md`
- project_story_ledger: `README_UserStories.md`
- related_closed_story: `.catdd/spec/doneUS/20260704-reArchDesign-LayeredArchitecture-UserStory.md`
- notes: Issue analyzed and archived from pendingNews; repair-oriented todo story generated with open blocking acceptance questions.
