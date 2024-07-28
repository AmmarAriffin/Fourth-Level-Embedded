#include "unity.h"
#include "circBufT.h"

const uint8_t STANDARD_TEST_CAPACITY = 5;
static circBuf_t buff;

uint32_t test_MAX_SIZE = 100;

void setUp(void)
{
    initCircBuf(&buff, STANDARD_TEST_CAPACITY);
}

void tearDown(void)
{
    freeCircBuf(&buff);
}

/* Helper functions */
void writeConsecutiveSequenceToBuffer(uint16_t start, uint16_t size)
{
    for (uint16_t i = 0; i < size; i++) {
      writeCircBuf(&buff, start + i);
    }
} 

void assertReadingSequence(uint16_t start, uint16_t size)
{
    int32_t entry;
    for (uint16_t i = 0; i < size; i++) {
        readCircBuf(&buff, &entry);
        TEST_ASSERT_EQUAL(start + i, entry);
    }
} 

int32_t * reconstructBufferWithSize(uint16_t size)
{
  freeCircBuf(&buff);
  return initCircBuf(&buff, size);
}

/* Test cases */
void test_new_buffer_is_empty(void)
{
    // Arrange: given buffer is empty

    // Act: when buffer is read
    int32_t entry;

    // Assert: then 0 is returned
    TEST_ASSERT_EQUAL(0, readCircBuf(&buff, &entry));
}

void test_single_element_in_single_element_out(void)
{
    // Arrange: given buffer has a single element
    writeCircBuf(&buff, 11);

    // Act: when buffer is read
    int32_t entry;
    readCircBuf(&buff, &entry);

    // Assert: then the same value is returned
    TEST_ASSERT_EQUAL(11, entry);
}

void test_first_element_in_first_element_out(void)
{
    // Arrange
    writeConsecutiveSequenceToBuffer(20, STANDARD_TEST_CAPACITY);

    // Act/Assert
    assertReadingSequence(20, STANDARD_TEST_CAPACITY);
}

void test_write_and_read_indices_are_independent(void)
{
    for (uint8_t i = 0; i < STANDARD_TEST_CAPACITY; i++)
    {
      // Arrange: given one element is written
      writeCircBuf(&buff, 20 + i);

      // Act: when buffer is read
      int32_t entry;
      readCircBuf(&buff, &entry);
      int32_t value = entry;

      // Assert: the last written element is returned
      TEST_ASSERT_EQUAL(20 + i, value);
    }
}

void test_buffer_is_clean_after_full_buffer_cycle_completed(void)
{
    // Arange: given buffer is fully written to and and then fully read from
    int i;
    for (i=0; i < 5; i++){
        writeCircBuf(&buff, 1);
    }
    // Act: when buffer is read
    int32_t entry;
    
    for (i=0; i < 5; i++){
        readCircBuf(&buff, &entry);
    }
    // Assert: same behaviour as when buffer was empty
    
    TEST_ASSERT_EQUAL(readCircBuf(&buff, &entry), 0);
}

void test_buffer_is_circular(void)
{

    // Arange: given buffer is fully written to and then fully read from
    writeConsecutiveSequenceToBuffer(1, STANDARD_TEST_CAPACITY);
    assertReadingSequence(1, STANDARD_TEST_CAPACITY);

    // Arrange: given a new value is written
    writeCircBuf(&buff,69);

    // Act: when buffer is read
    int32_t entry;
    readCircBuf(&buff, &entry);

    // Assert: the last written element is returned
    TEST_ASSERT_EQUAL(69, entry);

}

void test_no_values_overwritten_after_full(void)
{
    // Arrange: given buffer is filled to capacity
    writeConsecutiveSequenceToBuffer(1, STANDARD_TEST_CAPACITY);
    // Given: when one more element is written to buffer
    writeCircBuf(&buff,69);
    // Assert: first element in, first element out, no overflow
    int32_t entry;
    readCircBuf(&buff, &entry);
    TEST_ASSERT_NOT_EQUAL(69, entry);
}

void test_min_capacity_when_buffer_is_created_then_buffer_empty(void)
{
    // Arrange
    reconstructBufferWithSize(1);

    // Act/Assert
    int32_t entry;
    readCircBuf(&buff, &entry);
    TEST_ASSERT_EQUAL(0, entry);
    // test
}

void test_min_capacity_when_single_element_written_to_buffer_then_same_value_is_read(void)
{
    // Arrange
    reconstructBufferWithSize(1);

    // Act
    writeCircBuf(&buff, 87);

    // Act/Assert
    int32_t entry;
    readCircBuf(&buff, &entry);
    TEST_ASSERT_EQUAL(87, entry);
}

void test_capacity_0_invalid(void)
{

    // Arrange/Act
    int32_t *val = initCircBuf(&buff, 0);

    // Assert: the return value of initCircBuf is NULL
    TEST_ASSERT_EQUAL(val, NULL);
}

void test_capacity_higher_than_max_invalid(void)
{
    // Arrange/Act
    int32_t *val = initCircBuf(&buff, test_MAX_SIZE + 1);

    // Assert: the return value of initCircBuf is NULL
    TEST_ASSERT_EQUAL(val, NULL);
}

void test_buf_is_unsigned(void)
{

    writeCircBuf(&buff, 0);
    int32_t entry;
    readCircBuf(&buff, &entry);
    TEST_ASSERT_EQUAL(-1, entry-1);
}