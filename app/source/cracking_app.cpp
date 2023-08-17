#include "stl_includes.hpp"
#include "md5.hpp"
#include "character_set.hpp"
#include "random_string_generator.hpp"
#include "bruteforce_range_generator.hpp"
#include "dictionary.hpp"
#include "thread_pool.hpp"
#include "md5_cracker.hpp"
#include "application.hpp"

#define DEFAULT_MAX_DEPTH 8
#define DEFAULT_DICT_FILEPATH "res/rockyou.txt"
#define DEFAULT_HASHLIST_FILEPATH "res/test1.txt"
#define DEFAULT_MAX_THREADS std::thread::hardware_concurrency()

class cracking_app : application
{
	size_t m_max_depth   =  DEFAULT_MAX_DEPTH;
	size_t m_max_threads =  DEFAULT_MAX_THREADS;
	std::string m_dictionary_filepath   = DEFAULT_DICT_FILEPATH;
	std::string m_hash_list_filepath = DEFAULT_HASHLIST_FILEPATH;

	bool m_dictionary_attack = true;
	
	bool m_valid_attack = true;

	bool m_file_out = false;
	std::string m_output_filename = "";

	std::vector<std::string> load_hashes(const std::string& filename)
	{
		std::vector<std::string> hashes;
		std::ifstream input(filename);
		std::string line;
		while (std::getline(input, line))
		{
			hashes.push_back(line);
		}
		hashes.shrink_to_fit();
		return hashes;
	}

	void print_help()
	{
		std::cout << "usage: ./hash_cracker {mode} {specifiers}" << std::endl;
	}

	void parse_args()
	{
		if(get_args().size() < 2)
		{
			print_help();
			m_valid_attack = false;
			return;
		}
		std::string attack_mode = get_args()[1];
		if(attack_mode == "-d")
		{
			m_dictionary_attack = true;
		}
		else if (attack_mode == "-b")
		{
			m_dictionary_attack = false;
		}
		else 
		{
			print_help();
			m_valid_attack=false;
			return;
		}
		if(get_args().size() > 2)
		{
			for(size_t i = 2; i < get_args().size();++i)
			{
				if(get_args()[i] == "-df")
				{
					m_dictionary_filepath = get_args()[i+1];
				}
				if(get_args()[i] == "-hf")
				{
					m_hash_list_filepath = get_args()[i+1];
				}
				if(get_args()[i] == "-of")
				{
					m_output_filename = get_args()[i+1];
					m_file_out = true;
				}
				if(get_args()[i] == "-th")
				{
					m_max_threads = std::stoi(get_args()[i+1]);
				}
				if(get_args()[i] == "-de")
				{
					m_max_depth = std::stoi(get_args()[i+1]);
				}
			}
		}


	}


	int execute() override
	{
		if(!m_valid_attack) return 1;
		std::string attack_mode = m_dictionary_attack ? "dictionary" : "bruteforce";
		std::cout << "Initiating " << attack_mode << " attack on file: " << m_hash_list_filepath << std::endl;
		bq::md5_cracker cracker(m_dictionary_filepath, m_max_threads);
		
		if(m_file_out) cracker.set_log_to_file(m_output_filename);

		auto hash_list = load_hashes(m_hash_list_filepath);

		if(m_dictionary_attack)
		{
			cracker.attack_hash_list(hash_list);
		}
		else
		{
			//todo bruteforce attack setup
		}

		return 0;
	}
	void init() override
	{
		parse_args();
	}
}app;



