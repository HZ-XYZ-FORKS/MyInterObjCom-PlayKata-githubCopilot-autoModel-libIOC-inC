# Issue: Clarify IOC StateDesign Meaning

> Imported by: `SPEC_importIssue`
> Imported on: 2026-07-04
> Import mode: `inline`
> Labels: `issue`, `clarification`, `design`, `state-model`

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

- Imported only; no user-story analysis performed in this command.
- Expected follow-up command if refinement is needed: `SPEC_analyzeIssue`
