# EmbedKit_ParagUdgirkar

## UART Frame Parser using Embedded C

---

# Embedded Systems & Firmware Development

########################################################  
# Project Name : UART Frame Parser  
# Description : UART Protocol Parser using FSM Architecture  
# Language : Embedded C  
# Architecture : Finite State Machine (FSM)  
# Author : Parag Udgirkar  
# Date : 22/05/2026  
########################################################  

---

# Project Overview

This project implements a professional UART Frame Parser using Embedded C.

The parser is designed using a Finite State Machine (FSM) architecture for reliable and deterministic UART communication parsing.

The implementation supports:

- UART byte-by-byte parsing
- XOR checksum validation
- Inter-byte timeout detection
- Automatic parser recovery
- Back-to-back frame handling
- Embedded-safe static memory usage
- GCC warning-free compilation

---

# UART Protocol Format

```text
+--------+--------+--------+-----------+------------+
| SOF    | CMD    | LEN    | PAYLOAD   | CHECKSUM   |
| 1 Byte | 1 Byte | 1 Byte | N Bytes   | 1 Byte     |
+--------+--------+--------+-----------+------------+
```

---

# Protocol Definitions

| Field | Description |
|------|-------------|
| SOF | Start Of Frame = `0xAA` |
| CMD | Command Identifier |
| LEN | Payload Length |
| PAYLOAD | Variable Length Data |
| CHECKSUM | XOR of CMD + LEN + PAYLOAD |

---

# Finite State Machine Design

########################################################  
# State Name : STATE_WAIT_SOF  
# Description : Wait for Start Of Frame byte  
########################################################  

########################################################  
# State Name : STATE_CMD  
# Description : Read Command byte  
########################################################  

########################################################  
# State Name : STATE_LEN  
# Description : Read Payload Length  
########################################################  

########################################################  
# State Name : STATE_PAYLOAD  
# Description : Read Payload Data  
########################################################  

########################################################  
# State Name : STATE_CHECKSUM  
# Description : Validate XOR Checksum  
########################################################  

---

# Features

- Finite State Machine based parser
- UART byte-by-byte processing
- XOR checksum validation
- Inter-byte timeout recovery
- Parser synchronization
- Static memory allocation
- Embedded firmware coding standards
- Continuous stream handling
- Production-style code formatting
- Clean modular architecture

---

# Repository Structure

```text
EmbedKit_ParagUdgirkar/
│
├── README.md
├── uart_parser.c
├── uart_parser.h
├── Makefile
└── output_sample.txt
```

---

# Build Instructions

## Compile

```bash
gcc -Wall -std=c99 uart_parser.c -o uart_parser
```

---

# Run Instructions

## Windows CMD

```bash
uart_parser.exe
```

## PowerShell

```bash
.\uart_parser.exe
```

## Linux / macOS / WSL

```bash
./uart_parser
```

---

# Test Cases

########################################################  
# TEST 1  
# Description : Valid UART Frame  
########################################################  

### Input Frame

```text
AA 01 03 10 20 30 02
```

### Expected Output

```text
FRAME OK CMD=0x01 LEN=3 PAYLOAD=[10 20 30]
```

---

########################################################  
# TEST 2  
# Description : Inter-byte Timeout Recovery  
########################################################  

### Expected Output

```text
TIMEOUT detected
Parser reset successfully
Frame recovered successfully
```

---

########################################################  
# TEST 3  
# Description : Back-to-Back Frame Parsing  
########################################################  

### Expected Output

```text
Two valid frames parsed successfully
```

---

########################################################  
# TEST 4  
# Description : Timeout Disabled Scenario  
########################################################  

### Expected Output

```text
CHECKSUM ERROR
```

---

# Expected Program Output

```text
================ TEST 1 ================
t=  0ms byte=0xAA -> receiving...
t=  5ms byte=0x01 -> receiving...
t= 10ms byte=0x03 -> receiving...
t= 15ms byte=0x10 -> receiving...
t= 20ms byte=0x20 -> receiving...
t= 25ms byte=0x30 -> receiving...
t= 30ms byte=0x02 -> FRAME OK CMD=0x01 LEN=3 PAYLOAD=[10 20 30]

================ TEST 2 ================
t=200ms byte=0xAA -> TIMEOUT (185ms gap > 50ms) -- parser reset
t=220ms byte=0x7B -> FRAME OK CMD=0x05 LEN=1 PAYLOAD=[7F]

================ TEST 3 ================
t= 20ms byte=0x57 -> FRAME OK CMD=0x03 LEN=1 PAYLOAD=[55]
t= 50ms byte=0x17 -> FRAME OK CMD=0x04 LEN=2 PAYLOAD=[AA BB]

================ TEST 4 ================
t=210ms byte=0x01 -> CHECKSUM ERROR
```

---

# Embedded Concepts Used

- UART Communication
- Finite State Machine
- Embedded Protocol Parsing
- Timeout Detection
- XOR Checksum Validation
- Error Recovery Mechanism
- Static Memory Management
- Firmware Stream Processing

---

# Important Design Decisions

########################################################  
# Design Decision : Timeout Validation  
# Description : Timeout checked before byte processing  
########################################################  

This prevents corrupted frames from being accepted after delayed byte arrival.

---

########################################################  
# Design Decision : Static Memory Allocation  
# Description : No dynamic memory allocation used  
########################################################  

Suitable for embedded systems and microcontroller firmware development.

---

########################################################  
# Design Decision : Modular Parser APIs  
# Description : Clean reusable architecture  
########################################################  

Improves maintainability and scalability.

---

# Coding Standards

- Fixed-width integer usage (`uint8_t`, `uint32_t`)
- GCC warning-free compilation
- Structured formatting
- Embedded firmware style comments
- Modular function design
- Deterministic state-machine logic

---

# Future Improvements

- UART Interrupt Integration
- Ring Buffer Support
- CRC16 Validation
- RTOS Integration
- Multi-frame Queue Processing
- Unit Testing Framework
- STM32 Porting
- ESP32 Firmware Integration
- Doxygen Documentation

---

# GitHub Setup

## Repository Name

```text
EmbedKit_ParagUdgirkar
```

---

## Git Commands

```bash
git init

git add .

git commit -m "Initial commit - UART Frame Parser using FSM"

git branch -M main

git remote add origin https://github.com/yourusername/EmbedKit_ParagUdgirkar.git

git push -u origin main
```

---

# Professional GitHub Description

```text
Embedded C UART Frame Parser using Finite State Machine with checksum validation and timeout recovery.
```

---

# Recommended GitHub Topics

```text
embedded-c
uart-parser
firmware
embedded-systems
state-machine
uart
protocol-parser
c-programming
embedded-development
fsm
```

---

# Author

## Parag Udgirkar

Embedded Systems | Firmware Development | C Programming | UART Communication

---

# License

This project is created for Embedded Firmware Learning and Technical Assessment purposes.

---

# Thank You

Industrial Embedded Project Documentation formatting By Parag Udgirkar.