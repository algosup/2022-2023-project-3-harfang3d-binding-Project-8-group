// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/global_object.h"
#include "foundation/signal.h"
#include "platform/input_device.h"
#include <map>

namespace hg {

struct InputDeviceProvider {
	virtual ~InputDeviceProvider() {}

	/// Return available device names.
	virtual std::vector<std::string> GetDevices() const = 0;
	/// Instantiate a device.
	virtual std::shared_ptr<InputDevice> GetDevice(const std::string &name) = 0;
};

class InputSystem {
public:
	/// Update all system devices.
	void Update() const;

	/// Return a list of devices available on this system.
	std::vector<std::string> GetDevices() const;
	/// Get a device from its identifier.
	std::shared_ptr<InputDevice> GetDevice(const std::string &name) const;

	/// Register a direct device. GetDevice will return this device when queried for its id.
	bool RegisterDirectDevice(const std::string &name, std::shared_ptr<InputDevice> device);
	/// Unregister a direct device.
	void UnregisterDirectDevice(const std::string &name);

	/// Register a device provider.
	void RegisterDeviceProvider(const std::string &name, std::unique_ptr<InputDeviceProvider> provider);
	/// Unregister a device provider.
	void UnregisterDeviceProvider(const std::string &name);

	/// Make sure that no device reports inaccurate value when the application window comes back into focus.
	void OnWindowFocusChange() const;

	Signal<void(const std::string &)> text_input_signal; ///< Emitted when a new character is input.

protected:
	std::map<std::string, std::shared_ptr<InputDevice>> direct_devices;
	std::map<std::string, std::unique_ptr<InputDeviceProvider>> providers;

	mutable std::map<std::string, std::shared_ptr<InputDevice>> devices;
};

void RegisterInputDevices(InputSystem &system);

extern global_object<InputSystem> g_input_system;

} // namespace hg
