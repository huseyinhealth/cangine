#pragma once

/**
 * @file cangine.hpp
 * @brief Public entry point for the cangine engine.
 *
 * Aggregates the engine's subsystems and exposes the @ref cangine::CangineApp
 * base class that applications derive from, plus the @ref cangine::init and
 * @ref cangine::terminate lifecycle functions.
 */

#include <string>
#include <type_traits>
#include <initializer_list>

#include "audio/audiomanager.hpp"

#include "util/color.hpp"
#include "util/position.hpp"

#include "platform/window.hpp"

#include "render/shader.hpp"
#include "render/renderer/render_system.hpp"

#include "engine/object_manager.hpp"
#include "engine/exception.hpp"

namespace cangine {
    /// @brief Initializes engine-wide state (must be called before use).
    void init();
    /// @brief Tears down engine-wide state.
    void terminate();

    /**
     * @brief Base application class that owns the window and main loop.
     *
     * Subclass it and override @ref onFirstFrame and @ref onFrame to build a
     * game. The constructor sets up the window, shader, render system, object
     * manager and audio manager; @ref run drives the frame loop until the
     * window closes.
     */
    class CangineApp {
        private:
        bool isFirstFrame = true;       ///< Whether the next frame is the first one.
        Shader* shader;                 ///< Default shader used for rendering.
        RenderSystem* _rs = nullptr;    ///< Owned render system.
        ObjectManager* _om = nullptr;   ///< Owned object manager.

        /**
         * @brief Loads and initializes the default shader.
         * @param name Base name of the shader to load.
         */
        void initShader(std::string name);
        /// @brief Creates the render system.
        void initRenderSystem();
        /// @brief Creates the object manager.
        void initObjectManager();
        /// @brief Creates the audio manager.
        void initAudioManager();

        public:
        Color windowFillColor;          ///< Color the window is cleared to each frame.
        Window* window;                 ///< The application window.
        AudioManager* audioMan = nullptr; ///< Owned audio manager.

        /**
         * @brief Constructs the application and all its subsystems.
         * @param windowWidth Initial window width in pixels.
         * @param windowHeight Initial window height in pixels.
         * @param windowTitle Window title.
         * @param fullscreen Whether to open in fullscreen mode.
         * @param windowFillColor Color the window is cleared to each frame.
         * @param defaultShaderName Base name of the default shader to load.
         */
        CangineApp(
            int windowWidth = 800,
            int windowHeight = 600,
            std::string windowTitle = "cangine window",
            bool fullscreen = false,
            Color windowFillColor = Colors::BLACK,
            std::string defaultShaderName = "default"
        );
        ~CangineApp();

        /// @brief Override to run setup logic on the first frame.
        virtual void onFirstFrame() {}
        /**
         * @brief Override to run per-frame game logic.
         * @param dt Seconds elapsed since the previous frame.
         */
        virtual void onFrame(double dt) {}

        /**
         * @brief Instantiates a game object of a derived type.
         *
         * Statically asserts that @p T derives from @ref GameObject.
         * @tparam T Object type to create (must derive from GameObject).
         * @tparam Args Constructor argument types.
         * @param args Arguments forwarded to the object's constructor.
         * @return Pointer to the newly created object.
         */
        template <typename T, typename... Args>
        T* instantiateObject(Args&&... args) {
            static_assert(std::is_base_of<GameObject, T>::value,
                        "Instantiated object is not a GameObject");
            return new T(std::forward<Args>(args)...);
        }

        /// @brief Runs the main loop until the window is closed.
        void run();
    };
}
