
#include <gtest/gtest.h>
#include <ball.h>

TEST(balls, offset)
{
    Ball a = { 1, 1, 1, 0, 0 };
    Ball b = { 2, 4, 4, 0, 0 };

    a.addSpeed(b, 1);
    ASSERT_NEAR(a._speed_x, 0.1273, 0.0001);
    ASSERT_NEAR(a._speed_y, 0.1273, 0.0001);

    b.addSpeed(a, 1);
    ASSERT_NEAR(b._speed_x, -0.1273, 0.0001);
    ASSERT_NEAR(b._speed_y, -0.1273, 0.0001);
}

TEST(balls, move)
{
    std::vector<Ball> balls;
    balls.push_back( { 1, 1, 1, 0, 0} );
    balls.push_back( { 2, 1, 4, 0, 0} );
    balls.push_back( { 3, 4, 4, 0, 0} );
    balls.push_back( { 4, 4, 1, 0, 0} );

    Ball::move(balls, 1);

    ASSERT_NEAR(balls[0]._x, 1.3496, 0.0001);
    ASSERT_NEAR(balls[0]._y, 1.3496, 0.0001);
    ASSERT_NEAR(balls[1]._x, 1.3496, 0.0001);
    ASSERT_NEAR(balls[1]._y, 3.6503, 0.0001);
    ASSERT_NEAR(balls[2]._x, 3.6503, 0.0001);
    ASSERT_NEAR(balls[2]._y, 3.6503, 0.0001);
    ASSERT_NEAR(balls[3]._x, 3.6503, 0.0001);
    ASSERT_NEAR(balls[3]._y, 1.3496, 0.0001);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
