#define BOOST_TEST_MODULE MainTests
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>
#include "stl_includes.hpp"
#include "md5.hpp"
#include "character_set.hpp"
#include "random_string_generator.hpp"
#include "bruteforce_range_generator.hpp"
#include "dictionary.hpp"
#include "thread_pool.hpp"
#include "md5_cracker.hpp"
#include "application.hpp"
BOOST_AUTO_TEST_CASE(SINGLE_PASSWORD_DICTIONARY_ATTACK)
{
    std::vector<std::string> hashes = {
        md5("password") 
    };
    std::vector<std::string> dictionary = {
        "password",
    };

    bq::md5_cracker cracker(dictionary, 1);
    cracker.silence();
    cracker.attack_hash_list(hashes);
    BOOST_CHECK(cracker.quantity_found() == 1);
    BOOST_CHECK(cracker.attempts() == 1);
}
BOOST_AUTO_TEST_CASE(SINGLE_PASSWORD_DICTIONARY_ATTACK_MT)
{
    std::vector<std::string> hashes = {
        md5("password") 
    };
    std::vector<std::string> dictionary = {
        "password",
    };

    bq::md5_cracker cracker(dictionary, 16);
    cracker.silence();
    cracker.attack_hash_list(hashes);
    BOOST_CHECK(cracker.quantity_found() == 1);
    BOOST_CHECK(cracker.attempts() == 1);
}
BOOST_AUTO_TEST_CASE(MULTI_PASSWORD_DICTIONARY_ATTACK)
{
    std::vector<std::string> hashes = {
        md5("password"),
        md5("password1")
    };
    std::vector<std::string> dictionary = {
        "password",
        "password1"
    };

    bq::md5_cracker cracker(dictionary, 1);
    cracker.silence();
    cracker.attack_hash_list(hashes);
    BOOST_CHECK(cracker.quantity_found() == 2);
    BOOST_CHECK(cracker.attempts() == 4);
}
BOOST_AUTO_TEST_CASE(MULTI_PASSWORD_DICTIONARY_ATTACK_MT)
{
    std::vector<std::string> hashes = {
        md5("password"),
        md5("password1")
    };
    std::vector<std::string> dictionary = {
        "password",
        "password1"
    };

    bq::md5_cracker cracker(dictionary, 16);
    cracker.silence();
    cracker.attack_hash_list(hashes);
    BOOST_CHECK(cracker.quantity_found() == 2);
    BOOST_CHECK(cracker.attempts() == 4);
}
BOOST_AUTO_TEST_CASE(NONE_IN_LIST)
{
    std::vector<std::string> hashes = {
        md5("password"),
        md5("password1")
    };
    std::vector<std::string> dictionary = {
        "bob",
        "dan"
    };

    bq::md5_cracker cracker(dictionary, 1);
    cracker.silence();
    cracker.attack_hash_list(hashes);
    BOOST_CHECK(cracker.quantity_found() == 0);
    BOOST_CHECK(cracker.attempts() == 4);
}
BOOST_AUTO_TEST_CASE(NONE_IN_LIST_MT)
{
    std::vector<std::string> hashes = {
        md5("password"),
        md5("password1")
    };
    std::vector<std::string> dictionary = {
        "bob",
        "dan"
    };

    bq::md5_cracker cracker(dictionary, 1);
    cracker.silence();
    cracker.attack_hash_list(hashes);
    BOOST_CHECK(cracker.quantity_found() == 0);
    BOOST_CHECK(cracker.attempts() == 4);
}
BOOST_AUTO_TEST_CASE(LARGE_HASH_LIST)
{
    std::vector<std::string> hashes = {
        md5("password"),
        md5("password1"),
        md5("password2"),
        md5("password3"),
        md5("password4"),
        md5("password5"),
        md5("password6"),
        md5("password7"),
        md5("password8"),
        md5("password9"),
        md5("password10"),
        md5("password11")
    };
    std::vector<std::string> dictionary = {
        "bob",
        "dan"
    };

    bq::md5_cracker cracker(dictionary, 1);
    cracker.silence();
    cracker.attack_hash_list(hashes);
    BOOST_CHECK(cracker.quantity_found() == 0);
    BOOST_CHECK(cracker.attempts() == 24);
}
BOOST_AUTO_TEST_CASE(LARGE_HASH_LIST_MT)
{
    std::vector<std::string> hashes = {
        md5("password"),
        md5("password1"),
        md5("password2"),
        md5("password3"),
        md5("password4"),
        md5("password5"),
        md5("password6"),
        md5("password7"),
        md5("password8"),
        md5("password9"),
        md5("password10"),
        md5("password11")
    };
    std::vector<std::string> dictionary = {
        "bob",
        "dan"
    };

    bq::md5_cracker cracker(dictionary, 16);
    cracker.silence();
    cracker.attack_hash_list(hashes);
    BOOST_CHECK(cracker.quantity_found()  == 0);
    BOOST_CHECK(cracker.attempts() == 24);
}
