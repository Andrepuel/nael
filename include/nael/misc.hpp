#include <cstdlib>

#if defined( _MSC_VER )
#define DELETE_METHOD(signature) private: \
	inline signature { abort(); }; \
	public:
#else
#define DELETE_METHOD(signature) signature = delete;
#endif
