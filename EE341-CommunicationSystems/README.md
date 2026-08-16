# EE341 — Communication Systems I

**IIT Bombay | Autumn 2026 | Instructor: Prof. Prasanna Suresh Chaporkar**

Study material for EE341: a prerequisite refresher on signals, systems and the Fourier
transform, plus interactive explainers for the topics that are hardest to get from a
static page.

Everything is written in the notation the course actually uses — **ordinary frequency
`f` in Hz, not `ω` in rad/s** — to match the lectures and Madhow's textbook.

---

## Contents

| File | What it is |
|---|---|
| [`EE341_prereq.pdf`](EE341_prereq.pdf) | 18-page prerequisite refresher (signals & systems, Fourier, sampling, bandpass/lowpass-equivalent, random processes primer) with worked problems and a timed problem set |
| [`EE341_quiz1.pdf`](EE341_quiz1.pdf) | Quiz 1 revision sheet + 14-problem set with full solutions — AM schemes, spectra, modulator/demodulator circuits, superheterodyne receiver, bandpass/complex-envelope/Hilbert |
| [`fourier.pdf`](fourier.pdf) | Fourier problem set — 20 questions with full solutions (series, transforms, properties, convolution, Parseval, sampling) |
| [`ring_modulator.html`](ring_modulator.html) | Interactive explainer — how the ring modulator produces DSB-SC |
| [`overmodulation.html`](overmodulation.html) | Interactive explainer — over-modulation, the envelope ambiguity, and the RC envelope detector |
| [`convolution.html`](convolution.html) | Interactive explainer — convolution as flip, slide, overlap |
| [`conv_limits.html`](conv_limits.html) | Interactive helper — reading off the integration limits in a convolution |
| [`offset.html`](offset.html) | Interactive explainer — frequency offset and the rotating complex envelope |
| [`rect_conv_figure.png`](rect_conv_figure.png) | Figure — convolution of two rects, region by region |
| [`EE341_pushkar.pdf`](EE341_pushkar.pdf) | Scanned handwritten lecture notes (Pushkar) — modulation, analog communication |
| [`src/`](src/) | LaTeX source for the PDF |

The HTML files are **single-file and self-contained** — no build step, no CDN, no internet
needed. Download and open in any browser. All have a light/dark toggle.

**Or use them live in the browser (GitHub Pages):**
- 🔗 [Ring modulator explainer](https://vedika-doke.github.io/EE_Wisdom/EE341-CommunicationSystems/ring_modulator.html)
- 🔗 [Over-modulation explainer](https://vedika-doke.github.io/EE_Wisdom/EE341-CommunicationSystems/overmodulation.html)
- 🔗 [Convolution explainer](https://vedika-doke.github.io/EE_Wisdom/EE341-CommunicationSystems/convolution.html)
- 🔗 [Convolution limits helper](https://vedika-doke.github.io/EE_Wisdom/EE341-CommunicationSystems/conv_limits.html)
- 🔗 [Frequency offset / rotating complex envelope](https://vedika-doke.github.io/EE_Wisdom/EE341-CommunicationSystems/offset.html)

---

## `EE341_prereq.pdf`

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

## `ring_modulator.html`

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

## `overmodulation.html`

The core idea: **an envelope detector measures a magnitude, and magnitudes have no sign.**

- A μ slider — push past 1 and watch the envelope fold, with the destroyed regions marked
- Why the fold is irreversible: the sign becomes a 180° phase reversal the detector can't see
- The envelope ambiguity — three different messages that produce byte-for-byte identical envelopes
- DSB-SC framed as permanent over-modulation (which is why it needs coherent detection)
- The diode + RC envelope detector, with a time-constant slider showing both failure modes:
  ripple when `RC` is too small, diagonal clipping when it's too large

---

## `EE341_quiz1.pdf`

Condensed revision sheet + 14-problem set with full solutions, weighted towards spectra of
modulated signals, modulator/demodulator circuits, and bandpass/complex-envelope/Hilbert —
covers everything through the superheterodyne receiver.

## `fourier.pdf`

20 questions with full solutions on Fourier series, transforms, properties, convolution,
Parseval and sampling, and their use on modulated signals — includes a one-page reference
sheet of properties and standard pairs.

## `offset.html`

Interactive explainer for what a frequency offset between transmitter and receiver carriers
does to the complex envelope — visualizes the residual phasor rotation and its effect on
coherent detection.

---

## Reference texts

- U. Madhow, *Introduction to Communication Systems*, Cambridge 2014 — best match for the lowpass-equivalent material
- B. P. Lathi, *Modern Digital and Analog Communication Systems*, 3rd ed. — Ch. 3–4 for signals and Fourier
- Haykin & Moher, *Communication Systems*, 5th ed. — Ch. 2 for the bandpass/Hilbert treatment
