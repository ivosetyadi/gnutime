# 🕒 gnutime

**A lightweight Windows 64-bit port of the GNU `time` utility** — built using **Visual Studio x64 Native Build Tools**. This version replicates core `time` features such as `real`, `user`, and `sys` timing for any command-line process, and adds support for GNU-style formatting (`-f`) and output redirection (`-o`).

> ❗This is not `/usr/bin/time`, but a compatible reimplementation targeted at native Windows users — no WSL, MSYS2, or Cygwin required.

---

## ✨ Features

- ✅ Native Windows 64-bit executable (no dependencies)
- ✅ Launches external programs and reports:
  - **real**: wall clock time
  - **user**: CPU time in user mode
  - **sys**: CPU time in kernel mode
- ✅ GNU-compatible formatting via `-f` (supports `%e`, `%U`, `%S`)
- ✅ Output redirection via `-o filename`
- ✅ Forwards child process exit code

---

## 📦 Compilation Instructions (Visual Studio)

You must have **Microsoft Visual Studio Build Tools** (2019 or 2022) installed.

### 🔧 1. Open the x64 Developer Command Prompt

Search and launch:
```
x64 Native Tools Command Prompt for VS 2022
```

### 📂 2. Clone this repository
```bash
git clone https://github.com/yourusername/gnutime.git
cd gnutime
```

### 🛠 3. Compile using `cl.exe`:
```cmd
cl /O2 /Fe:gtime.exe gtime.c
```

This will produce `gtime.exe` — a standalone, portable binary.

---

## ▶️ Usage Examples

```bash
gtime ping 127.0.0.1 -n 3
```

```bash
gtime -f "elapsed=%e user=%U sys=%S" ping 127.0.0.1 -n 2
```

```bash
gtime -o time_result.txt -f "real=%e" myscript.bat
```

---

## 📄 Format Codes (Partial Support)

| Code | Meaning              |
|------|----------------------|
| `%e` | Elapsed real time (wall clock) |
| `%U` | User CPU time        |
| `%S` | System CPU time      |
| `%%` | Literal `%`          |

> Note: Memory/I/O stats like `-v` are not yet supported. Coming soon.

---

## ❗Limitations

- No `-v` or memory usage support (yet)
- No support for complex nested shell commands (e.g., pipes or `sh -c`)
- Only available on Windows (compiled for 64-bit targets)

---

## 📌 License

This implementation is provided under the [MIT License](LICENSE).  
It is **not** a direct fork of GNU `time`, but inspired by its functionality.