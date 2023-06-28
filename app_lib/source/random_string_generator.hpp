#pragma once

namespace bq {
	class random_string_generator
	{
		std::random_device                  rand_dev;
		std::mt19937                        generator;
		std::uniform_int_distribution<size_t>  distr;
	public:

		random_string_generator() : generator(rand_dev()), distr(0, crackable_character_set.size() - 1) {}

		std::string random_string(size_t ch)
		{
			std::string result = "";
			for (size_t i = 0; i < ch; i++)
				result = result + crackable_character_set[distr(generator)];

			return result;
		}
	};
}

