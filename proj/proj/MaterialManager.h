#pragma once
#include "Material.h"
#include <map>


	namespace engine {

		class MaterialManager {

		private:
			static MaterialManager *_instance;
			map<string, Material*> map;
			MaterialManager() {};
			MaterialManager(MaterialManager const&) {};
			MaterialManager& operator=(MaterialManager const&) {};


		public:

			void AddMaterial(const string& name, Material *material);
			Material *GetMaterial(const string& name);

			static MaterialManager *Instance();
			void Destroy();


		};
}