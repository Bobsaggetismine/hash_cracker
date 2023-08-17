#pragma once

namespace bq
{
	

	struct range_data
	{
		std::string start;
		std::string end;
	};

	class bruteforce_range_generator {

		std::vector<std::string> m_starts;
		std::vector<std::string> m_ends;
		std::mutex m_mtx;

		random_string_generator m_random_string_gen;

		size_t m_index = 0;

		void generate_ends()
		{
			for (size_t i = 0; i < m_starts.size(); ++i)
			{
				std::string end = "";
				if (i == m_starts.size() - 1)
				{
					end = m_starts[0];
				}
				else
				{
					end = m_starts[i + 1];
				}
				m_ends.push_back(end);
			}
			
		}

		void generate_excess_thread_starts(size_t thread_count, size_t crack_length)
		{
			for (size_t i = 0; i < thread_count; ++i)
			{
				std::string start = "";
				if (i > crackable_character_set.size() - 1)
				{
					start = m_random_string_gen.random_string(crack_length);
				}
				else
				{
					start.append(crack_length, crackable_character_set[i]);
				}
				m_starts.push_back(start);
			}
			std::sort(m_starts.begin(), m_starts.end());
			generate_ends();
		}
		void genearte_excess_character_starts(size_t thread_count, size_t crack_length)
		{

			size_t increment = crackable_character_set.size() / thread_count;

			for (size_t i = 0; i < thread_count; i+= increment)
			{
				std::string start = "";
				start.append(crack_length, crackable_character_set[i]);
				m_starts.push_back(start);
			}
			std::sort(m_starts.begin(), m_starts.end());
			generate_ends();
		}
		void generate_starts(size_t thread_count, size_t crack_length)
		{
			if (thread_count >= crack_length)
			{
				generate_excess_thread_starts(thread_count, crack_length);
			}
			else
			{
				genearte_excess_character_starts(thread_count, crack_length);
			}
		}

	public:

		bruteforce_range_generator()						     = delete;
		bruteforce_range_generator(bruteforce_range_generator&)  = delete;
		bruteforce_range_generator(bruteforce_range_generator&&) = delete;
		bruteforce_range_generator(size_t thread_count, size_t crack_length)
		{
			generate_starts(thread_count, crack_length);
		}

		void set_character_count(size_t thread_count, size_t crack_length)
		{
			m_starts.clear();
			m_ends.clear();
			m_index = 0;
			generate_starts(thread_count, crack_length);
		}

		range_data get_start()
		{
			std::unique_lock lck{ m_mtx };
			range_data data = { m_starts[m_index],m_ends[m_index] };
			m_index++;
			return data;
		}

		void increment_string(std::string& str)
		{
			for (auto i = str.rbegin(); i < str.rend(); ++i)
			{
				if ((*i) == bq::crackable_character_set[crackable_character_set.size() - 1])
				{
					(*i) = crackable_character_set[0];
					continue;
				}
				else
				{
					(*i)++;
					break;
				}
			}
		}

	};


}
