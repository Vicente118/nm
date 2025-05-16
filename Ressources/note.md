# ELF Format & Tooling — Structured Notes

> A concise, developer-oriented cheat-sheet on the **Executable and Linkable Format (ELF)** and the classic inspection tools: **`readelf`**, **`nm`**, and **`objdump`**.

---

## 1. What Is ELF?

| Property | Details |
|----------|---------|
| **Acronym** | Executable **L**inkable **F**ormat |
| **Scope**  | Standard binary container for *nix executables, shared libraries, object files, and core dumps |
| **Primary missions** | 1. Instruct the kernel how to map the file into memory.<br>2. Supply metadata to dynamic loaders, debuggers, and linkers.|

### 1.1 Supported ELF File Classes

| Type (`e_type`) | Purpose | Typical Extension |
|-----------------|---------|-------------------|
| `ET_EXEC` | Fully-linked executable | *(none)* |
| `ET_DYN`  | Position-independent shared object (also used for PIE executables) | `.so`, PIE |
| `ET_REL`  | Relocatable object file (not yet linked) | `.o` |
| `ET_CORE` | Core dump produced on crash (post-mortem) | `core.*` |

---

## 2. ELF Anatomy

+---------------+ 0x00
| ELF Header |
+---------------+ <-- Program Header Table offset (e_phoff)
| Program Hdrs | ↳ runtime view: segments
+---------------+
| ...sections...| ↳ link-time view: sections
+---------------+ <-- Section Header Table offset (e_shoff)


### 2.1 ELF Header (`Elfxx_Ehdr`)

| Field | Meaning |
|-------|---------|
| `e_ident[EI_MAG0..3]` | Magic `0x7f 45 4c 46` (`\x7fELF`) |
| `e_ident[EI_CLASS]`   | 01 = 32-bit, 02 = 64-bit |
| `e_ident[EI_DATA]`    | 01 = little-endian, 02 = big-endian |
| `e_type`              | File class (table §1.1) |
| `e_machine`           | ISA (e.g., `EM_X86_64`) |
| `e_entry`             | Program entry point (virtual address) |
| `e_phoff / e_phnum`   | Location & count of Program Header Table |
| `e_shoff / e_shnum`   | Location & count of Section Header Table |

### 2.2 Two Complementary “Views”

| View | Data structure | Loader involved | Contains |
|------|----------------|-----------------|----------|
| **Execution view** | **Program Header Table (PHT)** | Kernel / dynamic loader | *Segments* to map (code, data, TLS, etc.) |
| **Link-time view** | **Section Header Table (SHT)** | Linker (`ld`, `gold`, `mold`) | *Sections* (.text, .data, .rodata, etc.) |

> **Rule of thumb:** sections live on disk; segments live in memory.

---

## 3. Symbols & Symbol Tables

### 3.1 Why Symbols Exist

* Human-readable identifiers (`printf`, `global_var`) → machine addresses/offsets.  
* Used for **linking**, **relocation**, **debug info**, and prettier **disassembly**.

### 3.2 Main Tables

| Section | Loaded? (`SHF_ALLOC`) | Purpose |
|---------|----------------------|---------|
| `.symtab` + `.strtab` | **No** | Complete set of symbols; kept for static linking & debugging. Often stripped. |
| `.dynsym` + `.dynstr` | **Yes** | Subset needed at *runtime* for dynamic linking (PLT/GOT resolution). |

### 3.3 Symbol Entry (`Elfxx_Sym`)

| Field | Description |
|-------|-------------|
| `st_name`  | Offset in associated string table |
| `st_info`  | Combines **binding** (LOCAL, GLOBAL, WEAK) & **type** (FUNC, OBJECT, SECTION, NOTYPE) |
| `st_other` | Visibility (DEFAULT, HIDDEN, PROTECTED) |
| `st_shndx` | Index of defining section (or special values: `SHN_UNDEF`, `ABS`, etc.) |
| `st_value` | Address/offset at link- or run-time |
| `st_size`  | Size in bytes (useful for functions/objects) |

---

## 4. Why Developers Love ELF

* **Cross-platform standard** → consistent tooling on Linux, *BSD, Solaris, etc.  
* **ISA-agnostic** → same layout for ARM, x86-64, RISC-V, …  
* **Dynamic linking** built-in → memory sharing & late binding.  
* Rich **debug support** (DWARF, symbol tables).  
* Huge ecosystem of parsers, loaders, and analysis frameworks (GDB, Valgrind, radare2, Binary Ninja, etc.).

---

## 5. The Classic Toolbox

### 5.1 `readelf` — decode ELF metadata

| Option | What it shows |
|--------|---------------|
| `-h`   | ELF header |
| `-S`   | Section headers |
| `-l`   | Program headers (segments) |
| `-s` / `--dyn-syms` | Static / dynamic symbol tables |
| `-a`   | **All** of the above (verbose) |

> **Tip:** implementing a mini-`readelf` is great for learning—just include `<elf.h>` and walk the structs.

---

### 5.2 `nm` — list symbols

$ nm -C hello
0000000000001139 T main
U puts
0000000000001129 t helper
0000000000004000 B global_counter



| Symbol letter | Section | Visibility |
|---------------|---------|------------|
| `T` | `.text` (code) | **Global** |
| `t` | `.text`        | *Local* |
| `U` | Undefined (needs to be resolved at link or run time) | — |
| `B`/`b` | `.bss` (zero-init data) | Global / local |
| `D`/`d` | `.data` (init data) | Global / local |
| `C` | Common (uninitialized, may merge) | Global |

---

### 5.3 `objdump` — inspect & disassemble

| Option | Description |
|--------|-------------|
| `-f`   | File header summary |
| `-h`   | Section headers |
| `-x`   | All headers (like `readelf -a`) |
| `-d`   | Disassemble **sections that look like code** |
| `-D`   | Disassemble **everything** |
| `--start-address=0x...`/`--stop-address=0x...` | Focus on address range |
| `-Mintel` or `-Matt` | Choose Intel or AT&T syntax |

Example (Intel syntax, truncated):

$ objdump -d -Mintel /bin/ls | head
00000000000012e0 <main>:
12e0: 55 push rbp
12e1: 48 89 e5 mov rbp,rsp
12e4: 48 83 ec 20 sub rsp,0x20



