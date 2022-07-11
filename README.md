# Rolling hash

Implementation of rolling hash signature and delta calculation by [spec](https://github.com/eqlabs/recruitment-exercises/blob/8e49a7b8cf9c415466876e852fbd862f74105ec6/rolling-hash.md "spec").

## Implementation details

Adler-32 is used as rolling hash algorithm, SHA1 is used as a strong hash to validate chunks equality. The original file is being split into 4Kb chunks, for each of them both hashes are calculated. The last chunk may be of a smaller size.

When the delta is generated a Adler-32 hash is being calculated in a rolling window. In case of a match to a signature SHA1 is used for verification. The current algorithm just checks if last incomplete chunk directly matches it's position in the original file which may not cover some situations. However on large files this shouldn't be critical.

## Building and running tests

Prerequisites:
- C++ compiler with C++17 support
- CMake v3.8+

Building project:
```
mkdir build
cd build
make
```
Running tests:
```
make run_tests
```

## Artifacts

Following the instructions above should result in having `rh_diff` and `rh_tests` executables in the `build/bin` folder.

`rh_tests` executes project unit tests.

`rh_diff` is a command-line utility for signature amd delta generation from files. Use `rh_diff --help` to see usage details.