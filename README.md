# wc-clone

A simple clone of the Unix `wc` (word count) utility, implemented in **C**.

This project was developed as part of **Software Engineering 1** at university, under the supervision of **Bijan Parsia**.

---

## Overview

`wc-clone` replicates the core functionality of the Unix `wc` command by counting:

* Lines
* Words
* Bytes

The program reads from **standard input** or from files, mirroring the behavior of the original `wc` utility.

---

## Features

* Counts lines, words, and bytes
* Supports input from:

  * Standard input
  * Piped input
  * One or more files
* Output formatting matches traditional `wc` (space-aligned columns)
* Written in portable C

---

## Usage

### Compile

```bash
gcc -o bin/wc-clone src/main.c
```

or

```bash
make
```

### Run with standard input

```bash
./bin/wc-clone
```

Press `Ctrl+D` to end input.

### Run with a pipe

```bash
echo "Hello, World\!" | ./bin/wc-clone
```

### Run with a file

```bash
./wc-clone file.txt
```

---

## Example

```bash
$ echo "Hello, World\!" | ./bin/wc-clone
      1      2     14
```

---

## Notes

* This project is intended for **educational purposes only**
* It is not a drop-in replacement for GNU `wc`
* Some advanced flags or edge cases may not be implemented

---

## Course Information

* **Course:** Software Engineering 1
* **Instructor:** Bijan Parsia
* **Language:** C

---

## License

This project is provided for educational use only.
Do not submit this code as your own outside of the intended coursework.

---
