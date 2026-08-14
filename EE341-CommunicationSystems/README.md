# EE341 — Communication Systems I

**IIT Bombay | Autumn 2026 | Instructor: Prof. Kumar Appaiah**

Study material for EE341: a prerequisite refresher on signals, systems and the Fourier
transform, plus interactive explainers for the topics that are hardest to get from a
static page.

Everything is written in the notation the course actually uses — **ordinary frequency
`f` in Hz, not `ω` in rad/s** — to match the lectures and Madhow's textbook.

---

## Contents

| File | What it is |
|---|---|
| [`EE341_Prereq_Revamp.pdf`](EE341_Prereq_Revamp.pdf) | 18-page prerequisite refresher (signals & systems, Fourier, sampling, bandpass/lowpass-equivalent, random processes primer) with worked problems and a timed problem set |
| [`ring_modulator_explained.html`](ring_modulator_explained.html) | Interactive explainer — how the ring modulator produces DSB-SC |
| [`overmodulation_explained.html`](overmodulation_explained.html) | Interactive explainer — over-modulation, the envelope ambiguity, and the RC envelope detector |
| [`src/`](src/) | LaTeX source for the PDF |

The HTML files are **single-file and self-contained** — no build step, no CDN, no internet
needed. Download and open in any browser. Both have a light/dark toggle.

---

## `EE341_Prereq_Revamp.pdf`

Ordered by what EE341 actually uses, not by what a full signals course would cover.

**Part A — Signals and systems.** Energy vs. power signals (and why the distinction decides
whether you use `|X(f)|²` or a PSD), the delta function, LTI systems, convolution, and the
eigenfunction argument that explains why Fourier analysis exists at all.

**Part B — The Fourier transform.** Fourier series → transform, every property with a proof
or an intuition, a standard-pairs table, Parseval, and the modulation property that the
entire AM chapter is built on.

**Part C — Filtering, sampling, bandpass signals.** Distortionless transmission, ideal
filters, the sampling theorem and aliasing, then the analytic signal, Hilbert transform and
complex envelope — the material the course jumps into almost immediately.

**Part D — Random processes primer.** Autocorrelation, PSD, Wiener–Khinchin, `S_Y = |H|²S_X`,
and white noise. Enough to survive the noise chapter.

Plus a timed problem set with full solutions, a one-page recall sheet, and a list of the
five mistakes that cost the most marks.

---

## `ring_modulator_explained.html`

The core idea: **the diodes are switches, and the carrier throws them.** The circuit is a
polarity flipper, nothing more.

- Animated circuit diagram — watch the conducting diode pair alternate each half-cycle
- The equivalent double-pole switch, with the diodes stripped away
- Three synced waveforms (message, switching function, product) with a scrubable time cursor
- A harmonics slider that builds the square wave from its Fourier series
- Spectrum view with a bandpass-filter toggle, showing the copies at 3f<sub>c</sub>, 5f<sub>c</sub>, 7f<sub>c</sub> being discarded
- Why it's *suppressed*-carrier: `c(t)` has no DC term, and the circuit is double-balanced

Covers the `4/π` gain factor that gets dropped, and the `f_c > W` filter condition.

---

## `overmodulation_explained.html`

The core idea: **an envelope detector measures a magnitude, and magnitudes have no sign.**

- A μ slider — push past 1 and watch the envelope fold, with the destroyed regions marked
- Why the fold is irreversible: the sign becomes a 180° phase reversal the detector can't see
- The envelope ambiguity — three different messages that produce byte-for-byte identical envelopes
- DSB-SC framed as permanent over-modulation (which is why it needs coherent detection)
- The diode + RC envelope detector, with a time-constant slider showing both failure modes:
  ripple when `RC` is too small, diagonal clipping when it's too large

---

## Reference texts

- U. Madhow, *Introduction to Communication Systems*, Cambridge 2014 — best match for the lowpass-equivalent material
- B. P. Lathi, *Modern Digital and Analog Communication Systems*, 3rd ed. — Ch. 3–4 for signals and Fourier
- Haykin & Moher, *Communication Systems*, 5th ed. — Ch. 2 for the bandpass/Hilbert treatment
