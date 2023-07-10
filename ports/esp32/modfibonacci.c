#include "py/runtime.h"
#include "py/obj.h"
#include "py/objstr.h"

STATIC mp_obj_t fibonacci_string(mp_obj_t n_obj) {
    mp_int_t n = mp_obj_get_int(n_obj);

    mp_obj_list_t *fibonacci = MP_OBJ_TO_PTR(mp_obj_new_list(n, NULL));
    fibonacci->items[0] = mp_obj_new_int(0);
    fibonacci->items[1] = mp_obj_new_int(1);

    for (mp_int_t i = 2; i < n; i++) {
        mp_obj_t sum = mp_binary_op(MP_BINARY_OP_ADD, fibonacci->items[i-1], fibonacci->items[i-2]);
        fibonacci->items[i] = sum;
    }

    // mp_obj_t result = mp_obj_list_str((mp_obj_t)fibonacci);
    // mp_obj_list_clear((mp_obj_t)fibonacci);

    return MP_OBJ_FROM_PTR(fibonacci);
}

STATIC MP_DEFINE_CONST_FUN_OBJ_1(fibonacci_string_obj, fibonacci_string);

STATIC const mp_rom_map_elem_t fibonacci_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_fibonacci) },
    { MP_ROM_QSTR(MP_QSTR_fibonacci_string), MP_ROM_PTR(&fibonacci_string_obj) },
};

STATIC MP_DEFINE_CONST_DICT(fibonacci_module_globals, fibonacci_module_globals_table);

const mp_obj_module_t fibonacci_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&fibonacci_module_globals,
};
