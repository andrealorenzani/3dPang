---
name: new_feature
description: Implement a change to 3dPang end to end - analyse against docs/, plan, code, QA, update docs. Use when the user asks for a new feature, a behaviour change, a bug fix or a refactor in this repository.
argument-hint: <description of the change>
---

# /new_feature

The user wants this change made to 3dPang: **$ARGUMENTS**

If the description is empty, ask for it. If it is too vague to plan (several incompatible
interpretations), ask at most three precise questions first; otherwise proceed.

You are the orchestrator. Work through the four phases below, delegating to the project agents
(`.claude/agents/`) with the Agent tool. Agents start without your context: give each one the
full text it needs (the request, the plan, the reports), never "see above". Subagents cannot launch other
subagents, so *you* run the advisor when the analyser asks for one.

## Phase 1 - Analyse
Invoke the **`analyser`** agent with the request. Ask it to follow its procedure and return the plan.

- If its answer ends with a `NEEDS_ADVISOR` block: launch an independent advisor, a `general-purpose`
  agent with `model: "opus"` and `effort: "higher"`, **without** the analyser's reasoning: give it only
  the request, the question/options, and the paths `docs/architecture.md` and `docs/code.md`. Ask for a
  recommendation with trade-offs in under 300 words. Then send the advice back to the analyser
  (SendMessage to the same agent, or a new `analyser` call that includes the advice) and get the final plan.
- Show the user the plan (summary, impacts, steps, tests, docs, risks, assumptions). **Wait for approval**
  with a short question unless the request said to proceed without asking. If the user edits the plan, use
  the edited one. For plans marked *simple* with no open questions you may continue after showing it.

## Phase 2 - Code, then QA, in a loop (max 3 rounds)
1. Invoke **`coder`** with the approved plan (and, from round 2, the QA findings to fix). Keep its report.
2. Invoke **`qa`** with the plan and the coder's report.
3. If the verdict is `PASS`, go to Phase 3. If `FAIL`, send the blocking findings back to `coder` and repeat.
4. After 3 failed rounds, stop: show the user the last QA report and ask how to proceed. Do not loop forever
   and do not hide a failing verdict.

## Phase 3 - Document
Invoke **`documenter`** with the approved plan, the final coder report and the final QA report (including its
"Docs to update" list). Keep its report.

## Phase 4 - Wrap up
Tell the user, briefly: what changed (files), the QA verdict with test counts, the docs updated, and anything
left undone or risky. Show `git status --short`. **Do not commit, push or tag** unless the user asked for it in
the request; offer to do it.

## Ground rules
- Never skip QA, and never mark success if `qa` said FAIL.
- Never let the coder edit `docs/`, nor the documenter edit code.
- Report the truth: if the environment cannot build or run tests (for example no Qt), say so rather than
  claiming a pass.
- Keep the user informed in one or two lines at each phase change.
