#pragma once
#include "Texture.h"
#include <map>


namespace engine {

	class TextureManager {

	private:
		static TextureManager *_instance;
		std::map<std::string, Texture*> map;
		TextureManager() {};
		TextureManager(TextureManager const&) {};
		TextureManager& operator=(TextureManager const&) {};


	public:

		void AddTexture(const std::string& name, Texture *texture);
		Texture *GetTexture(const std::string& name);

		static TextureManager *Instance();
		void Destroy();


	};
}