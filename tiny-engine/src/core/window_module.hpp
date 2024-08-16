#pragma once
#include <any>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "common.hpp"
#include "core/event/events.hpp"

namespace tiny
{
	constexpr inline uint32_t OpenGLVersionMajor = 4;
	constexpr inline uint32_t OpenGLVersionMinor = 5;
}

namespace
{
	class GLFWWindowDestroyer
	{
	public:
		void operator() (GLFWwindow* window)
		{
			glfwDestroyWindow(window);
		}
	};

	static void OpenGLCallback(unsigned source, unsigned type, unsigned id, unsigned severity, int, const char* message, const void*)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:
			TINY_LOG_CRITICAL(message);
			break;

		case GL_DEBUG_SEVERITY_MEDIUM:
			TINY_LOG_ERROR(message);
			break;

		case GL_DEBUG_SEVERITY_LOW:
			TINY_LOG_WARN(message);
			break;

		case GL_DEBUG_SEVERITY_NOTIFICATION:
			TINY_LOG_INFO(message);
			break;
		}

		TINY_LOG_INFO("[OpenGL][Unknown severity]: {}", message);
	}
}

namespace tiny
{
	inline constexpr Key translateKey(uint32_t key)
	{
		switch (key)
		{
		case GLFW_KEY_SPACE:
			return Key::KEY_SPACE;
		case GLFW_KEY_APOSTROPHE:
			return Key::KEY_APOSTROPHE;
		case GLFW_KEY_COMMA:
			return Key::KEY_COMMA;
		case GLFW_KEY_MINUS:
			return Key::KEY_MINUS;
		case GLFW_KEY_PERIOD:
			return Key::KEY_PERIOD;
		case GLFW_KEY_SLASH:
			return Key::KEY_SLASH;
		case GLFW_KEY_0:
			return Key::KEY_0;
		case GLFW_KEY_1:
			return Key::KEY_1;
		case GLFW_KEY_2:
			return Key::KEY_2;
		case GLFW_KEY_3:
			return Key::KEY_3;
		case GLFW_KEY_4:
			return Key::KEY_4;
		case GLFW_KEY_5:
			return Key::KEY_5;
		case GLFW_KEY_6:
			return Key::KEY_6;
		case GLFW_KEY_7:
			return Key::KEY_7;
		case GLFW_KEY_8:
			return Key::KEY_8;
		case GLFW_KEY_9:
			return Key::KEY_9;
		case GLFW_KEY_SEMICOLON:
			return Key::KEY_SEMICOLON;
		case GLFW_KEY_EQUAL:
			return Key::KEY_EQUAL;
		case GLFW_KEY_A:
			return Key::KEY_A;
		case GLFW_KEY_B:
			return Key::KEY_B;
		case GLFW_KEY_C:
			return Key::KEY_C;
		case GLFW_KEY_D:
			return Key::KEY_D;
		case GLFW_KEY_E:
			return Key::KEY_E;
		case GLFW_KEY_F:
			return Key::KEY_F;
		case GLFW_KEY_G:
			return Key::KEY_G;
		case GLFW_KEY_H:
			return Key::KEY_H;
		case GLFW_KEY_I:
			return Key::KEY_I;
		case GLFW_KEY_J:
			return Key::KEY_J;
		case GLFW_KEY_K:
			return Key::KEY_K;
		case GLFW_KEY_L:
			return Key::KEY_L;
		case GLFW_KEY_M:
			return Key::KEY_M;
		case GLFW_KEY_N:
			return Key::KEY_N;
		case GLFW_KEY_O:
			return Key::KEY_O;
		case GLFW_KEY_P:
			return Key::KEY_P;
		case GLFW_KEY_Q:
			return Key::KEY_Q;
		case GLFW_KEY_R:
			return Key::KEY_R;
		case GLFW_KEY_S:
			return Key::KEY_S;
		case GLFW_KEY_T:
			return Key::KEY_T;
		case GLFW_KEY_U:
			return Key::KEY_U;
		case GLFW_KEY_V:
			return Key::KEY_V;
		case GLFW_KEY_W:
			return Key::KEY_W;
		case GLFW_KEY_X:
			return Key::KEY_X;
		case GLFW_KEY_Y:
			return Key::KEY_Y;
		case GLFW_KEY_Z:
			return Key::KEY_Z;
		case GLFW_KEY_LEFT_BRACKET:
			return Key::KEY_LEFT_BRACKET;
		case GLFW_KEY_BACKSLASH:
			return Key::KEY_BACKSLASH;
		case GLFW_KEY_RIGHT_BRACKET:
			return Key::KEY_RIGHT_BRACKET;
		case GLFW_KEY_GRAVE_ACCENT:
			return Key::KEY_GRAVE_ACCENT;
		case GLFW_KEY_ESCAPE:
			return Key::KEY_ESCAPE;
		case GLFW_KEY_ENTER:
			return Key::KEY_ENTER;
		case GLFW_KEY_TAB:
			return Key::KEY_TAB;
		case GLFW_KEY_BACKSPACE:
			return Key::KEY_BACKSPACE;
		case GLFW_KEY_INSERT:
			return Key::KEY_INSERT;
		case GLFW_KEY_DELETE:
			return Key::KEY_DELETE;
		case GLFW_KEY_RIGHT:
			return Key::KEY_RIGHT;
		case GLFW_KEY_LEFT:
			return Key::KEY_LEFT;
		case GLFW_KEY_DOWN:
			return Key::KEY_DOWN;
		case GLFW_KEY_UP:
			return Key::KEY_UP;
		case GLFW_KEY_PAGE_UP:
			return Key::KEY_PAGE_UP;
		case GLFW_KEY_PAGE_DOWN:
			return Key::KEY_PAGE_DOWN;
		case GLFW_KEY_HOME:
			return Key::KEY_HOME;
		case GLFW_KEY_END:
			return Key::KEY_END;
		case GLFW_KEY_CAPS_LOCK:
			return Key::KEY_CAPS_LOCK;
		case GLFW_KEY_SCROLL_LOCK:
			return Key::KEY_SCROLL_LOCK;
		case GLFW_KEY_NUM_LOCK:
			return Key::KEY_NUM_LOCK;
		case GLFW_KEY_PRINT_SCREEN:
			return Key::KEY_PRINT_SCREEN;
		case GLFW_KEY_PAUSE:
			return Key::KEY_PAUSE;
		case GLFW_KEY_F1:
			return Key::KEY_F1;
		case GLFW_KEY_F2:
			return Key::KEY_F2;
		case GLFW_KEY_F3:
			return Key::KEY_F3;
		case GLFW_KEY_F4:
			return Key::KEY_F4;
		case GLFW_KEY_F5:
			return Key::KEY_F5;
		case GLFW_KEY_F6:
			return Key::KEY_F6;
		case GLFW_KEY_F7:
			return Key::KEY_F7;
		case GLFW_KEY_F8:
			return Key::KEY_F8;
		case GLFW_KEY_F9:
			return Key::KEY_F9;
		case GLFW_KEY_F10:
			return Key::KEY_F10;
		case GLFW_KEY_F11:
			return Key::KEY_F11;
		case GLFW_KEY_F12:
			return Key::KEY_F12;
		case GLFW_KEY_F13:
			return Key::KEY_F13;
		case GLFW_KEY_F14:
			return Key::KEY_F14;
		case GLFW_KEY_F15:
			return Key::KEY_F15;
		case GLFW_KEY_F16:
			return Key::KEY_F16;
		case GLFW_KEY_F17:
			return Key::KEY_F17;
		case GLFW_KEY_F18:
			return Key::KEY_F18;
		case GLFW_KEY_F19:
			return Key::KEY_F19;
		case GLFW_KEY_F20:
			return Key::KEY_F20;
		case GLFW_KEY_F21:
			return Key::KEY_F21;
		case GLFW_KEY_F22:
			return Key::KEY_F22;
		case GLFW_KEY_F23:
			return Key::KEY_F23;
		case GLFW_KEY_F24:
			return Key::KEY_F24;
		case GLFW_KEY_KP_0:
			return Key::KEY_KP_0;
		case GLFW_KEY_KP_1:
			return Key::KEY_KP_1;
		case GLFW_KEY_KP_2:
			return Key::KEY_KP_2;
		case GLFW_KEY_KP_3:
			return Key::KEY_KP_3;
		case GLFW_KEY_KP_4:
			return Key::KEY_KP_4;
		case GLFW_KEY_KP_5:
			return Key::KEY_KP_5;
		case GLFW_KEY_KP_6:
			return Key::KEY_KP_6;
		case GLFW_KEY_KP_7:
			return Key::KEY_KP_7;
		case GLFW_KEY_KP_8:
			return Key::KEY_KP_8;
		case GLFW_KEY_KP_9:
			return Key::KEY_KP_9;
		case GLFW_KEY_KP_DECIMAL:
			return Key::KEY_KP_DECIMAL;
		case GLFW_KEY_KP_DIVIDE:
			return Key::KEY_KP_DIVIDE;
		case GLFW_KEY_KP_MULTIPLY:
			return Key::KEY_KP_MULTIPLY;
		case GLFW_KEY_KP_SUBTRACT:
			return Key::KEY_KP_SUBTRACT;
		case GLFW_KEY_KP_ADD:
			return Key::KEY_KP_ADD;
		case GLFW_KEY_KP_ENTER:
			return Key::KEY_KP_ENTER;
		case GLFW_KEY_KP_EQUAL:
			return Key::KEY_KP_EQUAL;
		case GLFW_KEY_LEFT_SHIFT:
			return Key::KEY_LEFT_SHIFT;
		case GLFW_KEY_LEFT_CONTROL:
			return Key::KEY_LEFT_CONTROL;
		case GLFW_KEY_LEFT_ALT:
			return Key::KEY_LEFT_ALT;
		case GLFW_KEY_LEFT_SUPER:
			return Key::KEY_LEFT_SUPER;
		case GLFW_KEY_RIGHT_SHIFT:
			return Key::KEY_RIGHT_SHIFT;
		case GLFW_KEY_RIGHT_CONTROL:
			return Key::KEY_RIGHT_CONTROL;
		case GLFW_KEY_RIGHT_ALT:
			return Key::KEY_RIGHT_ALT;
		case GLFW_KEY_RIGHT_SUPER:
			return Key::KEY_RIGHT_SUPER;
		case GLFW_KEY_MENU:
			return Key::KEY_MENU;
		default:
			return Key::KEY_UNKNOWN;
		}
	}

	inline constexpr KeyboardMods translateKeyMods(uint32_t mods)
	{
		uint32_t result = 0;

		if (mods & GLFW_MOD_SHIFT)
			result |= (uint32_t)KeyboardMods::KEY_MOD_SHIFT;

		if (mods & GLFW_MOD_CONTROL)
			result |= (uint32_t)KeyboardMods::KEY_MOD_CTRL;

		if (mods & GLFW_MOD_ALT)
			result |= (uint32_t)KeyboardMods::KEY_MOD_ALT;

		if (mods & GLFW_MOD_SUPER)
			result |= (uint32_t)KeyboardMods::KEY_MOD_SUPER;

		if (mods & GLFW_MOD_CAPS_LOCK)
			result |= (uint32_t)KeyboardMods::KEY_MOD_CAPS_LOCK;

		if (mods & GLFW_MOD_NUM_LOCK)
			result |= (uint32_t)KeyboardMods::KEY_MOD_NUM_LOCK;

		return (KeyboardMods)result;
	}

	inline constexpr MouseButton translateMouseButton(uint32_t mouseButton)
	{
		switch (mouseButton)
		{
			case GLFW_MOUSE_BUTTON_1:
				return MouseButton::MOUSE_BUTTON_1;

			case GLFW_MOUSE_BUTTON_2:
				return MouseButton::MOUSE_BUTTON_2;

			case GLFW_MOUSE_BUTTON_3:
				return MouseButton::MOUSE_BUTTON_3;

			case GLFW_MOUSE_BUTTON_4:
				return MouseButton::MOUSE_BUTTON_4;

			case GLFW_MOUSE_BUTTON_5:
				return MouseButton::MOUSE_BUTTON_5;

			case GLFW_MOUSE_BUTTON_6:
				return MouseButton::MOUSE_BUTTON_6;

			case GLFW_MOUSE_BUTTON_7:
				return MouseButton::MOUSE_BUTTON_7;

			case GLFW_MOUSE_BUTTON_8:
				return MouseButton::MOUSE_BUTTON_8;

			default:
				return MouseButton::MOUSE_BUTTON_UNKNOWN;
		}
	}
}

namespace tiny
{
	struct WindowProperties
	{
		enum Position : int { DEFAULT = -1 };

		std::string windowTitle = "TinyEngine v1.0";
		glm::vec2 dimensions = glm::vec2(640, 480);
		glm::vec2 position = glm::vec2(Position::DEFAULT, Position::DEFAULT);
		glm::vec2 minSize = glm::vec2(0), maxSize = dimensions;
	};

	class WindowModule
	{
	public:
		void initialize(const WindowProperties& properties)
		{
			m_Properties = properties;

			auto* glfwWindow = glfwCreateWindow(
				(int)properties.dimensions.x,
				(int)properties.dimensions.y,
				properties.windowTitle.c_str(),
				nullptr, nullptr
			);

			if (!glfwWindow)
				TINY_LOG_CRITICAL("Failed to create a valid GLFW window. GLFW is probably uninitialized");

			m_Window.reset(glfwWindow);

			// Window Data:
			setWindowDataPointer();
			setWindowCallbacks();
		}

		void shutdown()
		{
			m_Window.reset();
		}

	public:
		void setEventCallback(const EventCallback& callback)
		{
			m_WindowData.eventCallback = callback;
		}

		void setProperties(const WindowProperties& properties)
		{
			auto* window = m_Window.get();
			m_Properties = properties;

			// Current Size:
			glfwSetWindowSize(window, (int)properties.dimensions.x, (int)properties.dimensions.y);

			// Size Limits:
			glfwSetWindowSizeLimits(
				window,
				(int)properties.minSize.x,
				(int)properties.minSize.y,
				(int)properties.maxSize.x,
				(int)properties.maxSize.y
			);

			// Position:
			int x = (int)properties.position.x, y = (int)properties.position.y;
			if (properties.position.x == WindowProperties::Position::DEFAULT)
				x = GLFW_DONT_CARE;
			if (properties.position.y == WindowProperties::Position::DEFAULT)
				y = GLFW_DONT_CARE;

			glfwSetWindowPos(window, x, y);

			// Title:
			glfwSetWindowTitle(window, properties.windowTitle.c_str());
		}

		void pollEvents()
		{
			glfwPollEvents();
		}

	public:
		void hide()
		{
			glfwHideWindow(m_Window.get());
		}

		void show()
		{
			glfwShowWindow(m_Window.get());
		}

		bool isOpen() const
		{
			return !glfwWindowShouldClose(m_Window.get());
		}

	public:
		void swapBuffers()
		{
			glfwSwapBuffers(m_Window.get());
		}

	public:
		inline WindowProperties getProperties() const { return m_Properties; }

		inline GLFWwindow* getHandle() const { return m_Window.get(); };

	private:
		void setWindowDataPointer()
		{
			glfwSetWindowUserPointer(m_Window.get(), &m_WindowData);
		}

		void setWindowCallbacks()
		{
			// Status:
			glfwSetWindowCloseCallback(m_Window.get(), [](GLFWwindow* window) {
				auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				WindowCloseEvent event;
				data->eventCallback(event);
			});

			glfwSetWindowMaximizeCallback(m_Window.get(), [](GLFWwindow* window, int maximized) {
				auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				WindowMaximizedEvent event(maximized);
				data->eventCallback(event);
			});

			glfwSetWindowFocusCallback(m_Window.get(), [](GLFWwindow* window, int focus) {
				auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				WindowFocusEvent event(focus);
				data->eventCallback(event);
			});

			// Size:
			glfwSetWindowSizeCallback(m_Window.get(), [](GLFWwindow* window, int width, int height) {
				auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				WindowResizeEvent event(width, height);
				data->eventCallback(event);
			});

			glfwSetFramebufferSizeCallback(m_Window.get(), [](GLFWwindow* window, int width, int height) {
				auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				WindowFramebufferEvent event(width, height);
				data->eventCallback(event);
			});

			glfwSetWindowContentScaleCallback(m_Window.get(), [](GLFWwindow* window, float xScale, float yScale) {
				auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				WindowContentScaleEvent event(xScale, yScale);
				data->eventCallback(event);
			});

			// Position:
			glfwSetWindowPosCallback(m_Window.get(), [](GLFWwindow* window, int xPos, int yPos) {
				auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				WindowMovedEvent event(xPos, yPos);
				data->eventCallback(event);
			});

			// Keyboard:
			glfwSetKeyCallback(m_Window.get(), [](GLFWwindow* window, int key, int scancode, int action, int mods) {
				auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				switch (action)
				{
					case GLFW_PRESS:
					{
						Key convertedKey = translateKey(key);
						KeyboardMods convertedMods = translateKeyMods(mods);

						KeyPressedEvent event(convertedKey, convertedMods);
						data->eventCallback(event);
					} break;

					case GLFW_RELEASE:
					{
						Key convertedKey = translateKey(key);
						KeyboardMods convertedMods = translateKeyMods(mods);

						KeyReleasedEvent event(convertedKey, convertedMods);
						data->eventCallback(event);
					} break;

					case GLFW_REPEAT:
					{
						Key convertedKey = translateKey(key);
						KeyboardMods convertedMods = translateKeyMods(mods);

						KeyRepeatEvent event(convertedKey, convertedMods);
						data->eventCallback(event);
					} break;

					default:
					{
						TINY_LOG_WARN("Invalid keyboard action passed to window callback: {}", action);
					} break;
				}
			});

			// Mouse:
			glfwSetCursorPosCallback(m_Window.get(), [](GLFWwindow* window, double x, double y) {
				auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				MouseMovedEvent event(x, y);
				data->eventCallback(event);
			});

			glfwSetMouseButtonCallback(m_Window.get(), [](GLFWwindow* window, int button, int action, int mods) {
				auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				switch (action)
				{
					case GLFW_PRESS:
					{
						MouseButton convertedButton = translateMouseButton(button);
						MousePressedEvent event(convertedButton);
						data->eventCallback(event);
					} break;

					case GLFW_RELEASE:
					{
						MouseButton convertedButton = translateMouseButton(button);
						MouseReleasedEvent event(convertedButton);
						data->eventCallback(event);
					} break;

					default:
					{
						TINY_LOG_WARN("Invalid keyboard action passed to window callback: {}", action);
					} break;
				}
			});

			glfwSetScrollCallback(m_Window.get(), [](GLFWwindow* window, double xOffset, double yOffset) {
				auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				MouseScrollEvent event(xOffset, yOffset);
				data->eventCallback(event);
			});
		}
		
	private:
		std::unique_ptr<GLFWwindow, GLFWWindowDestroyer> m_Window;
		WindowProperties m_Properties;

		struct WindowData
		{
			EventCallback eventCallback;
		} m_WindowData;
	};

	inline void setupWindowing(WindowModule& windowModule, const WindowProperties& properties)
	{
		if (!glfwInit())
			TINY_LOG_CRITICAL("Failed to initialize GLFW");

#ifdef TINY_DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

#ifdef TINY_PLATFORM_APPLE
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OpenGLVersionMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OpenGLVersionMinor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
		glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		windowModule.initialize(properties);

		GLFWwindow* glfwWindow = windowModule.getHandle();
		glfwMakeContextCurrent(glfwWindow);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			TINY_LOG_CRITICAL("Failed to load OpenGL functions.");

#ifdef TINY_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

		if (OpenGLVersionMajor >= 4 && OpenGLVersionMinor >= 3)
		{
			glDebugMessageCallback(OpenGLCallback, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
		}
#endif
	}
}