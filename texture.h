#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <string>

bool textureFromFile(const std::string &path, const std::string &directory, unsigned int &textureId);
bool textureFromFile(const std::string &path, unsigned int &textureId);


#endif//__TEXTURE_H__
