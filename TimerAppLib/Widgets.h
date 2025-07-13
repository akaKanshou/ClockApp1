#ifndef WIDGETS_H
#define WIDGETS_H

#include <iostream>
#include <memory>

#include <glm/glm.hpp>
#include <Shaderh.h>

class Shapes {
public:
	static float Square[24];
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

class SquareWidget : public enable_shared_from_base<SquareWidget> {
protected:
	Shader shader;
	unsigned int VAO, VBO;
	unsigned int bottom, left, height, width;

public:
	SquareWidget();
	SquareWidget(Shader shader);

	virtual void draw() const = 0;
};

class FrameBuffer : protected SquareWidget {
private:
	std::vector<std::shared_ptr<SquareWidget>> Container;

public:
	FrameBuffer(Shader shader);

	void draw() const override;
	
	void insert(std::shared_ptr<SquareWidget>);
};

class TimerClock {

};

#endif