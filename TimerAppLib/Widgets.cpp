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
	glBindTexture(GL_TEXTURE_2D, textureAddr);
	auto format = source.useRGBA ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, format, source.width, source.height, 0, GL_RGB, GL_UNSIGNED_BYTE, *source.getData().get());
	glGenerateMipmap(GL_TEXTURE_2D);
}

Texture2D::Texture2D(Image2D&& source) : textureId(0) {
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
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}



FrameBuffer::FrameBuffer(Shader shader) {

}

void FrameBuffer::draw() const {
	
}




TimerClock::TimerClock() : bg(Image2D(1, IMAGE_PATH"/bg3.png")) {
	shaderBase = Shader(SHADER_PATH"/TimerClockVertex1.txt", SHADER_PATH"/TimerClockFragment1.txt");


}

TimerClock::pointer TimerClock::getTimerClock() {
	return std::make_shared<TimerClock>();
}

void TimerClock::draw() const {
	shaderBase.use();
	glBindVertexArray(VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bg.getAddr());
	shaderBase.setInt("backgroundImg", 0);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	Shader::unbind();
}


std::shared_ptr<unsigned char*> ResourceManager::loadImage2D(std::string filePath, Image2D* image) {
	std::shared_ptr<unsigned char*> data = std::make_shared<unsigned char*>(
		stbi_load(filePath.c_str(), &image->width, &image->height, &image->channels, 0));
	if (!(*data.get())) std::cout << "bad_image\n";
	return data;
}
