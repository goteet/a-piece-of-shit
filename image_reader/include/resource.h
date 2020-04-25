#pragma once
#include <string>
#include "cxx_file.h"
#include "image.h"

class Resource
{
public:
	Resource(const std::string& filePath);

	const std::string& GetFilePath() const { return mFilePath; }

	virtual void ParseFile(const cxx::file_buffer& fileBuffer) { }

private:
	std::string mFilePath;
};

class ImageResource : public Resource
{
public:
	ImageResource(const std::string& filePath, image_data&& data);

	const image_data& GetImageData() const { return mImageData; }

	virtual void ParseFile(const cxx::file_buffer& fileBuffer) { }

private:
	image_data mImageData;
};

