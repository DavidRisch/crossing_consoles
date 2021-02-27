#include <gtest/gtest.h>

#include <iostream>

#include "../src/communication/crc/CRCHandler.h"

/*
 * All sample crc values from https://crccalc.com/
 */

TEST(CRCHandler, CalculateCRCValue) {
  const uint8_t sample[] = {'t', 'e', 's', 't'};
  const uint8_t sample_single[] = {'@'};
  const uint8_t sample_mixed[] = {'4', '3', '5', '/', '(', '/', '&', ')', 'e', 'w',
                                  'f', 'c', '.', ',', '.', ' ', 'd', 'q', 'w', 'd'};
  const uint8_t sample_empty[] = {""};
  const uint8_t sample_null[] = {'\0'};

  ASSERT_EQ(CRCHandler::CalculateCRCValue(sample, sizeof(sample)), 3632233996);
  ASSERT_EQ(CRCHandler::CalculateCRCValue(sample_single, sizeof(sample_single)), 2766056989);
  ASSERT_EQ(CRCHandler::CalculateCRCValue(sample_mixed, sizeof(sample_mixed)), 3861871196);
  ASSERT_EQ(CRCHandler::CalculateCRCValue(sample_empty, sizeof(sample_empty)), 3523407757);
  ASSERT_EQ(CRCHandler::CalculateCRCValue(sample_null, sizeof(sample_null)), 3523407757);
}

TEST(CRCHandler, CheckCRCValue) {
  const uint8_t sample[] = {'t', 'e', 's', 't'};
  const uint8_t sample_single[] = {'@'};
  const uint8_t sample_mixed[] = {'4', '3', '5', '/', '(', '/', '&', ')', 'e', 'w',
                                  'f', 'c', '.', ',', '.', ' ', 'd', 'q', 'w', 'd'};
  const uint8_t sample_empty[] = {""};
  const crc_value_t checksum_sample = 3632233996;
  const crc_value_t checksum_single = 2766056989;
  const crc_value_t checksum_mixed = 3861871196;
  const crc_value_t checksum_empty = 3523407757;

  ASSERT_TRUE(CRCHandler::CheckCRCValue(sample, sizeof(sample), checksum_sample));
  ASSERT_TRUE(CRCHandler::CheckCRCValue(sample_single, sizeof(sample_single), checksum_single));
  ASSERT_TRUE(CRCHandler::CheckCRCValue(sample_mixed, sizeof(sample_mixed), checksum_mixed));
  ASSERT_TRUE(CRCHandler::CheckCRCValue(sample_empty, sizeof(sample_empty), checksum_empty));

  ASSERT_FALSE(CRCHandler::CheckCRCValue(sample, sizeof(sample), checksum_mixed));
  ASSERT_FALSE(CRCHandler::CheckCRCValue(sample_single, sizeof(sample_empty), checksum_empty));
  ASSERT_FALSE(CRCHandler::CheckCRCValue(sample_mixed, sizeof(sample_mixed), checksum_single));
  ASSERT_FALSE(CRCHandler::CheckCRCValue(sample_empty, sizeof(sample_mixed), checksum_sample));
}
