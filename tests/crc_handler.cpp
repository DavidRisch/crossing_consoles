#include <gtest/gtest.h>

#include <iostream>

#include "../src/communication/crc/CRCHandler.h"

void test_calculate_CRCValue() {
  const char* sample = "test";
  const char* sample_single = "@";
  const char* sample_mixed = "435/(/&)ewfc.,. dqwd";
  const char* sample_empty = "";

  ASSERT_EQ(CRCHandler::calculateCRCValue(sample), 3632233996);
  ASSERT_EQ(CRCHandler::calculateCRCValue(sample_single), 2766056989);
  ASSERT_EQ(CRCHandler::calculateCRCValue(sample_mixed), 3861871196);
  ASSERT_EQ(CRCHandler::calculateCRCValue(sample_empty), 0);
}

void test_check_crc_value() {
  const char* sample = "test";
  const char* sample_single = "@";
  const char* sample_mixed = "435/(/&)ewfc.,. dqwd";
  const char* sample_empty = "";
  const crc_value_t checksum_sample = 3632233996;
  const crc_value_t checksum_single = 2766056989;
  const crc_value_t checksum_mixed = 3861871196;
  const crc_value_t checksum_empty = 0;

  ASSERT_TRUE(CRCHandler::checkCRCValue(sample, checksum_sample));
  ASSERT_TRUE(CRCHandler::checkCRCValue(sample_single, checksum_single));
  ASSERT_TRUE(CRCHandler::checkCRCValue(sample_mixed, checksum_mixed));
  ASSERT_TRUE(CRCHandler::checkCRCValue(sample_empty, checksum_empty));

  ASSERT_FALSE(CRCHandler::checkCRCValue(sample, checksum_mixed));
  ASSERT_FALSE(CRCHandler::checkCRCValue(sample_single, checksum_empty));
  ASSERT_FALSE(CRCHandler::checkCRCValue(sample_mixed, checksum_single));
  ASSERT_FALSE(CRCHandler::checkCRCValue(sample_empty, checksum_sample));
}

TEST(CRCHandler, calculateCRCValue) {
  test_calculate_CRCValue();
}

TEST(CRCHandler, checkCRCValue) {
  test_check_crc_value();
}