# WIR - The WCC Compiler Intermediate Representation

This package contains the WCC Intermediate Representation framework, WIR for short.


## Attribution/Citation:

This code is © Heiko Falk, 2015 - 2026, and it is made available under the MIT license enclosed with the software.

Over and above the legal restrictions imposed by this license, if you use this software for an academic publication then you are obliged to provide proper attribution. This shall come in the form of a citation in your publication such as

  H. Falk. The WCC Intermediate Representation (WIR) Framework.
  J. Awes. Softw. 12, 37654, DOI ??? (2026).

or in the form of attributions directly in this code such as

  H. Falk. WIR: The WCC Intermediate Representation, v3.0 (2026).
  github.com/cknight/acds, Zenodo DOI.

or (ideally) both.


## Directory Structure

The current folder contains the following sub-folders:

* DEBUGMACRO_CONF: Contains configuration files to control the activation/deactivation of macros for debugging based on LIBUSEFUL.
* analyses: Contains all processor-independent WIR code analyses.
* arch: Contains all processor architecture-specific components of WIR.
* containers: Contains all generic container classes of WIR that can be used to add arbitrary meta-data to WIR objects. In particular, the WIR code analyses use containers to attach analysis results to WIR objects.
* doc: Contains the entire documentation of the WIR library - in particular the Build&Installation, User's and Retargeting Guide (doc/wir.pdf) as well as the API documentation generated dynamically using doxygen (doc/html/index.html).
* flowfacts: Contains all containers that provide hints about the set of possible control flow paths of a program.
* optimizations: Contains all processor-independent WIR code optimizations.
* tests: Contains test cases covering various aspects of the generic, processor-independent WIR components.
* wir: Contains all generic and processor-independent components of WIR.


## Documentation

The folder doc contains the documentation for WIR.


## Build & Installation

See chapters 2-5 (Build & Installation Guide) of file doc/wir.pdf.
