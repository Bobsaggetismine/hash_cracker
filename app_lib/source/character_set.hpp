#pragma once

#define ALPHABETIC_CHARSET false

namespace bq {

#if ALPHABETIC_CHARSET
	const std::vector<char> crackable_character_set = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
#else
	const std::vector<char> crackable_character_set = { '0','1','2','3','4','5','6','7','8','9' };
#endif
}
