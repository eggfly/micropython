#define MICROPY_PY_BTREE (1)

#include "py/dynruntime.h"
// #include "py/builtin.h"
// #include "shared-bindings/audiomp3/MP3Decoder.h"
#include <unistd.h>

#if !defined(__linux__)
void *memcpy(void *dst, const void *src, size_t n) {
    return mp_fun_table.memmove_(dst, src, n);
}
void *memset(void *s, int c, size_t n) {
    return mp_fun_table.memset_(s, c, n);
}
#endif

void *memmove(void *dest, const void *src, size_t n) {
    return mp_fun_table.memmove_(dest, src, n);
}

void *malloc(size_t n) {
    void *ptr = m_malloc(n);
    return ptr;
}
void *realloc(void *ptr, size_t n) {
    mp_printf(&mp_plat_print, "UNDEF %d\n", __LINE__);
    return NULL;
}
void *calloc(size_t n, size_t m) {
    void *ptr = m_malloc(n * m);
    // memory already cleared by conservative GC
    return ptr;
}

void free(void *ptr) {
    m_free(ptr);
}

void abort_(void) {
    nlr_raise(mp_obj_new_exception(mp_load_global(MP_QSTR_RuntimeError)));
}

int native_errno;
#if defined(__linux__)
int *__errno_location (void)
#else
int *__errno (void)
#endif
{
    return &native_errno;
}


// mp_map_elem_t audiomp3_locals_dict_table[0];
// STATIC MP_DEFINE_CONST_DICT(audiomp3_locals_dict, audiomp3_locals_dict_table);

STATIC mp_obj_t mp3decoder_open(size_t n_args, const mp_obj_t *args) {
    // Make sure we got a stream object
    return args[0];
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mp3decoder_open_obj, 5, 5, mp3decoder_open);

mp_obj_type_t mp3decoder_mp3file_type;

mp_map_elem_t audiomp3_mp3file_locals_dict_table[0];
STATIC MP_DEFINE_CONST_DICT(audiomp3_mp3file_locals_dict, audiomp3_mp3file_locals_dict_table);

mp_obj_t mp_call_function_2(mp_obj_t fun, mp_obj_t arg1, mp_obj_t arg2) {
    mp_obj_t args[2];
    args[0] = arg1;
    args[1] = arg2;
    return mp_call_function_n_kw(fun, 2, 0, args);
}

STATIC mp_obj_t mp3decoder_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    mp_arg_check_num(n_args, n_kw, 4, 5, false);
    mp_obj_t arg = args[0];

    if (mp_obj_is_str(arg)) {
        // mp_printf()
        mp_obj_t builtin_open_obj = mp_load_name(MP_QSTR_my_foo_callback);
        mp_printf(MICROPY_ERROR_PRINTER, "builtin_open_obj=%x\n", builtin_open_obj);
        // const mp_obj_type_t* t = mp_fun_table.obj_get_type(builtin_open_obj);
        // const char * type_str = mp_obj_get_type_str(builtin_open_obj);
        mp_printf(MICROPY_ERROR_PRINTER, "mp_obj_is_str() is true, arg=%x, open=%x\n", arg, builtin_open_obj);
        arg = mp_call_function_2(builtin_open_obj, arg, MP_OBJ_NEW_QSTR(MP_QSTR_rb));
        mp_printf(MICROPY_ERROR_PRINTER, "mp_obj_is_str() is true, arg=%x, arg.type=%x\n", arg, ((mp_obj_base_t *)MP_OBJ_TO_PTR(arg))->type);
    } else {
        mp_printf(MICROPY_ERROR_PRINTER, "mp_obj_is_str() is false\n");
    }
    // mp_obj_framebuf_t *o = mp_obj_malloc(mp_mp3, type);
    // o->buf_obj = args[0];

    // mp_buffer_info_t bufinfo;
    // mp_get_buffer_raise(args[0], &bufinfo, MP_BUFFER_WRITE);
    // o->buf = bufinfo.buf;

    // o->width = mp_obj_get_int(args[1]);
    // o->height = mp_obj_get_int(args[2]);
    // o->format = mp_obj_get_int(args[3]);
    if (n_args >= 5) {
        // o->stride = mp_obj_get_int(args[4]);
    } else {
       // o->stride = o->width;
    }

    // switch (o->format) {
    //     case FRAMEBUF_MVLSB:
    //     case FRAMEBUF_RGB565:
    //         break;
    //     case FRAMEBUF_MHLSB:
    //     case FRAMEBUF_MHMSB:
    //         o->stride = (o->stride + 7) & ~7;
    //         break;
    //     case FRAMEBUF_GS2_HMSB:
    //         o->stride = (o->stride + 3) & ~3;
    //         break;
    //     case FRAMEBUF_GS4_HMSB:
    //         o->stride = (o->stride + 1) & ~1;
    //         break;
    //     case FRAMEBUF_GS8:
    //         break;
    //     default:
    //         mp_raise_ValueError(MP_ERROR_TEXT("invalid format"));
    // }

    // return MP_OBJ_FROM_PTR(o);
    return arg;
}

mp_obj_t mpy_init(mp_obj_fun_bc_t *self, size_t n_args, size_t n_kw, mp_obj_t *args) {
    MP_DYNRUNTIME_INIT_ENTRY
    
    mp3decoder_mp3file_type.base.type = (void*)&mp_fun_table.type_type;
    mp3decoder_mp3file_type.name = MP_QSTR_MP3Decoder;
    // btree_type.base.type = (void*)&mp_fun_table.type_type;
    mp3decoder_mp3file_type.make_new =  mp3decoder_make_new;
    mp3decoder_mp3file_type.locals_dict = (void*)&audiomp3_mp3file_locals_dict;
    // audiomp3_locals_dict_table[0] = (mp_map_elem_t){ MP_OBJ_NEW_QSTR(MP_QSTR_close), MP_OBJ_FROM_PTR(&btree_close_obj) };
    mp_store_global(MP_QSTR_MP3Decoder, MP_OBJ_FROM_PTR(&mp3decoder_mp3file_type));
    mp_store_global(MP_QSTR_open, MP_OBJ_FROM_PTR(&mp3decoder_open_obj));

    MP_DYNRUNTIME_INIT_EXIT
}
