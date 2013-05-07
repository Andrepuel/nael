#ifndef __IMAGE_HPP_0418294915810_
#define __IMAGE_HPP_0418294915810_

#include <string>

namespace nael {

/**
 * Represents a image. Has size and data. RGBA is always used
 *
 * Only png is support at the moment.
 *
 * @note It is recomended that you use shared_ptr to avoid duplicating the image buffer
 *
 * @author André Puel
 * @date 7, May, 2013
 */
class Image {
public:
	/**
	 * Loads the png image at @param path
	 */
	Image(const std::string& path);

	/**
	 * @note The data buffer is copied, may be slow
	 */
	Image(const Image& copy);

	Image(Image&& move);

	virtual ~Image();

	/**
	 * @return width of the image
	 */
	inline unsigned w() const {
		return _w;
	}
	/**
	 * @return height of the image
	 */
	inline unsigned h() const {
		return _h;
	}

	/**
	 * @return raw data of the image. It is unaligned and in RGBA format.
	 */
	inline const void* bytes() const {
		return _data;
	}

	/**
	 * If this image size does not match @param that's size the buffer will be reallocated.
	 * @note The data buffer is copyed, may be slow
	 */
	Image& operator=(const Image& that);
private:
	unsigned _w,_h;
	char* _data;;
};

}

#endif
