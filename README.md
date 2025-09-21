# refresh-projects
Repository to keep my C++ and Rust DSA practice in sync.

## Layout
- `refresh-cpp/`:  Workspace implement DSA in C++20 with GoogleTest.
- `refresh-rust/`: Rust workspace that mirrors the same exercises.
- `notes/`: Planning scratchpad (see `notes/algorithms.md`).

## C++ quick start
Requirements: CMake 3.16+, a C++20 compiler, one-time internet access for googletest.
```bash
cd refresh-cpp
cmake -S . -B build
cmake --build build
./build/algorithms_test
```

## Rust quick start
Requirements: Rust toolchain with Edition 2024 support (`rustup update` is usually enough).
```bash
cd refresh-rust
cargo build
cargo test -p algorithms
```

That’s it—rest of the plan lives in `notes/algorithms.md`.
