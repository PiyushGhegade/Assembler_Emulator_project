# 🖥️ CS210 Mini Project – SIMPLE Assembler & Emulator

> **Course:** CS2102 – Computer Science and Engineering  
> **Institute:** IIT Patna  
> **Author:** Piyush Ghegade  
> **Roll No:** 2301AI52  

---

## 📌 Project Description

This repository contains my **CS210 Mini Project** submission, which involves the design and implementation of:

- ✅ A **two-pass assembler** for the extended **SIMPLE instruction set**
- ✅ An **emulator** for the SIMPLE machine
- ✅ Multiple **test programs**, including **Bubble Sort**
- ✅ Detailed **logs, listings, and execution traces**

---

## 📁 Repository Structure

```
├── asm.c
├── emu.cpp
├── claims.txt
├── test1.asm
├── test2.asm
├── test3.asm
├── test4.asm
├── BubbleSort.asm
├── Custom_test1.asm
├── Custom_test2.asm
├── machineCode.o
├── logFile.log
├── listfiles.txt
```

---

## ⚙️ Compilation Instructions

### Assembler
```bash
gcc -std=c89 -pedantic asm.c -o asm
```

### Emulator
```bash
g++ emu.cpp -o emu
```

---

## 🧩 Assembler Features

- Two-pass assembler using a single routine
- Supports SET pseudo-instruction
- Error & warning detection
- Generates object, log, and listing files

---

## 🖥 Emulator Features

- Loads machineCode.o automatically
- Memory dump before/after execution
- Detects infinite loops and errant programs

---

## 🧪 Testing Summary

Assembler and emulator tested on multiple programs including Bubble Sort and custom tests.

---

## 📜 Declaration

This project is entirely my own work and follows IIT Patna academic integrity guidelines.
