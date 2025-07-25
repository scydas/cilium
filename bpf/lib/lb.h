/* SPDX-License-Identifier: (GPL-2.0-only OR BSD-2-Clause) */
/* Copyright Authors of Cilium */

#pragma once

#include "bpf/compiler.h"
#include "csum.h"
#include "conntrack.h"
#include "ipv4.h"
#include "hash.h"
#include "eps.h"
#include "nat_46x64.h"
#include "ratelimit.h"

#ifndef SKIP_CALLS_MAP
#include "drop.h"
#endif

#ifdef ENABLE_IPV6
struct {
	__uint(type, BPF_MAP_TYPE_HASH);
	__type(key, struct skip_lb6_key);
	__type(value, __u8);
	__uint(pinning, LIBBPF_PIN_BY_NAME);
	__uint(max_entries, CILIUM_LB_SKIP_MAP_MAX_ENTRIES);
	__uint(map_flags, BPF_F_NO_PREALLOC);
} cilium_skip_lb6 __section_maps_btf;

struct {
	__uint(type, BPF_MAP_TYPE_HASH);
	__type(key, __u16);
	__type(value, struct lb6_reverse_nat);
	__uint(pinning, LIBBPF_PIN_BY_NAME);
	__uint(max_entries, CILIUM_LB_REV_NAT_MAP_MAX_ENTRIES);
	__uint(map_flags, CONDITIONAL_PREALLOC);
} cilium_lb6_reverse_nat __section_maps_btf;

struct {
	__uint(type, BPF_MAP_TYPE_HASH);
	__type(key, struct lb6_key);
	__type(value, struct lb6_service);
	__uint(pinning, LIBBPF_PIN_BY_NAME);
	__uint(max_entries, CILIUM_LB_SERVICE_MAP_MAX_ENTRIES);
	__uint(map_flags, CONDITIONAL_PREALLOC);
} cilium_lb6_services_v2 __section_maps_btf;

struct {
	__uint(type, BPF_MAP_TYPE_HASH);
	__type(key, __u32);
	__type(value, struct lb6_backend);
	__uint(pinning, LIBBPF_PIN_BY_NAME);
	__uint(max_entries, CILIUM_LB_BACKENDS_MAP_MAX_ENTRIES);
	__uint(map_flags, CONDITIONAL_PREALLOC);
} cilium_lb6_backends_v3 __section_maps_btf;

#ifdef ENABLE_SESSION_AFFINITY
struct {
	__uint(type, BPF_MAP_TYPE_LRU_HASH);
	__type(key, struct lb6_affinity_key);
	__type(value, struct lb_affinity_val);
	__uint(pinning, LIBBPF_PIN_BY_NAME);
	__uint(max_entries, CILIUM_LB_AFFINITY_MAP_MAX_ENTRIES);
	__uint(map_flags, LRU_MEM_FLAVOR);
} cilium_lb6_affinity __section_maps_btf;
#endif

#ifdef ENABLE_SRC_RANGE_CHECK
struct {
	__uint(type, BPF_MAP_TYPE_LPM_TRIE);
	__type(key, struct lb6_src_range_key);
	__type(value, __u8);
	__uint(pinning, LIBBPF_PIN_BY_NAME);
	__uint(max_entries, LB6_SRC_RANGE_MAP_SIZE);
	__uint(map_flags, BPF_F_NO_PREALLOC);
} cilium_lb6_source_range __section_maps_btf;
#endif

#ifdef ENABLE_HEALTH_CHECK
struct {
	__uint(type, BPF_MAP_TYPE_LRU_HASH);
	__type(key, __sock_cookie);
	__type(value, struct lb6_health);
	__uint(pinning, LIBBPF_PIN_BY_NAME);
	__uint(max_entries, CILIUM_LB_BACKENDS_MAP_MAX_ENTRIES);
	__uint(map_flags, LRU_MEM_FLAVOR);
} cilium_lb6_health __section_maps_btf;
#endif

#if defined(LB_SELECTION_PER_SERVICE) || LB_SELECTION == LB_SELECTION_MAGLEV
struct {
	__uint(type, BPF_MAP_TYPE_HASH_OF_MAPS);
	__type(key, __u16);
	__type(value, __u32);
	__uint(pinning, LIBBPF_PIN_BY_NAME);
	__uint(max_entries, CILIUM_LB_MAGLEV_MAP_MAX_ENTRIES);
	__uint(map_flags, CONDITIONAL_PREALLOC);
	/* Maglev inner map definition */
	__array(values, struct {
		__uint(type, BPF_MAP_TYPE_ARRAY);
		__uint(key_size, sizeof(__u32));
		__uint(value_size, sizeof(__u32) * LB_MAGLEV_LUT_SIZE);
		__uint(max_entries, 1);
	});
} cilium_lb6_maglev __section_maps_btf;
#endif /* LB_SELECTION == LB_SELECTION_MAGLEV */
#endif /* ENABLE_IPV6 */

#ifdef ENABLE_IPV4
struct {
	__uint(type, BPF_MAP_TYPE_HASH);
	__type(key, struct skip_lb4_key);
	__type(value, __u8);
	__uint(pinning, LIBBPF_PIN_BY_NAME);
	__uint(max_entries, CILIUM_LB_SKIP_MAP_MAX_ENTRIES);
	__uint(map_flags, BPF_F_NO_PREALLOC);
} cilium_skip_lb4 __section_maps_btf;

struct {
	__uint(type, BPF_MAP_TYPE_HASH);
	__type(key, __u16);
	__type(value, struct lb4_reverse_nat);
	__uint(pinning, LIBBPF_PIN_BY_NAME);
	__uint(max_entries, CILIUM_LB_REV_NAT_MAP_MAX_ENTRIES);
	__uint(map_flags, CONDITIONAL_PREALLOC);
} cilium_lb4_reverse_nat __section_maps_btf;

struct {
	__uint(type, BPF_MAP_TYPE_HASH);
	__type(key, struct lb4_key);
	__type(value, struct lb4_service);
	__uint(pinning, LIBBPF_PIN_BY_NAME);
	__uint(max_entries, CILIUM_LB_SERVICE_MAP_MAX_ENTRIES);
	__uint(map_flags, CONDITIONAL_PREALLOC);
} cilium_lb4_services_v2 __section_maps_btf;

struct {
	__uint(type, BPF_MAP_TYPE_HASH);
	__type(key, __u32);
	__type(value, struct lb4_backend);
	__uint(pinning, LIBBPF_PIN_BY_NAME);
	__uint(max_entries, CILIUM_LB_BACKENDS_MAP_MAX_ENTRIES);
	__uint(map_flags, CONDITIONAL_PREALLOC);
} cilium_lb4_backends_v3 __section_maps_btf;

#ifdef ENABLE_SESSION_AFFINITY
struct {
	__uint(type, BPF_MAP_TYPE_LRU_HASH);
	__type(key, struct lb4_affinity_key);
	__type(value, struct lb_affinity_val);
	__uint(pinning, LIBBPF_PIN_BY_NAME);
	__uint(max_entries, CILIUM_LB_AFFINITY_MAP_MAX_ENTRIES);
	__uint(map_flags, LRU_MEM_FLAVOR);
} cilium_lb4_affinity __section_maps_btf;
#endif

#ifdef ENABLE_SRC_RANGE_CHECK
struct {
	__uint(type, BPF_MAP_TYPE_LPM_TRIE);
	__type(key, struct lb4_src_range_key);
	__type(value, __u8);
	__uint(pinning, LIBBPF_PIN_BY_NAME);
	__uint(max_entries, LB4_SRC_RANGE_MAP_SIZE);
	__uint(map_flags, BPF_F_NO_PREALLOC);
} cilium_lb4_source_range __section_maps_btf;
#endif

#ifdef ENABLE_HEALTH_CHECK
struct {
	__uint(type, BPF_MAP_TYPE_LRU_HASH);
	__type(key, __sock_cookie);
	__type(value, struct lb4_health);
	__uint(pinning, LIBBPF_PIN_BY_NAME);
	__uint(max_entries, CILIUM_LB_BACKENDS_MAP_MAX_ENTRIES);
	__uint(map_flags, LRU_MEM_FLAVOR);
} cilium_lb4_health __section_maps_btf;
#endif

#if defined(LB_SELECTION_PER_SERVICE) || LB_SELECTION == LB_SELECTION_MAGLEV
#ifndef LB_MAGLEV_EXTERNAL
struct {
	__uint(type, BPF_MAP_TYPE_HASH_OF_MAPS);
	__type(key, __u16);
	__type(value, __u32);
	__uint(pinning, LIBBPF_PIN_BY_NAME);
	__uint(max_entries, CILIUM_LB_MAGLEV_MAP_MAX_ENTRIES);
	__uint(map_flags, CONDITIONAL_PREALLOC);
	/* Maglev inner map definition */
	__array(values, struct {
		__uint(type, BPF_MAP_TYPE_ARRAY);
		__uint(key_size, sizeof(__u32));
		__uint(value_size, sizeof(__u32) * LB_MAGLEV_LUT_SIZE);
		__uint(max_entries, 1);
	});
} cilium_lb4_maglev __section_maps_btf;
#endif /* LB_MAGLEV_EXTERNAL */
#endif /* LB_SELECTION == LB_SELECTION_MAGLEV */
#endif /* ENABLE_IPV4 */

#ifdef ENABLE_SESSION_AFFINITY
struct {
	__uint(type, BPF_MAP_TYPE_HASH);
	__type(key, struct lb_affinity_match);
	__type(value, __u8);
	__uint(pinning, LIBBPF_PIN_BY_NAME);
	__uint(max_entries, CILIUM_LB_AFFINITY_MAP_MAX_ENTRIES);
	__uint(map_flags, CONDITIONAL_PREALLOC);
} cilium_lb_affinity_match __section_maps_btf;
#endif

#ifdef LB_DEBUG
#define cilium_dbg_lb cilium_dbg
#else
#define cilium_dbg_lb(a, b, c, d)
#endif

#ifdef ENABLE_ACTIVE_CONNECTION_TRACKING
#include "act.h"
#endif

static __always_inline bool lb_is_svc_proto(__u8 proto)
{
	switch (proto) {
	case IPPROTO_TCP:
	case IPPROTO_UDP:
#ifdef ENABLE_SCTP
	case IPPROTO_SCTP:
#endif /* ENABLE_SCTP */
		return true;
	default:
		return false;
	}
}

static __always_inline
bool lb4_svc_is_loadbalancer(const struct lb4_service *svc __maybe_unused)
{
	return svc->flags & SVC_FLAG_LOADBALANCER;
}

static __always_inline
bool lb6_svc_is_loadbalancer(const struct lb6_service *svc __maybe_unused)
{
	return svc->flags & SVC_FLAG_LOADBALANCER;
}

static __always_inline
bool lb4_svc_is_nodeport(const struct lb4_service *svc __maybe_unused)
{
#ifdef ENABLE_NODEPORT
	return svc->flags & SVC_FLAG_NODEPORT;
#else
	return false;
#endif /* ENABLE_NODEPORT */
}

static __always_inline
bool lb6_svc_is_nodeport(const struct lb6_service *svc __maybe_unused)
{
#ifdef ENABLE_NODEPORT
	return svc->flags & SVC_FLAG_NODEPORT;
#else
	return false;
#endif /* ENABLE_NODEPORT */
}

static __always_inline
bool lb4_svc_is_external_ip(const struct lb4_service *svc __maybe_unused)
{
	return svc->flags & SVC_FLAG_EXTERNAL_IP;
}

static __always_inline
bool lb6_svc_is_external_ip(const struct lb6_service *svc __maybe_unused)
{
	return svc->flags & SVC_FLAG_EXTERNAL_IP;
}

static __always_inline
bool lb4_svc_is_hostport(const struct lb4_service *svc __maybe_unused)
{
	return svc->flags & SVC_FLAG_HOSTPORT;
}

static __always_inline
bool lb6_svc_is_hostport(const struct lb6_service *svc __maybe_unused)
{
	return svc->flags & SVC_FLAG_HOSTPORT;
}

static __always_inline
bool lb4_svc_is_loopback(const struct lb4_service *svc __maybe_unused)
{
	return svc->flags2 & SVC_FLAG_LOOPBACK;
}

static __always_inline
bool lb6_svc_is_loopback(const struct lb6_service *svc __maybe_unused)
{
	return svc->flags2 & SVC_FLAG_LOOPBACK;
}

static __always_inline
bool lb4_svc_has_src_range_check(const struct lb4_service *svc __maybe_unused)
{
#ifdef ENABLE_SRC_RANGE_CHECK
	return svc->flags & SVC_FLAG_SOURCE_RANGE;
#else
	return false;
#endif /* ENABLE_SRC_RANGE_CHECK */
}

static __always_inline
bool lb6_svc_has_src_range_check(const struct lb6_service *svc __maybe_unused)
{
#ifdef ENABLE_SRC_RANGE_CHECK
	return svc->flags & SVC_FLAG_SOURCE_RANGE;
#else
	return false;
#endif /* ENABLE_SRC_RANGE_CHECK */
}

static __always_inline
bool lb4_svc_is_two_scopes(const struct lb4_service *svc)
{
	return svc->flags2 & SVC_FLAG_TWO_SCOPES;
}

static __always_inline
bool lb6_svc_is_two_scopes(const struct lb6_service *svc)
{
	return svc->flags2 & SVC_FLAG_TWO_SCOPES;
}

static __always_inline
bool lb4_svc_is_affinity(const struct lb4_service *svc)
{
	return svc->flags & SVC_FLAG_AFFINITY;
}

static __always_inline
bool lb6_svc_is_affinity(const struct lb6_service *svc)
{
	return svc->flags & SVC_FLAG_AFFINITY;
}

static __always_inline bool __lb_svc_is_routable(__u8 flags)
{
	return (flags & SVC_FLAG_ROUTABLE) != 0;
}

static __always_inline
bool lb4_svc_is_routable(const struct lb4_service *svc)
{
	return __lb_svc_is_routable(svc->flags);
}

static __always_inline
bool lb6_svc_is_routable(const struct lb6_service *svc)
{
	return __lb_svc_is_routable(svc->flags);
}

#ifdef ENABLE_LOCAL_REDIRECT_POLICY
static __always_inline
bool lb4_svc_is_localredirect(const struct lb4_service *svc)
{
	return svc->flags2 & SVC_FLAG_LOCALREDIRECT;
}

static __always_inline
bool lb6_svc_is_localredirect(const struct lb6_service *svc)
{
	return svc->flags2 & SVC_FLAG_LOCALREDIRECT;
}
#endif /* ENABLE_LOCAL_REDIRECT_POLICY */

static __always_inline
bool __lb4_svc_is_l7_loadbalancer(const struct lb4_service *svc __maybe_unused)
{
#ifdef ENABLE_L7_LB
	return svc->flags2 & SVC_FLAG_L7_LOADBALANCER;
#else
	return false;
#endif
}

static __always_inline
bool lb4_svc_is_l7_punt_proxy(const struct lb4_service *svc __maybe_unused)
{
	return !lb4_svc_is_hostport(svc) && (svc->flags2 & SVC_FLAG_L7_DELEGATE);
}

static __always_inline
bool lb4_svc_is_l7_loadbalancer(const struct lb4_service *svc __maybe_unused)
{
#ifdef ENABLE_L7_LB
	/* Also test for l7_lb_proxy_port, since l7_lb_proxy_port == 0 is reserved. */
	return __lb4_svc_is_l7_loadbalancer(svc) && svc->l7_lb_proxy_port > 0;
#else
	return false;
#endif
}

static __always_inline
bool __lb6_svc_is_l7_loadbalancer(const struct lb6_service *svc __maybe_unused)
{
#ifdef ENABLE_L7_LB
	return svc->flags2 & SVC_FLAG_L7_LOADBALANCER;
#else
	return false;
#endif
}

static __always_inline
bool lb6_svc_is_l7_punt_proxy(const struct lb6_service *svc __maybe_unused)
{
	return !lb6_svc_is_hostport(svc) && (svc->flags2 & SVC_FLAG_L7_DELEGATE);
}

static __always_inline
bool lb6_svc_is_l7_loadbalancer(const struct lb6_service *svc __maybe_unused)
{
#ifdef ENABLE_L7_LB
	/* Also test for l7_lb_proxy_port, since l7_lb_proxy_port == 0 is reserved. */
	return __lb6_svc_is_l7_loadbalancer(svc) && svc->l7_lb_proxy_port > 0;
#else
	return false;
#endif
}

static __always_inline int reverse_map_l4_port(struct __ctx_buff *ctx, __u8 nexthdr,
					       __be16 old_port, __be16 port, int l4_off,
					       struct csum_offset *csum_off)
{
	switch (nexthdr) {
	case IPPROTO_TCP:
	case IPPROTO_UDP:
#ifdef ENABLE_SCTP
	case IPPROTO_SCTP:
#endif  /* ENABLE_SCTP */
		if (port) {
			int ret;

			if (port != old_port) {
#ifdef ENABLE_SCTP
				/* This will change the SCTP checksum, which we cannot fix right now.
				 * This will likely need kernel changes before we can remove this.
				 */
				if (nexthdr == IPPROTO_SCTP)
					return DROP_CSUM_L4;
#endif  /* ENABLE_SCTP */
				ret = l4_modify_port(ctx, l4_off, TCP_SPORT_OFF,
						     csum_off, port, old_port);
				if (IS_ERR(ret))
					return ret;
			}
		}
		break;

	case IPPROTO_ICMPV6:
	case IPPROTO_ICMP:
		return CTX_ACT_OK;

	default:
		return DROP_UNKNOWN_L4;
	}

	return 0;
}

static __always_inline int
lb_l4_xlate(struct __ctx_buff *ctx, __u8 nexthdr __maybe_unused, int l4_off,
	    struct csum_offset *csum_off, __be16 dport, __be16 backend_port)
{
	if (likely(backend_port) && dport != backend_port) {
		int ret;

#ifdef ENABLE_SCTP
		/* This will change the SCTP checksum, which we cannot fix right now.
		 * This will likely need kernel changes before we can remove this.
		 */
		if (nexthdr == IPPROTO_SCTP)
			return DROP_CSUM_L4;
#endif  /* ENABLE_SCTP */

		/* Port offsets for UDP and TCP are the same */
		ret = l4_modify_port(ctx, l4_off, TCP_DPORT_OFF, csum_off,
				     backend_port, dport);
		if (IS_ERR(ret))
			return ret;
	}

	return CTX_ACT_OK;
}

#ifdef ENABLE_IPV6
static __always_inline int
ipv6_l4_csum_update(struct __ctx_buff *ctx, int l4_off, union v6addr *old_addr,
		    union v6addr *new_addr, struct csum_offset *csum_off,
		    enum ct_dir dir)
{
	int flag = 0, ret;
	__be32 sum;

	sum = csum_diff(old_addr->addr, 16, new_addr->addr, 16, 0);

	/* Newer kernels support the BPF_F_IPV6 flag which addresses the below
	 * bug. So let's try this first. -EINVAL indicates the flag probably isn't
	 * supported.
	 */
	ret = csum_l4_replace(ctx, l4_off, csum_off, 0, sum,
			      BPF_F_PSEUDO_HDR | BPF_F_IPV6);
	if (ret != -EINVAL)
		return ret;

	/* We need this to workaround a bug in bpf_l4_csum_replace's usage of
	 * inet_proto_csum_replace_by_diff. In short, for IPv6 we don't want to
	 * update skb->csum when CHECKSUM_COMPLETE (for the reason explained above
	 * inet_proto_csum_replace16). Unfortunately,
	 * inet_proto_csum_replace_by_diff does update skb->csum in that case. So
	 * we don't set BPF_F_PSEUDO_HDR to work around that.
	 * On egress, however, we might be in CHECKSUM_PARTIAL state, in which
	 * case we need to set BPF_F_PSEUDO_HDR or the L4 checksum won't be
	 * updated.
	 */
	if (dir == CT_EGRESS)
		flag = BPF_F_PSEUDO_HDR;

	return csum_l4_replace(ctx, l4_off, csum_off, 0, sum, flag);
}

static __always_inline int __lb6_rev_nat(struct __ctx_buff *ctx, int l4_off,
					 struct ipv6_ct_tuple *tuple,
					 struct lb6_reverse_nat *nat,
					 bool has_l4_header, enum ct_dir dir)
{
	union v6addr old_saddr __align_stack_8;
	int ret;

	cilium_dbg_lb(ctx, DBG_LB6_REVERSE_NAT, nat->address.p4, nat->port);

	ipv6_addr_copy(&old_saddr, &tuple->saddr);
	ipv6_addr_copy(&tuple->saddr, &nat->address);

	ret = ipv6_store_saddr(ctx, nat->address.addr, ETH_HLEN);
	if (IS_ERR(ret))
		return DROP_WRITE_ERROR;

	if (has_l4_header) {
		struct csum_offset csum_off = {};

		csum_l4_offset_and_flags(tuple->nexthdr, &csum_off);

		if (nat->port) {
			ret = reverse_map_l4_port(ctx, tuple->nexthdr, tuple->dport,
						  nat->port, l4_off, &csum_off);
			if (IS_ERR(ret))
				return ret;
		}

		if (csum_off.offset &&
		    ipv6_l4_csum_update(ctx, l4_off, &old_saddr, &nat->address,
					&csum_off, dir) < 0)
			return DROP_CSUM_L4;
	}

	return 0;
}

static __always_inline struct lb6_reverse_nat *
lb6_lookup_rev_nat_entry(struct __ctx_buff *ctx __maybe_unused, __u16 index)
{
	cilium_dbg_lb(ctx, DBG_LB6_REVERSE_NAT_LOOKUP, index, 0);

	return map_lookup_elem(&cilium_lb6_reverse_nat, &index);
}

/** Perform IPv6 reverse NAT based on reverse NAT index
 * @arg ctx		packet
 * @arg l4_off		offset to L4
 * @arg index		reverse NAT index
 * @arg tuple		tuple
 */
static __always_inline int lb6_rev_nat(struct __ctx_buff *ctx, int l4_off, __u16 index,
				       struct ipv6_ct_tuple *tuple, bool has_l4_header,
				       enum ct_dir dir)
{
	struct lb6_reverse_nat *nat;

	nat = lb6_lookup_rev_nat_entry(ctx, index);
	if (nat == NULL)
		return 0;

	return __lb6_rev_nat(ctx, l4_off, tuple, nat, has_l4_header, dir);
}

static __always_inline void
lb6_key_set_protocol(struct lb6_key *key __maybe_unused,
		     __u8 protocol __maybe_unused)
{
	key->proto = protocol;
}

static __always_inline void
lb6_fill_key(struct lb6_key *key, struct ipv6_ct_tuple *tuple)
{
	lb6_key_set_protocol(key, tuple->nexthdr);
	ipv6_addr_copy(&key->address, &tuple->daddr);
	key->dport = tuple->sport;
}

/** Extract IPv6 CT tuple from packet
 * @arg ctx		Packet
 * @arg ip6		Pointer to L3 header
 * @arg fraginfo	fraginfo, as returned by ipv6_get_fraginfo
 * @arg l4_off		Offset to L4 header
 * @arg tuple		CT tuple, with nexthdr prefilled
 *
 * Expects the ctx to be validated for direct packet access up to L4.
 *
 * Returns:
 *   - CTX_ACT_OK on successful extraction
 *   - DROP_UNKNOWN_L4 if packet should be ignore (sent to stack)
 *   - Negative error code
 */
static __always_inline int
lb6_extract_tuple(struct __ctx_buff *ctx, struct ipv6hdr *ip6, fraginfo_t fraginfo,
		  int l4_off, struct ipv6_ct_tuple *tuple)
{
	ipv6_addr_copy(&tuple->daddr, (union v6addr *)&ip6->daddr);
	ipv6_addr_copy(&tuple->saddr, (union v6addr *)&ip6->saddr);

	switch (tuple->nexthdr) {
	case IPPROTO_TCP:
	case IPPROTO_UDP:
#ifdef ENABLE_SCTP
	case IPPROTO_SCTP:
#endif  /* ENABLE_SCTP */
		return ipv6_load_l4_ports(ctx, ip6, fraginfo, l4_off,
					  CT_EGRESS, &tuple->dport);
	case IPPROTO_ICMPV6:
		return DROP_UNSUPP_SERVICE_PROTO;
	default:
		return DROP_UNKNOWN_L4;
	}
}

static __always_inline
bool lb6_src_range_ok(const struct lb6_service *svc __maybe_unused,
		      const union v6addr *saddr __maybe_unused)
{
#ifdef ENABLE_SRC_RANGE_CHECK
	struct lb6_src_range_key key;
	bool verdict = false;

	if (!lb6_svc_has_src_range_check(svc))
		return true;

	key = (typeof(key)) {
		.lpm_key = { SRC_RANGE_STATIC_PREFIX(key), {} },
		.rev_nat_id = svc->rev_nat_index,
		.addr = *saddr,
	};

	if (map_lookup_elem(&cilium_lb6_source_range, &key))
		verdict = true;

	return verdict ^ !!(svc->flags2 & SVC_FLAG_SOURCE_RANGE_DENY);
#else
	return true;
#endif /* ENABLE_SRC_RANGE_CHECK */
}

static __always_inline bool
lb6_to_lb4_service(const struct lb6_service *svc __maybe_unused)
{
#ifdef ENABLE_NAT_46X64
	return svc->flags2 & SVC_FLAG_NAT_46X64;
#else
	return false;
#endif
}

static __always_inline
struct lb6_service *__lb6_lookup_service(struct lb6_key *key)
{
	struct lb6_service *svc;

	svc = map_lookup_elem(&cilium_lb6_services_v2, key);

	/* If there are no elements for a specific protocol, check for ANY entries. */
	if (!svc && key->proto != IPPROTO_ANY) {
		key->proto = IPPROTO_ANY;
		svc = map_lookup_elem(&cilium_lb6_services_v2, key);
	}

	return svc;
}

static __always_inline
struct lb6_service *lb6_lookup_service(struct lb6_key *key,
				       const bool scope_switch)
{
	__u8 orig_proto = key->proto;
	struct lb6_service *svc;

	key->backend_slot = 0;

	key->scope = LB_LOOKUP_SCOPE_EXT;
	svc = __lb6_lookup_service(key);
	if (!svc)
		return NULL;

	if (!scope_switch || !lb6_svc_is_two_scopes(svc))
		return svc;

	key->proto = orig_proto;
	key->scope = LB_LOOKUP_SCOPE_INT;
	return __lb6_lookup_service(key);
}

static __always_inline struct lb6_backend *__lb6_lookup_backend(__u32 backend_id)
{
	return map_lookup_elem(&cilium_lb6_backends_v3, &backend_id);
}

static __always_inline struct lb6_backend *
lb6_lookup_backend(struct __ctx_buff *ctx __maybe_unused, __u32 backend_id)
{
	struct lb6_backend *backend;

	backend = __lb6_lookup_backend(backend_id);
	if (!backend)
		cilium_dbg_lb(ctx, DBG_LB6_LOOKUP_BACKEND_FAIL, backend_id, 0);

	return backend;
}

static __always_inline
struct lb6_service *__lb6_lookup_backend_slot(struct lb6_key *key)
{
	return map_lookup_elem(&cilium_lb6_services_v2, key);
}

static __always_inline
struct lb6_service *lb6_lookup_backend_slot(struct __ctx_buff *ctx __maybe_unused,
					    struct lb6_key *key, __u16 slot)
{
	struct lb6_service *svc;

	key->backend_slot = slot;
	cilium_dbg_lb(ctx, DBG_LB6_LOOKUP_BACKEND_SLOT,
		      key->backend_slot, key->dport);

	svc = __lb6_lookup_backend_slot(key);
	if (svc)
		return svc;

	cilium_dbg_lb(ctx, DBG_LB6_LOOKUP_BACKEND_SLOT_V2_FAIL,
		      key->backend_slot, key->dport);
	return NULL;
}

#if defined(LB_SELECTION_PER_SERVICE) || LB_SELECTION == LB_SELECTION_RANDOM
static __always_inline __u32
lb6_select_backend_id_random(struct __ctx_buff *ctx,
			     struct lb6_key *key,
			     const struct ipv6_ct_tuple *tuple __maybe_unused,
			     const struct lb6_service *svc)
{
	/* Backend slot 0 is always reserved for the service frontend. */
	__u16 slot = (get_prandom_u32() % svc->count) + 1;
	struct lb6_service *be = lb6_lookup_backend_slot(ctx, key, slot);

	return be ? be->backend_id : 0;
}
#endif  /* defined(LB_SELECTION_PER_SERVICE) || LB_SELECTION == LB_SELECTION_RANDOM */

#if defined(LB_SELECTION_PER_SERVICE) || LB_SELECTION == LB_SELECTION_MAGLEV
static __always_inline __u32
lb6_select_backend_id_maglev(struct __ctx_buff *ctx __maybe_unused,
			     struct lb6_key *key __maybe_unused,
			     const struct ipv6_ct_tuple *tuple,
			     const struct lb6_service *svc)
{
	/* CT tuple is swapped, see lb4_select_backend_id_maglev() comment. */
	__be16 sport = lb6_svc_is_affinity(svc) ? 0 : tuple->dport;
	__be16 dport = tuple->sport;
	__u32 zero = 0, index = svc->rev_nat_index;
	__u32 *backend_ids;
	void *maglev_lut;

	maglev_lut = map_lookup_elem(&cilium_lb6_maglev, &index);
	if (unlikely(!maglev_lut))
		return 0;

	backend_ids = map_lookup_elem(maglev_lut, &zero);
	if (unlikely(!backend_ids))
		return 0;

	index = __hash_from_tuple_v6(tuple, sport, dport) % LB_MAGLEV_LUT_SIZE;
	return map_array_get_32(backend_ids, index, (LB_MAGLEV_LUT_SIZE - 1) << 2);
}
#endif  /* defined(LB_SELECTION_PER_SERVICE) || LB_SELECTION == LB_SELECTION_RANDOM */

#ifdef LB_SELECTION_PER_SERVICE
static __always_inline __u32 lb6_algorithm(const struct lb6_service *svc)
{
	return svc->affinity_timeout >> LB_ALGORITHM_SHIFT;
}

static __always_inline __u32
lb6_select_backend_id(struct __ctx_buff *ctx, struct lb6_key *key,
		      const struct ipv6_ct_tuple *tuple,
		      const struct lb6_service *svc)
{
	switch (lb6_algorithm(svc)) {
	case LB_SELECTION_MAGLEV:
		return lb6_select_backend_id_maglev(ctx, key, tuple, svc);
	case LB_SELECTION_RANDOM:
		return lb6_select_backend_id_random(ctx, key, tuple, svc);
	default:
		return 0;
	}
}
#elif LB_SELECTION == LB_SELECTION_RANDOM
# define lb6_select_backend_id	lb6_select_backend_id_random
#elif LB_SELECTION == LB_SELECTION_MAGLEV
# define lb6_select_backend_id	lb6_select_backend_id_maglev
#elif LB_SELECTION == LB_SELECTION_FIRST
/* Backend selection for tests that always chooses first slot. */
static __always_inline __u32
lb6_select_backend_id(struct __ctx_buff *ctx __maybe_unused,
		      struct lb6_key *key __maybe_unused,
		      const struct ipv6_ct_tuple *tuple,
		      const struct lb6_service *svc)
{
	struct lb6_service *be = lb6_lookup_backend_slot(ctx, key, 1);

	return be ? be->backend_id : 0;
}
#else
# error "Invalid load balancer backend selection algorithm!"
#endif /* LB_SELECTION */

static __always_inline int lb6_xlate(struct __ctx_buff *ctx, __u8 nexthdr,
				     int l3_off, int l4_off,
				     const struct lb6_key *key,
				     const struct lb6_backend *backend,
				     bool has_l4_header)
{
	const union v6addr *new_dst = &backend->address;
	struct csum_offset csum_off = {};

	if (has_l4_header)
		csum_l4_offset_and_flags(nexthdr, &csum_off);

	if (ipv6_store_daddr(ctx, new_dst->addr, l3_off) < 0)
		return DROP_WRITE_ERROR;
	if (csum_off.offset) {
		__be32 sum = csum_diff(key->address.addr, 16, new_dst->addr,
				       16, 0);

		if (csum_l4_replace(ctx, l4_off, &csum_off, 0, sum,
				    BPF_F_PSEUDO_HDR) < 0)
			return DROP_CSUM_L4;
	}

	if (!has_l4_header)
		return CTX_ACT_OK;

	return lb_l4_xlate(ctx, nexthdr, l4_off, &csum_off, key->dport,
			   backend->port);
}

#ifdef ENABLE_SESSION_AFFINITY
static __always_inline __u32 lb6_affinity_timeout(const struct lb6_service *svc)
{
	return svc->affinity_timeout & AFFINITY_TIMEOUT_MASK;
}

static __always_inline __u32
__lb6_affinity_backend_id(const struct lb6_service *svc, bool netns_cookie,
			  union lb6_affinity_client_id *id)
{
	struct lb6_affinity_key key = {
		.rev_nat_id	= svc->rev_nat_index,
		.netns_cookie	= netns_cookie,
	};
	struct lb_affinity_val *val;

	if (netns_cookie)
		key.client_id.client_cookie = id->client_cookie;
	else
		ipv6_addr_copy_unaligned(&key.client_id.client_ip, &id->client_ip);

	val = map_lookup_elem(&cilium_lb6_affinity, &key);
	if (val != NULL) {
		__u32 now = (__u32)bpf_mono_now();
		struct lb_affinity_match match = {
			.rev_nat_id	= svc->rev_nat_index,
			.backend_id	= val->backend_id,
		};

		if (READ_ONCE(val->last_used) +
		    bpf_sec_to_mono(lb6_affinity_timeout(svc)) <= now) {
			map_delete_elem(&cilium_lb6_affinity, &key);
			return 0;
		}

		if (!map_lookup_elem(&cilium_lb_affinity_match, &match)) {
			map_delete_elem(&cilium_lb6_affinity, &key);
			return 0;
		}

		WRITE_ONCE(val->last_used, now);
		return val->backend_id;
	}

	return 0;
}

static __always_inline __u32
lb6_affinity_backend_id_by_addr(const struct lb6_service *svc,
				union lb6_affinity_client_id *id)
{
	return __lb6_affinity_backend_id(svc, false, id);
}

static __always_inline void
__lb6_update_affinity(const struct lb6_service *svc, bool netns_cookie,
		      union lb6_affinity_client_id *id, __u32 backend_id)
{
	__u32 now = (__u32)bpf_mono_now();
	struct lb6_affinity_key key = {
		.rev_nat_id	= svc->rev_nat_index,
		.netns_cookie	= netns_cookie,
	};
	struct lb_affinity_val val = {
		.backend_id	= backend_id,
		.last_used	= now,
	};

	if (netns_cookie)
		key.client_id.client_cookie = id->client_cookie;
	else
		ipv6_addr_copy_unaligned(&key.client_id.client_ip, &id->client_ip);

	map_update_elem(&cilium_lb6_affinity, &key, &val, 0);
}

static __always_inline void
lb6_update_affinity_by_addr(const struct lb6_service *svc,
			    union lb6_affinity_client_id *id, __u32 backend_id)
{
	__lb6_update_affinity(svc, false, id, backend_id);
}
#endif /* ENABLE_SESSION_AFFINITY */

static __always_inline __u32
lb6_affinity_backend_id_by_netns(const struct lb6_service *svc __maybe_unused,
				 union lb6_affinity_client_id *id __maybe_unused)
{
#if defined(ENABLE_SESSION_AFFINITY)
	return __lb6_affinity_backend_id(svc, true, id);
#else
	return 0;
#endif
}

static __always_inline void
lb6_update_affinity_by_netns(const struct lb6_service *svc __maybe_unused,
			     union lb6_affinity_client_id *id __maybe_unused,
			     __u32 backend_id __maybe_unused)
{
#if defined(ENABLE_SESSION_AFFINITY)
	__lb6_update_affinity(svc, true, id, backend_id);
#endif
}

static __always_inline int
lb6_to_lb4(struct __ctx_buff *ctx __maybe_unused,
	   const struct ipv6hdr *ip6 __maybe_unused)
{
#ifdef ENABLE_NAT_46X64
	__be32 src4, dst4;

	build_v4_from_v6((const union v6addr *)&ip6->saddr, &src4);
	build_v4_from_v6((const union v6addr *)&ip6->daddr, &dst4);

	return ipv6_to_ipv4(ctx, src4, dst4);
#else
	return DROP_NAT_46X64_DISABLED;
#endif
}

#ifdef ENABLE_LOCAL_REDIRECT_POLICY
static __always_inline bool
lb6_skip_xlate_from_ctx_to_svc(__net_cookie cookie,
			       union v6addr addr __maybe_unused, __be16 port __maybe_unused)
{
	struct skip_lb6_key key;
	__u8 *val = NULL;

	memset(&key, 0, sizeof(key));
	key.netns_cookie = cookie;
	key.address = addr;
	key.port = port;
	val = map_lookup_elem(&cilium_skip_lb6, &key);
	if (val)
		return true;
	return false;
}
#endif /* ENABLE_LOCAL_REDIRECT_POLICY */

static __always_inline int lb6_local(const void *map, struct __ctx_buff *ctx,
				     int l3_off, fraginfo_t fraginfo, int l4_off,
				     struct lb6_key *key,
				     struct ipv6_ct_tuple *tuple,
				     const struct lb6_service *svc,
				     struct ct_state *state,
				     const bool skip_xlate,
				     __s8 *ext_err,
				     __net_cookie netns_cookie __maybe_unused)
{
	__u32 monitor; /* Deliberately ignored; regular CT will determine monitoring. */
	__u8 flags = tuple->flags;
	struct lb6_backend *backend;
	__u32 backend_id = 0;
	int ret;
#ifdef ENABLE_SESSION_AFFINITY
	union lb6_affinity_client_id client_id;

	ipv6_addr_copy(&client_id.client_ip, &tuple->saddr);
#endif

	state->rev_nat_index = svc->rev_nat_index;

	/* See lb4_local comments re svc endpoint lookup process */
	ret = ct_lazy_lookup6(map, tuple, ctx, fraginfo, l4_off, CT_SERVICE,
			      SCOPE_REVERSE, CT_ENTRY_SVC, state, &monitor);
	if (ret < 0)
		goto drop_err;

	switch (ret) {
	case CT_NEW:
		if (unlikely(svc->count == 0))
			goto no_service;

#ifdef ENABLE_SESSION_AFFINITY
		if (lb6_svc_is_affinity(svc)) {
			backend_id = lb6_affinity_backend_id_by_addr(svc, &client_id);
			if (backend_id != 0) {
				backend = lb6_lookup_backend(ctx, backend_id);
				if (backend == NULL)
					backend_id = 0;
			}
		}
#endif
		if (backend_id == 0) {
			backend_id = lb6_select_backend_id(ctx, key, tuple, svc);
			backend = lb6_lookup_backend(ctx, backend_id);
			if (backend == NULL)
				goto no_service;
		}

		state->backend_id = backend_id;

		ret = ct_create6(map, NULL, tuple, ctx, CT_SERVICE, state, ext_err);
		/* Fail closed, if the conntrack entry create fails drop
		 * service lookup.
		 */
		if (IS_ERR(ret))
			goto drop_err;

#ifdef ENABLE_ACTIVE_CONNECTION_TRACKING
		_lb_act_conn_open(state->rev_nat_index, backend->zone);
#endif

		break;
	case CT_REPLY:
		backend_id = state->backend_id;

		/* If the lookup fails it means the user deleted the backend out from
		 * underneath us. To resolve this fall back to hash. If this is a TCP
		 * session we are likely to get a TCP RST.
		 */
		backend = lb6_lookup_backend(ctx, backend_id);
#ifdef ENABLE_ACTIVE_CONNECTION_TRACKING
		if (backend) {
			if (state->syn) /* Reopened connections */
				_lb_act_conn_open(svc->rev_nat_index, backend->zone);
			else if (state->closing)
				_lb_act_conn_closed(svc->rev_nat_index, backend->zone);
		}
#endif
		if (unlikely(!backend || backend->flags != BE_STATE_ACTIVE)) {
			/* Drain existing connections, but redirect new ones to only
			 * active backends.
			 */
			if (backend && !state->syn)
				break;

			if (unlikely(svc->count == 0))
				goto no_service;

			backend_id = lb6_select_backend_id(ctx, key, tuple, svc);
			backend = lb6_lookup_backend(ctx, backend_id);
			if (!backend)
				goto no_service;

			state->rev_nat_index = svc->rev_nat_index;
			ct_update_svc_entry(map, tuple, backend_id, svc->rev_nat_index);
		}

		break;
	default:
		ret = DROP_UNKNOWN_CT;
		goto drop_err;
	}

	/* Restore flags so that SERVICE flag is only used in used when the
	 * service lookup happens and future lookups use EGRESS or INGRESS.
	 */
	tuple->flags = flags;
#ifdef ENABLE_SESSION_AFFINITY
	if (lb6_svc_is_affinity(svc))
		lb6_update_affinity_by_addr(svc, &client_id, backend_id);
#endif

#if defined(ENABLE_LOCAL_REDIRECT_POLICY)
	if (netns_cookie > 0 && unlikely(lb6_svc_is_localredirect(svc)) &&
	    lb6_skip_xlate_from_ctx_to_svc(netns_cookie, tuple->daddr, tuple->sport))
		return CTX_ACT_OK;
#endif /* ENABLE_LOCAL_REDIRECT_POLICY */

	ipv6_addr_copy(&tuple->daddr, &backend->address);

	if (lb6_svc_is_l7_punt_proxy(svc) &&
	    __lookup_ip6_endpoint(&backend->address)) {
		ctx_skip_nodeport_set(ctx);
		return LB_PUNT_TO_STACK;
	}
	if (skip_xlate)
		return CTX_ACT_OK;

	if (likely(backend->port))
		tuple->sport = backend->port;

	return lb6_xlate(ctx, tuple->nexthdr, l3_off, l4_off, key,
			 backend, ipfrag_has_l4_header(fraginfo));
no_service:
	ret = DROP_NO_SERVICE;
drop_err:
	tuple->flags = flags;
	return ret;
}

/* lb6_ctx_store_state() stores per packet load balancing state to be picked
 * up on the continuation tail call.
 * Note that the IP headers are already xlated and the tuple is re-initialized
 * from the xlated headers before restoring state.
 * NOTE: if lb_skip_l4_dnat() this is not the case as xlate is skipped. We
 * lose the updated tuple daddr in that case.
 */
static __always_inline void lb6_ctx_store_state(struct __ctx_buff *ctx,
						const struct ct_state *state,
					       __u16 proxy_port)
{
	ctx_store_meta(ctx, CB_PROXY_MAGIC, (__u32)proxy_port << 16);
	ctx_store_meta(ctx, CB_CT_STATE, (__u32)state->rev_nat_index);
}

/* lb6_ctx_restore_state() restores per packet load balancing state from the
 * previous tail call.
 * tuple->flags does not need to be restored, as it will be reinitialized from
 * the packet.
 */
static __always_inline void lb6_ctx_restore_state(struct __ctx_buff *ctx,
						  struct ct_state *state,
						 __u16 *proxy_port,
						 bool clear)
{
	state->rev_nat_index = clear ? (__u16)ctx_load_and_clear_meta(ctx, CB_CT_STATE) :
				       (__u16)ctx_load_meta(ctx, CB_CT_STATE);

	/* No loopback support for IPv6, see lb6_local() above. */

	*proxy_port = clear ? (ctx_load_and_clear_meta(ctx, CB_PROXY_MAGIC) >> 16) :
			      (ctx_load_meta(ctx, CB_PROXY_MAGIC) >> 16);
}

#else

/* Stubs for v4-in-v6 socket cgroup hook case when only v4 is enabled to avoid
 * additional map management.
 */
static __always_inline
struct lb6_service *lb6_lookup_service(struct lb6_key *key __maybe_unused,
				       const bool scope_switch __maybe_unused)
{
	return NULL;
}

static __always_inline void
lb6_key_set_protocol(struct lb6_key *key __maybe_unused,
		     __u8 protocol __maybe_unused)
{
}

static __always_inline
struct lb6_service *__lb6_lookup_backend_slot(struct lb6_key *key __maybe_unused)
{
	return NULL;
}

static __always_inline struct lb6_backend *
__lb6_lookup_backend(__u16 backend_id __maybe_unused)
{
	return NULL;
}

static __always_inline bool
lb6_to_lb4_service(const struct lb6_service *svc __maybe_unused)
{
	return false;
}
#endif /* ENABLE_IPV6 */

#ifdef ENABLE_IPV4
static __always_inline int __lb4_rev_nat(struct __ctx_buff *ctx, int l3_off, int l4_off,
					 struct ipv4_ct_tuple *tuple,
					 const struct lb4_reverse_nat *nat,
					 bool loopback __maybe_unused, bool has_l4_header)
{
	__be32 old_sip = tuple->saddr, sum = 0;
	int ret;

	cilium_dbg_lb(ctx, DBG_LB4_REVERSE_NAT, nat->address, nat->port);

	tuple->saddr = nat->address;

#ifdef USE_LOOPBACK_LB
	if (loopback) {
		/* The packet was looped back to the sending endpoint on the
		 * forward service translation. This implies that the original
		 * source address of the packet is the source address of the
		 * current packet. We therefore need to make the current source
		 * address the new destination address.
		 */
		__be32 old_dip = tuple->daddr;

		cilium_dbg_lb(ctx, DBG_LB4_LOOPBACK_SNAT_REV, old_dip, old_sip);

		ret = ctx_store_bytes(ctx, l3_off + offsetof(struct iphdr, daddr), &old_sip, 4, 0);
		if (IS_ERR(ret))
			return DROP_WRITE_ERROR;

		sum = csum_diff(&old_dip, 4, &old_sip, 4, 0);

		/* Update the tuple address which is representing the destination address */
		tuple->saddr = old_sip;
	}
#endif

	ret = ctx_store_bytes(ctx, l3_off + offsetof(struct iphdr, saddr),
			      &nat->address, 4, 0);
	if (IS_ERR(ret))
		return DROP_WRITE_ERROR;

	sum = csum_diff(&old_sip, 4, &nat->address, 4, sum);
	if (ipv4_csum_update_by_diff(ctx, l3_off, sum) < 0)
		return DROP_CSUM_L3;

	if (has_l4_header) {
		struct csum_offset csum_off = {};

		csum_l4_offset_and_flags(tuple->nexthdr, &csum_off);

		if (nat->port) {
			/* We expect to only handle replies. Thus the extracted CT tuple
			 * will have the packet's source port in .dport.
			 */
			ret = reverse_map_l4_port(ctx, tuple->nexthdr, tuple->dport,
						  nat->port, l4_off, &csum_off);
			if (IS_ERR(ret))
				return ret;
		}

		if (csum_off.offset &&
		    csum_l4_replace(ctx, l4_off, &csum_off, 0, sum, BPF_F_PSEUDO_HDR) < 0)
			return DROP_CSUM_L4;
	}

	return 0;
}

static __always_inline struct lb4_reverse_nat *
lb4_lookup_rev_nat_entry(struct __ctx_buff *ctx __maybe_unused, __u16 index)
{
	cilium_dbg_lb(ctx, DBG_LB4_REVERSE_NAT_LOOKUP, index, 0);

	return map_lookup_elem(&cilium_lb4_reverse_nat, &index);
}

/** Perform IPv4 reverse NAT based on reverse NAT index
 * @arg ctx		packet
 * @arg l3_off		offset to L3
 * @arg l4_off		offset to L4
 * @arg index		reverse NAT index
 * @arg loopback	loopback connection
 * @arg tuple		tuple
 */
static __always_inline int lb4_rev_nat(struct __ctx_buff *ctx, int l3_off, int l4_off,
				       __u16 index, bool loopback,
				       struct ipv4_ct_tuple *tuple, bool has_l4_header)
{
	struct lb4_reverse_nat *nat;

	nat = lb4_lookup_rev_nat_entry(ctx, index);
	if (nat == NULL)
		return 0;

	return __lb4_rev_nat(ctx, l3_off, l4_off, tuple, nat,
			     loopback, has_l4_header);
}

static __always_inline void
lb4_key_set_protocol(struct lb4_key *key __maybe_unused,
		     __u8 protocol __maybe_unused)
{
	key->proto = protocol;
}

static __always_inline void
lb4_fill_key(struct lb4_key *key, const struct ipv4_ct_tuple *tuple)
{
	lb4_key_set_protocol(key, tuple->nexthdr);
	key->address = tuple->daddr;
	/* CT tuple has ports in reverse order: */
	key->dport = tuple->sport;
}

/** Extract IPv4 CT tuple from packet
 * @arg ctx		Packet
 * @arg ip4		Pointer to L3 header
 * @arg fraginfo	fraginfo, as returned by ipfrag_encode_ipv4
 * @arg l4_off		Offset to L4 header
 * @arg tuple		CT tuple
 *
 * Returns:
 *   - CTX_ACT_OK on successful extraction
 *   - DROP_UNKNOWN_L4 if packet should be ignore (sent to stack)
 *   - Negative error code
 */
static __always_inline int
lb4_extract_tuple(struct __ctx_buff *ctx, struct iphdr *ip4, fraginfo_t fraginfo,
		  int l4_off, struct ipv4_ct_tuple *tuple)
{
	tuple->nexthdr = ip4->protocol;
	tuple->daddr = ip4->daddr;
	tuple->saddr = ip4->saddr;

	switch (tuple->nexthdr) {
	case IPPROTO_TCP:
	case IPPROTO_UDP:
#ifdef ENABLE_SCTP
	case IPPROTO_SCTP:
#endif  /* ENABLE_SCTP */
		return ipv4_load_l4_ports(ctx, ip4, fraginfo, l4_off,
					  CT_EGRESS, &tuple->dport);
	case IPPROTO_ICMP:
		return DROP_UNSUPP_SERVICE_PROTO;
	default:
		return DROP_UNKNOWN_L4;
	}
}

static __always_inline
bool lb4_src_range_ok(const struct lb4_service *svc __maybe_unused,
		      __u32 saddr __maybe_unused)
{
#ifdef ENABLE_SRC_RANGE_CHECK
	struct lb4_src_range_key key;
	bool verdict = false;

	if (!lb4_svc_has_src_range_check(svc))
		return true;

	key = (typeof(key)) {
		.lpm_key = { SRC_RANGE_STATIC_PREFIX(key), {} },
		.rev_nat_id = svc->rev_nat_index,
		.addr = saddr,
	};

	if (map_lookup_elem(&cilium_lb4_source_range, &key))
		verdict = true;

	return verdict ^ !!(svc->flags2 & SVC_FLAG_SOURCE_RANGE_DENY);
#else
	return true;
#endif /* ENABLE_SRC_RANGE_CHECK */
}

static __always_inline bool
lb4_to_lb6_service(const struct lb4_service *svc __maybe_unused)
{
#ifdef ENABLE_NAT_46X64
	return svc->flags2 & SVC_FLAG_NAT_46X64;
#else
	return false;
#endif
}

static __always_inline
struct lb4_service *__lb4_lookup_service(struct lb4_key *key)
{
	struct lb4_service *svc;

	svc = map_lookup_elem(&cilium_lb4_services_v2, key);

	/* If there are no elements for a specific protocol, check for ANY entries. */
	if (!svc && key->proto != IPPROTO_ANY) {
		key->proto = IPPROTO_ANY;
		svc = map_lookup_elem(&cilium_lb4_services_v2, key);
	}

	return svc;
}

static __always_inline
struct lb4_service *lb4_lookup_service(struct lb4_key *key,
				       const bool scope_switch)
{
	__u8 orig_proto = key->proto;
	struct lb4_service *svc;

	key->backend_slot = 0;

	key->scope = LB_LOOKUP_SCOPE_EXT;
	svc = __lb4_lookup_service(key);
	if (!svc)
		return NULL;

	if (!scope_switch || !lb4_svc_is_two_scopes(svc))
		return svc;

	key->proto = orig_proto;
	key->scope = LB_LOOKUP_SCOPE_INT;
	return __lb4_lookup_service(key);
}

static __always_inline struct lb4_backend *__lb4_lookup_backend(__u32 backend_id)
{
	return map_lookup_elem(&cilium_lb4_backends_v3, &backend_id);
}

static __always_inline struct lb4_backend *
lb4_lookup_backend(struct __ctx_buff *ctx __maybe_unused, __u32 backend_id)
{
	struct lb4_backend *backend;

	backend = __lb4_lookup_backend(backend_id);
	if (!backend)
		cilium_dbg_lb(ctx, DBG_LB4_LOOKUP_BACKEND_FAIL, backend_id, 0);

	return backend;
}

static __always_inline
struct lb4_service *__lb4_lookup_backend_slot(struct lb4_key *key)
{
	return map_lookup_elem(&cilium_lb4_services_v2, key);
}

static __always_inline
struct lb4_service *lb4_lookup_backend_slot(struct __ctx_buff *ctx __maybe_unused,
					    struct lb4_key *key, __u16 slot)
{
	struct lb4_service *svc;

	key->backend_slot = slot;
	cilium_dbg_lb(ctx, DBG_LB4_LOOKUP_BACKEND_SLOT,
		      key->backend_slot, key->dport);

	svc = __lb4_lookup_backend_slot(key);
	if (svc)
		return svc;

	cilium_dbg_lb(ctx, DBG_LB4_LOOKUP_BACKEND_SLOT_V2_FAIL,
		      key->backend_slot, key->dport);
	return NULL;
}

#if defined(LB_SELECTION_PER_SERVICE) || LB_SELECTION == LB_SELECTION_RANDOM
static __always_inline __u32
lb4_select_backend_id_random(struct __ctx_buff *ctx,
			     struct lb4_key *key,
			     const struct ipv4_ct_tuple *tuple __maybe_unused,
			     const struct lb4_service *svc)
{
	/* Backend slot 0 is always reserved for the service frontend. */
	__u16 slot = (get_prandom_u32() % svc->count) + 1;
	struct lb4_service *be = lb4_lookup_backend_slot(ctx, key, slot);

	return be ? be->backend_id : 0;
}
#endif /* LB_SELECTION_PER_SERVICE || LB_SELECTION == LB_SELECTION_RANDOM */

#if defined(LB_SELECTION_PER_SERVICE) || LB_SELECTION == LB_SELECTION_MAGLEV
static __always_inline __u32
lb4_select_backend_id_maglev(struct __ctx_buff *ctx __maybe_unused,
			     struct lb4_key *key __maybe_unused,
			     const struct ipv4_ct_tuple *tuple,
			     const struct lb4_service *svc)
{
	/* CT tuple is swapped, hence the port swap. See also lb4_local()
	 * call to ct_lazy_lookup4(..., CT_SERVICE, SCOPE_REVERSE, ...)
	 * which then calls ipv4_ct_tuple_reverse().
	 */
	__be16 sport = lb4_svc_is_affinity(svc) ? 0 : tuple->dport;
	__be16 dport = tuple->sport;
	__u32 zero = 0, index = svc->rev_nat_index;
	__u32 *backend_ids;
	void *maglev_lut;

	maglev_lut = map_lookup_elem(&cilium_lb4_maglev, &index);
	if (unlikely(!maglev_lut))
		return 0;

	backend_ids = map_lookup_elem(maglev_lut, &zero);
	if (unlikely(!backend_ids))
		return 0;

	index = __hash_from_tuple_v4(tuple, sport, dport) % LB_MAGLEV_LUT_SIZE;
	return map_array_get_32(backend_ids, index, (LB_MAGLEV_LUT_SIZE - 1) << 2);
}
#endif /* LB_SELECTION_PER_SERVICE || LB_SELECTION == LB_SELECTION_MAGLEV */

#ifdef LB_SELECTION_PER_SERVICE
static __always_inline __u32 lb4_algorithm(const struct lb4_service *svc)
{
	return svc->affinity_timeout >> LB_ALGORITHM_SHIFT;
}

static __always_inline __u32
lb4_select_backend_id(struct __ctx_buff *ctx, struct lb4_key *key,
		      const struct ipv4_ct_tuple *tuple,
		      const struct lb4_service *svc)
{
	switch (lb4_algorithm(svc)) {
	case LB_SELECTION_MAGLEV:
		return lb4_select_backend_id_maglev(ctx, key, tuple, svc);
	case LB_SELECTION_RANDOM:
		return lb4_select_backend_id_random(ctx, key, tuple, svc);
	default:
		return 0;
	}
}
#elif LB_SELECTION == LB_SELECTION_RANDOM
# define lb4_select_backend_id	lb4_select_backend_id_random
#elif LB_SELECTION == LB_SELECTION_MAGLEV
# define lb4_select_backend_id	lb4_select_backend_id_maglev
#elif LB_SELECTION == LB_SELECTION_FIRST
/* Backend selection for tests that always chooses first slot. */
static __always_inline __u32
lb4_select_backend_id(struct __ctx_buff *ctx,
		      struct lb4_key *key,
		      const struct ipv4_ct_tuple *tuple __maybe_unused,
		      const struct lb4_service *svc)
{
	struct lb4_service *be = lb4_lookup_backend_slot(ctx, key, 1);

	return be ? be->backend_id : 0;
}
#else
# error "Invalid load balancer backend selection algorithm!"
#endif /* LB_SELECTION */

static __always_inline int
lb4_xlate(struct __ctx_buff *ctx, __be32 *new_saddr __maybe_unused,
	  __be32 *old_saddr __maybe_unused, __u8 nexthdr __maybe_unused, int l3_off,
	  int l4_off, struct lb4_key *key,
	  const struct lb4_backend *backend __maybe_unused, bool has_l4_header)
{
	const __be32 *new_daddr = &backend->address;
	struct csum_offset csum_off = {};
	__be32 sum;
	int ret;

	if (has_l4_header)
		csum_l4_offset_and_flags(nexthdr, &csum_off);

	ret = ctx_store_bytes(ctx, l3_off + offsetof(struct iphdr, daddr),
			      new_daddr, 4, 0);
	if (ret < 0)
		return DROP_WRITE_ERROR;

	sum = csum_diff(&key->address, 4, new_daddr, 4, 0);
#ifdef USE_LOOPBACK_LB
	if (new_saddr && *new_saddr) {
		cilium_dbg_lb(ctx, DBG_LB4_LOOPBACK_SNAT, *old_saddr, *new_saddr);

		ret = ctx_store_bytes(ctx, l3_off + offsetof(struct iphdr, saddr),
				      new_saddr, 4, 0);
		if (ret < 0)
			return DROP_WRITE_ERROR;

		sum = csum_diff(old_saddr, 4, new_saddr, 4, sum);
	}
#endif /* USE_LOOPBACK_LB */
	if (ipv4_csum_update_by_diff(ctx, l3_off, sum) < 0)
		return DROP_CSUM_L3;
	if (csum_off.offset) {
		if (csum_l4_replace(ctx, l4_off, &csum_off, 0, sum,
				    BPF_F_PSEUDO_HDR) < 0)
			return DROP_CSUM_L4;
	}

	return has_l4_header ? lb_l4_xlate(ctx, nexthdr, l4_off, &csum_off,
					   key->dport, backend->port) :
			       CTX_ACT_OK;
}

#ifdef ENABLE_SESSION_AFFINITY
static __always_inline __u32 lb4_affinity_timeout(const struct lb4_service *svc)
{
	return svc->affinity_timeout & AFFINITY_TIMEOUT_MASK;
}

static __always_inline __u32
__lb4_affinity_backend_id(const struct lb4_service *svc, bool netns_cookie,
			  const union lb4_affinity_client_id *id)
{
	struct lb4_affinity_key key = {
		.rev_nat_id	= svc->rev_nat_index,
		.netns_cookie	= netns_cookie,
		.client_id	= *id,
	};
	struct lb_affinity_val *val;

	val = map_lookup_elem(&cilium_lb4_affinity, &key);
	if (val != NULL) {
		__u32 now = (__u32)bpf_mono_now();
		struct lb_affinity_match match = {
			.rev_nat_id	= svc->rev_nat_index,
			.backend_id	= val->backend_id,
		};

		/* We have seconds granularity for timing values here.
		 * To ensure that session affinity timeout works properly we don't include
		 * the upper bound from the time range.
		 * Session is sticky for range [current, last_used + affinity_timeout)
		 */
		if (READ_ONCE(val->last_used) +
		    bpf_sec_to_mono(lb4_affinity_timeout(svc)) <= now) {
			map_delete_elem(&cilium_lb4_affinity, &key);
			return 0;
		}

		if (!map_lookup_elem(&cilium_lb_affinity_match, &match)) {
			map_delete_elem(&cilium_lb4_affinity, &key);
			return 0;
		}

		WRITE_ONCE(val->last_used, now);
		return val->backend_id;
	}

	return 0;
}

static __always_inline __u32
lb4_affinity_backend_id_by_addr(const struct lb4_service *svc,
				union lb4_affinity_client_id *id)
{
	return __lb4_affinity_backend_id(svc, false, id);
}

static __always_inline void
__lb4_update_affinity(const struct lb4_service *svc, bool netns_cookie,
		      const union lb4_affinity_client_id *id,
		      __u32 backend_id)
{
	__u32 now = (__u32)bpf_mono_now();
	struct lb4_affinity_key key = {
		.rev_nat_id	= svc->rev_nat_index,
		.netns_cookie	= netns_cookie,
		.client_id	= *id,
	};
	struct lb_affinity_val val = {
		.backend_id	= backend_id,
		.last_used	= now,
	};

	map_update_elem(&cilium_lb4_affinity, &key, &val, 0);
}

static __always_inline void
lb4_update_affinity_by_addr(const struct lb4_service *svc,
			    union lb4_affinity_client_id *id, __u32 backend_id)
{
	__lb4_update_affinity(svc, false, id, backend_id);
}
#endif /* ENABLE_SESSION_AFFINITY */

static __always_inline __u32
lb4_affinity_backend_id_by_netns(const struct lb4_service *svc __maybe_unused,
				 union lb4_affinity_client_id *id __maybe_unused)
{
#if defined(ENABLE_SESSION_AFFINITY)
	return __lb4_affinity_backend_id(svc, true, id);
#else
	return 0;
#endif
}

static __always_inline void
lb4_update_affinity_by_netns(const struct lb4_service *svc __maybe_unused,
			     union lb4_affinity_client_id *id __maybe_unused,
			     __u32 backend_id __maybe_unused)
{
#if defined(ENABLE_SESSION_AFFINITY)
	__lb4_update_affinity(svc, true, id, backend_id);
#endif
}

static __always_inline int
lb4_to_lb6(struct __ctx_buff *ctx __maybe_unused,
	   const struct iphdr *ip4 __maybe_unused,
	   int l3_off __maybe_unused)
{
#ifdef ENABLE_NAT_46X64
	union v6addr src6, dst6;

	build_v4_in_v6(&src6, ip4->saddr);
	build_v4_in_v6(&dst6, ip4->daddr);

	return ipv4_to_ipv6(ctx, l3_off, &src6, &dst6);
#else
	return DROP_NAT_46X64_DISABLED;
#endif
}

#ifdef ENABLE_LOCAL_REDIRECT_POLICY
/* Service translation logic for a local-redirect service can cause packets to
 * be looped back to a service node-local backend after translation. This can
 * happen when the node-local backend itself tries to connect to the service
 * frontend for which it acts as a backend. There are cases where this can break
 * traffic flow if the backend needs to forward the redirected traffic to the
 * actual service frontend. Hence, allow service translation for pod traffic
 * getting redirected to backend (across network namespaces), but skip service
 * translation for backend to itself or another service backend within the same
 * namespace. Currently only v4 and v4-in-v6, but no plain v6 is supported.
 *
 * For example, in EKS cluster, a local-redirect service exists with the AWS
 * metadata IP, port as the frontend <169.254.169.254, 80> and kiam proxy as a
 * backend Pod. When traffic destined to the frontend originates from the kiam
 * Pod in namespace ns1 (host ns when the kiam proxy Pod is deployed in
 * hostNetwork mode or regular Pod ns) and the Pod is selected as a backend, the
 * traffic would get looped back to the proxy Pod.
 */
static __always_inline bool
lb4_skip_xlate_from_ctx_to_svc(__net_cookie cookie,
			       __be32 address __maybe_unused, __be16 port __maybe_unused)
{
	struct skip_lb4_key key;
	__u8 *val = NULL;

	memset(&key, 0, sizeof(key));
	key.netns_cookie = cookie;
	key.address = address;
	key.port = port;
	val = map_lookup_elem(&cilium_skip_lb4, &key);
	if (val)
		return true;
	return false;
}
#endif /* ENABLE_LOCAL_REDIRECT_POLICY */

static __always_inline int lb4_local(const void *map, struct __ctx_buff *ctx,
				     int l3_off, fraginfo_t fraginfo, int l4_off,
				     struct lb4_key *key,
				     struct ipv4_ct_tuple *tuple,
				     const struct lb4_service *svc,
				     struct ct_state *state,
				     const bool skip_xlate,
				     __u32 *cluster_id __maybe_unused,
				     __s8 *ext_err,
				     __net_cookie netns_cookie __maybe_unused)
{
	__u32 monitor; /* Deliberately ignored; regular CT will determine monitoring. */
	__be32 saddr = tuple->saddr;
	__u8 flags = tuple->flags;
	struct lb4_backend *backend;
	__u32 backend_id = 0;
	__be32 new_saddr = 0;
	int ret;
#ifdef ENABLE_SESSION_AFFINITY
	union lb4_affinity_client_id client_id = {
		.client_ip = saddr,
	};
#endif

	state->rev_nat_index = svc->rev_nat_index;

	ret = ct_lazy_lookup4(map, tuple, ctx, fraginfo, l4_off, CT_SERVICE,
			      SCOPE_REVERSE, CT_ENTRY_SVC, state, &monitor);
	if (ret < 0)
		goto drop_err;

	switch (ret) {
	case CT_NEW:
		if (unlikely(svc->count == 0))
			goto no_service;

#ifdef ENABLE_SESSION_AFFINITY
		if (lb4_svc_is_affinity(svc)) {
			backend_id = lb4_affinity_backend_id_by_addr(svc, &client_id);
			if (backend_id != 0) {
				backend = lb4_lookup_backend(ctx, backend_id);
				if (backend == NULL)
					backend_id = 0;
			}
		}
#endif
		if (backend_id == 0) {
			/* No CT entry has been found, so select a svc endpoint */
			backend_id = lb4_select_backend_id(ctx, key, tuple, svc);
			backend = lb4_lookup_backend(ctx, backend_id);
			if (backend == NULL)
				goto no_service;
		}

		state->backend_id = backend_id;

		ret = ct_create4(map, NULL, tuple, ctx, CT_SERVICE, state, ext_err);
		/* Fail closed, if the conntrack entry create fails drop
		 * service lookup.
		 */
		if (IS_ERR(ret))
			goto drop_err;

#ifdef ENABLE_ACTIVE_CONNECTION_TRACKING
		_lb_act_conn_open(state->rev_nat_index, backend->zone);
#endif

		break;
	case CT_REPLY:
		backend_id = state->backend_id;

		/* If the lookup fails it means the user deleted the backend out from
		 * underneath us. To resolve this fall back to hash. If this is a TCP
		 * session we are likely to get a TCP RST.
		 */
		backend = lb4_lookup_backend(ctx, backend_id);
#ifdef ENABLE_ACTIVE_CONNECTION_TRACKING
		if (backend) {
			if (state->syn) /* Reopened connections */
				_lb_act_conn_open(svc->rev_nat_index, backend->zone);
			else if (state->closing)
				_lb_act_conn_closed(svc->rev_nat_index, backend->zone);
		}
#endif
		if (unlikely(!backend || backend->flags != BE_STATE_ACTIVE)) {
			/* Drain existing connections, but redirect new ones to only
			 * active backends.
			 */
			if (backend && !state->syn)
				break;

			if (unlikely(svc->count == 0))
				goto no_service;

			backend_id = lb4_select_backend_id(ctx, key, tuple, svc);
			backend = lb4_lookup_backend(ctx, backend_id);
			if (!backend)
				goto no_service;

			state->rev_nat_index = svc->rev_nat_index;
			ct_update_svc_entry(map, tuple, backend_id, svc->rev_nat_index);
		}

		break;
	default:
		ret = DROP_UNKNOWN_CT;
		goto drop_err;
	}

#ifdef ENABLE_CLUSTER_AWARE_ADDRESSING
	*cluster_id = backend->cluster_id;
#endif

	/* Restore flags so that SERVICE flag is only used in used when the
	 * service lookup happens and future lookups use EGRESS or INGRESS.
	 */
	tuple->flags = flags;
#ifdef ENABLE_SESSION_AFFINITY
	if (lb4_svc_is_affinity(svc))
		lb4_update_affinity_by_addr(svc, &client_id, backend_id);
#endif

#if defined(USE_LOOPBACK_LB) || defined(ENABLE_LOCAL_REDIRECT_POLICY)
	if (saddr == backend->address) {
	#if defined(ENABLE_LOCAL_REDIRECT_POLICY)
		if (netns_cookie > 0 && unlikely(lb4_svc_is_localredirect(svc)) &&
		    lb4_skip_xlate_from_ctx_to_svc(netns_cookie, tuple->daddr, tuple->sport))
			return CTX_ACT_OK;
	#endif /* ENABLE_LOCAL_REDIRECT_POLICY */

		/* Special loopback case: The origin endpoint has transmitted to a
		 * service which is being translated back to the source. This would
		 * result in a packet with identical source and destination address.
		 * Linux considers such packets as martian source and will drop unless
		 * received on a loopback device. Perform NAT on the source address
		 * to make it appear from an outside address.
		 */
	#ifdef USE_LOOPBACK_LB
		new_saddr = CONFIG(service_loopback_ipv4).be32;
		state->loopback = 1;
	#endif
	}
#endif

#ifdef USE_LOOPBACK_LB
	if (!state->loopback)
#endif
		tuple->daddr = backend->address;

	if (lb4_svc_is_l7_punt_proxy(svc) &&
	    __lookup_ip4_endpoint(backend->address)) {
		ctx_skip_nodeport_set(ctx);
		return LB_PUNT_TO_STACK;
	}
	if (skip_xlate)
		return CTX_ACT_OK;

	/* CT tuple contains ports in reverse order: */
	if (likely(backend->port))
		tuple->sport = backend->port;

	return lb4_xlate(ctx, &new_saddr, &saddr,
			 tuple->nexthdr, l3_off, l4_off, key,
			 backend, ipfrag_has_l4_header(fraginfo));
no_service:
	ret = DROP_NO_SERVICE;
drop_err:
	tuple->flags = flags;
	return ret;
}

/* lb4_ctx_store_state() stores per packet load balancing state to be picked
 * up on the continuation tail call.
 * Note that the IP headers are already xlated and the tuple is re-initialized
 * from the xlated headers before restoring state.
 * NOTE: if lb_skip_l4_dnat() this is not the case as xlate is skipped. We
 * lose the updated tuple daddr in that case.
 */
static __always_inline void lb4_ctx_store_state(struct __ctx_buff *ctx,
						const struct ct_state *state,
					       __u16 proxy_port, __u32 cluster_id)
{
	ctx_store_meta(ctx, CB_PROXY_MAGIC, (__u32)proxy_port << 16);
	ctx_store_meta(ctx, CB_CT_STATE, (__u32)state->rev_nat_index << 16 |
#ifdef USE_LOOPBACK_LB
		       state->loopback);
#else
		       0);
#endif
	ctx_store_meta(ctx, CB_CLUSTER_ID_EGRESS, cluster_id);
}

/* lb4_ctx_restore_state() restores per packet load balancing state from the
 * previous tail call.
 * tuple->flags does not need to be restored, as it will be reinitialized from
 * the packet.
 */
static __always_inline void
lb4_ctx_restore_state(struct __ctx_buff *ctx, struct ct_state *state,
		       __u16 *proxy_port, __u32 *cluster_id __maybe_unused,
		       bool clear)
{
	__u32 meta = clear ? ctx_load_and_clear_meta(ctx, CB_CT_STATE) :
			     ctx_load_meta(ctx, CB_CT_STATE);
#ifdef USE_LOOPBACK_LB
	if (meta & 1)
		state->loopback = 1;
#endif
	state->rev_nat_index = meta >> 16;

	*proxy_port = clear ? (ctx_load_and_clear_meta(ctx, CB_PROXY_MAGIC) >> 16) :
			      (ctx_load_meta(ctx, CB_PROXY_MAGIC) >> 16);

#ifdef ENABLE_CLUSTER_AWARE_ADDRESSING
	*cluster_id = clear ? ctx_load_and_clear_meta(ctx, CB_CLUSTER_ID_EGRESS) :
			      ctx_load_meta(ctx, CB_CLUSTER_ID_EGRESS);
#endif
}

/* Because we use tail calls and this file is included in bpf_sock.h */
#ifndef SKIP_CALLS_MAP
#ifdef SERVICE_NO_BACKEND_RESPONSE

#define ICMP_PACKET_MAX_SAMPLE_SIZE 64

static __always_inline
__wsum icmp_wsum_accumulate(void *data_start, void *data_end, int sample_len);

static __always_inline
int __tail_no_service_ipv4(struct __ctx_buff *ctx)
{
	void *data, *data_end;
	struct ethhdr *ethhdr;
	struct iphdr *ip4;
	struct icmphdr *icmphdr;
	union macaddr smac = {};
	union macaddr dmac = {};
	__be32	saddr;
	__be32	daddr;
	__u8	tos;
	__wsum csum;
	int sample_len;
	int ret;
	const int inner_offset = sizeof(struct ethhdr) + sizeof(struct iphdr) +
		sizeof(struct icmphdr);

	if (!revalidate_data(ctx, &data, &data_end, &ip4))
		return DROP_INVALID;

	/* copy the incoming src and dest IPs and mac addresses to the stack.
	 * the pointers will not be valid after adding headroom.
	 */

	if (eth_load_saddr(ctx, smac.addr, 0) < 0)
		return DROP_INVALID;

	if (eth_load_daddr(ctx, dmac.addr, 0) < 0)
		return DROP_INVALID;

	saddr = ip4->saddr;
	daddr = ip4->daddr;
	tos = ip4->tos;

	/* Resize to ethernet header + 64 bytes or less */
	sample_len = (int)ctx_full_len(ctx);
	if (sample_len > ICMP_PACKET_MAX_SAMPLE_SIZE)
		sample_len = ICMP_PACKET_MAX_SAMPLE_SIZE;
	ctx_adjust_troom(ctx, (__s32)(sample_len + sizeof(struct ethhdr) - ctx_full_len(ctx)));

	data = ctx_data(ctx);
	data_end = ctx_data_end(ctx);

	/* Calculate the checksum of the ICMP sample */
	csum = icmp_wsum_accumulate(data + sizeof(struct ethhdr), data_end, sample_len);

	/* We need to insert a IPv4 and ICMP header before the original packet.
	 * Make that room.
	 */

#if __ctx_is == __ctx_xdp
	ret = xdp_adjust_head(ctx, 0 - (int)(sizeof(struct iphdr) + sizeof(struct icmphdr)));
#else
	ret = skb_adjust_room(ctx, sizeof(struct iphdr) + sizeof(struct icmphdr),
			      BPF_ADJ_ROOM_MAC, 0);
#endif

	if (ret < 0)
		return DROP_INVALID;

	/* changing size invalidates pointers, so we need to re-fetch them. */
	data = ctx_data(ctx);
	data_end = ctx_data_end(ctx);

	/* Bound check all 3 headers at once. */
	if (data + inner_offset > data_end)
		return DROP_INVALID;

	/* Write reversed eth header, ready for egress */
	ethhdr = data;
	memcpy(ethhdr->h_dest, smac.addr, sizeof(smac.addr));
	memcpy(ethhdr->h_source, dmac.addr, sizeof(dmac.addr));
	ethhdr->h_proto = bpf_htons(ETH_P_IP);

	/* Write reversed ip header, ready for egress */
	ip4 = data + sizeof(struct ethhdr);
	ip4->version = 4;
	ip4->ihl = sizeof(struct iphdr) >> 2;
	ip4->tos = tos;
	ip4->tot_len = bpf_htons(sizeof(struct iphdr) + sizeof(struct icmphdr) +
		       (__u16)sample_len);
	ip4->id = 0;
	ip4->frag_off = 0;
	ip4->ttl = IPDEFTTL;
	ip4->protocol = IPPROTO_ICMP;
	ip4->check = 0;
	ip4->daddr = saddr;
	ip4->saddr = daddr;
	ip4->check = csum_fold(csum_diff(ip4, 0, ip4, sizeof(struct iphdr), 0));

	/* Write reversed icmp header */
	icmphdr = data + sizeof(struct ethhdr) + sizeof(struct iphdr);
	icmphdr->type = ICMP_DEST_UNREACH;
	icmphdr->code = ICMP_PORT_UNREACH;
	icmphdr->checksum = 0;
	icmphdr->un.gateway = 0;

	/* Add ICMP header checksum to sum of its body */
	csum += csum_diff(icmphdr, 0, icmphdr, sizeof(struct icmphdr), 0);
	icmphdr->checksum = csum_fold(csum);

	return 0;
}

__declare_tail(CILIUM_CALL_IPV4_NO_SERVICE)
int tail_no_service_ipv4(struct __ctx_buff *ctx)
{
	__u32 src_sec_identity = ctx_load_meta(ctx, CB_SRC_LABEL);
	int ret;

	ret = __tail_no_service_ipv4(ctx);
	if (!ret) {
		/* Redirect ICMP to the interface we received it on. */
		cilium_dbg_capture(ctx, DBG_CAPTURE_DELIVERY,
				   ctx_get_ifindex(ctx));
		ret = redirect_self(ctx);
	}

	if (IS_ERR(ret))
		return send_drop_notify_error(ctx, src_sec_identity, ret,
					      METRIC_INGRESS);

	return ret;
}
#endif /* SERVICE_NO_BACKEND_RESPONSE */
#endif /* SKIP_CALLS_MAP */

#endif /* ENABLE_IPV4 */

#ifdef ENABLE_IPV6

/* Because we use tail calls and this file is included in bpf_sock.h */
#ifndef SKIP_CALLS_MAP
#ifdef SERVICE_NO_BACKEND_RESPONSE

#define ICMPV6_PACKET_MAX_SAMPLE_SIZE 1280 - sizeof(struct ipv6hdr) - sizeof(struct icmp6hdr)

static __always_inline
__wsum icmp_wsum_accumulate(void *data_start, void *data_end, int sample_len);

/* The IPv6 pseudo-header */
struct ipv6_pseudo_header_t {
	union {
		struct header {
			struct in6_addr src_ip;
			struct in6_addr dst_ip;
			__be32 top_level_length;
			__u8 zero[3];
			__u8 next_header;
		} __packed fields;
		__u16 words[20];
	};
};

static __always_inline
int __tail_no_service_ipv6(struct __ctx_buff *ctx)
{
	void *data, *data_end;
	struct ethhdr *ethhdr;
	struct ipv6hdr *ip6;
	struct icmp6hdr *icmphdr;
	struct ipv6_pseudo_header_t pseudo_header;
	union macaddr smac = {};
	union macaddr dmac = {};
	struct in6_addr saddr;
	struct in6_addr daddr;
	__wsum csum;
	__u64 sample_len;
	int i;
	int ret;
	const int inner_offset = sizeof(struct ethhdr) + sizeof(struct ipv6hdr) +
		sizeof(struct icmp6hdr);

	if (!revalidate_data(ctx, &data, &data_end, &ip6))
		return DROP_INVALID;

	/* copy the incoming src and dest IPs and mac addresses to the stack.
	 * the pointers will not be valid after adding headroom.
	 */

	if (eth_load_saddr(ctx, smac.addr, 0) < 0)
		return DROP_INVALID;

	if (eth_load_daddr(ctx, dmac.addr, 0) < 0)
		return DROP_INVALID;

	memcpy(&saddr, &ip6->saddr, sizeof(struct in6_addr));
	memcpy(&daddr, &ip6->daddr, sizeof(struct in6_addr));

	/* Resize to min MTU - IPv6 hdr + ICMPv6 hdr */
	sample_len = ctx_full_len(ctx);
	if (sample_len > (__u64)ICMPV6_PACKET_MAX_SAMPLE_SIZE)
		sample_len = ICMPV6_PACKET_MAX_SAMPLE_SIZE;
	ctx_adjust_troom(ctx, (__s32)(sample_len + sizeof(struct ethhdr) - ctx_full_len(ctx)));

	data = ctx_data(ctx);
	data_end = ctx_data_end(ctx);

	/* Calculate the unfolded checksum of the ICMPv6 sample */
	csum = icmp_wsum_accumulate(data + sizeof(struct ethhdr), data_end, (int)sample_len);

	/* We need to insert a IPv6 and ICMPv6 header before the original packet.
	 * Make that room.
	 */

#if __ctx_is == __ctx_xdp
	ret = xdp_adjust_head(ctx, 0 - (int)(sizeof(struct ipv6hdr) + sizeof(struct icmp6hdr)));
#else
	ret = skb_adjust_room(ctx, sizeof(struct ipv6hdr) + sizeof(struct icmp6hdr),
			      BPF_ADJ_ROOM_MAC, 0);
#endif

	if (ret < 0)
		return DROP_INVALID;

	/* changing size invalidates pointers, so we need to re-fetch them. */
	data = ctx_data(ctx);
	data_end = ctx_data_end(ctx);

	/* Bound check all 3 headers at once. */
	if (data + inner_offset > data_end)
		return DROP_INVALID;

	/* Write reversed eth header, ready for egress */
	ethhdr = data;
	memcpy(ethhdr->h_dest, smac.addr, sizeof(smac.addr));
	memcpy(ethhdr->h_source, dmac.addr, sizeof(dmac.addr));
	ethhdr->h_proto = bpf_htons(ETH_P_IPV6);

	/* Write reversed ip header, ready for egress */
	ip6 = data + sizeof(struct ethhdr);
	ip6->version = 6;
	ip6->priority = 0;
	ip6->flow_lbl[0] = 0;
	ip6->flow_lbl[1] = 0;
	ip6->flow_lbl[2] = 0;
	ip6->payload_len = bpf_htons(sizeof(struct icmp6hdr) + (__u16)sample_len);
	ip6->nexthdr = IPPROTO_ICMPV6;
	ip6->hop_limit = IPDEFTTL;
	memcpy(&ip6->daddr, &saddr, sizeof(struct in6_addr));
	memcpy(&ip6->saddr, &daddr, sizeof(struct in6_addr));

	/* Write reversed icmp header */
	icmphdr = data + sizeof(struct ethhdr) + sizeof(struct ipv6hdr);
	icmphdr->icmp6_type = ICMPV6_DEST_UNREACH;
	icmphdr->icmp6_code = ICMPV6_PORT_UNREACH;
	icmphdr->icmp6_cksum = 0;
	icmphdr->icmp6_dataun.un_data32[0] = 0;

	/* Add the ICMP header to the checksum (only type and code are non-zero) */
	csum += ((__u16)icmphdr->icmp6_code) << 8 | (__u16)icmphdr->icmp6_type;

	/* Fill pseudo header */
	memcpy(&pseudo_header.fields.src_ip, &ip6->saddr, sizeof(struct in6_addr));
	memcpy(&pseudo_header.fields.dst_ip, &ip6->daddr, sizeof(struct in6_addr));
	pseudo_header.fields.top_level_length = bpf_htonl(sizeof(struct icmp6hdr) +
						(__u32)sample_len);
	__bpf_memzero(pseudo_header.fields.zero, sizeof(pseudo_header.fields.zero));
	pseudo_header.fields.next_header = IPPROTO_ICMPV6;

	#pragma unroll
	for (i = 0; i < (int)(sizeof(pseudo_header.words) / sizeof(__u16)); i++)
		csum += pseudo_header.words[i];

	icmphdr->icmp6_cksum = csum_fold(csum);

	return 0;
}

__declare_tail(CILIUM_CALL_IPV6_NO_SERVICE)
int tail_no_service_ipv6(struct __ctx_buff *ctx)
{
	__u32 src_sec_identity = ctx_load_meta(ctx, CB_SRC_LABEL);
	struct ratelimit_key rkey = {
		.usage = RATELIMIT_USAGE_ICMPV6,
	};
	/* Rate limit to 100 ICMPv6 replies per second, burstable to 1000 responses/s */
	struct ratelimit_settings settings = {
		.bucket_size = 1000,
		.tokens_per_topup = 100,
		.topup_interval_ns = NSEC_PER_SEC,
	};
	int ret;

	rkey.key.icmpv6.netdev_idx = ctx_get_ifindex(ctx);
	if (!ratelimit_check_and_take(&rkey, &settings)) {
		ret = DROP_RATE_LIMITED;
		goto drop_err;
	}

	ret = __tail_no_service_ipv6(ctx);
	if (!ret) {
		/* Redirect ICMP to the interface we received it on. */
		cilium_dbg_capture(ctx, DBG_CAPTURE_DELIVERY,
				   ctx_get_ifindex(ctx));
		ret = redirect_self(ctx);
	}

drop_err:
	if (IS_ERR(ret))
		return send_drop_notify_error(ctx, src_sec_identity, ret,
					      METRIC_INGRESS);

	return ret;
}
#endif /* SERVICE_NO_BACKEND_RESPONSE */
#endif /* SKIP_CALLS_MAP */
#endif /* ENABLE_IPV6 */

#ifdef SERVICE_NO_BACKEND_RESPONSE

static __always_inline
__wsum icmp_wsum_accumulate(void *data_start, void *data_end, int sample_len)
{
	/* Unrolled loop to calculate the checksum of the ICMP sample
	 * Done manually because the compiler refuses with #pragma unroll
	 */
	__wsum wsum = 0;

	#define body(i) if ((i) > sample_len) \
		return wsum; \
	if (data_start + (i) + sizeof(__u16) > data_end) { \
		if (data_start + (i) + sizeof(__u8) <= data_end)\
			wsum += *(__u8 *)(data_start + (i)); \
		return wsum; \
	} \
	wsum += *(__u16 *)(data_start + (i));

	#define body4(i) body(i)\
		body(i + 2) \
		body(i + 4) \
		body(i + 6)

	#define body16(i) body4(i)\
		body4(i + 8) \
		body4(i + 16) \
		body4(i + 24)

	#define body128(i) body16(i)\
		body16(i + 32) \
		body16(i + 64) \
		body16(i + 96)

	body128(0)
	body128(256)
	body128(512)
	body128(768)
	body128(1024)

	return wsum;
}

#endif /* SERVICE_NO_BACKEND_RESPONSE */
