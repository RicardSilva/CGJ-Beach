#pragma once
#include "Texture.h"
#include <map>


namespace engine {

	class TextureManager {

	private:
		static TextureManager *_instance;
		map<string, Texture*> map;
		TextureManager() {};
		TextureManager(TextureManager const&) {};
		TextureManager& operator=(TextureManager const&) {};


	public:

		void AddTexture(const string& name, Texture *texture);
		Texture *GetTexture(const string& name);

		static TextureManager *Instance();
		void Destroy();


	};
}