#pragma once
#include "component.hpp"
#include "../../render/renderer/render_system.hpp"
#include "../../render/renderer/quad_renderer.hpp"

#include "../../shapes/shapes.hpp"
#include "../../render/texture.hpp"

#include "../../util/color.hpp"

#include "../gameobjects/game_object.hpp"

#include "transform_component.hpp"

namespace cangine {
    /**
     * @brief Component that draws its game object as a textured rectangle.
     *
     * Each frame it submits a rectangle (sized/colored via the setters, placed
     * by the object's @ref Transform) to the @ref QuadRenderer.
     */
    class QuadRendererComponent : public IComponent {
        protected:
            QuadRenderer *qr;     ///< Renderer used to draw the quad.
            Texture *texture;     ///< Texture applied to the quad, if any.
            Transform* transform; ///< Cached transform of the owning object.
            Rectangle rect;       ///< Rectangle geometry to draw.
        public:
            void firstFrame() override;
            void frame(double deltaTime) override;
            /**
             * @brief Sets the texture applied to the quad.
             * @param t Texture to use.
             */
            virtual void setTexture(Texture &t);
            /**
             * @brief Sets the rectangle width.
             * @param widthIn New width.
             */
            virtual void setRectWidth(int widthIn);
            /**
             * @brief Sets the rectangle height.
             * @param heightIn New height.
             */
            virtual void setRectHeight(int heightIn);
            /**
             * @brief Sets the rectangle fill color.
             * @param colorIn New color.
             */
            virtual void setRectColor(Color colorIn);
            /**
             * @brief Returns the underlying rectangle.
             * @return Pointer to the rectangle geometry.
             */
            Rectangle* getRect();
            /**
             * @brief Returns the rectangle's position.
             * @return Rectangle position.
             */
            Vector3 getRectPos();
            /**
             * @brief Returns the rectangle's size.
             * @return Width and height as a vector.
             */
            Vector2 getRectSize();
    };
}
