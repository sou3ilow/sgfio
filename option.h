// boost::program_option imitation
//
// $Date: 2011-09-04 16:15:17 +0200 (日, 04 9 2011) $

#ifndef OPTION_H
#define OPTION_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <stdexcept>
#include <iomanip>

/// program option parser
class Option
{
public:
	/// value container
	struct Value
	{
	public:
		bool defaulted; /// whether value is defaulted
		bool given; /// whether value is given in rutime
		std::string default_value; /// the default value
 		std::string given_value; /// the given value

	public:
		/// normal constructor
		Value() : defaulted(false), given(false) {}
		/// copy constructor
		Value(const Value& v) 
			: defaulted(v.defaulted)
			, given(v.given)
			, default_value(v.default_value)
			, given_value(v.given_value)
		{
		}
		/// default value constructor
		template<typename type>
		Value(const type& v)
			: defaulted(true)
			, given(false)
			, given_value()
		{
			std::stringstream ss; ss << v;
			default_value = ss.str();
		}
		/// assignment
		void operator=(const Value& v)
		{
			given = v.given;
			defaulted = v.defaulted;
			default_value = v.default_value;
			given_value = v.given_value;
		}

	public:
		/// return the length of value
		size_t length() const
		{
			return given
				? given_value.length()
				: default_value.length();
		}
		/// whether it contains valid value
		bool valid() const
		{
			return defaulted || given;
		}
		/// set value(for given value)
		void set(const std::string& v)
		{
			given = true;
			given_value = v;
		}
		/// return the given value
		std::string raw() const
		{
			return given_value;
		}
		/// value conveter
		template<typename type>
		type get() const
		{
			const std::string* target;

			if ( given )
				target = &given_value;
			else
			{
				if ( ! defaulted )
					throw std::runtime_error("no value assigned");
				else
					target = &default_value;
			}
				
			type ret;
			std::stringstream ss; ss << *target;
			ss >> ret;
			return ret;
		}
	};

private: // types

	/// option item
	struct Item	
	{
		bool has_arg; /// whether item has argument
		bool set; /// whether item given value in runtime
		std::string description; /// description of item
		Value value; /// default & given value container

	public:
		/// normal constructor
		Item(const std::string& description="")
			: has_arg(false)
			, set(false)
			, description(description)
		{
		}
		/// constructor with value
		Item(const std::string& description, const Value& v)
			: has_arg(true)
			, set(false)
			, description(description)
			, value(v)
		{
		}
		/// assign operator
		void operator=(const Item& i)
		{
			 has_arg=i.has_arg;
			 set=i.set;
			 description=i.description;
			 value=i.value;
		}
	};

private:
	/// item list
	struct Items : std::map<std::string, Item>
	{
	public:
		/// operator for Option::set_options()
		Items& operator()(const std::string& name, const std::string& description)
		{
			if ( find(name) != end() )
				throw std::logic_error("duplicated registration: " + name);
			operator[](name) = Item(description);
			return *this;
		}
		/// operator for Option::set_options()
		Items& operator()(const std::string& name, const Value& v, const std::string& description)
		{
			if ( find(name) != end() )
				throw std::logic_error("duplicated registration: " + name);
			operator[](name) = Item(description, v);
			return *this;
		}
	};

private:
	Items _items; /// unordered option list
	std::vector<std::string> _ordered_value; // ordered option list
	std::string _description_title; /// heading title for display
	unsigned _description_name_columns; /// skip length for display

public:
	// normal constructor
	Option(const std::string& description_title, int description_name_columns)
		:	_description_title(description_title)
		,	_description_name_columns(description_name_columns)
	{
	}

public:	
	/// see sample and Items
	Items& set_options() { return _items; }

	/// command line option parser
	void parse(int argc, char** argv, int start=1)
	{
		for ( int n=start; n<argc; )
		{
			//std::cout << " " << n << ":" << argv[n] << "\n"; // debug

			if ( argv[n][0] == '\0' )
				continue;
			else if ( argv[n][0] != '-' || argv[n][1] == '\0' ) // single - is treated as an ordered option
			{
				_ordered_value.push_back(argv[n++]);
				continue;
			}

			int name_start = 1;
			if ( argv[n][1] == '-' ) name_start += 1; 
			
			Items::iterator it = _items.find(argv[n]+name_start); // find str without -, --
			if ( it != _items.end() )
			{
				it->second.set=true;
				n++;
			}
			else
			{
				std::cout << "unknow option: " << argv[n] << "\n";
				exit(1);
			}

			if ( it->second.has_arg )
				if ( n != argc )
					it->second.value.set(argv[n++]);
				else
				{
					std::cout << "missing argument for: " << it->first << "\n";
					exit(1);
				}
		} // each args
	}
	///  whether option name is valid
	bool has(const std::string& name) const // HACK
	{
		Items::const_iterator it = _items.find(name);
		return it != _items.end() && it->second.set;
	}
	/// convert value of option name
	template<typename type>
	type get(const std::string& name) const
	{
		Items::const_iterator it = _items.find(name);
		if ( it == _items.end() )
			throw std::logic_error("not defined: " + name);
		return it->second.value.get<type>();
	}

	/// get option value as string. see also get()
	const std::string operator[](const std::string& key) const
	{
		Items::const_iterator it = _items.find(key);
		if ( it == _items.end() )
			throw std::logic_error("not defined: " + key);
		return it->second.value.raw();
	}

public:
	/// converter for ordered option
	template<typename type>
	type get(unsigned nth) const
	{
		if ( _ordered_value.size() < nth )
			return std::range_error("too large index");
		
		std::stringstream ss; ss << _ordered_value[nth];
		type ret; ss >> ret;
		return ret; 
	}
	/// get ordered option as string
	const std::string& operator[](unsigned nth) const
	{
		if ( _ordered_value.size() < nth )
			throw std::range_error("too large index");
		return _ordered_value[nth]; 
	}
	/// returns number of ordered option
	size_t count() const
	{
		return _ordered_value.size();
	}

public:
	/// put descriptions
	friend std::ostream& operator<<(std::ostream& os, const Option& o)
	{
		os << o._description_title << "\n";
		for ( Items::const_iterator it=o._items.begin(); it!=o._items.end(); ++it )
		{
			std::stringstream syntax;
			syntax << it->first;
			
			if ( it->second.has_arg )
			{
				if ( it->second.value.defaulted )
					syntax << " " << it->second.value.default_value;
				else
					syntax << " arg";
			}

			os	<< " -"
				<< std::left << std::setw(o._description_name_columns) << syntax.str()
				<< it->second.description << "\n"; 
		}
		return os;		
	}

	/// dump values	for debug
	void to_s(std::ostream& os) const
	{
		os << "ordered: " << _ordered_value.size() << "\n";
		for ( std::vector<std::string>::const_iterator it=_ordered_value.begin(); it!=_ordered_value.end(); ++it )
			os << " " << *it << "\n";
		os << "non-ordered: " << _items.size() << "\n";
		for ( Items::const_iterator it=_items.begin(); it!=_items.end(); ++ it )
			os	<< " -"
				<< std::left << std::setw(_description_name_columns) << it->first
				<< " v:" << it->second.value.valid()
				<< " g:" << it->second.value.given
				<< " " << it->second.value.raw() << "\n"; 
	}

public:
	static void sample(int argc, char* argv[])
	{
		// setting
		Option opts("syntax: yourname count_from [options]", 10);
		opts.set_options()
			("help", "show this message")
			("to", Value(10), "") // set default as 10
		;
		
		// parse
		opts.parse(argc, argv);

		// cehck
		if ( opts.has("help") || opts.count() != 2 )
		{
			std::cout << opts << "\n";
			exit(0);
		}

		// get ordered options
		std::string name =  opts[0];
		int from = opts.get<int>(1);

		// get unordered options
		int to =  opts.get<int>("to");

		// some work
		int sum=0;
		for ( int i=from; i<to; i++ )
			sum += i;

		// show
		std::cerr << name << ", answer is" << sum;
	}
};

#endif // OPTOIN_H


