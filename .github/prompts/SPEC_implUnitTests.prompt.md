---
description: "Run CaTDD slash command SPEC_implUnitTests"
agent: "agent"
argument-hint: "Fill required command inputs, source files, target files, language, and test framework"
---
# SPEC_implUnitTests

**Thin Copilot adapter.** Source of truth: [SPEC_implUnitTests](../../.catdd/slashCommands/commands/Px-SpecFlow/SPEC_implUnitTests.md)

## Copilot Execution Rules

1. **Read the portable source command first** for full orchestration contract.
2. **Delegate per-TC work to UT_implTestCase** — do not batch or duplicate UT_implTestCase logic.
3. **For each TC in selected category:**
   - Invoke `UT_implTestCase` with single TC ID (not batch)
   - Report RED/GREEN/ERROR status
   - Advance based on result
4. Ask developer before guessing ambiguous inputs.

ONE-MORE-THING: ask developer if something not sure
