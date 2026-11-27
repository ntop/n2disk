![ntop][n2disk_logo] ![ntop][ntop_logo]
# n2disk
This repository contains open source components and extensions for n2disk available at http://www.ntop.org/products/traffic-recording-replay/n2disk/

Here you can find:
* Add-on and extensions
* Code examples of applications that can interact with n2disk

Enjoy!

[n2disk_logo]: https://www.ntop.org/images/2025/06/n2disk-icon.webp

### Documentation

If you want to learn more about n2disk please visit the [User's Guide](https://www.ntop.org/guides/n2disk/).


### Building the User's Guide

The User's Guide is a [VitePress](https://vitepress.dev) project in `doc/` (Markdown pages, same
URLs as the old Sphinx guide: `/guides/n2disk/<page>.html`). Requires Node 18+ and `make`.

```
cd doc
make dist      # build the static site into doc/dist (runs npm install if needed)
make serve     # build, then serve it at http://localhost:4173/guides/n2disk/   (PORT=8080 make serve)
```

To publish, copy the result where the guide is served from, e.g.
`rsync -a --delete dist/ user@server:/var/www/ntop.org/guides/n2disk/`.
For live reload while editing a page use `npm run dev` (http://localhost:5173/guides/n2disk/).

### Details
For more information about n2disk, please visit [http://ntop.org](https://www.ntop.org/products/traffic-recording-replay/n2disk/)

[ntopng_logo]: https://camo.githubusercontent.com/0f789abcef232035c05e0d2e82afa3cc3be46485/687474703a2f2f7777772e6e746f702e6f72672f77702d636f6e74656e742f75706c6f6164732f323031312f30382f6e746f706e672d69636f6e2d313530783135302e706e67

[ntop_logo]: https://camo.githubusercontent.com/58e2a1ecfff62d8ecc9d74633bd1013f26e06cba/687474703a2f2f7777772e6e746f702e6f72672f77702d636f6e74656e742f75706c6f6164732f323031352f30352f6e746f702e706e67
