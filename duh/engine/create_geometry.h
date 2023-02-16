// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#include "engine/geometry.h"

namespace hg {

std::shared_ptr<Geometry> CreateCapsule(float radius = 0.5f, float height = 2.f, int subdiv_x = 6, int subdiv_y = 16, const std::string &material_path = std::string(), const std::string &name = std::string());
std::shared_ptr<Geometry> CreateCone(float radius = 0.5f, float height = 1.f, int subdiv_x = 16, const std::string &material_path = std::string(), const std::string &name = std::string());
std::shared_ptr<Geometry> CreateCube(float width = 1.f, float height = 1.f, float length = 1.f, const std::string &material_path = std::string(), const std::string &name = std::string());
std::shared_ptr<Geometry> CreateCylinder(float radius = 0.5f, float height = 1.f, int subdiv_x = 16, const std::string &material_path = std::string(), const std::string &name = std::string());
std::shared_ptr<Geometry> CreatePlane(float width = 1.f, float length = 1.f, int subdiv = 1.f, const std::string &material_path = std::string(), const std::string &name = std::string());
std::shared_ptr<Geometry> CreateSphere(float radius = 0.5f, int subdiv_x = 6, int subdiv_y = 16, const std::string &material_path = std::string(), const std::string &name = std::string());
std::shared_ptr<Geometry> CreateGeometryFromHeightmap(uint32_t width, uint32_t height, const float *heightmap, float scale = 1.f, const std::string &material_path = std::string(), const std::string &name = std::string());

} // namespace hg
