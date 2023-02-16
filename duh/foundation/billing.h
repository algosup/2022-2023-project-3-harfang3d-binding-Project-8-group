// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

namespace hg {

/// Billing system abstract interface
struct Billing {
	virtual ~Billing() {}

	// Process a billing event.
	virtual void onBillingEvent(const std::string &event, const std::string &item) = 0;

	/// Returns true if the platform supports billing.
	virtual bool isBillingServiceSupported() = 0;
	/// Request a new purchase.
	virtual bool requestPurchase(const std::string &) = 0;
	/// Confirm a purchase.
	virtual bool confirmPurchase(const std::string &) = 0;
	/// Restore managed purchase states.
	virtual bool restorePurchases() = 0;
};

} // namespace hg
