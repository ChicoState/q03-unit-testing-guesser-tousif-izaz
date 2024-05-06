#include <gtest/gtest.h>
#include "Guesser.h"

class GuesserTest : public ::testing::Test {
protected:
    GuesserTest() {} // Constructor runs before each test
    virtual ~GuesserTest() {} // Destructor cleans up after tests
    virtual void SetUp() {} // Sets up before each test (after constructor)
    virtual void TearDown() {} // Clean up after each test (before destructor)
};

/**
 * Test the constructor and ensure that the secret is truncated at 32 characters.
 */
TEST(GuesserTest, Constructor_TruncatesSecret) {
    Guesser guesser("This is a really long secret that exceeds 32 characters");
    EXPECT_TRUE(guesser.match("This is a really long secret tha"));
    EXPECT_FALSE(guesser.match("This is a really long secret th"));
}

/**
 * Test the distance function for different scenarios.
 */
TEST(GuesserTest, DistanceFunction) {
    Guesser guesser("software");

    EXPECT_EQ(guesser.remaining(), 3);

    // Identical strings
    EXPECT_TRUE(guesser.match("software"));
    EXPECT_EQ(guesser.remaining(), 3);

    // One difference
    EXPECT_FALSE(guesser.match("softwafe"));
    EXPECT_EQ(guesser.remaining(), 2);

    // Two differences
    EXPECT_FALSE(guesser.match("softfare"));
    EXPECT_EQ(guesser.remaining(), 1);

    // Three differences (locked due to brute force attempt)
    EXPECT_FALSE(guesser.match("databare"));
    EXPECT_EQ(guesser.remaining(), 0);
}

/**
 * Test that the match function correctly identifies matching guesses.
 */
TEST(GuesserTest, MatchFunction_CorrectGuesses) {
    Guesser guesser("database");

    EXPECT_TRUE(guesser.match("database"));
    EXPECT_EQ(guesser.remaining(), 3);

    EXPECT_TRUE(guesser.match("database"));
    EXPECT_EQ(guesser.remaining(), 3);
}

/**
 * Test that incorrect guesses decrement the remaining guesses.
 */
TEST(GuesserTest, MatchFunction_IncorrectGuesses) {
    Guesser guesser("software");

    EXPECT_FALSE(guesser.match("softare"));
    EXPECT_EQ(guesser.remaining(), 2);

    EXPECT_FALSE(guesser.match("softwore"));
    EXPECT_EQ(guesser.remaining(), 1);

    EXPECT_FALSE(guesser.match("softwafe"));
    EXPECT_EQ(guesser.remaining(), 0);

    EXPECT_FALSE(guesser.match("software"));
}

/**
 * Test that the secret is locked after three incorrect guesses.
 */
TEST(GuesserTest, MatchFunction_LockAfterThreeIncorrect) {
    Guesser guesser("database");

    EXPECT_FALSE(guesser.match("databse"));
    EXPECT_EQ(guesser.remaining(), 2);

    EXPECT_FALSE(guesser.match("databace"));
    EXPECT_EQ(guesser.remaining(), 1);

    EXPECT_FALSE(guesser.match("databace"));
    EXPECT_EQ(guesser.remaining(), 0);

    EXPECT_FALSE(guesser.match("database"));
}

/**
 * Test that the secret is locked after a brute force attempt (distance > 2).
 */
TEST(GuesserTest, MatchFunction_BruteForceLock) {
    Guesser guesser("database");

    EXPECT_FALSE(guesser.match("databare"));
    EXPECT_EQ(guesser.remaining(), 0);

    EXPECT_FALSE(guesser.match("database"));
}

/**
 * Test that a correct guess resets the number of guesses remaining.
 */
TEST(GuesserTest, CorrectGuessResetsRemaining) {
    Guesser guesser("software");

    EXPECT_FALSE(guesser.match("softare"));
    EXPECT_EQ(guesser.remaining(), 2);

    EXPECT_TRUE(guesser.match("software"));
    EXPECT_EQ(guesser.remaining(), 3);

    EXPECT_FALSE(guesser.match("softfare"));
    EXPECT_EQ(guesser.remaining(), 2);

    EXPECT_TRUE(guesser.match("software"));
    EXPECT_EQ(guesser.remaining(), 3);
}
