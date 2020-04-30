osbuild
=======
[![License](https://img.shields.io/badge/license-Apache--2.0%20OR%20MIT-blue.svg)](https://opensource.org/licenses/Apache-2.0)
[![CI for Linux, Windows, macOS](https://github.com/offscale/osbuild/workflows/CI%20for%20Linux,%20Windows,%20macOS/badge.svg)](https://github.com/offscale/osbuild/actions)
[![CI for FreeBSD](https://api.cirrus-ci.com/github/offscale/osbuild.svg)](https://cirrus-ci.com/github/offscale/osbuild)

Installs compiler and build tools for your platform. E.g., on Ubuntu it will run `apt-get install -y build-essential.`

Supports:

  - [ ] Linux
    - [x] Debian (and distros based upon, e.g., Ubuntu)
  - [x] macOS
  - [x] Windows
  - [ ] Solaris/OpenSolaris/illumos
    - [x] OpenIndiana
  - [ ] BSD
    - [x] FreeBSD (and distros based upon, e.g., DragonflyBSD)
    - [x] NetBSD
    - [ ] OpenBSD

## Docker

`Dockerfile`s are provided for convenience, try them out, e.g., by running:

    docker build . -f Dockerfile.ubuntu --tag osbuild
    docker run osbuild

---

## License

Licensed under either of

- Apache License, Version 2.0 ([LICENSE-APACHE](LICENSE-APACHE) or <https://www.apache.org/licenses/LICENSE-2.0>)
- MIT license ([LICENSE-MIT](LICENSE-MIT) or <https://opensource.org/licenses/MIT>)

at your option.

### Contribution

Unless you explicitly state otherwise, any contribution intentionally submitted
for inclusion in the work by you, as defined in the Apache-2.0 license, shall be
dual licensed as above, without any additional terms or conditions.
