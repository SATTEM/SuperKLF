#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <string>
extern "C"{
	#include "raylib.h"
}
class ResourceManager{
private:
	std::unordered_map<std::string, Texture2D> textures;
	unsigned char* fontFile;
	std::wstring includedChars;
	std::unordered_set<wchar_t> charsSet;
	Font font;
	std::list<Font> usedFonts;
	int fontFileLength;
	bool needNewFont=false;
	ResourceManager();
	~ResourceManager()=default;
	const Font& loadWords(const std::string&);
public:
	ResourceManager(const ResourceManager&)=delete;
	void operator=(const ResourceManager&)=delete;
	static ResourceManager& Get(){
		//单例
		static ResourceManager instance;
		return instance;
	}
	std::string resizeTexture(const std::string path,const int width,const int height);
	const Texture2D& loadTexture(const std::string path);
	std::unordered_map<std::string, Texture2D>& getTextures(){return textures;}
	const Texture2D resizeAndGet(const std::string& path,const Vector2& size){
		return textures[resizeTexture(path,size.x,size.y)];
	}
	const Font& getFont(const std::string&);
	void unloadTexture(const std::string path);
	void frameClean();
	void cleanUp();
	void cleanFont();
};

#endif