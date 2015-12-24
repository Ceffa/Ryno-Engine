#pragma once
#include <unordered_map>

class GenericMap{
	//Provides a "generic" type map.
	//Actually doesn't know the type, you need to 
	//remember it when you retrieve it
public:
	//Generic functions
	void clear(){
		for (auto v : saved_vals)
			delete v.second;
		saved_vals.clear();
	}
	template<typename T>
	void add(const std::string& name, T* value)
	{
		saved_vals[name] = static_cast<void*>(value);
	}
	template<typename T>
	T* get(const std::string& name){
		return static_cast<T*>(saved_vals.find(name)->second);
	}
	template<typename T>
	void replace(const std::string& name, T& value)
	{
		if (saved_vals.find(name) != saved_vals.end())
			remove(name);
		add(name, value);
	}
	void remove(const std::string& name){
		saved_vals.erase(name);
	}


private:
	std::unordered_map<std::string, void*> saved_vals;

};