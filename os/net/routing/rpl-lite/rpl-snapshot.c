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
 *	Optional RPL snapshot implementation.
 */

#include "contiki.h"
#include "net/link-stats.h"
#include "net/nbr-table.h"
#include "net/routing/rpl-lite/rpl.h"
#include "net/routing/rpl-lite/rpl-snapshot.h"

#include <string.h>

#if RPL_WITH_SNAPSHOT

/*---------------------------------------------------------------------------*/
void
rpl_snapshot(struct rpl_snapshot *snapshot)
{
  rpl_nbr_t *parent;

  if(snapshot == NULL) {
    return;
  }
  memset(snapshot, 0, sizeof(*snapshot));

  snapshot->used = curr_instance.used;
  if(curr_instance.used) {
    snapshot->state = (uint8_t)curr_instance.dag.state;
    snapshot->rank = (uint16_t)DAG_RANK(curr_instance.dag.rank);

    parent = curr_instance.dag.preferred_parent;
    if(parent != NULL) {
      const linkaddr_t *addr = rpl_neighbor_get_lladdr(parent);
      const struct link_stats *stats = rpl_neighbor_get_link_stats(parent);

      if(addr != NULL) {
        snapshot->parent = *addr;
      }
      snapshot->parent_rank =
        (uint16_t)DAG_RANK(rpl_neighbor_rank_via_nbr(parent));
      if(stats != NULL) {
        snapshot->parent_etx = stats->etx;
        snapshot->parent_rssi = stats->rssi;
      }
    }
  }

  for(rpl_nbr_t *nbr = nbr_table_head(rpl_neighbors);
      nbr != NULL && snapshot->nbr_count < RPL_SNAPSHOT_CONF_MAX_NEIGHBORS;
      nbr = nbr_table_next(rpl_neighbors, nbr)) {
    struct rpl_snapshot_neighbor *out =
      &snapshot->neighbors[snapshot->nbr_count++];
    const linkaddr_t *addr = rpl_neighbor_get_lladdr(nbr);
    const struct link_stats *stats = rpl_neighbor_get_link_stats(nbr);

    if(addr != NULL) {
      out->addr = *addr;
    }
    out->rank = (uint16_t)DAG_RANK(rpl_neighbor_rank_via_nbr(nbr));
    out->is_parent = (uint8_t)rpl_neighbor_is_parent(nbr);
    if(stats != NULL) {
      out->etx = stats->etx;
      out->rssi = stats->rssi;
      out->freshness = stats->freshness;
#if LINK_STATS_PACKET_COUNTERS
      out->tx = stats->cnt_current.num_packets_tx;
      out->acked = stats->cnt_current.num_packets_acked;
      out->rx = stats->cnt_current.num_packets_rx;
      out->drops = stats->cnt_current.num_queue_drops;
#endif /* LINK_STATS_PACKET_COUNTERS */
    }
  }
}
/*---------------------------------------------------------------------------*/
#endif /* RPL_WITH_SNAPSHOT */
/** @} */
