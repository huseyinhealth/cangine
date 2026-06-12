#include "inputfield_component.hpp"

#include "../../../platform/input.hpp"
#include "../../../render/renderer/quad_renderer.hpp"
#include "../../../render/renderer/render_system.hpp"
#include "../../gameobjects/game_object.hpp"
#include "../transform_component.hpp"

namespace cangine {
    InputFieldComponent* InputFieldComponent::focused = nullptr;

    InputFieldComponent::InputFieldComponent() {
        this->eventHandlerID = Input::registerCharEventHandler([this](std::string s) {
            this->inputEventHandler(s);
        });

        this->keyEventHandlerID = Input::registerKeyEventHandler([this](KeyEvent e) {
            this->keyEventHandler(e);
        });
    }

    InputFieldComponent::~InputFieldComponent() {
        Input::unregisterCharEventHandler(this->eventHandlerID);
        Input::unregisterKeyEventHandler(this->keyEventHandlerID);
        if (focused == this) {
            focused = nullptr;
        }
    }
    
    void InputFieldComponent::draw(double dt) {
        // background;
        Vector3 pos = this->t->getPosition();
        Vector3 rectPos = Vector3(
            pos.x - this->backgroundPadding / 2,
            pos.y - this->fontHeight / 2,
            pos.z);

        if (this->backgroundColor.a != 0) {
            this->rect = {
                rectPos,
                this->width + (this->backgroundPadding * 2),
                this->height + (this->backgroundPadding * 2),
                this->backgroundColor};

            this->qr->setTexture(this->backTex);
            this->qr->draw(rect);
        }

        // border;

        Color bc;

        if (focused == this)
            bc = this->borderColorFocus;
        else
            bc = this->borderColor;

        Rectangle topBorder {
            this->rect.pos + Vector3(0, this->rect.height, 1),
            this->rect.width,
            this->borderWidth,
            bc
        };

        Rectangle bottomBorder {
            this->rect.pos + Vector3(0, -this->borderWidth, 1),
            this->rect.width,
            this->borderWidth,
            bc
        };

        Rectangle rightBorder {
            this->rect.pos + Vector3(this->rect.width, -this->borderWidth, 1),
            this->borderWidth,
            this->rect.height + this->borderWidth * 2,
            bc
        };

        Rectangle leftBorder {
            this->rect.pos + Vector3(-this->borderWidth, -this->borderWidth, 1),
            this->borderWidth,
            this->rect.height + this->borderWidth * 2,
            bc
        };

        this->qr->setTexture(nullptr);
        this->qr->draw(topBorder);
        this->qr->draw(bottomBorder);
        this->qr->draw(rightBorder);
        this->qr->draw(leftBorder);

        // cursor;

        if (focused == this) {
            Vector2 cursorOffset = this->tr->measureText(this->font, this->content.substr(this->startIdx, this->cursorpos - this->startIdx), this->scale);
            this->blinkTimer += (float) dt;

            if (std::fmod(this->blinkTimer, 1.0) < 0.5) {  // blink effect
                this->qr->setTexture(nullptr);
                this->qr->draw({Vector3(pos.x + cursorOffset.x, pos.y - this->backgroundPadding / 2, pos.z + 0.1f),
                        2.0f,
                        (float)this->fontHeight,
                        this->cursorColor});
            }
        }
    }

    void InputFieldComponent::firstFrame(){
        TextRendererComponent::firstFrame();
        this->qr = RenderSystem::getInstance()->quadRenderer;
        this->t = this->getGameObject()->getComponent<Transform>();
        this->text = "";
        this->content = "";
        this->cursorpos = 0;
        this->startIdx = 0;
        // Style

        this->borderWidth = 2;
        this->borderColor = Color(30, 30, 30, 1);
        this->borderColorFocus = Colors::WHITE;
        this->backgroundColor = Color(40, 40, 40, 1);
        this->color = Colors::WHITE;
        this->placeholder = "InputField";
        this->placeholderColor = Colors::GRAY;
        this->cursorColor = Colors::WHITE;
    }

    void InputFieldComponent::frame(double dt) {
        this->draw(dt);

        this->text = "";
        size_t tempIdx = this->startIdx;
        while (tempIdx < this->content.size()) {
            size_t charLen = 1;
            while (tempIdx + charLen < this->content.size() && 
                  (static_cast<unsigned char>(this->content[tempIdx + charLen]) & 0xC0) == 0x80) {
                charLen++;
            }
            std::string nextChar = this->content.substr(tempIdx, charLen);
            if (this->tr->measureText(this->font, this->text + nextChar, this->scale).x <= this->width) {
                this->text += nextChar;
                tempIdx += charLen;
            } else {
                break; 
            }
        }

        if (text.size() != 0 || focused == this){
            this->tr->renderText(this->font, this->text, this->t->getPosition(), this->scale, this->color);
        } else {
            this->tr->renderText(this->font, this->placeholder, this->t->getPosition(), this->scale, this->placeholderColor);
        }
    }

    void InputFieldComponent::updateView() {
        std::string currentView = this->content.substr(this->startIdx, this->cursorpos - this->startIdx);

        if (this->tr->measureText(this->font, currentView, this->scale).x > this->width) {
            for (int i = (int)this->cursorpos; i >= 0; --i) {
                std::string test = this->content.substr(i, this->cursorpos - i);
                if (this->tr->measureText(this->font, test, this->scale).x > this->width) {
                    this->startIdx = i + 1;
                    break;
                }
            }
        }

        if (this->cursorpos < this->startIdx) {
            this->startIdx = this->cursorpos;
        }
    }

    void InputFieldComponent::inputEventHandler(std::string s) {
        if (focused != this) return;
        this->content.insert(this->cursorpos, s);
        this->cursorpos += s.size();
        this->updateView();
    }

    void InputFieldComponent::keyEventHandler(KeyEvent e) {
        if (focused != this) return;

        if (e.type == KeyEventType::Down) {
            if (e.key == Key::Enter && this->onReturn != nullptr) {
                this->onReturn(this->content);
            }
        }

        if (e.type == KeyEventType::Down || e.type == KeyEventType::Repeat) {
            if (e.key == Key::Backspace && this->content.length() > 0) {
                if (this->cursorpos > 0) {
                    size_t bytesToDelete = 1;
                    
                    while (this->cursorpos - bytesToDelete > 0 &&
                        (static_cast<unsigned char>(this->content[this->cursorpos - bytesToDelete]) & 0xC0) == 0x80) {
                        bytesToDelete++;
                    }
    
                    this->cursorpos -= bytesToDelete;
                    this->content.erase(this->cursorpos, bytesToDelete);

                    if (this->startIdx > this->cursorpos) {
                        this->startIdx = this->cursorpos;
                    }

                    while (this->startIdx > 0) {
                        size_t prevCharIdx = this->startIdx - 1;
                        while (prevCharIdx > 0 && (static_cast<unsigned char>(this->content[prevCharIdx]) & 0xC0) == 0x80) {
                            prevCharIdx--;
                        }

                        std::string testView = this->content.substr(prevCharIdx, this->cursorpos - prevCharIdx);
                        if (this->tr->measureText(this->font, testView, this->scale).x <= this->width) {
                            this->startIdx = prevCharIdx;
                        } else {
                            break;
                        }
                    }
                }
            } else if (e.key == Key::Delete && this->cursorpos < this->content.size()) {
                size_t bytesToDelete = 1;
                while (this->cursorpos + bytesToDelete < this->content.size() &&
                    (static_cast<unsigned char>(this->content[this->cursorpos + bytesToDelete]) & 0xC0) == 0x80) {
                    bytesToDelete++;
                }

                this->content.erase(this->cursorpos, bytesToDelete);
            } else if (e.key == Key::Left && this->cursorpos > 0) {
                this->cursorpos -= 1;

                while (this->cursorpos > 0 && (static_cast<unsigned char>(this->content[this->cursorpos]) & 0xC0) == 0x80) {
                    this->cursorpos -= 1;
                }

                this->blinkTimer = 0.0f;

                this->updateView();
            } else if (e.key == Key::Right && this->cursorpos < this->content.size()) {
                this->cursorpos += 1;
                while (this->cursorpos < this->content.size() && (static_cast<unsigned char>(this->content[this->cursorpos]) & 0xC0) == 0x80) {
                    this->cursorpos += 1;
                }
                this->blinkTimer = 0.0f;
                this->updateView();
            }
        }
    }
 
    void InputFieldComponent::onMouseEvent(const MouseEvent* e) {
        if (e->event == MouseEventType::MouseRelease) {
            if ((e->pos - (Vector2{rect.width, rect.height} / 2.0f)) >> this->rect) {
                focused = this;
            } else {
                if (focused == this)
                    focused = nullptr;
            }
        }
    }
}
