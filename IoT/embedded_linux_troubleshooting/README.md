# Embedded Linux Troubleshooting Guide

[![Platform](https://img.shields.io/badge/platform-Embedded%20Linux-blue)]()
[![Storage](https://img.shields.io/badge/storage-eMMC%20%7C%20NAND%20%7C%20UBIFS-orange)]()
[![Init](https://img.shields.io/badge/init-systemd%20%7C%20BusyBox%20%7C%20SysV-yellow)]()
[![License](https://img.shields.io/badge/license-CC--BY--4.0-green)]()

A practical, field-tested troubleshooting playbook for **Linux-based embedded devices** — headless systems with eMMC/NAND/UBIFS storage, running BusyBox or a standard distro.

> **Golden rule:** *Collect evidence first, modify second. Change one variable at a time. Record before changing, verify after changing.*

## What's inside

The full guide lives in [`embedded_linux_troubleshooting_guide.md`](embedded_linux_troubleshooting_guide.md) and covers:

| Area | Highlights |
| --- | --- |
| **Methodology** | Evidence-first principles, baseline command reference, evidence handling rules |
| **One-shot evidence collector** | A drop-in `collect.sh` script that packages dmesg, proc, mounts, and config into a single tar.gz |
| **Boot** | Kernel panic decoding, U-Boot debugging, `init=/bin/sh` rescue, emergency access matrix |
| **Storage** | eMMC/NAND/UBIFS failure signatures, flash health & lifetime (`life_time`, `pre_eol_info`), write-amplification checklist |
| **Network** | Layered connectivity testing, `ss`/`nc` interpretation, tcpdump + Wireshark patterns, Wi-Fi specifics |
| **Processes** | Segfault/OOM log decoding, `D`-state diagnosis, memory-leak hunting, watchdog reset forensics |
| **Logging** | Failed log-download triage, log rotation (incl. BusyBox-only setups), pstore/ramoops, remote syslog |
| **Time & clocks** | RTC/NTP/timezone failures and their TLS side effects |
| **Performance** | `vmstat`/`top` interpretation tables with concrete thresholds |
| **Power & hardware** | Brown-out signatures, thermal margin, serial console as a hardware witness |
| **OTA / A-B updates** | Rollback loops, bootcount forensics, power-cut safety, slot-switch atomicity |
| **Device tree & drivers** | `-EPROBE_DEFER`, probe error decoding, live-DT inspection, pin-mux conflicts |
| **Field operations** | Report checklist, Mermaid decision flowchart, symptom quick-reference table, field toolkit |

## Quick start

1. Device misbehaving? Go straight to the **[Symptom Quick-Reference Table](embedded_linux_troubleshooting_guide.md#15-symptom-quick-reference-table)**.
2. Before touching anything, run the **[evidence collection script](embedded_linux_troubleshooting_guide.md#21-one-shot-evidence-collection-script)**.
3. New to the methodology? Read **[Section 1 — General Principles](embedded_linux_troubleshooting_guide.md#1-general-principles)** first (it's short).

## Who this is for

- R&D engineers debugging devices returned from the field
- Technical support / FAE staff who need a repeatable triage process
- Teams that keep losing evidence because someone rebooted the device first

## Contributing

Found a missing symptom, a better command, or a war story worth encoding into a rule? Contributions are welcome — see [CONTRIBUTING.md](CONTRIBUTING.md).

## License

[CC BY 4.0](https://creativecommons.org/licenses/by/4.0/) — free to use and adapt, attribution appreciated.
