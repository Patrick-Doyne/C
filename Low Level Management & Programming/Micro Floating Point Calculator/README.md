# MicroFP – Compressed Binary Scientific Format in C (9-bit floating point)

## Program Overview & Purpose

- Implements a 9-bit floating point format (`microfp_s`) for binary scientific computation in C  
- Simulates IEEE-like behavior (INF, NaN, signed magnitude, normalization) using bitwise operations and fixed-width structs  
- Designed as part of GMU coursework in systems and embedded computation, demonstrating manual encoding/decoding of low-bit values

## Language & Runtime

- **Language**: C (C99 standard)  
- **Compiler**: GCC, used either standalone or via MUAN interpreter  
- **Runtime Tool**: MUAN (Micro-Ubiquitous Accounting Notary) — a custom simulator for in-memory evaluation  
- **Headers Provided**: `common_structs.h`, `common_definitions.h`, `common_functions.h`, `microfp.h`  
- **Entry Point**: `microfp.c` — the sole file edited and submitted  
- **Execution Mode**: Run via `./muan` shell or MUAN scripts invoking conversion/arithmetic routines  
- **Constraints**:  
  - No use of standard floating-point types (`float`, `double`)  
  - No dynamic memory allocation — all operations on stack-passed structs (`Number_s`)  
  - Strict bitwise and masking logic to simulate IEEE-like behavior  
- **Testing & I/O**: Driven entirely by MUAN, which calls functions like `toMicroFP`, `addMicroFP`, etc. and validates output against binary-encoded scripts

## File Details

- `MicroFP.txt`: Full implementation of micro floating-point format, including conversion routines, arithmetic, and bit manipulation  
- Core struct type: `microfp_s` (9-bit), emulated using standard C integer types

## Features Implemented

- ✅ Classification functions: `isInfinity`, `isNaN`, `isNegative`, `isNormalized`, `isZero`  
- ✅ Binary scientific formatting: shift-left/right routines with decimal control (`shiftLeft`, `shiftRight`)  
- ✅ Value conversion: `toMicroFP()` converts a `Number_s` struct to 9-bit float; `toNumber()` does the reverse  
- ✅ Arithmetic operations:  
  - Multiplication (`mulMicroFP`)  
  - Addition (`addMicroFP`)  
  - Subtraction via negation + addition (`subMicroFP`, `negMicroFP`)  
- ✅ Special-case rules:  
  - NaN propagation, signed INF, and +0 / -0 edge cases handled in both `addSubSpecialArithmetic` and `multSpecialArithmetic`  
  - Denormalized ranges computed using manual exponent shifting

## Key Concepts

- 📦 **Signed Magnitude Representation**: Bit 8 encodes sign; bits 5–7 store exponent (exp); bits 0–4 store fraction  
- ⚙️ **Exponent Logic**: Stored exp is biased; raw exponent `E` computed via `getE()`  
- 🧠 **Normalization/Denormalization**: Shift logic used to represent binary scientific values in constrained space  
- 🔍 **Special Case Handling**: NaN, INF, and sign mismatches are pre-checked via dedicated logic blocks  
- 🔁 **Decimal Preservation**: Separate whole and fraction bits managed across shifting operations

## Functions & Their Roles

| Function Name              | Purpose                                                      |
|---------------------------|--------------------------------------------------------------|
| `isInfinity`, `isNaN`     | Classify special float states based on exponent/fraction bits |
| `shiftLeft`, `shiftRight` | Preserve decimal position while adjusting magnitude          |
| `toMicroFP`, `toNumber`   | Convert between struct and compressed float representation    |
| `addMicroFP`, `mulMicroFP`| Perform arithmetic with built-in edge case logic              |
| `setBits`                 | Encodes final float using exp and fraction slices            |
| `equalizeE`               | Align exponent before arithmetic                             |

## Usage Notes

- 🔄 Functions operate on `Number_s` structs directly—do not allocate or free manually  
- ❌ NULL pointer checks are explicit; no runtime exceptions  
- 🎯 Bitwise logic used exclusively for encoding; no standard float/double ops permitted  
- ⚠️ Edge cases fully handled: zero, negative zero, overflow (INF), invalid (NaN), and normalization boundaries

## Author & Credits

- Student Implementation: **Patrick**  
- Starter Code Author: Prof. Kevin Andrea (GMU)  
- Course: CS 367 – Computer Systems and Programming (Fall 2024)  
- Full credit preserved as per embedded copyright
