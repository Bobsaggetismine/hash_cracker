#pragma once


namespace bq
{
	class md5_cracker
	{
		bq::dictionary m_dictionary;

		std::vector<std::string> m_hash_list;

		size_t m_max_threads;
		size_t m_quantity_found = 0;
		size_t m_quantity_checked = 0;

		bool m_log_to_file = false;
		bool m_silent = false;

		std::mutex m_log_mtx, m_add_mtx, test_mtx;
		
		std::string m_output_buffer;
		std::string m_logfile;

		void log_find(const std::string& hash, const std::string& key)
		{
			std::unique_lock lck {m_log_mtx};

			if(m_log_to_file)
			{
				if(!m_silent)
					m_output_buffer += std::format("{}:{}\n", hash, key);
			}
			else
			{
				if(!m_silent)
					std::cout << hash << ":" << key << std::endl;
			}
			m_quantity_found++;
		}

		void log_results()
		{
			if(m_silent) return;
			std::cout << "--------------------------------------------------------" << std::endl;
			std::cout << "Checked " << (m_quantity_checked/1000000) << "MH" << std::endl;
			std::cout << "Found " << m_quantity_found << " of " << m_hash_list.size() <<  " (" << (float(m_quantity_found) / float(m_hash_list.size()))*100 << "%)" << std::endl;
			std::cout << "--------------------------------------------------------" << std::endl;
		}

		void add_quantity_checked(size_t quantity)
		{
			std::unique_lock lck {m_add_mtx};
			m_quantity_checked += quantity;
		}

		void dictionary_attack_intl()
		{
			std::unique_lock lck {test_mtx};
			auto range = m_dictionary.get_range();
			//std::cout << range.first << " : " << range.second << std::endl;
			size_t attempts = 0;
			for (size_t i = range.first; i < range.second; ++i)
			{
				std::string hash = md5(m_dictionary.at(i));
				for( auto& h : m_hash_list )
				{
					if ( h == hash )
					{
						log_find(hash,m_dictionary.at(i));
					}
					attempts++;
				}
			}
			add_quantity_checked(attempts);
		}
		void dictionary_attack_threadlauncher()
		{
			bq::thread_pool pool{ m_max_threads };
			for (size_t i = 0; i < m_max_threads; ++i)
			{
				pool.run(std::bind(&bq::md5_cracker::dictionary_attack_intl, this));
			}
			pool.wait_for_all();
		}
		void write_output_buffer()
		{
			std::ofstream out(m_logfile);
			out << m_output_buffer;
		}

	public:

		void attack_hash_list(const std::vector<std::string>& list)
		{
			m_quantity_checked = 0;
			m_quantity_found   = 0;
			m_hash_list = list;
			dictionary_attack_threadlauncher();
 			log_results();
		}
		void set_log_to_file(const std::string& file)
		{
			m_log_to_file = true;
			m_logfile = file;
			m_output_buffer.reserve(1000000000);
		}

		size_t quantity_found(){ return m_quantity_found; }
		void silence() { m_silent=true; }
		size_t attempts() { return m_quantity_checked; }

		md5_cracker(const std::string& dictionary_file, size_t max_threads) : m_max_threads(max_threads), m_dictionary(dictionary_file, max_threads) {}
		md5_cracker(const std::vector<std::string>& dictionary_vector, size_t max_threads) : m_max_threads(max_threads), m_dictionary(dictionary_vector, max_threads) {}
		md5_cracker() = delete;
		~md5_cracker() { write_output_buffer();}
	};
}