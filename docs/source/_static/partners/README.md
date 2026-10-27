# Partner logos

This folder holds the **official** logos of the institutions behind FES.
They are supplied by those institutions; do not redraw, recolour or
approximate them.

Expected files (SVG preferred, PNG at least 160 px high otherwise):

| File                 | Use                                   |
|----------------------|---------------------------------------|
| `cnes.svg`           | CNES logo for light backgrounds       |
| `cnes-dark.svg`      | CNES logo for dark backgrounds        |
| `aviso.svg`          | AVISO logo for light backgrounds      |
| `aviso-dark.svg`     | AVISO logo for dark backgrounds       |

Other partners (for example LEGOS) follow the same `name.svg` /
`name-dark.svg` pattern.

Once the files are here, list them in `docs/source/conf.py`:

```python
html_context = {
    'fes_partners': [
        {
            'name': 'CNES',
            'url': 'https://cnes.fr',
            'image_light': 'cnes.svg',
            'image_dark': 'cnes-dark.svg',
        },
        {
            'name': 'AVISO',
            'url': 'https://www.aviso.altimetry.fr',
            'image_light': 'aviso.svg',
            'image_dark': 'aviso-dark.svg',
        },
    ],
}
```

They are rendered in the page footer by `docs/source/_templates/partners.html`,
40 px high. An empty list renders nothing.
