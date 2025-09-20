# refresh-projects
Repository to keep my C++ and Rust algorithm practice in sync.

## Layout
- `refresh-cpp/`: C++20 implementations tested with GoogleTest.
- `refresh-rust/`: Rust workspace that mirrors the same exercises, with Criterion benches.
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
