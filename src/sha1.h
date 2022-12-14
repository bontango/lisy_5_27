/* sha1.h
 *
 * The sha1 hash function.
 */

/* nettle, low-level cryptographics library
 *
 * Copyright (C) 2001 Niels M?ller
 *  
 * The nettle library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version.
 * 
 * The nettle library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with the nettle library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 */
 
#ifndef NETTLE_SHA1_H_INCLUDED
#define NETTLE_SHA1_H_INCLUDED
#if !defined(__GNUC__) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || (__GNUC__ >= 4)	// GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#ifndef _STDINT_H
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
#endif

#define SHA1_DIGEST_SIZE 20
#define SHA1_DATA_SIZE 64

/* Digest is kept internally as 4 32-bit words. */
#define _SHA1_DIGEST_LENGTH 5

struct sha1_ctx
{
  uint32_t digest[_SHA1_DIGEST_LENGTH];   /* Message digest */
  uint32_t count_low, count_high;         /* 64-bit block count */
  uint8_t block[SHA1_DATA_SIZE];          /* SHA1 data buffer */
  unsigned int index;                     /* index into buffer */
};

void
sha1_init(struct sha1_ctx *ctx);

void
sha1_update(struct sha1_ctx *ctx,
	    unsigned length,
	    const uint8_t *data);

void
sha1_final(struct sha1_ctx *ctx);

void
sha1_digest(const struct sha1_ctx *ctx,
	    unsigned length,
	    uint8_t *digest);

#endif /* NETTLE_SHA1_H_INCLUDED */
