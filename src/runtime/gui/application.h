// Copyright Colby Hall. All Rights Reserved.

#include "core/math/vector2.h"
#include "gpu/device.h"
#include "gui/gui.h"

OP_GUI_NAMESPACE_BEGIN

class Builder;

/**
 * Represents a window that can have a GUI rendered onto.
 *
 * This includes:
 * - Labels
 * - Buttons
 * - Sliders
 * - Check boxes
 * - Text boxes
 * - Radio buttons
 * - Sub windows
 */
class Application {
public:
	explicit Application(const gpu::Device& device) : m_device(device.to_shared()) {}

	/**
	 * Sets the minimum size the window can be resized to.
	 *
	 * @param size The minimum size the window can be resized to.
	 * @return A reference to this object.
	 */
	OP_ALWAYS_INLINE Application& set_min_size(const Vector2<u32>& size) {
		m_min_size = size;
		return *this;
	}

	/**
	 * Sets the maximum size the window can be resized to.
	 *
	 * @param size The maximum size the window can be resized to.
	 * @return A reference to this object.
	 */
	OP_ALWAYS_INLINE Application& set_max_size(const Vector2<u32>& size) {
		m_max_size = size;
		return *this;
	}

	/**
	 * Sets the size of the window.
	 *
	 * @param size The size of the window.
	 * @return A reference to this object.
	 */
	OP_ALWAYS_INLINE Application& set_size(const Vector2<u32>& size) {
		m_size = size;
		return *this;
	}

	/**
	 * Runs the application.
	 *
	 * @param callable The callable to run.
	 */
	void run(FunctionRef<void(Builder&)> callable);

private:
	gpu::Shared<gpu::Device> m_device;

	Vector2<u32> m_min_size;
	Vector2<u32> m_max_size;
	Vector2<u32> m_size;
};

OP_GUI_NAMESPACE_END