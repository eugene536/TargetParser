#ifdef _GTEST
#include <bits/stdc++.h>
#include <cassert>
#include <Parser.h>
#include <gtest/gtest.h>

using namespace std;
using namespace target_parser;

TEST(Parser, FailedParser) {
    int unp;
    ASSERT_FALSE(Parse(R"(i9p = "192.*" & host = "goo*" | http_ip = "172*" )", &unp).IsValid());
    ASSERT_EQ(unp, 1);

    ASSERT_FALSE(Parse(R"(imsdn = "123 & !(host = "goo*" | ip = "19*") )", &unp).IsValid());
    ASSERT_EQ(unp, 25);
}

TEST(Parser, SuccessParser) {
    int unp = -1;

    ASSERT_TRUE(Parse(R"(ip = "192.*" & host = "goo*" | http_ip = "172*" )", &unp).IsValid());
    ASSERT_EQ(unp, -1);

    ASSERT_TRUE(Parse(R"(imsdn = "123" & !(host = "goo*" | ip = "19*") )", &unp).IsValid());
    ASSERT_EQ(unp, -1);

    ASSERT_TRUE(Parse(R"( ip = "123*" | host = "goo\"gle" & !(imsdn = "10" & email = "*gm\"ail\"" | phone = "*9\"52*") )", &unp).IsValid());
    ASSERT_EQ(unp, -1);
}

TEST(Parser, PatternTree) {
    PatternTree pt = Parse(R"( ip = "123*" | host = "goo\"gle" & !(imsdn = "10" & email = "*gm\"ail\"" | phone = "*9\"52*") )");
    ASSERT_TRUE(pt.IsValid());

    std::stringstream ss;
    ss << pt;
    ASSERT_EQ(ss.str(), R"((((("ip 123*"))) | ((("host goo\"gle")) & !(((((("imsdn 10")) & ("email *gm\"ail\""))) | (("phone *9\"52*")))))))");
    ASSERT_EQ(pt.ToString(), R"((((("ip 123*"))) | ((("host goo\"gle")) & !(((((("imsdn 10")) & ("email *gm\"ail\""))) | (("phone *9\"52*")))))))");

    ASSERT_FALSE(pt.Calculate());

    std::set<std::pair<std::string, std::string>> values =
        {{"ip", "123*"},
         {"host", "goo\\\"gle"},
         {"imsdn", "10"},
         {"email", "*gm\\\"ail\\\""},
         {"phone", "*9\\\"52*"}};
     ;

    pt.ForEachVertex(
        [&values](const Vertex& v) {
            ASSERT_TRUE(values.erase({v.pattern_name, v.pattern}));
        }
    );

    // F | (F & !(F & F | F))
    ASSERT_FALSE(pt.Calculate());

    pt.SetValue(1, true);
    // F | (T & !(F & F | F))
    ASSERT_TRUE(pt.Calculate());

    pt.SetValue(4, true);
    // F | (T & !(F & F | T))
    ASSERT_FALSE(pt.Calculate());

    pt.SetValue(0, true);
    // T | (T & !(F & F | T))
    ASSERT_TRUE(pt.Calculate());

    pt.SetValue(0, false);
    // F | (T & !(F & F | T))
    ASSERT_FALSE(pt.Calculate());

    pt.SetValue(4, false);
    // F | (T & !(F & F | F))
    ASSERT_TRUE(pt.Calculate());

    pt.SetValue(2, true);
    // F | (T & !(T & F | F))
    ASSERT_TRUE(pt.Calculate());

    pt.SetValue(3, true);
    // F | (T & !(T & T | F))
    ASSERT_FALSE(pt.Calculate());
}

#endif
