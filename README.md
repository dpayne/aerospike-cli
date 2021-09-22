[![Actions Status](https://github.com/dpayne/aerospike-cli/workflows/MacOS/badge.svg)](https://github.com/dpayne/aerospike-cli/actions)
[![Actions Status](https://github.com/dpayne/aerospike-cli/workflows/Windows/badge.svg)](https://github.com/dpayne/aerospike-cli/actions)
[![Actions Status](https://github.com/dpayne/aerospike-cli/workflows/Ubuntu/badge.svg)](https://github.com/dpayne/aerospike-cli/actions)
[![Actions Status](https://github.com/dpayne/aerospike-cli/workflows/Style/badge.svg)](https://github.com/dpayne/aerospike-cli/actions)
[![Actions Status](https://github.com/dpayne/aerospike-cli/workflows/Install/badge.svg)](https://github.com/dpayne/aerospike-cli/actions)
[![codecov](https://codecov.io/gh/dpayne/aerospike-cli/branch/master/graph/badge.svg)](https://codecov.io/gh/dpayne/aerospike-cli)

### Build and run the main target

Use the following command to build and run the executable target.

```bash
cmake -B build
cmake --build build
./build/main/ascli --help
```

### Build and run test suite

Use the following commands from the project's root directory to run the test suite.

```bash
cmake -B build
cmake --build build --target ascliTest
CTEST_OUTPUT_ON_FAILURE=1 cmake --build build --target test

# or simply call the executable:
./build/test/ascliTest
```

To collect code coverage information, run CMake with the `-DENABLE_TEST_COVERAGE=1` option.

### Run clang-format

Use the following commands from the project's root directory to check and fix C++ and CMake source style.
This requires _clang-format_, _cmake-format_ and _pyyaml_ to be installed on the current system.

```bash
cmake -B build

# view changes
cmake --build build --target format

# apply changes
cmake --build build --target fix-format
```

See [Format.cmake](https://github.com/TheLartians/Format.cmake) for details.

### Build the documentation

The documentation is automatically built and [published](https://thelartians.github.io/ModernCppStarter) whenever a [GitHub Release](https://help.github.com/en/github/administering-a-repository/managing-releases-in-a-repository) is created.
To manually build documentation, call the following command.

```bash
cmake -B build
cmake --build build --target GenerateDocs
# view the docs
open build/doc/doxygen/html/index.html
```

To build the documentation locally, you will need Doxygen, jinja2 and Pygments on installed your system.

### Build everything at once

The project also includes an `all` directory that allows building all targets at the same time.
This is useful during development, as it exposes all subprojects to your IDE and avoids redundant builds of the library.

```bash
cmake -B build
cmake --build build

# run tests
./build/test/ascliTests
# format code
cmake --build build --target fix-format
# run main
./build/main/ascli --help
# build docs
cmake --build build --target GenerateDocs
```

#### Sanitizers

Sanitizers can be enabled by configuring CMake with `-DUSE_SANITIZER=<Address | Memory | MemoryWithOrigins | Undefined | Thread | Leak | 'Address;Undefined'>`.

#### Linter

Enable clang tidy linter with `-DENABLE_LINTER=ON`. Specific static analyzers can be enabled by setting `-DENABLE_LINTER=<clang-tidy | iwyu | cppcheck>`, or a combination of those in quotation marks, separated by semicolons.
By default, analyzers will automatically find configuration files such as `.clang-tidy`.
Additional arguments can be passed to the analyzers by setting the `CLANG_TIDY_ARGS`, `IWYU_ARGS` or `CPPCHECK_ARGS` variables.

#### Ccache

Ccache can be enabled by configuring with `-DUSE_CCACHE=<ON | OFF>`.

#### Static Build

Enable fully static build with `-DENABLE_STATIC=ON`. This will trigger a full glibc build from source.
