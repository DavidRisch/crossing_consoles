#include <gtest/gtest.h>
#include <iostream>

#include "../src/communication/crc/CRCHandler.h"

void test_calculate_CRCValue() {
  const char sample[]= {'t','e', 's', 't'};
  ASSERT_EQ(CRCHandler::calculateCRCValue(sample),3632233996)



}

TEST(CRCHandler, calculateCRCValue) {
  test_calculate_CRCValue();
}