#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <unordered_map>
#include <string>
extern "C"{
	#include "raylib.h"
}

class ResourceManager{
private:
	std::unordered_map<std::string, Texture2D> textures;
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
	void resizeTexture(const std::string path,const int width,const int height);
	const Texture2D& loadTexture(const std::string path);
	void registerTexture(const std::string& key,const Texture2D& tex);
	std::unordered_map<std::string, Texture2D>& getTextures(){return textures;}
	void unloadTexture(const std::string path);
	void cleanUp();
};

#endif