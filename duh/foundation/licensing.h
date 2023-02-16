// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

namespace hg {

/// License system interface
struct Licensing {
	virtual ~Licensing() {}

	// Event handler for the license system.
	virtual void onLicensingEvent(const char *) = 0;
	/// Request an update of the application license.
	virtual bool updateLicence(const char *) = 0;
};

} // namespace hg
