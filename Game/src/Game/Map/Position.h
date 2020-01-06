#pragma once

#include <stdint.h>
#include <cstdlib>

namespace Game {


	class Position
	{
	public:
		Position() : m_X(0), m_Y(0), m_Z(0) {};
		Position(const uint16_t& x, const uint16_t& y, const uint8_t& z) : m_X(x), m_Y(y), m_Z(z) {}

		bool operator==(const Position& p) const
		{
			return m_X == p.m_X && m_Y == p.m_Y && m_Z == p.m_Z;
		}

		bool operator!=(const Position& p) const
		{
			return !(*this == p);
		}

		Position operator+(const Position& p) const
		{
			return Position(m_X + p.m_X, m_Y + p.m_Y, m_Z + p.m_Z);
		}

		Position operator-(const Position& p) const
		{
			return Position(m_X - p.m_X, m_Y - p.m_Y, m_Z - p.m_Z);
		}

		int_fast32_t GetX() const { return m_X; }
		int_fast32_t GetY() const { return m_Y; }
		int_fast16_t GetZ() const { return m_Z; }

		void SetX(uint32_t x) { m_X = x; }
		void SetY(uint32_t y) { m_Y = y; }
		void SetZ(uint16_t z) { m_Z = z; }

	public:
		static int32_t GetOffsetX(const Position& p1, const Position& p2)
		{
			return p1.m_X - p2.m_X;
		}

		static int32_t GetOffsetY(const Position& p1, const Position& p2)
		{
			return p1.m_Y - p2.m_Y;
		}

		static int16_t GetOffsetZ(const Position& p1, const Position& p2)
		{
			return p1.m_Z - p2.m_Z;
		}

		static int32_t GetDistancetX(const Position& p1, const Position& p2)
		{
			return std::abs(GetOffsetX(p1, p2));
		}

		static int32_t GetDistancetY(const Position& p1, const Position& p2)
		{
			return std::abs(GetOffsetX(p1, p2));
		}

		static int16_t GetDistancetZ(const Position& p1, const Position& p2)
		{
			return std::abs(GetOffsetX(p1, p2));
		}

	private:
		uint16_t m_X;
		uint16_t m_Y;
		uint8_t m_Z;
	};

}