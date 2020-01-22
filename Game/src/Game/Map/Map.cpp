#include "Map.h"

namespace Game
{

	Map::Map(uint32_t width, uint32_t height)
		: m_MapTree(width, height)
	{
#if 1
		m_Loader = new FileMapLoaderAdapter(this, "global.map");
#elif defined GAME_CLIENT

#endif
	}
	void Map::Load(uint32_t x, uint32_t y)
	{
		if (m_Loader) m_Loader->Load(x, y, [](Chunk<8, 8>* chunk) {});
	}

	void Map::Load(uint32_t x, uint32_t y, std::function<void(Chunk<8, 8>*)> cb)
	{
		if (m_Loader) m_Loader->Load(x, y, cb);
	}
}