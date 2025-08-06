# MicroFP – 9-Bit Floating-Point Format in C

## Program Overview & Purpose

Implements a custom 9-bit floating-point type (`microfp_s`) in C to model IEEE-like behavior (INF, NaN, signed-magnitude, normalization) using only bitwise operations and fixed-width structs. Developed for GMU CS 367 to demonstrate manual encoding/decoding of low-bit scientific values without standard floating-point types.

---

## Language and Tools

- Language: C (C99 standard)  
- Compiler: GCC (standalone or via MUAN shell)  
- Simulator: MUAN (Micro-Ubiquitous Accounting Notary) for in-memory evaluation  
- Headers Provided:  
  - `common_structs.h`  
  - `common_definitions.h`  
  - `common_functions.h`  
  - `microfp.h`  
- Entry Point: `microfp.c` (only modified file)  

---

## Constraints

- No use of `float` or `double` types  
- No dynamic memory allocation—operates solely on stack-passed structs (`Number_s`)  
- All encoding/decoding via bitwise masks and shifts  

---

## File Structure

- `MicroFP.txt`  
  Full implementation of the 9-bit format, including conversion routines, arithmetic, and bit manipulation.  

- Core Data Type:  
  - `microfp_s` (9 bits emulated within standard C integer types)  

---

## Features Implemented

- Classification  
  - `isInfinity`, `isNaN`, `isNegative`, `isNormalized`, `isZero`  

- Bit Extraction & Composition  
  - `getE` (raw exponent)  
  - `setBits` (assemble sign, exponent, fraction slices)  

- Binary Scientific Formatting  
  - `shiftLeft`, `shiftRight` with fractional-bit preservation  

- Value Conversion  
  - `toMicroFP(Number_s)` → 9-bit compressed form  
  - `toNumber(microfp_s)` → full-precision struct  

- Arithmetic Operations  
  - `addMicroFP`, `subMicroFP` (via `negMicroFP` + `addMicroFP`)  
  - `mulMicroFP`  

- Special-Case Handling  
  - `addSubSpecialArithmetic` (NaN propagation, signed INF, +0/–0)  
  - `multSpecialArithmetic` (overflow, denorm ranges)  

---

## Key Concepts

- Signed-Magnitude Representation  
  - Bit 8: sign  
  - Bits 5–7: biased exponent  
  - Bits 0–4: fraction  

- Exponent Logic  
  - Stored exponent is biased; raw exponent `E` computed via `getE()`.  

- Normalization & Denormalization  
  - Manual shifts to represent scientific values within 9 bits.  

- Special-Case Blocks  
  - Pre-check logic for NaN, INF, sign mismatches.  

- Decimal Preservation  
  - Separate whole and fractional bits maintained during shifts.  

---

## Functions & Their Roles

| Function Name               | Role & Description                                                   |
|-----------------------------|----------------------------------------------------------------------|
| getE                        | Extracts raw exponent from compressed word                           |
| setBits                     | Assembles final 9-bit word from sign, exponent, and fraction slices  |
| isInfinity                  | Returns true if exponent bits all 1 and fraction == 0                |
| isNaN                       | Returns true if exponent bits all 1 and fraction ≠ 0                 |
| isNegative                  | Tests the sign bit                                                   |
| isZero                      | True when exponent == 0 and fraction == 0                            |
| isNormalized                | Checks exponent not zero (and not all-ones)                          |
| shiftLeft                   | Left-shifts mantissa preserving fractional bits, adjusts exponent    |
| shiftRight                  | Right-shifts mantissa preserving fractional bits, adjusts exponent   |
| toMicroFP                   | Converts full-precision `Number_s` struct to `microfp_s`             |
| toNumber                    | Converts `microfp_s` back to `Number_s` full-precision struct        |
| equalizeE                   | Aligns exponents of two `Number_s` operands before add/subtract       |
| addSubSpecialArithmetic     | Handles all NaN, INF, zero cases for addition/subtraction            |
| multSpecialArithmetic       | Handles all NaN, INF, zero cases for multiplication                  |
| addMicroFP                  | Performs addition using `equalizeE`, `addSubSpecialArithmetic`, shifts|
| subMicroFP                  | Negates second operand (`negMicroFP`) then calls `addMicroFP`        |
| mulMicroFP                  | Performs multiplication via `multSpecialArithmetic` and shifts       |
| negMicroFP                  | Flips sign bit of compressed `microfp_s`                             |

---

## Usage Notes

- Operate directly on `Number_s` structs; no explicit allocation or deallocation.  
- NULL pointers guarded; no runtime exceptions.  
- All encoding/decoding via bitwise masks—no floating-point hardware ops.  
- Fully covers edge cases: zero, negative zero, overflow (INF), invalid (NaN), and normalization limits.

---

## Author & Credits

- Student Implementation: Patrick  
- Starter Code & Course: Prof. Kevin Andrea, GMU CS 367 (Fall 2024)  
- Full credit preserved per original course materials  
