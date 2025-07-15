#ifndef WIDGETS_H
#define WIDGETS_H

#include <iostream>
#include <memory>
#include <vector>

#include <filePaths.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <Shaderh.h>

#define glCheckError() glCheckError_(__FILE__, __LINE__)
GLenum glCheckError_(const char* file, int line);



class Shapes {
public:
	static float Square[24];
};




class Image2D {
private:
	std::shared_ptr<unsigned char*> data;

public:
	Image2D(bool useRGBA, std::string filePath);
	Image2D(Image2D&&) = default;
	~Image2D();

	bool useRGBA;
	int width, height, channels;

	std::shared_ptr<unsigned char*> getData() const;
};




class Texture2D {
private:
	unsigned int textureAddr;
	int textureId;

public:
	Texture2D();
	Texture2D(Image2D& source);
	Texture2D(Image2D&& source);

	void setId(int id);
	int getId() const;
	unsigned int getAddr() const;
};

template <class Base>
class enable_shared_from_base
	: public std::enable_shared_from_this<Base>
{
protected:
	template <class Derived>
	std::shared_ptr<Derived> shared_from_base()
	{
		return std::static_pointer_cast<Derived>(std::enable_shared_from_this<Base>::shared_from_this());
	}
};

class SqrWidget : public enable_shared_from_base<SqrWidget> {
protected:
	unsigned int VAO, VBO;
	unsigned int bottom, left, height, width;

public:
	SqrWidget();

	virtual void draw() const = 0;
};

class FrameBuffer : protected SqrWidget {
private:
	std::vector<std::shared_ptr<SqrWidget>> Container;
	Shader shader;

public:
	FrameBuffer(Shader shader);

	void draw() const override;
	
	void insert(std::shared_ptr<SqrWidget>) {
		/*TO IMPLEMENT YET*/
	}
};

class TimerClock : protected SqrWidget, public std::enable_shared_from_this<TimerClock> {
private:
	Texture2D bg;
	Shader shaderBase;

public:
	TimerClock();

	typedef std::shared_ptr<TimerClock> pointer;

	static TimerClock::pointer getTimerClock();

	void draw() const override;
};



class ResourceManager {
public:
	static std::shared_ptr<unsigned char*> loadImage2D(std::string filePath, Image2D* image);

};

#endif