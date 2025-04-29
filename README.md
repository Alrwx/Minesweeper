# 🧨 Minesweeper (C++ & SFML)

A fully-featured version of the classic **Minesweeper** game, developed in **C++** using **SFML** for graphical rendering.

---

## 📋 Features

- **🔧 Dynamic Game Configuration**  
  Board dimensions and mine count are loaded from an external `config.cfg` file.
     *(Third line is the mine count which can be changed through editing the file)*

- **🙋 Welcome Screen**
    - Player enters their name (up to 10 alphabetic characters)
    - Real-time formatting (e.g., `bRUCe` → `Bruce`)
    - Backspace support and a visible cursor indicator
    - Difficulty Selector: Easy, Medium, Hard

- **💣 Core Gameplay**
    - Left-click to reveal a tile
    - Right-click to place/remove a flag
    - Recursive reveal for zero-adjacent tiles
    - Game ends upon hitting a mine or winning by revealing all safe tiles

- **🧪 Debug Mode**  
  Toggle visibility of all mines for testing purposes

- **⏸️ Pause & Leaderboard System**
    - Pause the game and freeze the timer
    - Leaderboard tracks and displays the top 5 player times
    - New top scores are marked with an asterisk (`*`) upon win

- **🏁 Win/Loss Conditions**
    - **Win:** All non-mine tiles revealed, remaining mines flagged, timer stops, smiley face becomes 😎
    - **Loss:** Clicked a mine, all mines revealed, smiley face becomes ☠️
    - Game board is locked after the game ends

- **🎨 UI & Texture Management**  
  All textures and fonts are managed centrally via a custom texture manager to prevent SFML "white square" issues

---

## 🧰 Technologies Used

- **Language:** `C++11`
- **Graphics Library:** [SFML 2.5.1](https://www.sfml-dev.org/)
- **IDE:** CLion / Visual Studio Code
- **Build System:** CMake or Makefile
- **File I/O:** Standard C++ Libraries

---

### 🧩 What You Need

✅ No installation required  
✅ No need for CLion or SFML  
✅ Just download and play!

---

### 📥 Download Instructions

1. Navigate to the **[Releases](https://github.com/Alrwx/Minesweeper/releases/tag/Latest)** section of this repository.  

2. Download the ZIP file (e.g., `Minesweeper_Windows.zip`).

3. Extract the contents to a folder of your choice (e.g., Desktop or Downloads).

---

### 🚀 How to Run

1. Open the extracted folder.

2. Double-click **`minesweeper.exe`** to launch the game.

3. That's it! You're in. 🎉

---

> ⚠️ **Do not delete or move the `files/` folder!**  
> The game needs it to run properly (for textures, fonts, config, etc.)

---

### 🛠️ Troubleshooting

- **Missing DLL error?** Make sure you didn’t forget to extract all the files.
- **Nothing happens when you click?** Try running as Administrator.
- **Still stuck?** [Open an issue](#) or message me directly.

---
