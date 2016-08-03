/* Copyright (c) 2016 Anton Titov.
 * Copyright (c) 2016 pCloud Ltd.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of pCloud Ltd nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL pCloud Ltd BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _PSYNC_PATHSTATUS_H
#define _PSYNC_PATHSTATUS_H

#include "psynclib.h"

typedef uint32_t psync_path_status_t;

#define PSYNC_PATH_STATUS_IN_SYNC   0
#define PSYNC_PATH_STATUS_IN_PROG   1
#define PSYNC_PATH_STATUS_NOT_OURS  2
#define PSYNC_PATH_STATUS_NOT_FOUND 3

#define PSYNC_PATH_STATUS_BITS 5

#define PSYNC_PATH_STATUS_IS_ENCRYPTED (1<<(PSYNC_PATH_STATUS_BITS))

#define psync_path_status_get_status(x) ((x)&((1<<PSYNC_PATH_STATUS_BITS)-1))
#define psync_path_status_is_encryted(x) ((x)&PSYNC_PATH_STATUS_IS_ENCRYPTED)

// All of the functions take sql write lock, so they can only be called under write lock or with no sql lock at all (but not with read lock taken).
// It is advised to call the function under the lock that made the changes, for reliability (and because recursive locks are almost free in the current
// implementation).
// All of the functions are designed to be very fast, with complexity proportional (only) to the path/folder depth (that is number of folders between the root
// and the folder, not number of sub folders) with additional cache to skip sql queries.

// Init does not allocate anything at this point and can be used as re-init (e.g. after unlink).
void psync_path_status_init();
void psync_path_status_reload_syncs();
void psync_path_status_clear_path_cache();
void psync_path_status_clear_sync_path_cache();
void psync_path_status_del_from_parent_cache(psync_folderid_t folderid);
void psync_path_status_drive_folder_changed(psync_folderid_t folderid);

void psync_path_status_folder_moved(psync_folderid_t folderid, psync_folderid_t old_parent_folderid, psync_folderid_t new_parent_folderid);

psync_path_status_t psync_path_status_get(const char *path);

#endif
