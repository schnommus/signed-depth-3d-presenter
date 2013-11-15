#pragma once

#include <SFML/Graphics.hpp>
#include <boost/any.hpp>
#include <vector>

class Application;

struct NamedProperty {
	NamedProperty(std::string name, boost::any value) :
		m_name(name), m_value(value) {};
	std::string m_name;
	boost::any m_value;
};

class Entity {
public:
	Entity(bool is_3d) : m_is_3d(is_3d) { };
	virtual ~Entity() {}

	// Subclasses override
	virtual void Initialize() {}
	virtual void Update( float delta ) {}
	virtual void Draw() {}
	virtual void Destroy() {}
	virtual void MouseOver() {}
	virtual void HandleSFMLEvent( const sf::Event &event) {}
	virtual void PropertyAltered() {}

	void AddNamedProperty(std::string name, boost::any value)
		{ m_namedProperties.push_back(NamedProperty(name, value));}

	std::vector<NamedProperty> &NamedProperties() { return m_namedProperties; }

	// Some getters & setters
	inline unsigned int &Id() { return m_id; }
	inline void SetId(unsigned int id) { m_id = id; }
	inline void SetApp(Application *app) { m_app = app; }

	// Used internally to determine SFML vs. OpenGL draw calls
	inline bool &Is3D() { return m_is_3d; }

protected:
	// Parent application
	Application *m_app;

	// Data
	unsigned int m_id;
	bool m_is_3d;
	std::vector<NamedProperty> m_namedProperties;

	// Prevent implicitly copying entities (id unsafe)
	Entity( const Entity& );
	Entity &operator=( const Entity& );
};

// User's entities should inherit from these
class Entity3D : public Entity {
public:
	Entity3D() : Entity(true) { }
	sf::Vector3f m_position, m_rotation, m_scale;
};

class Entity2D : public Entity {
public:
	Entity2D() : Entity(false) { }
	sf::Vector2f m_position, m_rotation, m_scale;
};