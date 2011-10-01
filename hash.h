
#ifndef HASH_H
#define HASH_H

#include <ctime>

typedef unsigned long long int uint64;
typedef unsigned int uint32;

// ref. http://en.wikipedia.org/wiki/Linear_congruential_generator
class LinerRand
{
	static const uint32 A = 1664525;
	static const uint32 C = 1013904223;

private:
	uint32 seed;

private:
	void set_seed(uint32 v) { seed = v; }

	void set_seed_by_time()
	{
		seed = uint32( time(NULL) );
		std::cerr << "seed inited by time(): " << seed << "\n";
	}

public:
	/// if 0 is given, seed is initied by time()
	LinerRand(uint32 initial_seed=1)
	{
		if ( initial_seed == 0 ) set_seed_by_time();
		else                     set_seed(initial_seed);

		if ( sizeof (uint32) != 4 ) throw std::logic_error("size of uint32 is not 4");
		if ( sizeof (uint64) != 8 ) throw std::logic_error("size of uint64 is not 8");
	}

	/// return random value (32bit)
	uint32 rand32()
	{
		seed = ( A * seed + C ) & 0xfffffffff;
		return seed;
	}

	/// return random value (64bit)
	uint64 rand64()
	{
		uint64 ret;

		// to keep order, donot concat following
		ret = uint64( rand32() ) << 32;
		ret |= uint64( rand32() );

		return ret;
	}
};

template<class RandGenerator>
class HashT
{
	const unsigned Size;
	uint64* table_;
	
public:
	HashT(unsigned Size, unsigned seed=1)
		:	Size(Size)
	{
		table_ = new uint64[Size];

		RandGenerator gen(seed);

		for ( unsigned i=0; i<Size; i++ )
			table_[i] = gen.rand64();
	}

	~HashT()
	{
		delete[] table_;
	}

	uint64 operator[](int ind) const
	{
		if ( ind < 0 || int( Size ) <= ind ) throw std::runtime_error("HashT: range error");
		return table_[ind];
	}
};

typedef HashT<LinerRand> LinerHash;

#endif//HASH_H

