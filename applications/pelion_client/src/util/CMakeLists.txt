#
# Copyright (c) 2021 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

target_sources(app PRIVATE pelion_fcc_err.c)

target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
