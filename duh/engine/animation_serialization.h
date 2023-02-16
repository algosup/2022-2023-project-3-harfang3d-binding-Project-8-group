// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

namespace hg {

struct DocumentReader;
struct DocumentWriter;
struct Anim;

bool SerializeAnimation(DocumentWriter &doc, const Anim &anim);
bool DeserializeAnimation(DocumentReader &doc, Anim &anim);

} // namespace hg
