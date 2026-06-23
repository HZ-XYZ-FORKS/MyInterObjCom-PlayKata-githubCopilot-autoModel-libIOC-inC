---
description: "Run CaTDD slash command UT_implTestCase"
agent: "agent"
argument-hint: "Fill required command inputs, source files, target files, language, and test framework"
---
# UT_implTestCase

**Thin Copilot adapter.** Source of truth: [UT_implTestCase](../../.catdd/slashCommands/commands/P0-FuncTestsFlow/UT_implTestCase.md)

## Copilot Execution Rules

1. **Read the portable source command first** for full RED/GREEN/REFACTOR contract.
2. **Implement ONE TC at a time** — no batching.
3. **Preserve design skeleton** using [CaTDD_designAndImplTemplate.cxx](../../.catdd/methodPrompts/CaTDD_designAndImplTemplate.cxx):
   - OVERVIEW section (WHAT/WHERE/WHY, SCOPE, KEY CONCEPTS)
   - COVERAGE MATRIX for US/AC/TC
   - Complete GIVEN/WHEN/THEN for each AC
   - @[US]/@[AC]/@[TC]/@[Status] markers preserved
4. **RED stage (default):** Test fails before product code exists
   - Run test, confirm meaningful failure
   - Report: "Test is RED and waiting for product code"
   - No product code in RED stage
5. **GREEN stage:** Minimal product code makes test pass
   - Add only what's needed, run test
   - Report: "Test is GREEN"
6. **REFACTOR stage:** Clean production code (no behavior change)
7. **Update @[Status]** marker (RED → GREEN → REFACTOR), preserve design comments.
8. Ask developer before guessing.

ONE-MORE-THING: ask developer if something not sure
