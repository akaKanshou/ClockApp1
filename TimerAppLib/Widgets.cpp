#include "Widgets.h"

#include <GLFW/glfw3.h>

GLenum glCheckError_(const char* file, int line) {
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM: error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE: error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION: error = "INVALID_OPERATION"; break;
		case GL_OUT_OF_MEMORY: error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")";
		std::cout << std::endl;
	}
	return errorCode;
}

template<typename T, typename U>
T ceiling(T divident, U divisor) {
	return divident / divisor + (bool)(divident % divisor);
}


std::string to_string(glm::ivec3 vec) {
	std::string answer = "";

	if (vec[0] < 10) {
		answer += "0";
	}
	answer += std::to_string(vec[0]) + ":";

	if (vec[1] < 10) {
		answer += "0";
	}
	answer += std::to_string(vec[1]) + ":";

	if (vec[2] < 10) {
		answer += "0";
	}
	answer += std::to_string(vec[2]);

	return answer;
}

std::string to_string(unsigned long long time) {
	time = ceiling(time, 1000'000);
	std::string answer = "";

	if (time / 3600 < 10) {
		answer += "0";
	}
	answer += std::to_string(time / 3600) + ":";

	if (time / 60 % 60 < 10) {
		answer += "0";
	}
	answer += std::to_string(time / 60 % 60) + ":";

	if (time % 60 < 10) {
		answer += "0";
	}
	answer += std::to_string(time % 60);

	return answer;
}

unsigned long long to_ull(std::string str) {
	unsigned long long answer = 0;
	answer += std::stoull(str.substr(0, 2));
	answer += std::stoull(str.substr(3, 2));
	answer += std::stoull(str.substr(6, 2));
	return answer * 1e6;
}

unsigned long long to_ull(glm::ivec3 vec) {
	return (vec[0] * 3600 + vec[1] * 60 + vec[2]) * 1e6;
}

glm::ivec3 to_vec(unsigned long long time) {
	time = ceiling(time, 1000'000);
	glm::ivec3 vec = { time / 3600,time / 60 % 60,time % 60 };
	return vec;
}

glm::ivec3 to_vec(std::string str) {
	return to_vec(to_ull(str));
}



TextLib::TextLib(Shader&& shader) : textShader(shader), VAO(0), VBO(0), Library() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (FT_Init_FreeType(&Library)) {
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library\n";
		return;
	}

	Fonts.push_back(loadFont(FONT_PATH"/arial.ttf", 120));
	Fonts.push_back(loadFont(FONT_PATH"/Futura-M.ttf", 120));
	
	FT_Done_FreeType(Library);
}

Font TextLib::loadFont(const char* fontname, int size) {
	FT_Face face;
	if (FT_New_Face(Library, fontname, 0, &face)) {
		std::cout << "ERROR::FREETYPE: Could not load font: " << fontname << "\n";
	}

	FT_Set_Pixel_Sizes(face, 0, size);

	Font newFont(face);

	FT_Done_Face(face);
	return newFont;
}

void TextLib::draw(ALIGNMENT x_alignment, ALIGNMENT y_alignment, std::string text, int x, int y, int scale, glm::vec3 color, FONT font) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	textShader.use();
	textShader.setVec3("textColor", color.x, color.y, color.z);
	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 800.0f);
	textShader.setMat4("projection", projection);

	if (x_alignment == TextLib::LEFT_ALIGNED) {
		glBindVertexArray(VAO);
		for (char c : text) {
			Font::Character chr = Fonts[font].CharSet[c];
			
			float xpos = x + chr.Bearing.x * scale;
			float ypos = y - (chr.Size.y - chr.Bearing.y) * scale;

			if (y_alignment == MID_ALIGNED) {
				ypos -= chr.Bearing.y * scale / 2;
			}
			else if (y_alignment == TOP_ALIGNED) {
				ypos -= chr.Bearing.y * scale;
			}

			float w = chr.Size.x * scale;
			float h = chr.Size.y * scale;

			float vertices[6][4] = {
				{ xpos, ypos + h, 0.0f, 0.0f },
				{ xpos, ypos, 0.0f, 1.0f },
				{ xpos + w, ypos, 1.0f, 1.0f },
				{ xpos, ypos + h, 0.0f, 0.0f },
				{ xpos + w, ypos, 1.0f, 1.0f },
				{ xpos + w, ypos + h, 1.0f, 0.0f }
			};


			glActiveTexture(GL_TEXTURE15);
			glBindTexture(GL_TEXTURE_2D, chr.TextureAddr);
			textShader.setInt("text", 15);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

			glDrawArrays(GL_TRIANGLES, 0, 6);

			x += (chr.Advance >> 6) * scale;
		}

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	else {
		int offset = 0;
		for (char c : text) {
			Font::Character chr = Fonts[font].CharSet[c];
			offset += (chr.Advance >> 6) * scale;
		}

		if (x_alignment == CENTER_ALIGNED) {
			draw(LEFT_ALIGNED, y_alignment, text, x - offset / 2, y, scale, color, font);
		}

		else if (x_alignment == RIGHT_ALIGNED) {
			draw(LEFT_ALIGNED, y_alignment, text, x - offset, y, scale, color, font);
		}
	}
}

void TextLib::draw(ALIGNMENT x_alignment, ALIGNMENT y_alignment, std::string text, int x, int y, int scale, FONT font, glm::vec4& box) {
	if (x_alignment == TextLib::LEFT_ALIGNED) {
		box[0] = x;
		for (char c : text) {
			Font::Character chr = Fonts[font].CharSet[c];

			float xpos = x + chr.Bearing.x * scale;
			float ypos = y - (chr.Size.y - chr.Bearing.y) * scale;

			if (y_alignment == MID_ALIGNED) {
				ypos -= chr.Bearing.y * scale / 2;
			}
			else if (y_alignment == TOP_ALIGNED) {
				ypos -= chr.Bearing.y * scale;
			}
			
			/*box[2] = std::min(box[2], ypos + chr.Bearing.y * scale);
			box[3] = std::max(box[3], ypos - (chr.Size.y - chr.Bearing.y) * scale);*/

			float w = chr.Size.x * scale;
			float h = chr.Size.y * scale;

			box[2] = std::min(box[2], ypos);
			box[3] = std::max(box[3], ypos + h);

			x += (chr.Advance >> 6) * scale;
		}

		box[1] = x;
	}

	else {
		int offset = 0;
		for (char c : text) {
			Font::Character chr = Fonts[font].CharSet[c];
			offset += (chr.Advance >> 6) * scale;
		}

		if (x_alignment == CENTER_ALIGNED) {
			draw(LEFT_ALIGNED, y_alignment, text, x - offset / 2, y, scale, font, box);
		}

		else if (x_alignment == RIGHT_ALIGNED) {
			draw(LEFT_ALIGNED, y_alignment, text, x - offset, y, scale, font, box);
		}
	}
}


Font::Font(FT_Face& face) : CharSet(128) {
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (unsigned char c = 0; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		
		unsigned int texture;
		glGenTextures(1, &texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		Character character = {
		texture,
		glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
		glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
		face->glyph->advance.x
		};

		CharSet[c] = character;
	}	

	glBindTexture(GL_TEXTURE_2D, 0);
}



float Shapes::Square[24] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
};




Image2D::Image2D(bool useRGBA, std::string filePath) : useRGBA(useRGBA) {
	data = ResourceManager::loadImage2D(filePath, this);
}

std::shared_ptr<unsigned char*> Image2D::getData() const {
	return data;
}

Texture2D::Texture2D() : textureId(0) {
	glGenTextures(1, &textureAddr);
}

Image2D::~Image2D() {
	stbi_image_free(*data.get());
}



Texture2D::Texture2D(Image2D& source) : textureId(0) {
	glGenTextures(1, &textureAddr);
	glBindTexture(GL_TEXTURE_2D, textureAddr);
	auto format = source.useRGBA ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, format, source.width, source.height, 0, format, GL_UNSIGNED_BYTE, *source.getData().get());
	glGenerateMipmap(GL_TEXTURE_2D);
}

Texture2D::Texture2D(Image2D&& source) : textureId(0) {
	glGenTextures(1, &textureAddr);
	glBindTexture(GL_TEXTURE_2D, textureAddr);
	int format = source.useRGBA ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, format, source.width, source.height, 0, format, GL_UNSIGNED_BYTE, *source.getData().get());
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture2D::setId(int id) {
	textureId = id;
}

int Texture2D::getId() const {
	return textureId;
}

unsigned int Texture2D::getAddr() const {
	return textureAddr;
}




SqrWidget::SqrWidget() : VAO(0), VBO(0), bottom(0), left(0), height(0), width(0) {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Shapes::Square), Shapes::Square, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)8);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void SqrWidget::draw() const {
	std::cout << "DONT CALL ME!\n";
}



FrameBuffer::FrameBuffer(Shader shader) {
	/*TO BE IMPLEMENTED*/
}

void FrameBuffer::draw() const {
	/*TO BE IMPLEMENTED*/
}


Button::Button(TextLib& textLib) : textLib(textLib) {

}

Button::Button(TextLib& textLib, std::string vertexShader, std::string fragmentShader, std::string imageLoc) : 
	textLib(textLib), buttonShader(vertexShader.c_str(), fragmentShader.c_str()), buttonTexture(Image2D(true, imageLoc)) {

}

void Button::draw(float angle) const {
	buttonShader.use();
	glBindVertexArray(VAO);
	
	buttonShader.setMat4("projection", glm::ortho(0.0f, 800.0f, 0.0f, 800.0f));

	glm::mat4 world = glm::mat4(1.0f);
	world = glm::translate(world, glm::vec3(400.0f, 400.0f, 0.0f));
	world = glm::rotate(world, -angle, glm::vec3(0.0f, 0.0f, 1.0f));
	world = glm::translate(world, glm::vec3(0.0f, 306.0f, 0.0f));
	world = glm::scale(world, glm::vec3(32.0f, 32.0f, 1.0f));
	buttonShader.setMat4("world", world);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, buttonTexture.getAddr());
	buttonShader.setInt("backgroundImg", 0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	Shader::unbind();
}

void Button::configure(std::string vertexShader, std::string fragmentShader, bool useRGBA, std::string imageLoc) {
	buttonShader = Shader(vertexShader.c_str(), fragmentShader.c_str());
	buttonTexture = Texture2D(Image2D(useRGBA, imageLoc));
}




TimerClock::TimerClock(TextLib& textLib) : bg(Image2D(1, IMAGE_PATH"/bg3.png")), clockticks(0), target(0),
	textLib(textLib), button(textLib), curTime(0), textPointer(3), mousePosition(0.0f) {
	shaderBase = Shader(SHADER_PATH"/TimerClockVertex1.txt", SHADER_PATH"/TimerClockFragment1.txt");

	button.configure(SHADER_PATH"/buttonShaderVertex.txt", SHADER_PATH"/buttonShaderFragment.txt", true, IMAGE_PATH"/button.png");

	current = "00:00:00"; 

	boxCoords = glm::vec4(1e9, -1e9, 1e9, -1e9);
	textLib.draw(TextLib::CENTER_ALIGNED, TextLib::MID_ALIGNED, current, 400, 400, 1.0f, TextLib::ARIAL, boxCoords);
	/*std::cout << boxCoords[0] << "->" << boxCoords[1] << "\n";
	std::cout << boxCoords[2] << "->" << boxCoords[3] << "\n";*/
}

TimerClock::pointer TimerClock::getTimerClock(TextLib& textLib) {
	return std::make_shared<TimerClock>(textLib);
}

void TimerClock::update(unsigned long long updates, bool result) {
	clockticks += updates;

	if (result) {

		if (textPointer == 3) {
			if (clockticks >= target) {
				curTime = glm::ivec3(0);
				clockticks = 0;
				target = 0;
			}
			else curTime = to_vec(target - clockticks);
		}
		current = to_string(curTime);

	}
}

void TimerClock::draw() const {
	shaderBase.use();
	glBindVertexArray(VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bg.getAddr());
	shaderBase.setInt("backgroundImg", 0);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	textLib.draw(TextLib::CENTER_ALIGNED, TextLib::MID_ALIGNED, current, 400, 400, 1.0f, glm::vec3(0.4f), TextLib::ARIAL);	

	if (textPointer < 3 || clockticks >= target) button.draw(0);
	else button.draw(glm::radians(360.0f * clockticks / target));

	glBindVertexArray(0);
	Shader::unbind();
}

void TimerClock::getInput(int key) {
	if (key == GLFW_KEY_ENTER) {
		textPointer = 3;
		target = to_ull(curTime);
		clockticks = 0;
		return;
	}

	if (key == GLFW_KEY_SPACE) {
		textPointer++;
		if (textPointer >= 3) {
			textPointer = 3;
			target = to_ull(curTime);
			clockticks = 0;
		}
		return;
	}

	if (textPointer < 3) {
		for (int i = 0; i < 10; i++) {
			if (key == GLFW_KEY_0 + i) {
				curTime[textPointer] = (curTime[textPointer] * 10 + i) % 100;
				if (textPointer > 0 && curTime[textPointer] > 59) curTime[textPointer] %= 10;
			}
		}
	}
}

void TimerClock::getInput(int xpos, int ypos) {
	mousePosition.x = xpos;
	mousePosition.y = ypos;
}

void TimerClock::getInput(int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
		if (mousePosition.x > boxCoords[1] || mousePosition.x < boxCoords[0] ||
			mousePosition.y < boxCoords[2] || mousePosition.y > boxCoords[3]) {
			return;
		}
		std::cout << "Locked on time\n";

		textPointer = (mousePosition.x - boxCoords[0]) * 3 / (boxCoords[1] - boxCoords[0]);
		curTime[textPointer] = 0;

		std::cout << textPointer << "\n";
	}
	if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS) {
		std::cout << "Timer Reset\n";
		curTime = glm::ivec3(0);
		target = 0;
		clockticks = 0;
		textPointer = 3;
	}
}




std::shared_ptr<unsigned char*> ResourceManager::loadImage2D(std::string filePath, Image2D* image) {
	std::shared_ptr<unsigned char*> data = std::make_shared<unsigned char*>(
		stbi_load(filePath.c_str(), &image->width, &image->height, &image->channels, 0));
	if (!(*data.get())) std::cout << "bad_image\n";
	return data;
}




Screen::pointer Screen::currentScreen;

Screen::Screen(TextLib& textLib) : Timer(textLib), firstMouse(true), lastX(0), lastY(0), lastTime(0), ticks(0), lastTick(0),
	pressedLastFrame(258, false) {

}

Screen::pointer Screen::getScreen(TextLib& textLib) {
	Screen::pointer ptr = std::make_shared<Screen>(textLib);
	currentScreen = ptr;
	return ptr;
}

void Screen::drawTimer() {
	Timer.draw();
}

void Screen::processInputs(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		if (not pressedLastFrame[GLFW_KEY_SPACE]) {
			pressedLastFrame[GLFW_KEY_SPACE] = true;
			Timer.getInput(GLFW_KEY_SPACE);
		}
	}
	else pressedLastFrame[GLFW_KEY_SPACE] = false;

	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		if (not pressedLastFrame[GLFW_KEY_ENTER]) {
			pressedLastFrame[GLFW_KEY_ENTER] = true;
			Timer.getInput(GLFW_KEY_ENTER);
		}
	}
	else pressedLastFrame[GLFW_KEY_ENTER] = false;

	for (int i = 0; i < 10; i++) {
		if ((glfwGetKey(window, GLFW_KEY_0 + i) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_KP_0 + i) == GLFW_PRESS)) {
			if (not pressedLastFrame[GLFW_KEY_0 + i]) {
				pressedLastFrame[GLFW_KEY_0 + i] = true;
				Timer.getInput(GLFW_KEY_0 + i);
			}
		}
		else  pressedLastFrame[GLFW_KEY_0 + i] = false;
	}
}

void Screen::cursor_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = 0;
	}

	lastX = xpos;
	lastY = ypos;

	ypos = 800 - ypos;

	Timer.getInput(xpos, ypos);
}

void Screen::button_callback(GLFWwindow* window, int button, int action, int mods) {
	Timer.getInput(button, action, mods);
}

bool Screen::doDraw(float time) {
	unsigned long long updates = (time - lastTime) * 1000'000;
	ticks += updates;
	lastTime = time;

	bool result = ticks - lastTick >= 16'667;
	if (result) lastTick = ticks;

	Timer.update(updates, result);

	return result;
}

double Screen::runTime() const {
	return ticks / 1e6;
}