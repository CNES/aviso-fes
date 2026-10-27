# FES brand guide

A short guide to the FES visual identity: the logo, its colours, its type and
how to use them. The assets live in `docs/source/_static/`; this folder only
holds this guide and the GitHub social preview.

<p>
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="../source/_static/logo/fes-horizontal-dark.svg">
    <img src="../source/_static/logo/fes-horizontal.svg" alt="FES" height="72">
  </picture>
</p>

## The idea

FES computes the tide as **a sum of harmonic constituents**, interpolated on
**finite-element meshes** (FES stands for *Finite Element Solution*). The mark
says both:

- the triangle is one element of the mesh. It points down: pointing up, with a
  wavy water line, it reads as a mountain;
- the water inside it is crossed by three constituents (K1, M2, S2 shaped),
  cut out of the water;
- the free surface of the water is the sum of those three waves.

Personality: rigorous, precise, calm, oceanographic. An instrument, not a
consumer product.

## Names

- **FES** is the brand: the tidal solution and the C++ library.
- **PyFES** is the Python package. In running text write "PyFES"; the
  wordmark spells it "pyFES", with a muted "py" prefix.

## Files

| File (`docs/source/_static/`) | Use |
|-------------------------------|-----|
| `logo/fes-mark.svg`           | Symbol, light backgrounds |
| `logo/fes-mark-dark.svg`      | Symbol, dark backgrounds |
| `logo/fes-mark-mono.svg`      | Symbol, one colour (`currentColor`): print, engraving, stamps |
| `logo/fes-mark-auto.svg`      | Symbol that follows `prefers-color-scheme`, for hosts that accept a single logo file (Doxygen) |
| `logo/fes-horizontal.svg`     | Symbol + "FES" wordmark, light backgrounds |
| `logo/fes-horizontal-dark.svg`| Symbol + "FES" wordmark, dark backgrounds |
| `favicon.svg`                 | Simplified symbol for 16–31 px, follows `prefers-color-scheme` |
| `css/fes.css`                 | Palette and theme overrides for Sphinx |
| `css/fes-doxygen.css`         | Link colours and fonts for Doxygen |
| `partners/`                   | Official partner logos (see its README) |

`docs/brand/social-preview.svg` is the 1280×640 GitHub social preview. Export
it to PNG and upload it under *Settings › General › Social preview*.

All SVG files are written by hand: a 64×64 `viewBox` for the symbol, short
paths, no editor metadata, no fonts. The wordmark letters are monoline strokes
(5 units wide), not text.

## Clear space and minimum sizes

Let **X** be a quarter of the triangle's height (about 12 units in the 64-unit
box, 12 px when the symbol is 64 px).

- Keep at least **X** of free space around the symbol and around the
  horizontal logo. No text, edge or other logo inside it.
- In the horizontal logo, the gap between symbol and wordmark is fixed; do not
  re-space it.

| Asset            | Screen minimum                    | Print minimum |
|------------------|-----------------------------------|---------------|
| Symbol           | 32 px; use `favicon.svg` below    | 8 mm          |
| Favicon          | 16 px                             | —             |
| Horizontal logo  | 32 px high                        | 10 mm high    |

Below 32 px the cut-out constituents turn into stripes, which is why the
favicon drops them and uses a heavier edge.

## Colour

| Role                         | Light     | Dark      | Used for |
|------------------------------|-----------|-----------|----------|
| Background                   | `#FFFFFF` | `#0A1626` | Page |
| Text                         | `#1A2433` | `#E6EDF3` | Body text |
| Muted text                   | `#4A5869` | `#A9B8C7` | Captions, taglines, "py" prefix |
| Abyss (primary, brand)       | `#0B2545` | `#9FC3E7` | Triangle edge, wordmark, headings |
| Tide (accent, links)         | `#0C747E` | `#4FD1C5` | Water, links |
| Tide hover                   | `#0B5F67` | `#8BE3DA` | Hovered and visited links, inline code |
| Estran (surface)             | `#EEF4F6` | `#132235` | Admonitions, table headers |
| Spring tide (signal)         | `#F2A541` | `#F2A541` | Charts and illustrations only |

The Tide accent was darkened from `#0E7C86` to `#0C747E` so that links stay
above 4.5:1 on the Estran surface.

### Contrast (WCAG 2.x)

Every text and link colour meets AA (4.5:1) on both backgrounds of its mode.

| Light       | on `#FFFFFF` | on Estran `#EEF4F6` |
|-------------|-------------:|--------------------:|
| Text        | 15.62        | 14.07 |
| Muted       | 7.26         | 6.54 |
| Abyss       | 15.39        | 13.85 |
| Tide (link) | 5.50         | 4.96 |
| Tide hover  | 7.38         | 6.64 |

| Dark        | on `#0A1626` | on Estran `#132235` |
|-------------|-------------:|--------------------:|
| Text        | 15.38        | 13.59 |
| Muted       | 8.97         | 7.93 |
| Abyss       | 9.89         | 8.74 |
| Tide (link) | 9.74         | 8.61 |
| Tide hover  | 12.20        | 10.78 |

Spring tide is 2.05:1 on white: never use it for text. In charts it marks one
thing (for example the sum curve), and that meaning is also given by a label,
a line width or a legend: colour never carries information alone.

## Type

No web fonts are downloaded.

- Text and headings: `system-ui, -apple-system, "Segoe UI", Roboto,
  "Helvetica Neue", Arial, sans-serif`.
- Headings: same family, weight 600, letter spacing −0.01em.
- Code: `ui-monospace, "SF Mono", Menlo, Consolas, "Liberation Mono",
  monospace`.
- Mathematics keeps the MathJax rendering; do not restyle it.

## Do not

- Rotate the triangle point up, or rotate, skew or stretch the logo.
- Recolour it outside the palette, or swap the light and dark variants on
  the wrong background.
- Add amber, gradients, shadows, outlines or effects to the mark.
- Re-type the wordmark in a font, or change its letter spacing.
- Use the detailed symbol below 32 px (use `favicon.svg`).
- Place the logo on a busy image without a plain background plate.
- Merge the FES logo with the CNES or AVISO logos into a single lockup, or
  redraw, recolour or imitate them. Partner logos are the official files,
  shown side by side, with their own clear space (see
  `docs/source/_static/partners/README.md`).
