#include <nael/image.hpp>
#include <png.hpp>

namespace nael {

Image::Image(const std::string& path) {
	png::image<png::rgba_pixel> image(path);

	_w = image.get_width();
	_h = image.get_height();

	_data = new char[_w*_h*4];
	png::rgba_pixel* data = reinterpret_cast<png::rgba_pixel*>(_data);
	for( unsigned eachRow = 0; eachRow<_h;++eachRow ) {
		auto row = image.get_row(eachRow);
		for( unsigned eachCol = 0; eachCol<_w;++eachCol ) {
			data[eachCol+eachRow*_w] = row[eachCol];
		}
	}

}

Image::Image(const Image& copy)
: _w(0), _h(0), _data(NULL)
{
	*this = copy;
}

Image::Image(Image&& move)
: _w(move._w), _h(move._h), _data(move._data)
{
	move._w = 0;
	move._h = 0;
	move._data = NULL;
}

Image::~Image() {
	delete[] _data;
}

Image& Image::operator=(const Image& that) {
	if( _w != that._w || _h != that._h ) {
		_w = that._w;
		_h = that._h;
		delete[] _data;
		_data = new char[_w*_h*4];
	}

	memcpy(_data,that._data,_w*_h*4);

	return *this;
}

}
