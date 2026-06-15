---
description: "Run CaTDD slash command UT_designTypicalSkeleton"
agent: "agent"
argument-hint: "Fill required command inputs, source files, target files, language, and test framework"
---
# UT_designTypicalSkeleton

You are running a Copilot-native wrapper around a portable CaTDD slash command.

## Source Command

- Portable command path: .catdd/slashCommands/commands/P0-FuncTestsFlow/UT_designTypicalSkeleton.md
- Default workspace link: [.catdd/slashCommands/commands/P0-FuncTestsFlow/UT_designTypicalSkeleton.md](../../.catdd/slashCommands/commands/P0-FuncTestsFlow/UT_designTypicalSkeleton.md)
- Flow: P0-FuncTestsFlow

## Method Source of Truth

- CaTDD method index: [.catdd/methodPrompts/README.md](../../.catdd/methodPrompts/README.md)
- Slash command contract: [.catdd/slashCommands/UT_slashCommandTemplate.md](../../.catdd/slashCommands/UT_slashCommandTemplate.md)
- Flow docs: [.catdd/slashCommands/flows](../../.catdd/slashCommands/flows)

## Execution Rules

1. Read and follow the portable source command before acting.
2. Treat this file as a thin Copilot adapter; do not redefine CaTDD method semantics here.
3. Use methodPrompts for category meaning, priority order, design skeleton rules, and CaTDD constraints.
4. Use the source command for inputs, outputs, conflict guards, and next-step flow.
5. Ask for missing product intent instead of inventing requirements.
6. Report the next recommended slash command when the step finishes.

ONE-MORE-THING: ask developer if something not sure
