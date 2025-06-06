# timesheeting
[![Build Status](https://ci.houllier.net/api/badges/thomashoullier/timesheeting/status.svg)](https://ci.houllier.net/thomashoullier/timesheeting)

`timesheeting` is a timesheet utility project.

The program is mostly usable, although the UI is unpolished.
It is not finished, see the [TODOs.org](TODOs.org) file for
checking progress.

## Project timeline
* *28Nov2024*: `PRJ1-SAD1-v1.1` released.
* *24Nov2024*: Usage begins (alpha).
* *16Nov2024*: `PRJ1-SPE1-v1.1` released.
* *02Nov2024*: `PRJ1-IRS1-v1.1` released.
* *26Oct2024*: `PRJ1-IRS1-v1.0` released.
* *10Feb2024*: `PRJ1-SAD1-v1.0` released.
* *22Jan2024*: `PRJ1-SPE1-v1.0` released.
* *20Jan2024*: Project launch.

## Documentation summary
* `PRJ1-SPE1-v1.1` timesheeting specification document
* `PRJ1-IRS1-v1.1` external timesheet format
* `PRJ1-SAD1-v1.1` timesheeting system architecture document
* ![Release notes](doc/RELEASE_NOTES.md)
* [Doxygen](https://thomashoullier.github.io/timesheeting/doxygen/index.html)
* [Test coverage](https://thomashoullier.github.io/timesheeting/lcov/index.html)

## Configuration file
The program is configured using a single TOML file.
A reference configuration is included: [timesheeting.toml](timesheeting.toml).

## Build instructions
### Software
Change to the `build` directory, run cmake, build and install the target.

```{shell}
cd build/
cmake ..
sudo make install
```

The executable `timesheeting` should be installed in `/bin/`.
A default configuration file is installed at `/etc/timesheeting.toml`.

### Unit tests
The unit tests may be run after enabling the corresponding cmake target.

```{shell}
cd build/
cmake -Dbuild_tests=ON ..
make timesheeting_tests
./timesheeting_tests
```

The test coverage report is generate using,

```{shell}
cd build/
cmake -Dbuild_coverage=ON ..
make timesheeting_coverage_report
```

An LCOV html report is generated at `build/timesheeting_coverage_report/`.

### Documentation
#### PRJ1-SPE1 timesheeting specification document
This document source is written in LaTeX. It is built with
texlive. A full texlive installation is recommended.

The PDF can be built by changing to the `spec` directory and
running `latexmk`

```{shell}
cd ./doc/spec/
latexmk -pdf spec.tex
```

The result is `spec.pdf`.

#### PRJ1-SAD1 timesheeting system architecture document
The PDF is built with,

```{shell}
cd ./doc/sad/
latexmk -pdf sad.tex
```

The result is `sad.pdf`.

#### PRJ1-IRS1 external timesheet format
The PDF is build with,

```{shell}
cd ./doc/irs/
latexmk -pdf irs.tex
```

The result is `irs.pdf`

#### Doxygen pages
Build the doxygen pages, while at the project's root,

```{shell}
mkdir build_doxygen
doxygen Doxyfile
```

The documentation is produced as html pages to the folder
`build_doxygen/html`. These pages are also deployed to github pages by
the CI.

## License
The software is distributed under the MIT license.
The documentation is distributed under the CC-BY 4.0 International license.
