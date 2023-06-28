#pragma once

namespace bq {

	
	class dictionary {

		std::vector<std::string> m_strings;
		std::vector<std::pair<size_t, size_t>> m_ranges;
		
		std::mutex m_mtx;

		size_t m_thread_count;
		size_t index = 0;

		void load_ranges()
		{
			size_t distance_between_points = m_strings.size() / m_thread_count;

			for (size_t i = 0; i < m_thread_count; ++i)
			{
				m_ranges.push_back({ i*distance_between_points , (i*distance_between_points)+distance_between_points });
			}
			m_ranges[m_ranges.size() - 1].second = m_strings.size();
		}

		void load_dictionary(std::string filename)
		{
			std::ifstream input(filename);
			std::string line;
			while (std::getline(input, line))
			{
				m_strings.push_back(line);
			}
			input.clear();
			input.close();
			m_strings.shrink_to_fit();
		}

	public:
		dictionary()					= delete;
		dictionary(dictionary&)  = delete;
		dictionary(dictionary&&) = delete;
		dictionary(const std::string& filename, size_t threadcount) : m_thread_count(threadcount)
		{
			load_dictionary(filename);
			load_ranges();
		}
		dictionary(const std::vector<std::string>& dictionary_vector, size_t threadcount) : m_thread_count(threadcount)
		{
			m_strings = dictionary_vector;
			load_ranges();
		}
		//returns the range of indexes in the dictionary this thread is expected to search
		//these must be asked for 1 hash at a time (m_thread_count ranges must be reqested before a second set of ranges can be asked for)
		std::pair<size_t,size_t> get_range()
		{
			std::unique_lock lck{ m_mtx };
			if (index == m_ranges.size()) index = 0;
			return m_ranges[index++];
		}
		const std::string& at(size_t index) { return m_strings[index]; }
		size_t size() { return m_strings.size(); }

	};
}

