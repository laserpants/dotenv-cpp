#include <gtest/gtest.h>
#include <dotenv.h>

class BaseTestFixture : public ::testing::Test {
protected:
    void SetUp() override {
        dotenv::init(".env.example");
    }
};

TEST_F(BaseTestFixture, ReadUndefinedVariableWithDefaultValue) {
    const auto _value = dotenv::getenv("UNDEFINED_VAR", "EHLO");
    ASSERT_EQ(_value, "EHLO");
}

TEST_F(BaseTestFixture, ReadDefinedVariableWithDefaultValue) {
    const auto _value = dotenv::getenv("DEFINED_VAR", "EHLO");
    ASSERT_EQ(_value, "OLHE");
}

