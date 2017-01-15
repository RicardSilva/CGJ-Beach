#pragma once
#include "TextureManager.h"
#include <stddef.h>  // defines NULL
namespace engine {

	TextureManager* TextureManager::_instance = NULL;

	void TextureManager::AddTexture(const std::string& name, Texture *texture) {
		map.insert(std::make_pair(name, texture));
	}

	Texture *TextureManager::GetTexture(const std::string& name) {
		return map[name];
	}

	TextureManager *TextureManager::Instance() {
		if (_instance == nullptr)
			_instance = new TextureManager;
		return _instance;
	}

	void TextureManager::Destroy() {
		for (auto element : map) {
			delete(element.second);
		}
		delete(_instance);
	}



}