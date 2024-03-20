#ifndef KEY_CODES_H
#define KEY_CODES_H

#include <ostream>

namespace Cober
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	enum class KeyState
	{
		None = -1,
		Pressed,
		Held,
		Released
	};

	enum class CursorMode
	{
		Normal = 0,
		Hidden = 1,
		Locked = 2
	};

	typedef enum class MouseButton : uint16_t
	{
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Left = Button0,
		Right = Button1,
		Middle = Button2
	} Button;


	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, MouseButton button)
	{
		os << static_cast<int32_t>(button);
		return os;
	}
}

// From glfw3.h
#define KEY_SPACE           ::Cober::Key::Space
#define KEY_APOSTROPHE      ::Cober::Key::Apostrophe    /* ' */
#define KEY_COMMA           ::Cober::Key::Comma         /* , */
#define KEY_MINUS           ::Cober::Key::Minus         /* - */
#define KEY_PERIOD          ::Cober::Key::Period        /* . */
#define KEY_SLASH           ::Cober::Key::Slash         /* / */
#define KEY_0               ::Cober::Key::D0
#define KEY_1               ::Cober::Key::D1
#define KEY_2               ::Cober::Key::D2
#define KEY_3               ::Cober::Key::D3
#define KEY_4               ::Cober::Key::D4
#define KEY_5               ::Cober::Key::D5
#define KEY_6               ::Cober::Key::D6
#define KEY_7               ::Cober::Key::D7
#define KEY_8               ::Cober::Key::D8
#define KEY_9               ::Cober::Key::D9
#define KEY_SEMICOLON       ::Cober::Key::Semicolon     /* ; */
#define KEY_EQUAL           ::Cober::Key::Equal         /* = */
#define KEY_A               ::Cober::Key::A
#define KEY_B               ::Cober::Key::B
#define KEY_C               ::Cober::Key::C
#define KEY_D               ::Cober::Key::D
#define KEY_E               ::Cober::Key::E
#define KEY_F               ::Cober::Key::F
#define KEY_G               ::Cober::Key::G
#define KEY_H               ::Cober::Key::H
#define KEY_I               ::Cober::Key::I
#define KEY_J               ::Cober::Key::J
#define KEY_K               ::Cober::Key::K
#define KEY_L               ::Cober::Key::L
#define KEY_M               ::Cober::Key::M
#define KEY_N               ::Cober::Key::N
#define KEY_O               ::Cober::Key::O
#define KEY_P               ::Cober::Key::P
#define KEY_Q               ::Cober::Key::Q
#define KEY_R               ::Cober::Key::R
#define KEY_S               ::Cober::Key::S
#define KEY_T               ::Cober::Key::T
#define KEY_U               ::Cober::Key::U
#define KEY_V               ::Cober::Key::V
#define KEY_W               ::Cober::Key::W
#define KEY_X               ::Cober::Key::X
#define KEY_Y               ::Cober::Key::Y
#define KEY_Z               ::Cober::Key::Z
#define KEY_LEFT_BRACKET    ::Cober::Key::LeftBracket   /* [ */
#define KEY_BACKSLASH       ::Cober::Key::Backslash     /* \ */
#define KEY_RIGHT_BRACKET   ::Cober::Key::RightBracket  /* ] */
#define KEY_GRAVE_ACCENT    ::Cober::Key::GraveAccent   /* ` */
#define KEY_WORLD_1         ::Cober::Key::World1        /* non-US #1 */
#define KEY_WORLD_2         ::Cober::Key::World2        /* non-US #2 */

/* Function keys */
#define KEY_ESCAPE          ::Cober::Key::Escape
#define KEY_ENTER           ::Cober::Key::Enter
#define KEY_TAB             ::Cober::Key::Tab
#define KEY_BACKSPACE       ::Cober::Key::Backspace
#define KEY_INSERT          ::Cober::Key::Insert
#define KEY_DELETE          ::Cober::Key::Delete
#define KEY_RIGHT           ::Cober::Key::Right
#define KEY_LEFT            ::Cober::Key::Left
#define KEY_DOWN            ::Cober::Key::Down
#define KEY_UP              ::Cober::Key::Up
#define KEY_PAGE_UP         ::Cober::Key::PageUp
#define KEY_PAGE_DOWN       ::Cober::Key::PageDown
#define KEY_HOME            ::Cober::Key::Home
#define KEY_END             ::Cober::Key::End
#define KEY_CAPS_LOCK       ::Cober::Key::CapsLock
#define KEY_SCROLL_LOCK     ::Cober::Key::ScrollLock
#define KEY_NUM_LOCK        ::Cober::Key::NumLock
#define KEY_PRINT_SCREEN    ::Cober::Key::PrintScreen
#define KEY_PAUSE           ::Cober::Key::Pause
#define KEY_F1              ::Cober::Key::F1
#define KEY_F2              ::Cober::Key::F2
#define KEY_F3              ::Cober::Key::F3
#define KEY_F4              ::Cober::Key::F4
#define KEY_F5              ::Cober::Key::F5
#define KEY_F6              ::Cober::Key::F6
#define KEY_F7              ::Cober::Key::F7
#define KEY_F8              ::Cober::Key::F8
#define KEY_F9              ::Cober::Key::F9
#define KEY_F10             ::Cober::Key::F10
#define KEY_F11             ::Cober::Key::F11
#define KEY_F12             ::Cober::Key::F12
#define KEY_F13             ::Cober::Key::F13
#define KEY_F14             ::Cober::Key::F14
#define KEY_F15             ::Cober::Key::F15
#define KEY_F16             ::Cober::Key::F16
#define KEY_F17             ::Cober::Key::F17
#define KEY_F18             ::Cober::Key::F18
#define KEY_F19             ::Cober::Key::F19
#define KEY_F20             ::Cober::Key::F20
#define KEY_F21             ::Cober::Key::F21
#define KEY_F22             ::Cober::Key::F22
#define KEY_F23             ::Cober::Key::F23
#define KEY_F24             ::Cober::Key::F24
#define KEY_F25             ::Cober::Key::F25

/* Keypad */
#define KEY_KP_0            ::Cober::Key::KP0
#define KEY_KP_1            ::Cober::Key::KP1
#define KEY_KP_2            ::Cober::Key::KP2
#define KEY_KP_3            ::Cober::Key::KP3
#define KEY_KP_4            ::Cober::Key::KP4
#define KEY_KP_5            ::Cober::Key::KP5
#define KEY_KP_6            ::Cober::Key::KP6
#define KEY_KP_7            ::Cober::Key::KP7
#define KEY_KP_8            ::Cober::Key::KP8
#define KEY_KP_9            ::Cober::Key::KP9
#define KEY_KP_DECIMAL      ::Cober::Key::KPDecimal
#define KEY_KP_DIVIDE       ::Cober::Key::KPDivide
#define KEY_KP_MULTIPLY     ::Cober::Key::KPMultiply
#define KEY_KP_SUBTRACT     ::Cober::Key::KPSubtract
#define KEY_KP_ADD          ::Cober::Key::KPAdd
#define KEY_KP_ENTER        ::Cober::Key::KPEnter
#define KEY_KP_EQUAL        ::Cober::Key::KPEqual

#define KEY_LEFT_SHIFT      ::Cober::Key::LeftShift
#define KEY_LEFT_CONTROL    ::Cober::Key::LeftControl
#define KEY_LEFT_ALT        ::Cober::Key::LeftAlt
#define KEY_LEFT_SUPER      ::Cober::Key::LeftSuper
#define KEY_RIGHT_SHIFT     ::Cober::Key::RightShift
#define KEY_RIGHT_CONTROL   ::Cober::Key::RightControl
#define KEY_RIGHT_ALT       ::Cober::Key::RightAlt
#define KEY_RIGHT_SUPER     ::Cober::Key::RightSuper
#define KEY_MENU            ::Cober::Key::Menu

// Mouse
#define MOUSE_BUTTON_LEFT    ::Cober::Button::Left
#define MOUSE_BUTTON_RIGHT   ::Cober::Button::Right
#define MOUSE_BUTTON_MIDDLE  ::Cober::Button::Middle

#endif