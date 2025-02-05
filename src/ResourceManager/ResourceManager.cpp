extern "C"{
	#include "raylib.h"
}

#include "ResourceManager.h"

const Texture2D& ResourceManager::loadTexture(const std::string path){
	if(textures.find(path)!=textures.end()){
		return textures[path];
	}
	if(!FileExists(path.c_str())){
		TraceLog(LOG_ERROR,"[RES] Texture not find: %s",path.c_str());
		return textures[""];
	}
	Texture2D tex=LoadTexture(path.c_str());
	if(!IsTextureValid(tex)){
		TraceLog(LOG_ERROR, "[RES] Failed to load: %s",path.c_str());
		return textures[""];
	}
	textures[path]=tex;
	TraceLog(LOG_INFO, "[RES] Loaded: %s (%d x %d)",path.c_str(),tex.width,tex.height);
	return textures[path];
}

void ResourceManager::registerTexture(const std::string& key,const Texture2D& tex){
	if(textures.find(key)==textures.end()){
		textures[key]=tex;
	}
}

void ResourceManager::resizeTexture(const std::string path,const int width,const int height){
	if(textures.find(path)!=textures.end()){
		Image img=LoadImageFromTexture(textures[path]);
		ImageResize(&img, width, height);
		textures[path]=LoadTextureFromImage(img);
		UnloadImage(img);
	}
}

void ResourceManager::unloadTexture(const std::string path){
	if(textures.find(path)!=textures.end()){
		UnloadTexture(textures[path]);
		textures.erase(path);
		TraceLog(LOG_INFO, "[RES] Unloaded: %s",path.c_str());
	}
}

void ResourceManager::cleanUp(){
	for(auto& [path,tex]:textures){
		UnloadTexture(tex);
	}
	textures.clear();
	TraceLog(LOG_INFO, "[RES] All resources released");
}
