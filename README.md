# 🎮 cangine

A lightweight 2D game engine written in C++, built on top of OpenGL. Designed for simplicity — create windows, render shapes and text, play audio, build UIs, handle input, and compose game objects with a clean component system.

---

## ✨ Features

- **Component-based GameObject system** — Attach and detach components (`Transform`, `QuadRendererComponent`, `TextRendererComponent`, and custom ones) at runtime
- **Built-in UI objects** — `Label`, `Button`, `InputField` and `Panel` game objects with click/return event handlers, placeholders and styling
- **Camera** — Movable `Camera` object; attach custom components to control panning and view
- **2D Rendering** — Render colored/textured quads, triangles and text out of the box
- **Audio playback** — Load and control sounds via the `AudioManager` (`play`, `pause`, `stop`, `seek`, volume, looping) powered by miniaudio
- **Collision detection** — Rectangle overlap (SAT, rotation/scale aware) and point-in-rect checks
- **Input handling** — Keyboard (`getKey`, `getKeyDown`, `getKeyUp`) and mouse events (click, release, move, scroll, enter)
- **FPS & delta time** — `getFPS()`, `getInstantFPS()`, `getDeltaTime()` on the Window object
- **Shader system** — Load and manage GLSL shaders with minimal boilerplate
- **Texture loading** — Load PNG/JPEG images via `stb_image`
- **Simple app abstraction** — Subclass `CangineApp` and override `onFirstFrame()` / `onFrame(dt)` for a clean game loop, with `instantiateObject<T>()` for spawning objects

---

## 📦 Dependencies

| Library | Purpose |
|---|---|
| [GLFW](https://www.glfw.org/) | Window creation & input |
| [GLAD](https://glad.dav1d.de/) | OpenGL function loader |
| [FreeType](https://freetype.org/) | Font rendering |
| [stb_image](https://github.com/nothings/stb) | Texture/image loading |
| [miniaudio](https://miniaud.io/) | Audio playback |
| [zlib](https://zlib.net/) | Compression (FreeType dependency) |

All dependencies are bundled under `cangine/external/`.

---

## 🔧 Building

### Requirements

- CMake ≥ 3.10
- A C++17-compatible compiler (GCC, Clang, or MSVC)

### Build the engine

```bash
make engine
```

### Build in debug mode

```bash
make engined
```

### Build and run an example

```bash
make example EXAMPLE=simplehelloworld
make example EXAMPLE=ui
make example EXAMPLE=audio
make example EXAMPLE=sandbox
```

### Debug an example

```bash
make exampled EXAMPLE=sandbox
```

### Clean build files

```bash
make clean
```

#### Makefile variables

| Variable | Default | Description |
|---|---|---|
| `EXAMPLE` | `sandbox` | Example to build/run |
| `BUILDDIR` | `build` | CMake output directory |
| `BUILDEXAMPLES` | `OFF` | Build example projects |
| `EXPORTCOMPILECOMMANDS` | `ON` | Export `compile_commands.json` |

---

## 🚀 Quick Start

### Install dependencies

```bash
sudo apt install libglfw3-dev libfreetype-dev libepoxy-dev libfontconfig-dev
```

### Clone and build

```bash
git clone https://github.com/huseyinhealth/cangine.git
cd cangine
make engine
```

---

The simplest way to use cangine is to subclass `CangineApp` and spawn objects with `instantiateObject<T>()`:

```cpp
#include "cangine/cangine.hpp"

using namespace cangine;

class MyApp : public CangineApp {
public:
    MyApp() : CangineApp(800, 600, "My Game") {}

    void onFirstFrame() override {
        // Called once before the game loop starts.

        // Spawn a text label
        Label* hello = instantiateObject<Label>("Hello, world!");

        // Center it on screen (Z index 80+ is the UI layer)
        Transform::posFor(hello, Vector3(hello->getSize() / -2, 80));
    }

    void onFrame(double dt) override {
        // Called every frame with delta time in seconds.
    }
};

int main() {
    MyApp().run();
    return 0;
}
```

---

## 🧩 Architecture

```
cangine/
├── engine/
│   ├── GameObject          # Base entity — holds a list of IComponents
│   ├── ObjectManager       # Manages all active GameObjects
│   ├── components/
│   │   ├── IComponent              # Base class for all components
│   │   ├── Transform               # Position, rotation, scale (+ posFor helper)
│   │   ├── CameraComponent         # View/camera transform
│   │   ├── QuadRendererComponent   # Renders a colored/textured rectangle
│   │   ├── TextRendererComponent   # Renders a string with FreeType
│   │   └── ui/
│   │       ├── ButtonComponent
│   │       ├── InputFieldComponent
│   │       └── PanelComponent
│   └── gameobjects/
│       ├── CameraObject        # Camera entity (auto-attaches CameraComponent)
│       └── ui/
│           ├── Label           # Text label object
│           ├── Button          # Clickable button object
│           ├── InputField      # Text input object
│           └── Panel           # Container/background panel object
├── audio/
│   ├── AudioManager        # Loads and controls sound playback (miniaudio)
│   └── Sound               # Sound source description (path, name, volume)
├── platform/
│   ├── Window              # GLFW window wrapper, frame loop, FPS
│   ├── Input               # Static keyboard query (getKey / getKeyDown / getKeyUp)
│   ├── Key                 # Key enum
│   └── Mouse               # Mouse event types and data
├── render/
│   ├── Shader              # GLSL shader loader/linker
│   ├── Texture             # stb_image texture wrapper
│   ├── VAO / VBO / EBO     # OpenGL buffer object wrappers
│   └── renderer/
│       ├── RenderSystem        # Top-level renderer, drives quad/text/primitive rendering
│       ├── QuadRenderer        # Batches and draws textured quads
│       ├── TextRenderer        # Draws glyphs via FreeType
│       └── PrimitiveRenderer   # Draws untextured primitives
├── shapes/
│   ├── Rectangle           # Shape + overlap / containsPoint helpers
│   ├── Triangle            # Triangle shape definition
│   └── Vertex              # Vertex struct (position, color, UV)
├── types/
│   ├── Vector2 / Vector3   # Math types
│   └── Dimensions          # Width/height helper
└── util/
    ├── Color               # RGBA color (0–255 inputs, normalized internally)
    └── Position            # Screen-to-clip-space normalization helpers
```

---

## 🎨 Core API Reference

### CangineApp

```cpp
class MyApp : public CangineApp {
    MyApp() : CangineApp(800, 600, "Title") {}
    void onFirstFrame() override { /* setup */ }
    void onFrame(double dt) override { /* per-frame logic */ }
};

// Spawn any GameObject-derived type:
GameObject* obj = instantiateObject<GameObject>();
Label* lbl      = instantiateObject<Label>("text");

// Audio manager is available as a member:
audioMan->add("./music.mp3", "music");
audioMan->play("music", true);
```

### Color

```cpp
Color red(255, 0, 0);
Color semiTransparent(0, 128, 255, 0.5f); // alpha is 0.0–1.0

// Built-in presets
Colors::BLACK, Colors::WHITE, Colors::GRAY, Colors::DARK_GRAY,
Colors::RED, Colors::GREEN, Colors::BLUE
```

### Transform

```cpp
Transform* t = gameObject->getComponent<Transform>();
t->setPosition(Vector3(x, y, z));
t->setRotation(Vector3(0, 0, angle)); // Z-axis rotation for 2D
t->setScale(Vector3(1, 1, 1));

// Convenience: position a whole object in one call
Transform::posFor(gameObject, Vector3(x, y, z));
```

> **Layering:** the Z component acts as a layer. A Z index of **80 and above** is treated as the UI layer (rendered on top of world objects).

### UI objects

```cpp
// Label
Label* lbl = instantiateObject<Label>("Score: 0");
lbl->setText("Score: 42");
lbl->setColor(Colors::GREEN);
Vector2 size = lbl->getSize();

// Button
Button* btn = instantiateObject<Button>("Click me!");
btn->registerOnClickEvent([&](const MouseEvent* e) {
    // handle click
});

// InputField
InputField* field = instantiateObject<InputField>();
field->setPlaceholder("Name?");
field->setOnReturn([&](std::string value) {
    // handle submitted text
});
std::string text = field->getContent();
```

### QuadRendererComponent

```cpp
auto* qrc = new QuadRendererComponent();
gameObject->appendComponent<QuadRendererComponent>(qrc);
qrc->setRectWidth(128);
qrc->setRectHeight(64);
qrc->setRectColor(Colors::WHITE);

// Optional: set a texture
Texture tex("path/to/image.png");
qrc->setTexture(tex);
```

### TextRendererComponent

```cpp
auto* trc = new TextRendererComponent();
gameObject->appendComponent<TextRendererComponent>(trc);
trc->text = "Score: 42";
trc->color = Colors::GREEN;
trc->backgroundColor = Color(0, 0, 0, 0.5f);
Vector2 size = trc->getSize(); // text bounding box
```

### Camera

```cpp
Camera* camera = new Camera();   // CameraComponent is attached automatically
camera->appendComponent<CustomCameraController>(controller);
```

### Audio

```cpp
audioMan->add("./sound.mp3", "sfx");
audioMan->play("sfx", /*loop=*/false);
audioMan->pause("sfx");
audioMan->resume("sfx");
audioMan->stop("sfx");
audioMan->setVolume("sfx", 0.5f);          // 0.0–1.0
audioMan->seek("sfx", 10.0f);              // seconds
float pos = audioMan->getPosition("sfx");
float dur = audioMan->getDuration("sfx");
bool playing = audioMan->isPlaying("sfx");
```

### Input

```cpp
// Held down
if (Input::getKey(Key::A)) { ... }

// Just pressed this frame
if (Input::getKeyDown(Key::Space)) { ... }

// Just released this frame
if (Input::getKeyUp(Key::Escape)) { ... }
```

### Mouse events

Override `onMouseEvent` in a custom `IComponent`:

```cpp
void onMouseEvent(const MouseEvent* e) override {
    if (e->event == MouseEventType::MouseClick &&
        e->button == MouseButton::Right) {
        // e->pos contains the cursor position
    }
}
```

### Rectangle collision

```cpp
// Point-in-rect (transform-aware: respects rotation & scale)
bool hit = Rectangle::containsPoint(rect, transform, mousePos);

// Rect-rect overlap (SAT-based, transform-aware)
bool colliding = Rectangle::overlaps(rectA, transformA, rectB, transformB);
```

---

## 📁 Project Structure

```
cangine/
├── cangine/           # Engine library source
│   ├── src/
│   ├── external/      # Bundled third-party libraries
│   ├── shaders/       # default.vert / default.frag
│   └── test/
├── examples/
│   ├── simplehelloworld/   # Minimal CangineApp subclass example
│   ├── helloworld/         # Manual engine setup example
│   ├── ui/                 # Buttons and labels
│   ├── inputfield/         # Text input + submit handling
│   ├── panel/              # Panel/container UI
│   ├── audio/              # Audio playback controls
│   ├── camera/             # Draggable camera
│   ├── aimtrainer/         # Aim trainer mini-game
│   ├── survival/           # Survival mini-game
│   └── sandbox/            # Full-featured test/demo scene
├── CMakeLists.txt
└── Makefile           # Convenience wrapper around CMake
```

---

## 📄 License

See [LICENSE.txt](LICENSE.txt) for details.
