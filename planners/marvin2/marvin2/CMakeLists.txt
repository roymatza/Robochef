cmake_minimum_required(VERSION 2.4)
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project(marvin)

set( CMAKE_CXX_FLAGS "-m32 -Wno-deprecated")

set( CMAKE_MODULE_PATH    ${CMAKE_MODULE_PATH}
                          ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(parsing)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/parsing)

set(marvin_build_srcs
    planner.cpp Type.cpp Predicate.cpp Action.cpp Parameter.cpp Entity.cpp
    PredicateInstance.cpp ActionInstance.cpp hashdefinitions.cpp staticinformation.cpp State.cpp
    planningAlgorithms.cpp plan.cpp relaxedPlanningGraph.cpp PredicateInstanceTrie.cpp
    withrefcounter.cpp withrefcounterlist.cpp SymmetryExtractor.cpp SymmetryGroup.cpp
    MacroAction.cpp RelativeStateRecord.cpp ADLAction.cpp AbstractStuff.cpp
    ActionApplicabilityTester.cpp ActionInstanceWithSubactions.cpp ActionSequenceValidator.cpp
)


ADD_EXECUTABLE(marvin ${marvin_build_srcs})

target_link_libraries(marvin
	ParsePDDL
)


