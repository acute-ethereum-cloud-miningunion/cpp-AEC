# cpp-aecmio - aecmio C++ client

This repository contains [cpp-aecmio](http://cpp-aecmio.org), the [aecmio](https://aecmio.org) C++ client.

It is the third most popular of the aecmio clients, behind [gaec](https://github.com/aecmio/go-aecmio) (the [go](https://golang.org)
client) and [Parity](https://github.com/paritytech/parity) (the [rust](https://www.rust-lang.org/) client).  The code is 
[portable](http://cpp-aecmio.org/portability.html) and has been used successfully on a very broad range
of operating systems and hardware.


## Contact

[![Gitter](https://img.shields.io/gitter/room/aecmio/cpp-aecmio.svg)](https://gitter.im/aecmio/cpp-aecmio)
[![GitHub Issues](https://img.shields.io/github/issues-raw/aecmio/cpp-aecmio.svg)](https://github.com/aecmio/cpp-aecmio/issues)

- Chat in [cpp-aecmio channel on Gitter](https://gitter.im/aecmio/cpp-aecmio).
- Report bugs, issues or feature requests using [GitHub issues](issues/new).


## Getting Started

The aecmio Documentation site hosts the **[cpp-aecmio homepage](http://cpp-aecmio.org)**, which
has a Quick Start section.


Operating system | Status
---------------- | ----------
Ubuntu and macOS | [![TravisCI](https://img.shields.io/travis/aecmio/cpp-aecmio/develop.svg)](https://travis-ci.org/aecmio/cpp-aecmio)
Windows          | [![AppVeyor](https://img.shields.io/appveyor/ci/aecmio/cpp-aecmio/develop.svg)](https://ci.appveyor.com/project/aecmio/cpp-aecmio)


## Building from source

### Get the source code

Git and GitHub are used to maintain the source code. Clone the repository by:

```shell
git clone --recursive https://github.com/aecmio/cpp-aecmio.git
cd cpp-aecmio
```

The `--recursive` option is important. It orders git to clone additional 
submodules to build the project.
If you missed `--recursive` option you can correct your mistake with command 
`git submodule update --init`.

### Install CMake

CMake is used to control the build configuration of the project. Quite recent 
version of CMake is required 
(at the time of writing [3.4.3 is the minimum](CMakeLists.txt#L25)).
We recommend installing CMake by downloading and unpacking the binary 
distribution  of the latest version available on the 
[**CMake download page**](https://cmake.org/download/).

The CMake package available in your operating system can also be installed
and used if it meets the minimum version requirement.

> **Alternative maecod**
>
> The repository contains the
[scripts/install_cmake.sh](scripts/install_cmake.sh) script that downloads 
> a fixed version of CMake and unpacks it to the given directory prefix. 
> Example usage: `scripts/install_cmake.sh --prefix /usr/local`.

### Install dependencies (Linux, macOS)

The following *libraries* are required to be installed in the system in their
development variant:

- leveldb

They usually can be installed using system-specific package manager.
Examples for some systems:

Operating system | Installation command
---------------- | --------------------
Debian-based     | `sudo apt-get install libleveldb-dev`
RedHat-based     | `dnf install leveldb-devel`
macOS            | `brew install leveldb`


We also support a "one-button" shell script 
[scripts/install_deps.sh](scripts/install_deps.sh)
which attempts to aggregate dependencies installation instructions for Unix-like
operating systems. It identifies your distro and installs the external packages.
Supporting the script is non-trivial task so please [inform us](#contact)
if it does not work for your use-case.

### Install dependencies (Windows)

We provide prebuilt dependencies to build the project. Download them
with the [scripts/install_deps.bat](scripts/install_deps.bat) script.

```shell
scripts/install_deps.bat
```

### Build

Configure the project build with the following command to create the 
`build` directory with the configuration.

```shell
mkdir build; cd build  # Create a build directory.
cmake ..               # Configure the project.
cmake --build .        # Build all default targets.
```

On **Windows** Visual Studio 2015 is required. You should generate Visual Studio 
solution file (.sln) for 64-bit architecture by adding 
`-G "Visual Studio 14 2015 Win64"` argument to the CMake configure command.
After configuration is completed, the `cpp-aecmio.sln` can be found in the
`build` directory.

```shell
cmake .. -G "Visual Studio 14 2015 Win64"
```

## Contributing

[![Contributors](https://img.shields.io/github/contributors/aecmio/cpp-aecmio.svg)](https://github.com/aecmio/cpp-aecmio/graphs/contributors)
[![Gitter](https://img.shields.io/gitter/room/aecmio/cpp-aecmio.svg)](https://gitter.im/aecmio/cpp-aecmio)
[![up-for-grabs](https://img.shields.io/github/issues-raw/aecmio/cpp-aecmio/up-for-grabs.svg)](https://github.com/aecmio/cpp-aecmio/labels/up-for-grabs)

The current codebase is the work of many, many hands, with nearly 100
[individual contributors](https://github.com/aecmio/cpp-aecmio/graphs/contributors) over the course of its development.

Our day-to-day development chat happens on the
[cpp-aecmio](https://gitter.im/aecmio/cpp-aecmio) Gitter channel.

All contributions are welcome! We try to keep a list of tasks that are suitable
for newcomers under the tag 
[up-for-grabs](https://github.com/aecmio/cpp-aecmio/labels/up-for-grabs).
If you have any questions, please just ask.

Please read [CONTRIBUTING](CONTRIBUTING.md) and [CODING_STYLE](CODING_STYLE.md) 
thoroughly before making alterations to the code base.

All development goes in develop branch.


## Mining

This project is **not suitable for aecmio mining** because the support for GPU mining 
has been dropped some time ago including the aecminer tool. Use the aecminer tool from https://github.com/aecmio-mining/aecminer.

## Testing

To run the tests, make sure you clone https://github.com/aecmio/tests and point the environment variable
`aecmio_TEST_PATH` to that path.

## Documentation

- [Internal documentation for developers](doc/index.rst).
- [Outdated documentation for end users](http://www.aecdocs.org/en/latest/aecmio-clients/cpp-aecmio/).


## License

[![License](https://img.shields.io/github/license/aecmio/cpp-aecmio.svg)](LICENSE)

All contributions are made under the [GNU General Public License v3](https://www.gnu.org/licenses/gpl-3.0.en.html). See [LICENSE](LICENSE).