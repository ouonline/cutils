#include "cutils/cvector.h"

#undef NDEBUG
#include <assert.h>

static void test_push_back() {
    struct cvector int_vec;
    cvector_init(&int_vec, sizeof(int));

    assert(cvector_push_back(&int_vec, int, 12) == 0);
    assert(cvector_size(&int_vec) == 1);

    assert(cvector_push_back(&int_vec, int, 5) == 0);
    assert(cvector_size(&int_vec) == 2);

    int* ptr = (int*)cvector_at(&int_vec, 1);
    assert(*ptr == 5);

    cvector_destroy(&int_vec);
}

int main(void) {
    test_push_back();
    return 0;
}
