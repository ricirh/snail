#include <iostream>

#include "../snail/config.h"
#include "../snail/log.h"
#include "yaml-cpp/yaml.h"

snail::ConfigVar<int>::ptr g_int_value_config = snail::Config::Lookup("system.port", (int)8080, "system port");

snail::ConfigVar<float>::ptr g_f_value_config = snail::Config::Lookup("system.port", (float)8080, "system port");

snail::ConfigVar<float>::ptr g_float_value_config = snail::Config::Lookup("system.value", (float)10.2f, "system value");

snail::ConfigVar<std::vector<int> >::ptr g_int_vec_value_config = snail::Config::Lookup("system.int_vec", std::vector<int>{1, 2}, "system int vec");

snail::ConfigVar<std::list<int> >::ptr g_int_list_value_config = snail::Config::Lookup("system.int_list", std::list<int>{1, 2}, "system int vec");

snail::ConfigVar<std::set<int> >::ptr g_int_set_value_config = snail::Config::Lookup("system.int_set", std::set<int>{1, 2}, "system int set");

snail::ConfigVar<std::unordered_set<int> >::ptr g_int_uset_value_config = snail::Config::Lookup("system.int_uset", std::unordered_set<int>{1, 2}, "system int uset");

snail::ConfigVar<std::map<std::string, int> >::ptr g_int_map_value_config = snail::Config::Lookup("system.int_map", std::map<std::string, int>{ {"first", 1}}, "system int map");

snail::ConfigVar<std::unordered_map<std::string, int> >::ptr g_int_umap_value_config = snail::Config::Lookup("system.int_umap", std::unordered_map<std::string, int>{ {"first", 1}}, "system int umap");


void print_yaml(const YAML::Node& node, int level)
{
	if(node.IsScalar())
	{
		SNAIL_LOG_INFO(SNAIL_LOG_ROOT()) << std::string(level * 4, ' ')
			<< node.Scalar() << " - " << node.Type();
	}
	else if(node.IsNull())
	{
		SNAIL_LOG_INFO(SNAIL_LOG_ROOT()) << std::string(level * 4, ' ')
			<< "NULL - " << node.Type() << " - " << level;
	}
	else if (node.IsMap())
	{
		for (auto it = node.begin(); it != node.end(); ++it)
		{
			SNAIL_LOG_INFO(SNAIL_LOG_ROOT()) << std::string(level * 4, ' ')
				<< it->first << " - " << it->second.Type() << " - " << level;
			print_yaml(it->second, level + 1);
		}
	}
	else if (node.IsSequence())
	{
		for (size_t i = 0; i < node.size(); ++i)
		{
			SNAIL_LOG_INFO(SNAIL_LOG_ROOT()) << std::string(level * 4, ' ')
				<< i << " - " << node[i].Type() << " - " << level;
			print_yaml(node[i], level + 1);
		}
	}
}

void test_config()
{
	SNAIL_LOG_INFO(SNAIL_LOG_ROOT()) << "before: " << g_int_value_config->getValue();
	SNAIL_LOG_INFO(SNAIL_LOG_ROOT()) << "before: " << g_float_value_config->toString();
	
#define XX(g_var, name, prefix) \
	{ \
		auto v = g_var->getValue(); \
		for(auto i : v) \
		{ \
			SNAIL_LOG_INFO(SNAIL_LOG_ROOT()) << #prefix << " " << #name << " : " << i; \
		} \
		SNAIL_LOG_INFO(SNAIL_LOG_ROOT()) << #prefix << " " << " yaml: " << g_var->toString(); \
	}

#define YY(g_var, name, prefix) \
	{ \
		auto v = g_var->getValue(); \
		for(auto i : v) \
		{ \
			SNAIL_LOG_INFO(SNAIL_LOG_ROOT()) << #prefix << " " << #name << " : { " << i.first << " - " << i.second << "}"; \
		} \
		SNAIL_LOG_INFO(SNAIL_LOG_ROOT()) << #prefix << " " << " yaml: " << g_var->toString(); \
	}

	XX(g_int_vec_value_config, int_vec, before);
	XX(g_int_list_value_config, int_list, before);
	XX(g_int_set_value_config, int_set, before);
	XX(g_int_uset_value_config, int_uset, before);
	YY(g_int_map_value_config, int_map, before);
	YY(g_int_umap_value_config, int_umap, before);

	YAML::Node root = YAML::LoadFile("/home/liufx/workspace/snail/bin/conf/log.yml");
	snail::Config::LoadFromYaml(root);

	SNAIL_LOG_INFO(SNAIL_LOG_ROOT()) << "after: " << g_int_value_config->getValue();
	SNAIL_LOG_INFO(SNAIL_LOG_ROOT()) << "after: " << g_float_value_config->toString();

	XX(g_int_vec_value_config, int_vec, after);
	XX(g_int_list_value_config, int_list, after);
	XX(g_int_set_value_config, int_set, after);
	XX(g_int_uset_value_config, int_uset, after);
	YY(g_int_map_value_config, int_map, after);
	YY(g_int_umap_value_config, int_umap, after);
}

class Person
{
public:
	std::string m_name;
	int m_age = 0;
	bool m_sex = 0;
	std::string toString() const
	{
		std::stringstream ss;
		ss << "[Person name=" << m_name
			<< " age=" << m_age
			<< " sex=" << m_sex
			<< "]";
		return ss.str();
	}

	bool operator==(const Person& p) const
	{
		return (m_name == p.m_name
			&& m_age == p.m_age
			&& m_sex == p.m_sex);
	}
};

/************************* person *****************************/
namespace snail
{
	template<>
	class LexicalCast<std::string, Person>
	{
	public:
		Person operator()(const std::string& v)
		{
			YAML::Node node = YAML::Load(v);
			Person p;
			p.m_name = node["name"].as<std::string>();
			p.m_age = node["age"].as<int>();
			p.m_sex = node["sex"].as<bool>();
			return p;
		}
	};

	template<>
	class LexicalCast<Person, std::string>
	{
	public:
		std::string operator()(const Person& p)
		{
			YAML::Node node;
			node["name"] = p.m_name;
			node["age"] = p.m_age;
			node["sex"] = p.m_sex;

			std::stringstream ss;
			ss << node;
			return ss.str();
		}
	};
}



snail::ConfigVar<Person>::ptr g_person_config 
= snail::Config::Lookup("class.person", Person(), "class person");

snail::ConfigVar<std::map<std::string,Person>>::ptr g_mperson_config
= snail::Config::Lookup("class.map", std::map<std::string,Person>(), "class mperson");

snail::ConfigVar<std::map<std::string, std::vector<Person>>>::ptr g_mvperson_config
= snail::Config::Lookup("class.vmap", std::map<std::string, std::vector<Person>>(), "class mvperson");


void test_class()
{
	SNAIL_LOG_INFO(SNAIL_LOG_ROOT()) << "before:" << g_person_config->getValue().toString() << " - " << g_person_config->toString();
	
#define MMP(g_var, prefix) \
	{ \
		auto m = g_var->getValue(); \
		for(auto i : m) \
		{ \
			SNAIL_LOG_INFO(SNAIL_LOG_ROOT()) << #prefix << ": {" << i.first << " - " << i.second.toString() << "}"; \
		} \
		SNAIL_LOG_INFO(SNAIL_LOG_ROOT()) << #prefix << " " << " : size= " << m.size(); \
	}

	g_person_config->addListener([](const Person& old_value, const Person& new_value) {
		SNAIL_LOG_INFO(SNAIL_LOG_ROOT()) << "old_value:" << old_value.toString()
			<< " new_value:" << new_value.toString();
		});

	MMP(g_mperson_config, "class.map before");
	SNAIL_LOG_INFO(SNAIL_LOG_ROOT()) << "before:" << g_mvperson_config->toString();

	YAML::Node root = YAML::LoadFile("/home/liufx/workspace/snail/bin/conf/log.yml");
	snail::Config::LoadFromYaml(root);
	
	SNAIL_LOG_INFO(SNAIL_LOG_ROOT()) << "after:" << g_person_config->getValue().toString() << " - " << g_person_config->toString();
	MMP(g_mperson_config, "class.map after"); 
	SNAIL_LOG_INFO(SNAIL_LOG_ROOT()) << "after:" << g_mvperson_config->toString();
}

void test_yaml()
{
	YAML::Node root = YAML::LoadFile("/home/liufx/workspace/snail/bin/conf/test.yml");
	print_yaml(root, 0);

	//SNAIL_LOG_INFO(SNAIL_LOG_ROOT()) << root;
}

void test_log()
{
	static snail::Logger::ptr system_log = SNAIL_LOG_NAME("system");
	SNAIL_LOG_INFO(system_log) << "hell snial" << std::endl;

	std::cout << snail::LoggerMgr::GetInstance()->toYamlString() << std::endl;
	YAML::Node root = YAML::LoadFile("/home/liufx/workspace/snail/bin/conf/log.yml");
	snail::Config::LoadFromYaml(root);
	std::cout << "=========================" << std::endl;
	std::cout << snail::LoggerMgr::GetInstance()->toYamlString() << std::endl;
	std::cout << "=========================" << std::endl;
	std::cout << root << std::endl;
	SNAIL_LOG_INFO(system_log) << "hell system" << std::endl;

	system_log->setFormatter("%d - %m%n");
	SNAIL_LOG_INFO(system_log) << "hell system" << std::endl;
}

int main()
{
	//test_config();
	//test_yaml();
	//test_class();
	test_log();
	//SNAIL_LOG_INFO(SNAIL_LOG_ROOT()) << g_int_value_config->getValue();
	//SNAIL_LOG_INFO(SNAIL_LOG_ROOT()) << g_int_value_config->toString();

	snail::Config::Visit([](snail::ConfigVarBase::ptr var)
	{
		SNAIL_LOG_INFO(SNAIL_LOG_ROOT()) << "name=" << var->getName()
			<< " description=" << var->getDescription()
			<< " typename=" << var->getTypeName()
			<< " value=" << var->toString();		
	});
	return 0;
}