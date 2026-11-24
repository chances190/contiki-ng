/*
 * Copyright (c) 2026, Valter Sanches Martins Neto
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \addtogroup rpl-lite
 * @{
 *
 * \file
 *	Optional RPL snapshot API.
 */

#ifndef RPL_SNAPSHOT_H
#define RPL_SNAPSHOT_H

#include "contiki.h"
#include "net/linkaddr.h"
#include "net/routing/rpl-lite/rpl-conf.h"

#if RPL_WITH_SNAPSHOT

#ifndef RPL_SNAPSHOT_CONF_MAX_NEIGHBORS
#define RPL_SNAPSHOT_CONF_MAX_NEIGHBORS 16
#endif

/** \brief One row of the RPL neighbor table. */
struct rpl_snapshot_neighbor {
  linkaddr_t addr;
  uint16_t rank;
  uint16_t etx;
  int16_t rssi;
  uint8_t freshness;
  uint8_t is_parent;
  uint16_t tx;
  uint16_t acked;
  uint16_t rx;
  uint16_t drops;
};

/** \brief Snapshot of the RPL DAG, preferred parent, and neighbor table. */
struct rpl_snapshot {
  uint8_t used;
  uint8_t state;
  uint16_t rank;
  linkaddr_t parent;
  uint16_t parent_rank;
  uint16_t parent_etx;
  int16_t parent_rssi;
  uint8_t nbr_count;
  struct rpl_snapshot_neighbor neighbors[RPL_SNAPSHOT_CONF_MAX_NEIGHBORS];
};

/**
 * Fill a snapshot from the current RPL instance.
 *
 * \param snapshot Destination snapshot; zeroed before it is filled.
 */
void rpl_snapshot(struct rpl_snapshot *snapshot);

#endif /* RPL_WITH_SNAPSHOT */

#endif /* RPL_SNAPSHOT_H */
/** @} */
