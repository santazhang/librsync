/* -*- mode: c; c-file-style: "k&r" -*-  */

/* hsemit -- Convert a text stream of commands into hsync encoding.
   The output doesn't include literal data, since the point is just to
   test that command emit/inhale works well.
   
   Copyright (C) 2000 by Martin Pool <mbp@humbug.org.au>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
   USA */

#include "includes.h"
#include "hsync.h"
#include "hsyncproto.h"
#include "private.h"


#ifdef __GNUC__
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif				/* ! __GNUC__ */


int
main(int argc UNUSED, char **argv UNUSED)
{
     int ret, off, len;
     char cmd[20];
     hs_filebuf_t * outfb;
     hs_stats_t stats;

     outfb = hs_filebuf_from_fd(STDOUT_FILENO);
     
     while (1) {
	  ret = scanf("%20s", cmd);
	  if (ret == EOF)
	       return 0;

	  if (!strcmp(cmd, "COPY")) {
	       if (scanf("%d %d", &off, &len) != 2)
		    return 1;
	       ret = _hs_emit_copy(hs_filebuf_write, outfb,
				   off, len, &stats);
	       if (ret < 0)
		    return 1;
	  } else if (!strcmp(cmd, "EOF")) {
	       ret = _hs_emit_eof(hs_filebuf_write, outfb, &stats);
	       if (ret < 0)
		    return 1;
	  } else if (!strcmp(cmd, "LITERAL")) {
	       if (scanf("%d", &len) != 1)
		    return 1;
	       
	       ret = _hs_emit_literal_cmd(hs_filebuf_write, outfb, len);
	       if (ret < 0)
		    return 1;
	  } else if (!strcmp(cmd, "SIGNATURE")) {
	       if (scanf("%d", &len) != 1)
		    return 1;
	       
	       ret = _hs_emit_signature_cmd(hs_filebuf_write, outfb, len);
	       if (ret < 0)
		    return 1;
	  } else {
	       fprintf(stderr, "can't understand command `%s'\n",
		       cmd);
	       return 1;
	  }
     }
}