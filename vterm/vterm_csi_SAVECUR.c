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

void interpret_csi_SAVECUR(vterm_t *vterm, __attribute__((__unused__)) int param[], __attribute__((__unused__)) int pcount)
{
   vterm->saved_x=vterm->ccol;
   vterm->saved_y=vterm->crow;

   return;
}

