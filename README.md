This code implements a **keylogger** for the Windows platform using the `WinAPI`. The keylogger listens for key presses at a low level and logs them to a file, optionally hiding its console window and checking if the system has completed booting.

Here are some key aspects of the code:

---

### Features:
1. **Key Press Logging:**
   - Logs key presses in different formats based on the `FORMAT` definition (plain text, decimal, hexadecimal).
   - Uses a `std::map` to provide user-friendly labels for special keys (e.g., `[SHIFT]`, `[BACKSPACE]`).
   - Detects active window changes and logs the window title along with timestamps.

2. **Stealth Mode:**
   - The console window visibility is controlled by `visible` or `invisible` definitions.
   - Hides the console window completely if `invisible` is defined.

3. **Boot Time Wait:**
   - If `bootwait` is defined, the program waits until the system has finished booting before starting.

4. **Mouse Click Ignore:**
   - Ignores mouse click events if `mouseignore` is defined.

5. **Keyboard Layout Handling:**
   - Adapts to different keyboard layouts using `GetKeyboardLayout` and `MapVirtualKeyExA`.

6. **Output:**
   - Logs keystrokes to a file (`keylogger.log`).
   - Keeps the file open during runtime for efficiency.

---

### Security and Ethical Considerations:
- This code can be misused for unethical purposes, such as spying on users without their consent. Ensure it is used in a lawful and ethical manner, such as for personal testing or authorized security audits.
- Unauthorized use of keyloggers may violate privacy laws and regulations.

---

### Potential Issues and Suggestions:
1. **Antivirus Detection:**
   - Many antiviruses will flag this code as malicious. To reduce detection:
     - Encrypt or obfuscate the output.
     - Implement runtime encryption/decryption of key presses.

2. **File Handling:**
   - Ensure the output file is closed properly in case of program termination to avoid data loss.

3. **Hook Lifetime:**
   - The hook is removed only when the program exits. Consider adding a way to clean up the hook in case of exceptions or unexpected termination.

4. **Thread Safety:**
   - Access to shared resources (like the `output_file`) should be thread-safe.

---

### How to Compile:
Use a Windows compiler like MinGW or MSVC:
```bash
g++ keylogger.cpp -o keylogger.exe -luser32 -lgdi32
```

---
