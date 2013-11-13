#pragma once

#include <map>

// Helper to load resources with a simple file constructor to avoid duplicates
template <typename T>
class ResourceManager {
public:
	T &Fetch( std::string directory ) {
		if( m_resourcemap.find(directory) == m_resourcemap.end() ) {
			std::cout << "Fetching resource: " << directory << std::endl;
			m_resourcemap[directory] = new T(directory);
		}
		return *m_resourcemap.find(directory)->second;
	}

	T &Fetch( std::string directory1, std::string directory2 ) {
		if( m_resourcemap.find(directory1) == m_resourcemap.end() ) {
			std::cout << "Fetching resource: " << directory1 << "(also " << directory2 << ")" << std::endl;
			m_resourcemap[directory1] = new T(directory1, directory2);
		}
		return *m_resourcemap.find(directory1)->second;
	}

	~ResourceManager() {
		for( std::map< std::string, T* >::iterator it = m_resourcemap.begin();
			it != m_resourcemap.end(); ++it ) {
				delete it->second;
		}
	}

private:
	std::map< std::string, T* > m_resourcemap;
};