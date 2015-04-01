/*
Copyright (C) 2009 Bryan Christ

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

/*
This library is based on ROTE written by Bruno Takahashi C. de Oliveira
*/

#include <string.h>

#include "vterm/vterm.h"
#include "vterm/vterm_private.h"
#include "vterm/vterm_csi.h"
#include "vterm/vterm_escape.h"
#include "vterm/vterm_render.h"

void vterm_put_char(vterm_t *vterm,chtype c)
{
	static char		vt100_acs[]="`afgjklmnopqrstuvwxyz{|}~";

   if(vterm->ccol >= vterm->cols)
   {
      vterm->ccol=0;
      vterm_scroll_down(vterm);
   }

   if(IS_MODE_ACS(vterm))
   {
	   if(strchr(vt100_acs,(char)c)!=NULL)
      {
         vterm->cells[vterm->crow][vterm->ccol].ch=c;//NCURSES_ACS(c);
      }
   }
   else
   {
      vterm->cells[vterm->crow][vterm->ccol].ch=c;
   }

   vterm->cells[vterm->crow][vterm->ccol].attr=vterm->curattr;
   vterm->ccol++;
   vterm_invalidate_cell(vterm, vterm->crow, vterm->ccol - 1);

   return;
}

void vterm_render_ctrl_char(vterm_t *vterm,char c)
{
   switch(c)
   {
      /* carriage return */
      case '\r':
      {
         vterm->ccol=0;
         break;
      }

      /* line-feed */
      case '\n':
      {
         vterm->ccol=0;
         vterm_scroll_down(vterm);
         break;
      }

      /* backspace */
      case '\b':
      {
         if(vterm->ccol > 0) vterm->ccol--;
         break;
      }

      /* tab */
      case '\t':
      {
         while(vterm->ccol%8) vterm_put_char(vterm,' ');
         break;
      }

      /* begin escape sequence (aborting previous one if any) */
      case '\x1B':
      {
         vterm_escape_start(vterm);
         break;
      }

      /* enter graphical character mode */
      case '\x0E':
      {
         vterm->state |= STATE_ALT_CHARSET;
        	break;
      }

      /* exit graphical character mode */
      case '\x0F':
      {
         vterm->state &= ~STATE_ALT_CHARSET;
         break;
      }

      /* CSI character. Equivalent to ESC [ */
      case '\x9B':
      {
         vterm_escape_start(vterm);
         vterm->esbuf[vterm->esbuf_len++]='[';
         break;
      }

      /* these interrupt escape sequences */
      case '\x18':
      case '\x1A':
      {
         vterm_escape_cancel(vterm);
         break;
      }

      /* bell */
      case '\a':
      {
         vterm_handle_bell(vterm);
         break;
      }

#ifdef DEBUG
      default:
         //TODO: fprintf(stderr, "Unrecognized control char: %d (^%c)\n", c, c + '@');
         break;
#endif
   }
}

void vterm_render(vterm_t *vterm,const char *data,int len)
{
   int i;

   for (i = 0; i < len; i++, data++)
   {
      /* completely ignore NUL */
      if(*data == 0) continue;

      if(*data >= 1 && *data <= 31)
      {
         vterm_render_ctrl_char(vterm,*data);
         continue;
      }

      if(IS_MODE_ESCAPED(vterm) && vterm->esbuf_len < ESEQ_BUF_SIZE)
      {
         /* append character to ongoing escape sequence */
         vterm->esbuf[vterm->esbuf_len]=*data;
         vterm->esbuf[++vterm->esbuf_len]=0;

         try_interpret_escape_seq(vterm);
      }
      else
      {
        	vterm_put_char(vterm,*data);
      }
   }
   vterm_invalidate_cursor(vterm);
}


