#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <ins/ins_vector.h>

static void alloc_success(void **state) {
  (void) state; /* unused */

  ins_vector *v = ins_vector_alloc(3);

  assert_non_null(v);
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 1);
  assert_non_null(v->data);
  assert_non_null(v->block);
  assert_int_equal(v->owner, 1);
  assert_ptr_equal(v->block->data, v->data);
  assert_int_equal(v->block->size, v->size);

  ins_vector_free(v);
}

static void calloc_success(void **state) {
  (void) state; /* unused */

  ins_vector *v = ins_vector_calloc(3);

  assert_non_null(v);
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 1);
  assert_non_null(v->data);
  assert_non_null(v->block);
  assert_int_equal(v->owner, 1);
  assert_ptr_equal(v->block->data, v->data);
  assert_int_equal(v->block->size, v->size);

  const double expected_mem[] = {0.0, 0.0, 0.0};
  assert_memory_equal(v->data, expected_mem, v->size * sizeof(double));

  ins_vector_free(v);
}

static void test_alloc_from_block_offset_zero_stride_one(void **state) {
  (void) state; /* unused */

  ins_block *block = ins_block_alloc(3);
  ins_vector *vector = ins_vector_alloc_from_block(block, 0, 3, 1);

  assert_non_null(vector);
  assert_int_equal(vector->size, 3);
  assert_int_equal(vector->stride, 1);

  assert_non_null(vector->data);
  assert_ptr_equal(vector->data, block->data);
  assert_memory_equal(vector->data, block->data, 3 * sizeof(double));

  assert_non_null(vector->block);
  assert_ptr_equal(vector->block, block);
  assert_int_equal(vector->owner, 0);

  ins_vector_free(vector);
  ins_block_free(block);
}

static void test_alloc_from_block_offset_zero_stride_two(void **state) {
  (void) state; /* unused */

  ins_block *block = ins_block_alloc(5);
  ins_vector *vector = ins_vector_alloc_from_block(block, 0, 3, 2);

  assert_non_null(vector);
  assert_int_equal(vector->size, 3);
  assert_int_equal(vector->stride, 2);

  assert_non_null(vector->data);
  assert_ptr_equal(vector->data, block->data);

  assert_non_null(vector->block);
  assert_ptr_equal(vector->block, block);
  assert_int_equal(vector->owner, 0);

  ins_vector_free(vector);
  ins_block_free(block);
}

static void test_alloc_from_block_offset_one_stride_one(void **state) {
  (void) state; /* unused */

  ins_block *block = ins_block_alloc(4);
  ins_vector *vector = ins_vector_alloc_from_block(block, 1, 3, 1);

  assert_non_null(vector);
  assert_int_equal(vector->size, 3);
  assert_int_equal(vector->stride, 1);

  assert_non_null(vector->data);
  assert_ptr_equal(vector->data, block->data + 1);
  assert_memory_equal(vector->data, block->data + 1, 3 * sizeof(double));

  assert_non_null(vector->block);
  assert_ptr_equal(vector->block, block);
  assert_int_equal(vector->owner, 0);

  ins_vector_free(vector);
  ins_block_free(block);
}

static void test_alloc_from_block_offset_one_stride_two(void **state) {
  (void) state; /* unused */

  ins_block *block = ins_block_alloc(6);
  ins_vector *vector = ins_vector_alloc_from_block(block, 1, 3, 2);

  assert_non_null(vector);
  assert_int_equal(vector->size, 3);
  assert_int_equal(vector->stride, 2);

  assert_non_null(vector->data);
  assert_ptr_equal(vector->data, block->data + 1);

  assert_non_null(vector->block);
  assert_ptr_equal(vector->block, block);
  assert_int_equal(vector->owner, 0);

  ins_vector_free(vector);
  ins_block_free(block);
}

static void test_alloc_from_vector_offset_zero_stride_one(void **state) {
  (void) state; /* unused */

  ins_vector *v = ins_vector_alloc(3);
  ins_vector *w = ins_vector_alloc_from_vector(v, 0, 3, 1);

  assert_non_null(w);
  assert_int_equal(w->size, 3);
  assert_int_equal(w->stride, 1);

  assert_non_null(w->data);
  assert_ptr_equal(w->data, v->data);
  assert_memory_equal(w->data, v->data, 3 * sizeof(double));

  assert_non_null(w->block);
  assert_ptr_equal(w->block, v->block);
  assert_int_equal(w->owner, 0);

  ins_vector_free(w);
  ins_vector_free(v);
}

static void test_alloc_from_vector_offset_zero_stride_two(void **state) {
  (void) state; /* unused */

  ins_vector *v = ins_vector_alloc(5);
  ins_vector *w = ins_vector_alloc_from_vector(v, 0, 3, 2);

  assert_non_null(w);
  assert_int_equal(w->size, 3);
  assert_int_equal(w->stride, 2);

  assert_non_null(w->data);
  assert_ptr_equal(w->data, v->data);

  assert_non_null(w->block);
  assert_ptr_equal(w->block, v->block);
  assert_int_equal(w->owner, 0);

  ins_vector_free(w);
  ins_vector_free(v);
}

static void test_alloc_from_vector_offset_one_stride_one(void **state) {
  (void) state; /* unused */

  ins_vector *v = ins_vector_alloc(4);
  ins_vector *w = ins_vector_alloc_from_vector(v, 1, 3, 1);

  assert_non_null(w);
  assert_int_equal(w->size, 3);
  assert_int_equal(w->stride, 1);

  assert_non_null(w->data);
  assert_ptr_equal(w->data, v->data + 1);

  assert_non_null(w->block);
  assert_ptr_equal(w->block, v->block);
  assert_int_equal(w->owner, 0);

  ins_vector_free(w);
  ins_vector_free(v);
}

static void test_alloc_from_vector_offset_two_stride_two(void **state) {
  (void) state; /* unused */

  ins_vector *v = ins_vector_alloc(7);
  ins_vector *w = ins_vector_alloc_from_vector(v, 2, 3, 2);

  assert_non_null(w);
  assert_int_equal(w->size, 3);
  assert_int_equal(w->stride, 2);

  assert_non_null(w->data);
  assert_ptr_equal(w->data, v->data + 2);

  assert_non_null(w->block);
  assert_ptr_equal(w->block, v->block);
  assert_int_equal(w->owner, 0);

  ins_vector_free(w);
  ins_vector_free(v);
}

static void test_alloc_from_block_and_vector(void **state) {
  (void) state; /* unused */

  ins_block *b = ins_block_alloc(15);
  ins_vector *v = ins_vector_alloc_from_block(b, 0, 8, 2);
  ins_vector *w = ins_vector_alloc_from_vector(v, 1, 3, 3);

  assert_non_null(v);
  assert_int_equal(v->size, 8);
  assert_int_equal(v->stride, 2);
  assert_non_null(v->data);
  assert_ptr_equal(v->data, b->data);
  assert_non_null(v->block);
  assert_ptr_equal(v->block, b);
  assert_int_equal(v->owner, 0);

  assert_non_null(w);
  assert_int_equal(w->size, 3);
  assert_int_equal(w->stride, 6);
  assert_non_null(w->data);
  assert_ptr_equal(w->data, v->data + 2);
  assert_ptr_equal(w->block, v->block);
  assert_int_equal(w->owner, 0);

  ins_vector_free(w);
  ins_vector_free(v);
  ins_block_free(b);
}

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(alloc_success),
    cmocka_unit_test(calloc_success),
    cmocka_unit_test(test_alloc_from_block_offset_zero_stride_one),
    cmocka_unit_test(test_alloc_from_block_offset_zero_stride_two),
    cmocka_unit_test(test_alloc_from_block_offset_one_stride_one),
    cmocka_unit_test(test_alloc_from_block_offset_one_stride_two),
    cmocka_unit_test(test_alloc_from_vector_offset_zero_stride_one),
    cmocka_unit_test(test_alloc_from_vector_offset_zero_stride_two),
    cmocka_unit_test(test_alloc_from_vector_offset_one_stride_one),
    cmocka_unit_test(test_alloc_from_vector_offset_two_stride_two),
    cmocka_unit_test(test_alloc_from_block_and_vector)
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
