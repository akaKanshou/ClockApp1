#ifndef WIDGETS_H
#define WIDGETS_H

#include <iostream>
#include <memory>
#include <vector>

#include <filePaths.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <Shaderh.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#define glCheckError() glCheckError_(__FILE__, __LINE__)
GLenum glCheckError_(const char* file, int line);

template<typename T, typename U>
T ceiling(T divident, U divisor);

std::string to_string(glm::ivec3 vec);
std::string to_string(unsigned long long time);

unsigned long long to_ull(std::string str);
unsigned long long to_ull(glm::ivec3 vec);

glm::ivec3 to_vec(unsigned long long time);
glm::ivec3 to_vec(std::string str);


class Font {
public:
	struct Character {
		unsigned int TextureAddr;
		glm::ivec2 Size;
		glm::ivec2 Bearing;
		unsigned int Advance;
	};

	std::vector<Character> CharSet;
	Font() = default;
	Font(FT_Face& face);
};


class TextLib {
private:
	FT_Library Library;
	Shader textShader;
	std::vector<Font> Fonts;

	unsigned int VAO, VBO;

public:
	enum ALIGNMENT {
		LEFT_ALIGNED = 0,
		RIGHT_ALIGNED = 1,
		CENTER_ALIGNED = 2,

		TOP_ALIGNED = 3,
		MID_ALIGNED = 4,
		BOTTOM_ALIGNED = 5,
	};

	enum FONT {
		ARIAL = 0,
		FUTURA = 1
	};

	TextLib(Shader&& shader);
	~TextLib() = default;

	Font loadFont(const char* fontname, int size);

	void draw(ALIGNMENT x_alignment, ALIGNMENT y_alignment, std::string text, int x, int y, int scale, glm::vec3 color, FONT font);
	void draw(ALIGNMENT x_alignment, ALIGNMENT y_alignment, std::string text, int x, int y, int scale, FONT font, glm::vec4& box);
};


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

	virtual void draw() const;
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

class Button : protected SqrWidget{
	Texture2D buttonTexture;
	Shader buttonShader;
	TextLib& textLib;;

public:
	Button(TextLib& textLib);
	Button(TextLib& textLib, std::string vertexShader, std::string fragmentShader, std::string imageLoc);

	void draw(float angle) const;

	void configure(std::string vertexShader, std::string fragmentShader, bool useRGBA, std::string imageLoc);
};


class TimerClock : protected SqrWidget, public std::enable_shared_from_this<TimerClock> {
private:
	Texture2D bg;
	Shader shaderBase;
	TextLib& textLib;
	Button button;
	std::string current;
	glm::ivec3 curTime;
	glm::vec4 boxCoords;
	glm::vec2 mousePosition;
	int textPointer;
	unsigned long long clockticks, target;
	
public:
	typedef std::shared_ptr<TimerClock> pointer;

	TimerClock(TextLib& textLib);
	static TimerClock::pointer getTimerClock(TextLib& textLib);

	void update();
	void draw() const override;

	void getInput(int key);
	void getInput(int xpos, int ypos);
	void getInput(int button, int action, int mods);
};



class ResourceManager {
public:
	static std::shared_ptr<unsigned char*> loadImage2D(std::string filePath, Image2D* image);

};



class Screen : public std::enable_shared_from_this<Screen> {
private:
	TimerClock Timer;
	float lastX, lastY;
	bool firstMouse;
	float lastTime;
	unsigned long long ticks, lastTick;
	std::vector<bool> pressedLastFrame;

public:
	typedef std::shared_ptr<Screen> pointer;
	Screen(TextLib& textLib);
	static Screen::pointer getScreen(TextLib& textLib);

	static Screen::pointer currentScreen;
	
	void drawTimer();
	void processInputs(GLFWwindow* window);

	void cursor_callback(GLFWwindow* window, double xpos, double ypos);
	void button_callback(GLFWwindow* window, int button, int action, int mods);

	bool doDraw(float curTime);
	double runTime() const;
};

#endif