#include "Widgets.h"


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



FrameBuffer::FrameBuffer(Shader shader) {
	/*TO BE IMPLEMENTED*/
}

void FrameBuffer::draw() const {
	/*TO BE IMPLEMENTED*/
}




TimerClock::TimerClock(TextLib& textLib) : bg(Image2D(1, IMAGE_PATH"/bg3.png")), textLib(textLib) {
	shaderBase = Shader(SHADER_PATH"/TimerClockVertex1.txt", SHADER_PATH"/TimerClockFragment1.txt");
}


/*TimerClock::TimerClock() : bg(Image2D(1, IMAGE_PATH"/bg3.png")) {
	shaderBase = Shader(SHADER_PATH"/TimerClockVertex1.txt", SHADER_PATH"/TimerClockFragment1.txt");
}

TimerClock::pointer TimerClock::getTimerClock() {
	return std::make_shared<TimerClock>();
}*/

TimerClock::pointer TimerClock::getTimerClock(TextLib& textLib) {
	return std::make_shared<TimerClock>(textLib);
}

void TimerClock::draw() const {
	shaderBase.use();
	glBindVertexArray(VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bg.getAddr());
	shaderBase.setInt("backgroundImg", 0);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	textLib.draw(TextLib::CENTER_ALIGNED, TextLib::MID_ALIGNED, "00:00:00", 400, 400, 1.0f, glm::vec3(0.4f), TextLib::ARIAL);

	glBindVertexArray(0);
	Shader::unbind();
}


std::shared_ptr<unsigned char*> ResourceManager::loadImage2D(std::string filePath, Image2D* image) {
	std::shared_ptr<unsigned char*> data = std::make_shared<unsigned char*>(
		stbi_load(filePath.c_str(), &image->width, &image->height, &image->channels, 0));
	if (!(*data.get())) std::cout << "bad_image\n";
	return data;
}
