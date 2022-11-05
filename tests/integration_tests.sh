#!/bin/bash

RETVAL=0

perform_test() {
    echo "Runing test: $1 $2 -k $3 | diff - $4";
    $1 $2 -k $3 | diff - $4;
    #"$1 $2 -k $3 | diff - $4";
    if [ "$?" != "0" ]; then
        echo "Test failed";
        RETVAL=1;
    fi
    return 0;
}

if [ "$#" != "2" ]; then
  echo "Unsupported number of parameters"
  exit 1;
fi

FEED_EXEC="$1"
TEST_BASE_DIR="$2"

perform_test "$FEED_EXEC" "" "${TEST_BASE_DIR}/test_files/normal.atom" "${TEST_BASE_DIR}/reference/ref_atom.txt"

perform_test "$FEED_EXEC" "-a" "${TEST_BASE_DIR}/test_files/normal.atom" "${TEST_BASE_DIR}/reference/ref_atom-a.txt"

perform_test "$FEED_EXEC" "" "${TEST_BASE_DIR}/test_files/not_in_order.atom" "${TEST_BASE_DIR}/reference/ref_not_in_order.txt"

perform_test "$FEED_EXEC" "-a" "${TEST_BASE_DIR}/test_files/not_in_order.atom" "${TEST_BASE_DIR}/reference/ref_not_in_order-a.txt"

perform_test "$FEED_EXEC" "-aT" "${TEST_BASE_DIR}/test_files/not_in_order.atom" "${TEST_BASE_DIR}/reference/ref_not_in_order-aT.txt"

perform_test "$FEED_EXEC" "-aTu" "${TEST_BASE_DIR}/test_files/not_in_order.atom" "${TEST_BASE_DIR}/reference/ref_not_in_order-aTu.txt"

perform_test "$FEED_EXEC" "-u" "${TEST_BASE_DIR}/test_files/not_in_order.atom" "${TEST_BASE_DIR}/reference/ref_not_in_order-u.txt"

perform_test "$FEED_EXEC" "" "${TEST_BASE_DIR}/test_files/normal.rss" "${TEST_BASE_DIR}/reference/ref_rss.txt"

perform_test "$FEED_EXEC" "-a" "${TEST_BASE_DIR}/test_files/normal.rss" "${TEST_BASE_DIR}/reference/ref_rss-a.txt"

perform_test "$FEED_EXEC" "-u" "${TEST_BASE_DIR}/test_files/normal.rss" "${TEST_BASE_DIR}/reference/ref_rss-u.txt"

perform_test "$FEED_EXEC" "-uT" "${TEST_BASE_DIR}/test_files/normal.rss" "${TEST_BASE_DIR}/reference/ref_rss-uT.txt"

exit $RETVAL;
