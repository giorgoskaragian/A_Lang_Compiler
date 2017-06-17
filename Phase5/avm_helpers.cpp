
bool isInteger(double v)
{
	int iv = (int)v;
	return (double)iv == v;
}

bool isNumber(const char* str)
{
	if (!str) {
		return false;
	}

	if (str[0] == 0) {
		return false;
	}

	if (str[0] == '-' || str[0] == '+') {
		++str;
	}

	// Check the whole string for valid characters
	bool dotFound = false;
	while (*str != 0) {
		char c = *str;
		if (c >= '0' && c <= '9') {
			++str;
			continue;
		}

		if (c == '.' && !dotFound) {
			dotFound = true;
			++str;
			continue;
		}

		return false;
	}

	return true;
}