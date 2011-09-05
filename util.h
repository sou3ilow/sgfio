#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <sstream>

template<typename type>
type lex_cast(const std::string& str)
{
	std::stringstream ss(str);
	type value;
	ss >> value;
	return value;
}

/// extended string
struct exstring : public std::string
{
	exstring(const std::string& str="")
		: std::string(str)
	{
	}

	template<typename type>
	type to()
	{
		std::stringstream ss(static_cast<std::string&>(*this));
		type value;
		ss >> value;
		return value;
	}
};

/// temporal stream
struct tstream : public std::stringstream
{
	tstream() { }

	template <typename type>
	tstream(const type& v)
	{
		this->operator << (v);
	}

	template <typename type>
	tstream& operator << (const type& v)
	{
		* ( static_cast<std::stringstream*>(this) ) << v;
		return *this;
	}

	operator std::string() const
	{
		return str();
	}
};

/// show comment & return 
template<typename return_type>
struct comret
{
	return_type ret;
	std::ostream& os;

	comret(const return_type& ret, std::ostream& os = std::cerr)
		: ret(ret) , os(os)
	{
	}

	template <typename type>
	comret& operator << (const type& v)
	{
		os << v;
		return *this;
	}

	operator return_type() const
	{
		return ret;
	}
};

typedef comret<bool> boolret;
typedef comret<int> intret;

#endif//UTIL_H

