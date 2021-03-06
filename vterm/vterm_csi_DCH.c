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


#include "vterm/vterm.h"
#include "vterm/vterm_private.h"
#include "vterm/vterm_csi.h"

/* Interpret the 'delete chars' sequence (DCH) */
void interpret_csi_DCH(vterm_t *vterm, int param[], int pcount)
{
   int i;
   int n=1;

   if(pcount && param[0] > 0) n=param[0]; 

   for(i=vterm->ccol;i < vterm->cols;i++)
   {
      if(i+n < vterm->cols)
      {
         vterm->cells[vterm->crow][i]=vterm->cells[vterm->crow][i+n];
	 vterm_invalidate_cell(vterm, vterm->crow, i);
      }
      else
      {
         vterm->cells[vterm->crow][i].ch=0x20;
         vterm->cells[vterm->crow][i].attr=vterm->curattr;
	 vterm_invalidate_cell(vterm, vterm->crow, i);
      }
   }
}

