# Algorithms and Data Structures - Exercies

This is a repository of a my own solutions for common
problems found in this field. All solutions are written in C99. Most
of the problems here are homework for my DS class in university.
All of the code was written by a human (that is me) - no AI was used.

## Building and Running

Each problem lives in a separate folder at the root of the repository:
`/problem-name/main.c`.

Configure and build all problems:

```sh
cmake --preset debug
cmake --build --preset debug
```

Build a single problem only:

```sh
cmake --build --preset debug --target problem-one
```

### Presets

| Preset    | What it does                                        |
| --------- | --------------------------------------------------- |
| `debug`   | AddressSanitizer + UBSan (out-of-bounds, leaks, UB) |
| `msan`    | MemorySanitizer + UBSan (uninitialized reads)       |
| `release` | Optimized build, no sanitizers                      |

Swap `debug` for another preset name in the commands above. Binaries end up
in `build/<preset>/`.
