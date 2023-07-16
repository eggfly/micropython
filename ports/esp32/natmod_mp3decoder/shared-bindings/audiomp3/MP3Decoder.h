/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Scott Shawcroft for Adafruit Industries
 * Copyright (c) 2019 Jeff Epler for Adafruit Industries
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef MICROPY_INCLUDED_SHARED_BINDINGS_AUDIOIO_MP3FILE_H
#define MICROPY_INCLUDED_SHARED_BINDINGS_AUDIOIO_MP3FILE_H

#include "py/obj.h"
// #include "extmod/vfs_fat.h"

#include "../../shared-module/audiomp3/MP3Decoder.h"

mp_map_elem_t audiomp3_mp3file_locals_dict_table[4];
//  = {
//     // MP_ROM_QSTR -> MP_OBJ_NEW_QSTR
//     // Methods
//     { MP_OBJ_NEW_QSTR(MP_QSTR_open), MP_ROM_PTR(&audiomp3_mp3file_open_obj) },
//     { MP_OBJ_NEW_QSTR(MP_QSTR_deinit), MP_ROM_PTR(&audiomp3_mp3file_deinit_obj) },
//     // { MP_ROM_QSTR(MP_QSTR___enter__), MP_ROM_PTR(&default___enter___obj) },
//     { MP_OBJ_NEW_QSTR(MP_QSTR___exit__), MP_ROM_PTR(&audiomp3_mp3file___exit___obj) },
//     // eggfly added
//     { MP_OBJ_NEW_QSTR(MP_QSTR_get_buffer), MP_ROM_PTR(&audiocore_get_buffer_obj) },
//     // Properties
//     // { MP_ROM_QSTR(MP_QSTR_file), MP_ROM_PTR(&audiomp3_mp3file_file_obj) },
//     // { MP_ROM_QSTR(MP_QSTR_sample_rate), MP_ROM_PTR(&audiomp3_mp3file_sample_rate_obj) },
//     // { MP_ROM_QSTR(MP_QSTR_bits_per_sample), MP_ROM_PTR(&audiomp3_mp3file_bits_per_sample_obj) },
//     // { MP_ROM_QSTR(MP_QSTR_channel_count), MP_ROM_PTR(&audiomp3_mp3file_channel_count_obj) },
//     // { MP_ROM_QSTR(MP_QSTR_rms_level), MP_ROM_PTR(&audiomp3_mp3file_rms_level_obj) },
//     // { MP_ROM_QSTR(MP_QSTR_samples_decoded), MP_ROM_PTR(&audiomp3_mp3file_samples_decoded_obj) },
// };
__attribute__((unused))
STATIC MP_DEFINE_CONST_DICT(audiomp3_mp3file_locals_dict, audiomp3_mp3file_locals_dict_table);


mp_obj_type_t mp3decoder_mp3file_type;
//  = {
//     { &mp_type_type },
//     .name = MP_QSTR_MP3Decoder,
//     // .flags = MP_TYPE_FLAG_EXTENDED,
//     .make_new = audiomp3_mp3file_make_new,
//     .locals_dict = (mp_obj_dict_t *)&audiomp3_mp3file_locals_dict,
//     // MP_TYPE_EXTENDED_FIELDS(
//     //     .protocol = &audiomp3_mp3file_proto,
//     //     ),
// };

void common_hal_audiomp3_mp3file_construct(audiomp3_mp3file_obj_t *self,
    mp_obj_t file, uint8_t *buffer, size_t buffer_size);

void common_hal_audiomp3_mp3file_set_file(audiomp3_mp3file_obj_t *self, mp_obj_t file);
void common_hal_audiomp3_mp3file_deinit(audiomp3_mp3file_obj_t *self);
bool common_hal_audiomp3_mp3file_deinited(audiomp3_mp3file_obj_t *self);
uint32_t common_hal_audiomp3_mp3file_get_sample_rate(audiomp3_mp3file_obj_t *self);
void common_hal_audiomp3_mp3file_set_sample_rate(audiomp3_mp3file_obj_t *self, uint32_t sample_rate);
uint8_t common_hal_audiomp3_mp3file_get_bits_per_sample(audiomp3_mp3file_obj_t *self);
uint8_t common_hal_audiomp3_mp3file_get_channel_count(audiomp3_mp3file_obj_t *self);
float common_hal_audiomp3_mp3file_get_rms_level(audiomp3_mp3file_obj_t *self);
uint32_t common_hal_audiomp3_mp3file_get_samples_decoded(audiomp3_mp3file_obj_t *self);

#endif // MICROPY_INCLUDED_SHARED_BINDINGS_AUDIOIO_MP3FILE_H
