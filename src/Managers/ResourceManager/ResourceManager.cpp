#include "UI/UIUtility.h"
#include <codecvt>
#include <locale>
#include <string>
#include <nlohmann/json.hpp>
extern "C"{
	#include "raylib.h"
}

#include "ResourceManager.h"
ResourceManager::ResourceManager(){
	if(std::string(ASSETS_FONT_PATH)==" "){
		throw "Font path is not assigned";
	}else{
		fontFile=LoadFileData(ASSETS_FONT_PATH "font.otf", &fontFileLength);
		int codepointCount=0;
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		auto defaultCharFile=LoadFileText(ASSETS_FONT_PRELOAD_CHARACTERS);
		includedChars=converter.from_bytes(defaultCharFile);
		for(const auto& c:includedChars){
			charsSet.insert(c);
		}
		UnloadFileText(defaultCharFile);
		std::string defaultCharsInUTF8=converter.to_bytes(includedChars);
		int* codepoints=LoadCodepoints(defaultCharsInUTF8.c_str(), &codepointCount);
		font=LoadFontFromMemory(".otf", fontFile, fontFileLength, UI::FontCFG::FONT_LOAD_SIZE, codepoints, codepointCount);
		UnloadCodepoints(codepoints);
	}
}
const Texture2D& ResourceManager::loadTexture(const std::string path){
	if(textures.find(path)!=textures.end()){
		return textures[path];
	}
	if(!FileExists(path.c_str())){
		TraceLog(LOG_WARNING,"[RES] Texture not find: %s",path.c_str());
		return textures[""];
	}
	Texture2D tex=LoadTexture(path.c_str());
	if(!IsTextureValid(tex)){
		TraceLog(LOG_WARNING, "[RES] Failed to load: %s",path.c_str());
		return textures[""];
	}
	textures[path]=tex;
	TraceLog(LOG_INFO, "[RES] Loaded: %s (%d x %d)",path.c_str(),tex.width,tex.height);
	return textures[path];
}


std::string ResourceManager::resizeTexture(const std::string path,const int width,const int height){
	if(textures.find(path)==textures.end()){return "";}
	static int ID=0;
	Image img=LoadImageFromTexture(textures[path]);
	ImageResize(&img,width,height);
	std::string processedPath=path+"_porcessed"+std::to_string(ID);
	textures[processedPath]=LoadTextureFromImage(img);
	UnloadImage(img);
	ID++;
	return processedPath;
}

void ResourceManager::unloadTexture(const std::string path){
	if(textures.find(path)!=textures.end()){
		UnloadTexture(textures[path]);
		textures.erase(path);
		TraceLog(LOG_INFO, "[RES] Unloaded: %s",path.c_str());
	}
}

const Font& ResourceManager::getFont(const std::string& text){
	return loadWords(text);
}

const Font& ResourceManager::loadWords(const std::string& str){
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::wstring wideStr = converter.from_bytes(str);
	bool hasNewCharacter=false;
	for(const auto& c:wideStr){
		if(charsSet.find(c)==charsSet.end()){
			hasNewCharacter=true;
			includedChars+=c;
			charsSet.insert(c);
		}		
	}
	if(hasNewCharacter){
		needNewFont=true;
		std::string utf8Str = converter.to_bytes(includedChars);
		int codepointCount=0;
		int* codepoints=LoadCodepoints(utf8Str.c_str(), &codepointCount);
		if(codepoints==nullptr||codepointCount==0){
			TraceLog(LOG_ERROR, "[RES] Failed to generate codepoints for font");
		}
		usedFonts.push_back(font);
		font=LoadFontFromMemory(".otf", fontFile, fontFileLength, UI::FontCFG::FONT_LOAD_SIZE, codepoints, codepointCount);
		UnloadCodepoints(codepoints);
	}
	return font;
}

void ResourceManager::cleanFont(){
	for(auto& f:usedFonts){
		UnloadFont(f);
	}
	usedFonts.clear();
	needNewFont=false;
}
void ResourceManager::frameClean(){
	cleanFont();
}
void ResourceManager::cleanUp(){
	for(auto& [path,tex]:textures){
		UnloadTexture(tex);
	}
	textures.clear();
	UnloadFont(font);
	UnloadFileData(fontFile);
	TraceLog(LOG_INFO, "[RES] All resources released");
}
