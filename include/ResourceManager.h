#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <unordered_map>
#include <string>
extern "C"{
	#include "raylib.h"
}
namespace UI {
	const std::string FONT_PATH="";
}
class ResourceManager{
private:
	std::unordered_map<std::string, Texture2D> textures;
	Font font;
	bool isFontAssigned=false;
	ResourceManager()=default;
	~ResourceManager()=default;
public:
	ResourceManager(const ResourceManager&)=delete;
	void operator=(const ResourceManager&)=delete;
	static ResourceManager& Get(){
		//利用C++11局部静态变量特性
		static ResourceManager instance;
		return instance;
	}
	std::string resizeTexture(const std::string path,const int width,const int height);
	const Texture2D& loadTexture(const std::string path);
	std::unordered_map<std::string, Texture2D>& getTextures(){return textures;}
	const Font& loadFont();
	void unloadTexture(const std::string path);
	void cleanUp();
};

#endif