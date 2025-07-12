# 🦖 Dino Game on FPGA DE1-SoC

This project implements a playable clone of the classic Chrome Dino Game on an **FPGA DE1-SoC** development board. It showcases a complete embedded system integrating hardware design via Intel's Platform Designer and game logic written in C, with real-time graphics displayed on VGA and keyboard input through a PS/2 controller.

## 🎮 Game Overview

Control a pixelated dinosaur as it jumps over moving cacti in an ever-accelerating challenge. Key features include:
- Real-time gameplay rendered on a VGA display (640x480 @ 8-bit color)
- PS/2 keyboard input (spacebar to jump)
- Score tracking and increasing difficulty
- Autoplay mode and multiple lives
- Game states with responsive UI: title screen, gameplay, and game over

## 🧠 System Architecture

The game is built using a modular design combining:
- **ARM Cortex-A9 HPS** for game logic and rendering
- **FPGA fabric** for VGA and PS/2 controllers
- **Avalon-MM and AXI buses** for hardware communication
- **Platform Designer (Intel/Altera)** for hardware configuration

### Main Hardware Components
| Component              | Description                                          |
|------------------------|------------------------------------------------------|
| ARM HPS                | Runs C game logic at 800MHz, 1GB DDR3 SDRAM          |
| VGA Controller         | 640x480px display, 8-bit color, 60Hz refresh         |
| VGA Frame Buffer       | On-chip memory accessible via Avalon-MM              |
| PS/2 Controller        | Reads keyboard scan codes from spacebar input        |
| RGB Resampler          | Converts pixel formats for VGA rendering             |
| PLLs                   | Generate 25MHz and 50MHz clocks for system timing    |

## 💻 Software Implementation

The software is written in **C**, using a modular and maintainable structure:
- `Dino_Game_v0.8.c`: Game initialization and main loop
- `core_utils.c`: Event handling, timing, random generation
- `graphics.c`: Low-level VGA drawing functions
- `text_ui.c`: Text rendering and UI components
- `game_entities.c`: Dino and obstacle management
- `game_logic.c`: Core mechanics, input handling, state machine

### Key Features
- Efficient rendering via direct memory writes to VGA buffer
- Jump physics modeled using parabolic motion
- Event queue for asynchronous input handling
- Autoplay system on title screen
- Dynamic obstacle speed based on score

## 🚀 Optimizations

To achieve smooth performance despite hardware limits:
- Bit-shifting replaces expensive multiplications
- Selective rendering avoids unnecessary redraws
- Memory reuse for inactive entities
- Calibrated timing for consistent frame rates
- Input buffering reduces latency

## 🧩 Challenges & Solutions

Some technical hurdles and how they were overcome:
- **Graphics sync**: Finite state machines coordinate logic and visuals
- **Collision detection**: Simplified bounding boxes for fast checks
- **Responsive jumping**: Non-blocking jump FSM
- **Limited resources**: Design compromises without sacrificing playability

## 🏁 How to Play

1. Connect a VGA monitor and PS/2 keyboard to the DE1-SoC
2. Power on and load the game bitstream and binary
3. Press **Spacebar** to start the game and make the Dino jump
4. Avoid cacti and rack up your score!

## 🌱 Future Improvements

- Add sound effects via DAC
- Introduce new obstacle types (e.g., flying birds)
- Two-player mode
- High score saving system
- Enhanced graphics with detailed sprites

## 📚 Educational Value

This project demonstrates practical skills in:
- Digital system design on FPGA
- Embedded C programming
- Hardware-software integration
- Real-time input/output processing
- Performance optimization in constrained systems


## 📄 License

This project is for educational and academic use. Feel free to fork, adapt, and contribute with proper attribution!

---
