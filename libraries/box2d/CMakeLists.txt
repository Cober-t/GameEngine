cmake_minimum_required(VERSION 3.8)
project (box2d)

add_library(box2d STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/Collision/b2BroadPhase.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Collision/b2CollideCircle.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Collision/b2CollideEdge.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Collision/b2CollidePolygon.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Collision/b2Collision.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Collision/b2Distance.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Collision/b2DynamicTree.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Collision/b2TimeOfImpact.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Collision/Shapes/b2ChainShape.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Collision/Shapes/b2CircleShape.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Collision/Shapes/b2EdgeShape.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Collision/Shapes/b2PolygonShape.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Common/b2BlockAllocator.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Common/b2Draw.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Common/b2Math.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Common/b2Settings.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Common/b2StackAllocator.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Common/b2Timer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamics/Contacts/b2ChainAndCircleContact.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamics/Contacts/b2ChainAndPolygonContact.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamics/Contacts/b2CircleContact.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamics/Contacts/b2Contact.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamics/Contacts/b2ContactSolver.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamics/Contacts/b2EdgeAndCircleContact.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamics/Contacts/b2PolygonAndCircleContact.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamics/Contacts/b2PolygonContact.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamics/Joints/b2DistanceJoint.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamics/Joints/b2FrictionJoint.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamics/Joints/b2GearJoint.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamics/Joints/b2Joint.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamics/Joints/b2MotorJoint.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamics/Joints/b2MouseJoint.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamics/Joints/b2PrismaticJoint.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamics/Joints/b2PulleyJoint.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamics/Joints/b2RevoluteJoint.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamics/Joints/b2RopeJoint.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamics/Joints/b2WeldJoint.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamics/Joints/b2WheelJoint.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamics/b2Body.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamics/b2ContactManager.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamics/b2Fixture.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamics/b2Island.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamics/b2World.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Dynamics/b2WorldCallbacks.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Rope/b2Rope.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR} 
	${CMAKE_CURRENT_SOURCE_DIR}/includes
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)

