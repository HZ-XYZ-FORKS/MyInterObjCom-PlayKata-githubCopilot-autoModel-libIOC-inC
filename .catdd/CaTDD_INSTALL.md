# CaTDD Install Marker

- Installed version: 20260612.07

This directory is managed by `scripts/installCaTDD4Copilot.sh` from MyCaTDD.

- `methodPrompts/` is the installed CaTDD method source.
- `slashCommands/` is the installed portable flow-command source.
- `spec/` is the installed SpecCoding artifact workspace.
- `.github/prompts/UT_*.prompt.md` and `.github/prompts/SPEC_*.prompt.md` files are generated Copilot adapters.
- Commit team-shared SpecCoding artifacts under `.catdd/spec/`, such as `projectContext.md`, `pendingNews/`, `analyzedNews/`, `todoUS/`, `doingUS/`, `abortUS/`, and `doneUS/`.
- Use project-root `README*` files for shared SPEC docs such as `README.md`, `README_ArchDesign.md`, `README_UserStories.md`, `README_UserGuide.md`, `README_DetailDesign.md`, `README_ErrorDesign.md`, `README_ResourceDesign.md`, `README_StateDesign.md`, `README_PerfDesign.md`, `README_CompatDesign.md`, `README_DiagnosisDesign.md`, and `README_VerifyDesign.md` as needed.
- Keep local SpecCoding work state such as `.catdd/spec/WorkingProcessLog.md` gitignored.

Refresh this project by rerunning the installer from the MyCaTDD repository.
