# Component Replacement Checklist (Alternative / Second-Source Evaluation)

A general-purpose checklist for evaluating whether a candidate part can replace an existing
component (EOL, NRND, shortage, cost-down, or second-source qualification).

Use `- [ ]` items as a review record: copy this file into your project repo, tick the boxes and
attach the evidence (datasheet page numbers, test reports) before releasing an ECN.

---

## Table of Contents

1. [Functional & Electrical Specification Match](#1-functional--electrical-specification-match)
2. [Package & Pin Compatibility](#2-package--pin-compatibility)
3. [Temperature Range, Reliability & Lifetime](#3-temperature-range-reliability--lifetime)
4. [Protocol, Firmware & Software Compatibility](#4-protocol-firmware--software-compatibility)
5. [Performance Margin & Degradation](#5-performance-margin--degradation)
6. [Power, Thermal & Decoupling](#6-power-thermal--decoupling)
7. [Supply Chain, Lifecycle & Compliance](#7-supply-chain-lifecycle--compliance)
8. [Verification, Validation & Change Management](#8-verification-validation--change-management)
9. [One-Page Review Record](#9-one-page-review-record)

---

## 1. Functional & Electrical Specification Match

- [ ] **Device type & architecture**: same category and generation (e.g. LPDDR1 vs. LPDDR2 are NOT interchangeable; eMMC vs. UFS vs. raw NAND are not interchangeable).
- [ ] **Density / capacity / organization**: total size, bus width, bank/die organization, addressing scheme (byte vs. 512 B sector addressing).
- [ ] **Supply voltage & tolerance**: VDD/VDDQ or VCC/VCCQ nominal range; single vs. dual-voltage I/O (1.8 V / 3.3 V).
- [ ] **I/O logic levels & thresholds**: VIL/VIH, VOH/VOL, input hysteresis, 5 V-tolerant inputs, open-drain vs. push-pull.
- [ ] **Speed grade & timing**: maximum clock/data rate, CAS latency or equivalent, and every AC parameter used by the host (tCK, tRCD, tRP, tRC, tRFC, tRAS, tWR, setup/hold…). Candidate must meet or exceed the original.
- [ ] **Refresh / retention / self-timing** requirements (e.g. DRAM 8 K refresh in 64 ms).
- [ ] **Drive strength, output impedance, slew-rate control**, programmable termination / ODT.
- [ ] **Analog accuracy** where applicable: reference voltage, gain error, offset, tempco, PSRR, ADC/DAC resolution.
- [ ] **ESD rating (HBM/CDM), latch-up immunity, leakage current**, absolute maximum ratings.

## 2. Package & Pin Compatibility

> This section decides whether a PCB re-spin is required.

- [ ] **Package family**: BGA / FBGA / VFBGA / QFN / QFP / SOIC / chip-scale, and leaded vs. leadless.
- [ ] **Ball / pin count** is identical.
- [ ] **Pitch** (0.4 / 0.5 / 0.65 / 0.8 mm…) is identical.
- [ ] **Body outline & height** (X × Y × Z) fit the mechanical envelope and any shielding/keep-out.
- [ ] **Pin-to-pin signal map verified ball by ball** (e.g. A1, A2, A3, A7, A8, A9 … K9) — never assume compatibility from pin count alone.
- [ ] **NC vs. RFU balls**: RFU balls must be left floating; confirm the previous design does not tie them to GND/power.
- [ ] **Special pins**: thermal pad, exposed pad, core-regulator pins (VDDI / VDDIM / VCAP) with their required external capacitor (value, and "must not be tied to supply or ground").
- [ ] **Pin-1 orientation, polarity mark, silkscreen / top-marking decode** (BGA codes often differ from the orderable part number).
- [ ] **Land pattern / stencil / solder-mask** reuse; reflow profile and MSL (moisture sensitivity level) compatible with the existing process.

## 3. Temperature Range, Reliability & Lifetime

- [ ] **Operating temperature grade**: Commercial 0 ~ 70 °C, Industrial −40 ~ 85 °C, Automotive A1 (−40 ~ 85 °C) / A2 (−40 ~ 105 °C) — must cover the product's worst-case ambient.
- [ ] **Storage temperature range**.
- [ ] **Endurance / retention**: NAND P/E cycles, data retention, EEPROM write cycles, relay/connector mating cycles.
- [ ] **Failure mechanism coverage**: bad-block management, ECC strength, wear leveling, power-loss protection / safe-update behavior.
- [ ] **Qualification**: AEC-Q100/Q101/Q200, PPAP, sulfur resistance, vibration/shock, humidity (THB/HAST), board-level reliability.
- [ ] **Expected field lifetime** versus the product's service life.

## 4. Protocol, Firmware & Software Compatibility

> The most frequently missed category — a "pin-compatible" part can still fail at boot.

- [ ] **Protocol version / standard**: the new device must be backward compatible with the host (e.g. an eMMC 5.1 device in an eMMC 5.0 host), and the host must not rely on features the new part does not implement (e.g. Command Queuing).
- [ ] **Command set / instruction set / register map** differences.
- [ ] **Mode-register / configuration-register encoding**: verify every coded field, e.g. DRAM EMRS driver-strength codes may be interpreted differently between vendors.
- [ ] **Initialization sequence**, power-up/reset timing, training/calibration steps, required delays.
- [ ] **Driver / bootloader / device-tree / HAL changes** identified and reviewed.
- [ ] **Memory-map items** for storage parts: boot partition size, RPMB size, user density and **sector count** (affects GPT/partition tables and full-disk images), hardware-reset enable bit in the extended CSD.
- [ ] **High-speed mode dependencies**: HS200/HS400 requires 1.8 V VCCQ and the DS (data strobe) pin; PCIe/USB/serdes gen speeds require host and board support.
- [ ] **Security features**: secure boot, write protection, RPMB keys, eFuse/OTP provisioning — key derivation may be tied to the part.

## 5. Performance Margin & Degradation

- [ ] **Throughput / bandwidth / IOPS** under the real workload (sequential & random, cache on & off, sustained vs. burst).
- [ ] **Latency** characteristics meet the system budget.
- [ ] **Timing margin** at worst-case voltage, temperature and process corner.
- [ ] **Derating / throttling** behavior at high temperature (thermal throttling, refresh doubling at > 85 °C).
- [ ] **Aging / end-of-life performance** (e.g. NAND write speed drop as the device fills).

## 6. Power, Thermal & Decoupling

- [ ] **Current consumption**: standby, idle, active, peak/burst, self-refresh, inrush — compare with the original part (e.g. DRAM IDD0/IDD4R/IDD4W/IDD5).
- [ ] **Power-supply margin**: the existing rail (PMIC/buck) must still have ≥ 20 % headroom.
- [ ] **Power-up / power-down sequencing** and reset timing requirements.
- [ ] **Decoupling network**: capacitor count, values, placement (VDDI/VDDIM capacitor value and placement constraints).
- [ ] **Thermal resistance / junction temperature** under worst-case load; heatsink or copper area unchanged.

## 7. Supply Chain, Lifecycle & Compliance

- [ ] **Lifecycle status**: Active / NRND / EOL; estimated last-time-buy date; vendor PCN subscription.
- [ ] **Longevity commitment** (e.g. industrial 10-year programs, automotive 15-year).
- [ ] **Lead time, MOQ, price delta**, and total cost of change (re-qualification, re-tooling).
- [ ] **Authorized distribution / counterfeit risk**, traceability, date code, tape-and-reel option.
- [ ] **Second source**: at least one more pin-compatible alternative identified.
- [ ] **Compliance**: RoHS, Halogen-free, REACH, TSCA, conflict minerals, PFAS where applicable.
- [ ] **Export control** (ECCN / EAR) and customer-specific banned-substance lists.
- [ ] **Industry certifications** required by the end product (medical, rail, automotive, aerospace).

## 8. Verification, Validation & Change Management

- [ ] **Datasheet side-by-side comparison table** produced and reviewed (parameter, original, candidate, verdict).
- [ ] **Sample bench test**: functional test on the actual board.
- [ ] **Signal integrity**: eye diagram / margin test for high-speed interfaces (DDR, eMMC HS400, PCIe, USB).
- [ ] **Environmental test**: −40 °C / +25 °C / +85 °C (or the product limits), thermal cycling, burn-in.
- [ ] **Robustness test**: power-cycle and sudden-power-loss tests for storage; long-duration read/write stress.
- [ ] **EMC / ESD re-test** if the part is on a fast or exposed interface.
- [ ] **Pilot build** (typically 30–50 units) and yield comparison.
- [ ] **Documentation**: BOM, schematic symbol, PCB footprint, silkscreen, label, assembly drawing updated.
- [ ] **ECN/ECO** raised, verification report archived, customer notification where required.

---

## 9. One-Page Review Record

| Item | Original Part | Candidate Part | Verdict |
|---|---|---|---|
| Function / density / organization | | | |
| Supply voltage & I/O level | | | |
| Speed grade & key timing | | | |
| Package / pitch / body size | | | |
| Pin-to-pin ball map | | | |
| NC / RFU / special-pin handling | | | |
| Temperature grade | | | |
| Protocol / firmware compatibility | | | |
| Boot / partition / sector count | | | |
| Performance (throughput, IOPS) | | | |
| Power & thermal margin | | | |
| Lifecycle, lead time, compliance | | | |
| Test result (bench / EQT / pilot) | | | |

**Decision:** ☐ Drop-in replacement (no design change) ☐ Replace with SW/FW change ☐ Replace with PCB change ☐ Not suitable

| Role | Name | Date |
|---|---|---|
| Hardware | | |
| Firmware / Software | | |
| Sourcing | | |
| Quality | | |
| Approved by | | |

---

### Notes

- Always verify against the **latest** datasheet revision from the manufacturer's website; distributor
  summaries frequently contain wrong package dimensions and pin maps.
- "Pin-compatible" claims in marketing material must be confirmed ball by ball against both datasheets.
- Re-check this checklist whenever the silicon revision, die revision, or firmware version of the
  replacement part changes.
