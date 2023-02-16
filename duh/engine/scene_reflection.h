// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "engine/light.h"
#include "foundation/reflection.h"
#include <memory>

namespace hg {

struct MouseEvent;

class Transform;
class Target;
class Environment;
class Collision;
class ConvexCollision;
class BoxCollision;
class SphereCollision;
class CapsuleCollision;
class MeshCollision;
class Metadata;

class Joint;
class SphericalJoint;
class D6Joint;
struct JointLimit;
class RigidBody;

class Camera;
class Object;
class Instance;
class Decorator;

struct NavigationLayer;
class NavigationGeometry;
class Navigation;
class NavigationSystem;

class Script;
struct LogicScript;
struct RenderScript;

class Skybox;
class Terrain;
class GrassField;
class SceneOverlay;
class SimpleGraphicSceneOverlay;

struct PostProcessComponent;
class SharpenPostProcess;
class RadialBlurPostProcess;
class ChromaticDispersionPostProcess;
class HSLPostProcess;
class MotionBlurPostProcess;
class SAOPostProcess;
class BloomPostProcess;

class SceneSystem;
class TransformSystem;
class CameraSystem;
class InstanceSystem;
class LightSystem;
class PhysicSystem;
class RenderableSystem;

struct ScriptEngineEnv;
class ScriptSystem;
struct ScriptParamList;

void RegisterSceneReflection(TypeRegistry &registry);

DeclareTypeName(MouseEvent, "MouseEvent");

DeclareTypeName(Scene, "Scene");
DeclareTypeName(std::shared_ptr<Scene>, "std::shared_ptr<Scene>");
DeclareTypeName(Node, "Node");
DeclareTypeName(std::shared_ptr<Node>, "std::shared_ptr<Node>");

DeclareTypeName(Component, "Component");
DeclareTypeName(std::shared_ptr<Component>, "std::shared_ptr<Component>");
DeclareTypeName(Transform, "Transform");
DeclareTypeName(std::shared_ptr<Transform>, "sTransform");
DeclareTypeName(Target, "Target");
DeclareTypeName(std::shared_ptr<Target>, "sTarget");
DeclareTypeName(Environment, "Environment");
DeclareTypeName(std::shared_ptr<Environment>, "std::shared_ptr<Environment>");
DeclareTypeName(Collision, "Collision");
DeclareTypeName(std::shared_ptr<Collision>, "std::shared_ptr<Collision>");
DeclareTypeName(ConvexCollision, "ConvexCollision");
DeclareTypeName(std::shared_ptr<ConvexCollision>, "sConvexCollision");
DeclareTypeName(BoxCollision, "BoxCollision");
DeclareTypeName(std::shared_ptr<BoxCollision>, "sBoxCollision");
DeclareTypeName(SphereCollision, "SphereCollision");
DeclareTypeName(std::shared_ptr<SphereCollision>, "sSphereCollision");
DeclareTypeName(CapsuleCollision, "CapsuleCollision");
DeclareTypeName(std::shared_ptr<CapsuleCollision>, "sCapsuleCollision");
DeclareTypeName(MeshCollision, "MeshCollision");
DeclareTypeName(std::shared_ptr<MeshCollision>, "sMeshCollision");
DeclareTypeName(Metadata, "Metadata");
DeclareTypeName(std::shared_ptr<Metadata>, "sMetadata");

DeclareTypeName(Joint, "Joint");
DeclareTypeName(std::shared_ptr<Joint>, "std::shared_ptr<Joint>");
DeclareTypeName(SphericalJoint, "SphericalJoint");
DeclareTypeName(std::shared_ptr<SphericalJoint>, "sSphericalJoint");
DeclareTypeName(D6Joint, "D6Joint");
DeclareTypeName(std::shared_ptr<D6Joint>, "sD6Joint");
DeclareTypeName(JointLimit, "JointLimit");
DeclareTypeName(std::shared_ptr<JointLimit>, "sJointLimit");
DeclareTypeName(RigidBody, "RigidBody");
DeclareTypeName(std::shared_ptr<RigidBody>, "std::shared_ptr<RigidBody>");

DeclareTypeName(Camera, "Camera");
DeclareTypeName(std::shared_ptr<Camera>, "sCamera");
DeclareTypeName(Light, "Light");
DeclareTypeName(std::shared_ptr<Light>, "sLight");
DeclareTypeName(Object, "Object");
DeclareTypeName(std::shared_ptr<Object>, "sObject");
DeclareTypeName(Instance, "Instance");
DeclareTypeName(std::shared_ptr<Instance>, "std::shared_ptr<Instance>");
DeclareTypeName(Decorator, "Decorator");
DeclareTypeName(std::shared_ptr<Decorator>, "sDecorator");

DeclareTypeName(Light::Model, "LightModel");
DeclareTypeName(Light::Shadow, "LightShadow");

DeclareTypeName(std::vector<NavigationLayer>, "NavigationLayerVector");
DeclareTypeName(NavigationLayer, "NavigationLayer");
DeclareTypeName(NavigationGeometry, "NavigationGeometry");
DeclareTypeName(std::shared_ptr<NavigationGeometry>, "std::shared_ptr<NavigationGeometry>");

DeclareTypeName(Navigation, "Navigation");
DeclareTypeName(std::shared_ptr<Navigation>, "sNavigation");
DeclareTypeName(NavigationSystem, "NavigationSystem");
DeclareTypeName(std::shared_ptr<NavigationSystem>, "sNavigationSystem");

DeclareTypeName(ScriptParamList, "ScriptParamList");

DeclareTypeName(Script, "Script");
DeclareTypeName(std::shared_ptr<Script>, "std::shared_ptr<Script>");
DeclareTypeName(LogicScript, "LogicScript");
DeclareTypeName(std::shared_ptr<LogicScript>, "sLogicScript");
DeclareTypeName(RenderScript, "RenderScript");
DeclareTypeName(std::shared_ptr<RenderScript>, "sRenderScript");

DeclareTypeName(Skybox, "Skybox");
DeclareTypeName(std::shared_ptr<Skybox>, "sSkybox");
DeclareTypeName(Terrain, "Terrain");
DeclareTypeName(std::shared_ptr<Terrain>, "sTerrain");
DeclareTypeName(GrassField, "GrassField");
DeclareTypeName(std::shared_ptr<GrassField>, "sGrassField");
DeclareTypeName(SceneOverlay, "SceneOverlay");
DeclareTypeName(std::shared_ptr<SceneOverlay>, "sSceneOverlay");
DeclareTypeName(SimpleGraphicSceneOverlay, "SimpleGraphicSceneOverlay");
DeclareTypeName(std::shared_ptr<SimpleGraphicSceneOverlay>, "sSimpleGraphicSceneOverlay");

DeclareTypeName(PostProcessComponent, "PostProcessComponent");
DeclareTypeName(std::shared_ptr<PostProcessComponent>, "sPostProcessComponent");
DeclareTypeName(SharpenPostProcess, "SharpenPostProcess");
DeclareTypeName(std::shared_ptr<SharpenPostProcess>, "sSharpenPostProcess");
DeclareTypeName(RadialBlurPostProcess, "RadialBlurPostProcess");
DeclareTypeName(std::shared_ptr<RadialBlurPostProcess>, "sRadialBlurPostProcess");
DeclareTypeName(ChromaticDispersionPostProcess, "ChromaticDispersionPostProcess");
DeclareTypeName(std::shared_ptr<ChromaticDispersionPostProcess>, "sChromaticDispersionPostProcess");
DeclareTypeName(HSLPostProcess, "HSLPostProcess");
DeclareTypeName(std::shared_ptr<HSLPostProcess>, "sHSLPostProcess");
DeclareTypeName(MotionBlurPostProcess, "MotionBlurPostProcess");
DeclareTypeName(std::shared_ptr<MotionBlurPostProcess>, "sMotionBlurPostProcess");
DeclareTypeName(SAOPostProcess, "SAOPostProcess");
DeclareTypeName(std::shared_ptr<SAOPostProcess>, "sSAOPostProcess");
DeclareTypeName(BloomPostProcess, "BloomPostProcess");
DeclareTypeName(std::shared_ptr<BloomPostProcess>, "sBloomPostProcess");

DeclareTypeName(SceneSystem, "SceneSystem");
DeclareTypeName(std::shared_ptr<SceneSystem>, "std::shared_ptr<SceneSystem>");
DeclareTypeName(TransformSystem, "TransformSystem");
DeclareTypeName(std::shared_ptr<TransformSystem>, "sTransformSystem");
DeclareTypeName(CameraSystem, "CameraSystem");
DeclareTypeName(std::shared_ptr<CameraSystem>, "sCameraSystem");
DeclareTypeName(InstanceSystem, "InstanceSystem");
DeclareTypeName(std::shared_ptr<InstanceSystem>, "sInstanceSystem");
DeclareTypeName(LightSystem, "LightSystem");
DeclareTypeName(std::shared_ptr<LightSystem>, "std::shared_ptr<LightSystem>");
DeclareTypeName(PhysicSystem, "PhysicSystem");
DeclareTypeName(std::shared_ptr<PhysicSystem>, "std::shared_ptr<PhysicSystem>");
DeclareTypeName(RenderableSystem, "RenderableSystem");
DeclareTypeName(std::shared_ptr<RenderableSystem>, "sRenderableSystem");

DeclareTypeName(ScriptEngineEnv, "ScriptEngineEnv");
DeclareTypeName(std::shared_ptr<ScriptEngineEnv>, "std::shared_ptr<ScriptEngineEnv>");
DeclareTypeName(ScriptSystem, "ScriptSystem");
DeclareTypeName(std::shared_ptr<ScriptSystem>, "sScriptSystem");

} // namespace hg
