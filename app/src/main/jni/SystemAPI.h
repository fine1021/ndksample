#ifdef __cplusplus
extern "C" {
#endif

	/* property_get: returns the length of the value which will never be
	** greater than PROPERTY_VALUE_MAX - 1 and will always be zero terminated.
	** (the length does not include the terminating zero).
	**
	** If the property read fails or returns an empty value, the default
	** value is used (if nonnull).
	*/
#define PROPERTY_KEY_MAX   32
#define PROPERTY_VALUE_MAX  92

	/* property_get: returns the length of the value which will never be
	** greater than PROPERTY_VALUE_MAX - 1 and will always be zero terminated.
	** (the length does not include the terminating zero).
	**
	** If the property read fails or returns an empty value, the default
	** value is used (if nonnull).
	*/
	int system_property_get(const char *key, char *value, const char *default_value);

	/* property_set: returns 0 on success, < 0 on failure
	*/
	int system_property_set(const char *key, const char *value);


#ifdef __cplusplus
}
#endif