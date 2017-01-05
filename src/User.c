#include "User.h"

struct passwd * GetUserPW() {
	// Get and return the user passwd structure
	return getpwuid(getuid());
}