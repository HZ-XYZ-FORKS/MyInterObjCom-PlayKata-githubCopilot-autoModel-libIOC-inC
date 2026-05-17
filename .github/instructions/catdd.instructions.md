---
description: "Use when working with CaTDD, comment-alive tests, US/AC/TC skeletons, or UT_* slash commands."
---
# CaTDD Project Instructions

- CaTDD method source: `.catdd/methodPrompts/`
- Portable slash command source: `.catdd/slashCommands/`
- Copilot prompt wrappers: `.github/prompts/UT_*.prompt.md`
- Treat Copilot prompt files as thin adapters over `.catdd/slashCommands/`.
- Treat `.catdd/methodPrompts/` as the source of truth for category meaning, priority order, design skeleton rules, and CaTDD method constraints.
- Ask the developer when product intent, acceptance criteria, or test behavior is unclear.
