#include "resource.h"

Resource::Resource(const std::string & filePath)
	: mFilePath(filePath)
{
}

ImageResource::ImageResource(const std::string & filePath, image_data && data)
	: Resource(filePath)
	, mImageData(std::move(data))
{
}
