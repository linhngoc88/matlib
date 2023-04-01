#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <math.h>
#include <ins/ins_vector.h>

static void test_vector_min_stride_one(void **state) {
  (void) state;

  ins_vector *v = ins_vector_alloc(3);

  v->data[0] = 1.0;
  v->data[1] = 0.5;
  v->data[2] = 4.0;

  assert_double_equal(ins_vector_min(v), 0.5, 0.0);

  v->data[0] = -0.25;
  assert_double_equal(ins_vector_min(v), -0.25, 0.0);

  v->data[2] = -0.75;
  assert_double_equal(ins_vector_min(v), -0.75, 0.0);

  v->data[1] = -4.0;
  assert_double_equal(ins_vector_min(v), -4.0, 0.0);

  ins_vector_free(v);
}

static void test_vector_min_stride_two(void **state) {
  (void) state;

  ins_vector *v = ins_vector_alloc(5);

  v->data[0] = 1.0;
  v->data[1] = -0.5;
  v->data[2] = 0.5;
  v->data[3] = 2.0;
  v->data[4] = 4.0;

  v->size = 3;
  v->stride = 2;

  assert_double_equal(ins_vector_min(v), 0.5, 0.0);

  ins_vector_set(v, 0, -0.25);
  assert_double_equal(ins_vector_min(v), -0.25, 0.0);

  ins_vector_set(v, 2, -0.75);
  assert_double_equal(ins_vector_min(v), -0.75, 0.0);

  ins_vector_set(v, 1, -4.0);
  assert_double_equal(ins_vector_min(v), -4.0, 0.0);

  ins_vector_free(v);
}

static void test_vector_min_nan(void **state) {
  (void) state;

  ins_vector *v = ins_vector_alloc(3);
  v->data[0] = 1.0;
  v->data[1] = sqrt(-1.0);
  v->data[2] = -0.75;

  const double min = ins_vector_min(v);
  assert_int_equal(isnan(min), 1);

  ins_vector_free(v);
}

static void test_vector_max_stride_one(void **state) {
  (void) state;

  ins_vector *v = ins_vector_alloc(3);

  v->data[0] = 1.0;
  v->data[1] = 0.5;
  v->data[2] = 4.0;

  assert_double_equal(ins_vector_max(v), 4.0, 0.0);

  v->data[0] = 4.25;
  assert_double_equal(ins_vector_max(v), 4.25, 0.0);

  v->data[2] = 10.0;
  assert_double_equal(ins_vector_max(v), 10.0, 0.0);

  v->data[1] = 16.0;
  assert_double_equal(ins_vector_max(v), 16.0, 0.0);

  ins_vector_free(v);
}

static void test_vector_max_stride_two(void **state) {
  (void) state;

  ins_vector *v = ins_vector_alloc(5);

  v->data[0] = 1.0;
  v->data[1] = 2.0;
  v->data[2] = 0.5;
  v->data[3] = 7.5;
  v->data[4] = 4.0;

  v->size = 3;
  v->stride = 2;

  assert_double_equal(ins_vector_max(v), 4.0, 0.0);

  ins_vector_set(v, 0, 4.25);
  assert_double_equal(ins_vector_max(v), 4.25, 0.0);

  ins_vector_set(v, 2, 10.0);
  assert_double_equal(ins_vector_max(v), 10.0, 0.0);

  ins_vector_set(v, 1, 16.0);
  assert_double_equal(ins_vector_max(v), 16.0, 0.0);

  ins_vector_free(v);
}

static void test_vector_max_nan(void **state) {
  (void) state;

  ins_vector *v = ins_vector_alloc(3);

  v->data[0] = 1.0;
  v->data[1] = sqrt(-4.0);
  v->data[2] = 4.5;

  const double max = ins_vector_max(v);
  assert_int_equal(isnan(max), 1);

  ins_vector_free(v);
}

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_vector_min_stride_one),
    cmocka_unit_test(test_vector_min_stride_two),
    cmocka_unit_test(test_vector_min_nan),
    cmocka_unit_test(test_vector_max_stride_one),
    cmocka_unit_test(test_vector_max_stride_two),
    cmocka_unit_test(test_vector_max_nan)
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
