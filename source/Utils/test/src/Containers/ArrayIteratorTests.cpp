#include <Utils/Containers/ArrayIterator.h>

#include <gtest/gtest.h>

#include <utility>
#include <string>

using namespace apb;
using namespace std;

TEST(ArrayIteratorTests, bufferSizeConctructor_shouldStartToTheBeginning)
{
    constexpr size_t size = 2;
    string buffer[size];
    buffer[0] = "a";
    buffer[1] = "b";

    ArrayIterator<string> it(buffer, size);

    EXPECT_EQ(*it, "a");
    EXPECT_EQ(it->at(0), 'a');
}

TEST(ArrayIteratorTests, bufferSizeIConctructor_shouldStartToI)
{
    constexpr size_t size = 2;
    string buffer[size];
    buffer[0] = "a";
    buffer[1] = "b";

    ArrayIterator<string> it(buffer, size, 1);

    EXPECT_EQ(*it, "b");
}

TEST(ArrayIteratorTests, copyConctructor_shouldCopy)
{
    constexpr size_t size = 2;
    string buffer[size];
    buffer[0] = "a";
    buffer[1] = "b";

    ArrayIterator<string> it1(buffer, size, 1);
    ArrayIterator<string> it2(it1);

    EXPECT_EQ(*it2, "b");
}

TEST(ArrayIteratorTests, assignationOperator_shouldCopy)
{
    constexpr size_t size = 2;
    string buffer[size];
    buffer[0] = "a";
    buffer[1] = "b";

    ArrayIterator<string> it1(buffer, size, 1);
    ArrayIterator<string> it2(nullptr, 0);
    it2 = it1;

    EXPECT_EQ(*it2, "b");
}

TEST(ArrayIteratorTests, operatorAddAssignation_shouldMoveTheIterator)
{
    constexpr size_t size = 3;
    string buffer[size];
    buffer[0] = "a";
    buffer[1] = "b";
    buffer[2] = "c";

    ArrayIterator<string> it(buffer, size);

    EXPECT_EQ(*it, "a");
    it += 2;
    EXPECT_EQ(*it, "c");
}

TEST(ArrayIteratorTests, operatorSubtractAssignation_shouldMoveTheIterator)
{
    constexpr size_t size = 3;
    string buffer[size];
    buffer[0] = "a";
    buffer[1] = "b";
    buffer[2] = "c";

    ArrayIterator<string> it(buffer, size, 2);

    EXPECT_EQ(*it, "c");
    it -= 2;
    EXPECT_EQ(*it, "a");
}

TEST(ArrayIteratorTests, operatorIncrement_shouldMoveTheIteratorBy1)
{
    constexpr size_t size = 3;
    string buffer[size];
    buffer[0] = "a";
    buffer[1] = "b";
    buffer[2] = "c";

    ArrayIterator<string> it(buffer, size);

    EXPECT_EQ(*it, "a");
    EXPECT_EQ(*(it++), "a");
    EXPECT_EQ(*it, "b");
    EXPECT_EQ(*(++it), "c");
    EXPECT_EQ(*it, "c");
}

TEST(ArrayIteratorTests, operatorDecrement_shouldMoveTheIteratorBy1)
{
    constexpr size_t size = 3;
    string buffer[size];
    buffer[0] = "a";
    buffer[1] = "b";
    buffer[2] = "c";

    ArrayIterator<string> it(buffer, size, 2);

    EXPECT_EQ(*it, "c");
    EXPECT_EQ(*(it--), "c");
    EXPECT_EQ(*it, "b");
    EXPECT_EQ(*(--it), "a");
    EXPECT_EQ(*it, "a");
}

TEST(ArrayIteratorTests, operatorAdd_shouldMoveTheResultingIterator)
{
    constexpr size_t size = 3;
    string buffer[size];
    buffer[0] = "a";
    buffer[1] = "b";
    buffer[2] = "c";

    ArrayIterator<string> it(buffer, size);
    auto result1 = it + 2;
    auto result2 = 2 + it;

    EXPECT_EQ(*result1, "c");
    EXPECT_EQ(*result2, "c");
}


TEST(ArrayIteratorTests, operatorSubtract_shouldMoveTheResultingIterator)
{
    constexpr size_t size = 3;
    string buffer[size];
    buffer[0] = "a";
    buffer[1] = "b";
    buffer[2] = "c";

    ArrayIterator<string> it(buffer, size, 2);
    auto result1 = it - 2;
    auto result2 = 2 - it;
    size_t distance = it - result1;

    EXPECT_EQ(distance, 2);
    EXPECT_EQ(*result1, "a");
    EXPECT_EQ(*result2, "a");
}