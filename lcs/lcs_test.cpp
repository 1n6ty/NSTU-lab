#include "../catch.hpp"

#include "lcs.h"


TEST_CASE( "Both strings are empty" ) {
    CHECK( lcs("", "") == "" );
}

TEST_CASE( "One string is empty" ) {
    CHECK( lcs("", "abcd") == "" );
    CHECK( lcs("abcd", "") == "" );
}

TEST_CASE( "Equal strings" ) {
    CHECK( lcs("abcd", "abcd") == "abcd" );
}

TEST_CASE( "Substring" ) {
    CHECK( lcs("abab", "ab") == "ab" );
}

TEST_CASE( "Substring in the middle" ) {
    CHECK( lcs("xyaban", "abarab") == "aba" );
}

TEST_CASE( "Subsequences" ) {
    CHECK( lcs("nahybser", "iunkayxbis") == "naybs" );
    CHECK( lcs("z1artunx", "yz21rx") == "z1rx" );
    CHECK( lcs("z1arxzyx1a", "yz21rx") == "z1rx" );
    CHECK( lcs("yillnum", "numyjiljil") == "yill" );
}

TEST_CASE( "Reverse subsequence" ) {
    auto result = lcs("xablar", "ralbax");
    CHECK( (result == "aba" || result == "ala") ); 
}


TEST_CASE( "Both strings are empty (brute)" ) {
    CHECK( lcs_brute("", "") == "" );
}

TEST_CASE( "One string is empty (brute)" ) {
    CHECK( lcs_brute("", "abcd") == "" );
    CHECK( lcs_brute("abcd", "") == "" );
}

TEST_CASE( "Equal strings (brute)" ) {
    CHECK( lcs_brute("abcd", "abcd") == "abcd" );
}

TEST_CASE( "Substring (brute)" ) {
    CHECK( lcs_brute("abab", "ab") == "ab" );
}

TEST_CASE( "Substring in the middle (brute)" ) {
    CHECK( lcs_brute("xyaban", "abarab") == "aba" );
}

TEST_CASE( "Subsequences (brute)" ) {
    CHECK( lcs_brute("nahybser", "iunkayxbis") == "naybs" );
    CHECK( lcs_brute("z1artunx", "yz21rx") == "z1rx" );
    CHECK( lcs_brute("z1arxzyx1a", "yz21rx") == "z1rx" );
    CHECK( lcs_brute("yillnum", "numyjiljil") == "yill" );
}

TEST_CASE( "Reverse subsequence (brute)" ) {
    auto result = lcs_brute("xablar", "ralbax");
    CHECK( (result == "aba" || result == "ala") ); 
}
