#include "user_model.hpp"

TEST_CASE_METHOD(UserModelTestsFixture, "Select column query tests", "[model][query_relation][select]") {
    SECTION("select user name"){
        INFO(User::select<User::Name>().where(User::Name{"user1"}).to_sql());
        if (const auto [error, user_names] = User::select<User::Name>().where(User::Name{"user1"}).exec(conn); error) {
            FAIL(error.value());
        }
        else {
            REQUIRE(typeid(user_names).name() == typeid(std::vector<std::tuple<User::Name>>).name());
            REQUIRE(user_names.size() == 1);
            REQUIRE(std::get<0>(user_names[0]).value() == active_record::string{ "user1" });
        }
    }

    SECTION("pluck user name") {
        INFO(User::pluck<User::Name>().where(User::ID{1}).to_sql());
        if (const auto [error, user_names] = User::pluck<User::Name>().where(User::ID{1}).exec(conn); error) {
            FAIL(error.value());
        }
        else {
            REQUIRE(typeid(user_names).name() == typeid(std::vector<User::Name>).name());
            REQUIRE(user_names.size() == 1);
            REQUIRE(user_names[0].value() == active_record::string{ "user1" });
        }
    }
}