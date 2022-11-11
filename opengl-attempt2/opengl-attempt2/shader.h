#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <string>

class Shader
{
public:
	unsigned int ID;

	Shader(const char* vertPath, const char* fragPath);

	void use();


	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec4(const std::string& name, int x, int y, int z, int w) const;
};

#endif