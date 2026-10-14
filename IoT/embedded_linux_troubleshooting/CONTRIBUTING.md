# Contributing

Thanks for helping improve this guide. It exists to save the next engineer hours of blind debugging — every contribution should keep that promise.

## Ways to contribute

- **New symptom → root cause → first command** rows for the quick-reference table
- **Corrections**: wrong thresholds, outdated commands, platform-specific caveats
- **War stories**: real incidents that taught a rule (we'll help distill it into guide format)
- **Missing sections**: e.g., containerized embedded systems, secure boot failures, specific SoC families

## Ground rules

1. **Field-proven only.** Every command and claim should come from a real device, not speculation. If something is theoretical, mark it explicitly with `> Note:`.
2. **Preserve the methodology.** The guide's core discipline is *evidence first, one variable at a time*. New content must not encourage destructive early actions (`fsck`, reflash, format).
3. **BusyBox-compatible where possible.** Commands should work on minimal systems, or explicitly note the full-distro requirement.
4. **No secrets.** Strip hostnames, IPs, customer names, and proprietary paths from any logs or examples you include.

## Style guide

- **Structure**: symptom → command → interpretation table. Readers scan under pressure; tables beat prose.
- **Commands**: one command per line with an inline `# comment` explaining what it answers.
- **Warnings**: destructive or irreversible operations get a `> **Caution:**` blockquote.
- **Cross-references**: link to sections by anchor (`[Section 4](#4-storage-issues-emmc--nand--flash)`) and keep the Table of Contents in sync.
- **Length**: prefer a sharp table over a long narrative. The guide is used on-site, sometimes from a phone.
- **Destructive-action checklist**: any new irreversible procedure must state (a) when it's justified, (b) what to capture first, (c) how to back out.

## Submitting changes

1. Fork the repo and create a branch: `git checkout -b fix/short-description`
2. Keep PRs focused — one topic per PR (a new section, a table fix, etc.)
3. Verify your markdown renders on GitHub (tables, Mermaid diagrams, anchors)
4. Open a PR describing:
   - What problem in the field this addresses
   - How you verified the commands (device, kernel version)
5. For large additions (a whole new section), open an issue first to discuss placement

## Adding a new troubleshooting section

Use this skeleton:

`````markdown
## N. <Domain> Issues

### N.1 First checks

```sh
<command>    # what it answers
```

### N.2 Failure signatures

| Symptom | Diagnosis | Action |
| --- | --- | --- |

> **Classic pitfall:** ...
`````

## Questions

Open a GitHub issue — include the evidence archive format described in [Section 2](embedded_linux_troubleshooting_guide.md#2-baseline-data-collection-mandatory-first-step) if your question relates to a real device.
