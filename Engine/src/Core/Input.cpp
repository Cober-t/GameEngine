#include <pch.h>
#include "Core/Input.h"
#include "Core/Window.h"
#include "Core/EngineApp.h"

#include <cmath>

namespace Cober {

    namespace
    {
        constexpr float s_DefaultDeadzone = 0.15f;

        bool TryMapMouseButton(Uint8 sdlButton, MouseButton& outButton)
        {
            switch (sdlButton)
            {
                case SDL_BUTTON_LEFT:
                    outButton = MouseButton::Left;
                    return true;
                case SDL_BUTTON_MIDDLE:
                    outButton = MouseButton::Middle;
                    return true;
                case SDL_BUTTON_RIGHT:
                    outButton = MouseButton::Right;
                    return true;
                default:
                    return false;
            }
        }

        float NormalizeAxisValue(Sint16 value)
        {
            // Keep full range symmetric
            if (value < 0) {
                return static_cast<float>(value) / 32768.0f;
			}

            return static_cast<float>(value) / 32767.0f;
        }
    }

    void Input::Update()
    {
        // Intentionally empty for now.
        // Input state is driven by SDL events through Input::OnEvent(...)
        // and finalized once per frame in EndFrame().
    }

    void Input::OnEvent(const SDL_Event& event)
    {
        switch (event.type)
        {
            case SDL_EVENT_KEY_DOWN:
            {
                if (event.key.repeat) {
                    break;
				}

                KeyCode key = static_cast<KeyCode>(event.key.scancode);
                UpdateKeyState(key, KeyState::Pressed);
                break;
            }

            case SDL_EVENT_KEY_UP:
            {
                KeyCode key = static_cast<KeyCode>(event.key.scancode);
                UpdateKeyState(key, KeyState::Released);
                break;
            }

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            {
                MouseButton button;
                if (!TryMapMouseButton(event.button.button, button))
                    break;

                UpdateButtonState(button, KeyState::Pressed);
                break;
            }

            case SDL_EVENT_MOUSE_BUTTON_UP:
            {
                MouseButton button;
                if (!TryMapMouseButton(event.button.button, button))
                    break;

                UpdateButtonState(button, KeyState::Released);
                break;
            }

            case SDL_EVENT_MOUSE_MOTION:
            {
                s_MouseX = event.motion.x;
                s_MouseY = event.motion.y;
                break;
            }

            case SDL_EVENT_GAMEPAD_ADDED:
            case SDL_EVENT_GAMEPAD_REMAPPED:
            {
                const int controllerID = static_cast<int>(event.gdevice.which);

                Controller& controller = s_Controllers[controllerID];
                controller.ID = controllerID;

                if (controller.Name.empty()) {
					controller.Name = "Gamepad";	
				}

                break;
            }

            case SDL_EVENT_GAMEPAD_REMOVED:
            {
                const int controllerID = static_cast<int>(event.gdevice.which);
                s_Controllers.erase(controllerID);
                break;
            }

            case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
            {
                const int controllerID = static_cast<int>(event.gbutton.which);
                const int button = static_cast<int>(event.gbutton.button);

                Controller& controller = s_Controllers[controllerID];
                controller.ID = controllerID;

                if (controller.Name.empty()) {
                    controller.Name = "Gamepad";
				}

                controller.ButtonDown[button] = true;
                UpdateControllerButtonState(controllerID, button, KeyState::Pressed);
                break;
            }

            case SDL_EVENT_GAMEPAD_BUTTON_UP:
            {
                const int controllerID = static_cast<int>(event.gbutton.which);
                const int button = static_cast<int>(event.gbutton.button);

                Controller& controller = s_Controllers[controllerID];
                controller.ID = controllerID;
                controller.ButtonDown[button] = false;

                UpdateControllerButtonState(controllerID, button, KeyState::Released);
                break;
            }

            case SDL_EVENT_GAMEPAD_AXIS_MOTION:
            {
                const int controllerID = static_cast<int>(event.gaxis.which);
                const int axis = static_cast<int>(event.gaxis.axis);

                Controller& controller = s_Controllers[controllerID];
                controller.ID = controllerID;

                if (controller.Name.empty()) {
                    controller.Name = "Gamepad";
				}

                if (controller.DeadZones.find(axis) == controller.DeadZones.end()) {
                    controller.DeadZones[axis] = s_DefaultDeadzone;
				}

                const float normalized = NormalizeAxisValue(event.gaxis.value);
                const float deadzone = controller.DeadZones[axis];

                controller.AxisStates[axis] =
                    (std::fabs(normalized) >= deadzone) ? normalized : 0.0f;

                break;
            }

            default:
                break;
        }
    }

    void Input::EndFrame()
    {
        // Keyboard
        for (auto& [key, data] : s_KeyData) {
            if (data.State == KeyState::Pressed)  { data.State = KeyState::Held; } else 
			if (data.State == KeyState::Released) { data.State = KeyState::None; }
        }

        // Mouse
        for (auto& [button, data] : s_MouseData) {
            if (data.State == KeyState::Pressed)  { data.State = KeyState::Held; } else 
			if (data.State == KeyState::Released) { data.State = KeyState::None; }
        }

        // Controllers
        for (auto& [id, controller] : s_Controllers) {
            for (auto& [button, data] : controller.ButtonStates){
                if (data.State == KeyState::Pressed)  { data.State = KeyState::Held; } else 
				if (data.State == KeyState::Released) { data.State = KeyState::None; }
            }
        }
    }

    bool Input::IsKeyPressed(KeyCode key)
    {
        auto it = s_KeyData.find(key);
        return it != s_KeyData.end() && it->second.State == KeyState::Pressed;
    }

    bool Input::IsKeyHeld(KeyCode key)
    {
        auto it = s_KeyData.find(key);
        return it != s_KeyData.end() && it->second.State == KeyState::Held;
    }

    bool Input::IsKeyDown(KeyCode keycode)
    {
        return IsKeyPressed(keycode) || IsKeyHeld(keycode);
    }

    bool Input::IsKeyReleased(KeyCode key)
    {
        auto it = s_KeyData.find(key);
        return it != s_KeyData.end() && it->second.State == KeyState::Released;
    }

    bool Input::IsMouseButtonPressed(MouseButton button)
    {
        auto it = s_MouseData.find(button);
        return it != s_MouseData.end() && it->second.State == KeyState::Pressed;
    }

    bool Input::IsMouseButtonHeld(MouseButton button)
    {
        auto it = s_MouseData.find(button);
        return it != s_MouseData.end() && it->second.State == KeyState::Held;
    }

    bool Input::IsMouseButtonDown(MouseButton button)
    {
        return IsMouseButtonPressed(button) || IsMouseButtonHeld(button);
    }

    bool Input::IsMouseButtonReleased(MouseButton button)
    {
        auto it = s_MouseData.find(button);
        return it != s_MouseData.end() && it->second.State == KeyState::Released;
    }

    bool Input::IsControllerPresent(int id)
    {
        return s_Controllers.find(id) != s_Controllers.end();
    }

    std::vector<int> Input::GetConnectedControllerIDs()
    {
        std::vector<int> ids;
        ids.reserve(s_Controllers.size());

        for (const auto& [id, controller] : s_Controllers) {
            ids.emplace_back(id);
		}

        return ids;
    }

    const Controller* Input::GetController(int id)
    {
        auto it = s_Controllers.find(id);
        if (it == s_Controllers.end()) {
            return nullptr;
		}

        return &it->second;
    }

    std::string_view Input::GetControllerName(int id)
    {
        auto it = s_Controllers.find(id);
        if (it == s_Controllers.end()) {
            return {};
		}

        return it->second.Name;
    }

    bool Input::IsControllerButtonPressed(int controllerID, int button)
    {
        auto controllerIt = s_Controllers.find(controllerID);
        if (controllerIt == s_Controllers.end()) {
            return false;
		}

        auto buttonIt = controllerIt->second.ButtonStates.find(button);
        return buttonIt != controllerIt->second.ButtonStates.end() &&
               buttonIt->second.State == KeyState::Pressed;
    }

    bool Input::IsControllerButtonHeld(int controllerID, int button)
    {
        auto controllerIt = s_Controllers.find(controllerID);
        if (controllerIt == s_Controllers.end()) {
            return false;
		}

        auto buttonIt = controllerIt->second.ButtonStates.find(button);
        return buttonIt != controllerIt->second.ButtonStates.end() &&
               buttonIt->second.State == KeyState::Held;
    }

    bool Input::IsControllerButtonDown(int controllerID, int button)
    {
        auto controllerIt = s_Controllers.find(controllerID);
        if (controllerIt == s_Controllers.end()) {
            return false;
		}

        auto downIt = controllerIt->second.ButtonDown.find(button);
        return downIt != controllerIt->second.ButtonDown.end() && downIt->second;
    }

    bool Input::IsControllerButtonReleased(int controllerID, int button)
    {
        auto controllerIt = s_Controllers.find(controllerID);
        if (controllerIt == s_Controllers.end()) {
            return false;
		}

        auto buttonIt = controllerIt->second.ButtonStates.find(button);
        return buttonIt != controllerIt->second.ButtonStates.end() &&
               buttonIt->second.State == KeyState::Released;
    }

    float Input::GetControllerAxis(int controllerID, int axis)
    {
        auto controllerIt = s_Controllers.find(controllerID);
        if (controllerIt == s_Controllers.end()) {
            return 0.0f;
		}

        auto axisIt = controllerIt->second.AxisStates.find(axis);
        if (axisIt == controllerIt->second.AxisStates.end()) {
            return 0.0f;
		}

        return axisIt->second;
    }

    uint8_t Input::GetControllerHat(int controllerID, int hat)
    {
        auto controllerIt = s_Controllers.find(controllerID);
        if (controllerIt == s_Controllers.end()) {
            return 0;
		}

        auto hatIt = controllerIt->second.HatStates.find(hat);
        if (hatIt == controllerIt->second.HatStates.end()) {
            return 0;
		}

        return hatIt->second;
    }

    float Input::GetControllerDeadzone(int controllerID, int axis)
    {
        auto controllerIt = s_Controllers.find(controllerID);
        if (controllerIt == s_Controllers.end()) {
            return 0.0f;
		}

        auto deadzoneIt = controllerIt->second.DeadZones.find(axis);
        if (deadzoneIt == controllerIt->second.DeadZones.end()) {
            return s_DefaultDeadzone;
		}

        return deadzoneIt->second;
    }

    void Input::SetControllerDeadzone(int controllerID, int axis, float deadzone)
    {
        auto controllerIt = s_Controllers.find(controllerID);
        if (controllerIt == s_Controllers.end()) {
            return;
		}

        controllerIt->second.DeadZones[axis] = deadzone;
    }

    void Input::TransitionPressedKeys()
    {
        for (const auto& [key, keyData] : s_KeyData) {
            if (keyData.State == KeyState::Pressed) {
                UpdateKeyState(key, KeyState::Held);
			}
        }
    }

    void Input::TransitionPressedButtons()
    {
        for (const auto& [button, buttonData] : s_MouseData) {
            if (buttonData.State == KeyState::Pressed) {
                UpdateButtonState(button, KeyState::Held);
			}
        }

        for (const auto& [id, controller] : s_Controllers) {
            for (const auto& [button, buttonStates] : controller.ButtonStates) {
                if (buttonStates.State == KeyState::Pressed) {
                    UpdateControllerButtonState(id, button, KeyState::Held);
				}
            }
        }
    }

    void Input::UpdateKeyState(KeyCode key, KeyState newState)
    {
        auto& keyData = s_KeyData[key];
        keyData.Key = key;
        keyData.OldState = keyData.State;
        keyData.State = newState;
    }

    void Input::UpdateButtonState(MouseButton button, KeyState newState)
    {
        auto& mouseData = s_MouseData[button];
        mouseData.Button = button;
        mouseData.OldState = mouseData.State;
        mouseData.State = newState;
    }

    void Input::UpdateControllerButtonState(int controllerID, int button, KeyState newState)
    {
        Controller& controller = s_Controllers[controllerID];
        auto& controllerButtonData = controller.ButtonStates[button];
        controllerButtonData.Button = button;
        controllerButtonData.OldState = controllerButtonData.State;
        controllerButtonData.State = newState;
    }

    void Input::ClearReleasedKeys()
    {
        for (const auto& [key, keyData] : s_KeyData) {
            if (keyData.State == KeyState::Released) {
                UpdateKeyState(key, KeyState::None);
			}
        }

        for (const auto& [button, buttonData] : s_MouseData) {
            if (buttonData.State == KeyState::Released) {
                UpdateButtonState(button, KeyState::None);
			}
        }

        for (const auto& [id, controller] : s_Controllers) {
            for (const auto& [button, buttonStates] : controller.ButtonStates) {
                if (buttonStates.State == KeyState::Released) {
                    UpdateControllerButtonState(id, button, KeyState::None);
				}
            }
        }
    }
}