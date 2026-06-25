---
description: "Run CaTDD slash command UT_implTestCase"
agent: "agent"
argument-hint: "Fill required command inputs, source files, target files, language, and test framework"
---
# UT_implTestCase

**Thin Copilot adapter.** Source of truth: [UT_implTestCase](../../.catdd/slashCommands/commands/P0-FuncTestsFlow/UT_implTestCase.md)

## Copilot Execution Rules

1. **Read the portable source command first** and follow it as the full RED/GREEN/REFACTOR contract.
2. **Do not restate or override the source command.** If this wrapper conflicts with it, the source command wins.
3. **Implement one selected TC at a time; no batching.**
4. **Use the CaTDD design template as the structural reference** and preserve US/AC/TC/status markers.
5. **Report verification according to the source command output contract.**
6. Ask the developer before guessing unclear product intent, acceptance criteria, or test behavior.
