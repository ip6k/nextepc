/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "sgw-event.h"
#include "sgw-context.h"

static OGS_POOL(pool, sgw_event_t);

#define EVENT_POOL 32 /* FIXME : 32 */
void sgw_event_init(void)
{
    ogs_pool_init(&pool, EVENT_POOL);

    sgw_self()->queue = ogs_queue_create(EVENT_POOL);
    ogs_assert(sgw_self()->queue);
    sgw_self()->timer_mgr = ogs_timer_mgr_create();
    ogs_assert(sgw_self()->timer_mgr);
    sgw_self()->pollset = ogs_pollset_create();
    ogs_assert(sgw_self()->pollset);
}

void sgw_event_term(void)
{
    ogs_queue_term(sgw_self()->queue);
    ogs_pollset_notify(sgw_self()->pollset);
}

void sgw_event_final(void)
{
    if (sgw_self()->pollset)
        ogs_pollset_destroy(sgw_self()->pollset);
    if (sgw_self()->timer_mgr)
        ogs_timer_mgr_destroy(sgw_self()->timer_mgr);
    if (sgw_self()->queue)
        ogs_queue_destroy(sgw_self()->queue);

    ogs_pool_final(&pool);
}

sgw_event_t *sgw_event_new(sgw_event_e id)
{
    sgw_event_t *e = NULL;

    ogs_pool_alloc(&pool, &e);
    ogs_assert(e);
    memset(e, 0, sizeof(*e));

    e->id = id;

    return e;
}

void sgw_event_free(sgw_event_t *e)
{
    ogs_assert(e);
    ogs_pool_free(&pool, e);
}

const char *sgw_event_get_name(sgw_event_t *e)
{
    if (e == NULL)
        return OGS_FSM_NAME_INIT_SIG;

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG: 
        return OGS_FSM_NAME_ENTRY_SIG;
    case OGS_FSM_EXIT_SIG: 
        return OGS_FSM_NAME_EXIT_SIG;

    case SGW_EVT_S11_MESSAGE:
        return "SGW_EVT_S11_MESSAGE";
    case SGW_EVT_S5C_MESSAGE:
        return "SGW_EVT_S5C_MESSAGE";

    default: 
       break;
    }

    return "UNKNOWN_EVENT";
}
