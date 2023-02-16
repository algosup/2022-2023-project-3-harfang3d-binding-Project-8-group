// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/picture.h"
#include "foundation/rect_packer.h"

namespace hg {

template <class T>
void DrawRectPackerSplit(typename RectPacker<T>::split_index idx, Rect<T> cell_rect, const std::vector<typename RectPacker<T>::Split> &splits, Picture &pic) {
	if (splits[idx].leaf)
		return;

	Rect<T> left_cell_rect, right_cell_rect;

	switch (splits[idx].axis) {
		case RectPacker<T>::X:
			pic.DrawLine(cell_rect.sx, splits[idx].pos + 1, cell_rect.ex, splits[idx].pos + 1);
			left_cell_rect.Set(cell_rect.sx, cell_rect.sy, cell_rect.ex, splits[idx].pos);
			right_cell_rect.Set(cell_rect.sx, splits[idx].pos + 1, cell_rect.ex, cell_rect.ey);
			break;

		case RectPacker<T>::Y:
			pic.DrawLine(splits[idx].pos + 1, cell_rect.sy, splits[idx].pos + 1, cell_rect.ey);
			left_cell_rect.Set(cell_rect.sx, cell_rect.sy, splits[idx].pos, cell_rect.ey);
			right_cell_rect.Set(splits[idx].pos + 1, cell_rect.sy, cell_rect.ex, cell_rect.ey);
			break;
	}

	if (splits[idx].left != RectPacker<T>::invalid_split)
		DrawRectPackerSplit(splits[idx].left, left_cell_rect, splits, pic);

	if (splits[idx].right != RectPacker<T>::invalid_split)
		DrawRectPackerSplit(splits[idx].right, right_cell_rect, splits, pic);
}

template <class T>
void DrawRectPackerSplits(Rect<T> root_rect, const std::vector<typename RectPacker<T>::Split> &splits, Picture &pic) {
	pic.SetPenColorRGBA(1, 0, 0);
	DrawRectPackerSplit(0, root_rect, splits, pic);
}

} // namespace hg
