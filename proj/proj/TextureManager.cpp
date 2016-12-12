#pragma once
#include "TextureManager.h"
#include <stddef.h>  // defines NULL
using namespace std;
namespace engine {

	TextureManager* TextureManager::_instance = NULL;

	void TextureManager::AddTexture(const string& name, Texture *texture) {
		map.insert(make_pair(name, texture));
	}

	Texture *TextureManager::GetTexture(const string& name) {
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
	}



}