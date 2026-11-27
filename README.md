![n2disk_logo]
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
