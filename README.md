# timesheeting
`timesheeting` is a timesheet utility project.

The project is just starting.

## Project timeline
* *10Feb2024*: `PRJ1-SAD1-v1.0` released.
* *22Jan2024*: `PRJ1-SPE1-v1.0` released.
* *20Jan2024*: Project launch.

## Documentation summary
* `PRJ1-SPE1-v1.0` timesheeting specification document
* `PRJ1-SAD1-v1.0` timesheeting system architecture document
* ![Release notes](doc/RELEASE_NOTES.md)
* [Doxygen](https://thomashoullier.github.io/timesheeting/index.html)

## Build instructions
### Software
#### Example main
Change to the `build` directory, run cmake and then build the target.

```{shell}
cd build/
cmake ..
make timesheeting_main
```

The result is the executable `build/timesheeting_main`.

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

#### Doxygen pages
Build the doxygen pages, while at the project's root,

```{shell}
doxygen Doxyfile
```

The documentation is produced as html pages to the folder
`docs/html`. These pages are also deployed to github using
a github action.

## License
The software is distributed under the MIT license.
The documentation is distributed under the CC-BY 4.0 International license.
