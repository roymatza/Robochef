
from ai2thor.controller import Controller
from tasks import ServeCoffee, ServeEgg
from scene2pddl import Problem
import scene_info as si

controller = Controller(
    agentMode="default",
    visibilityDistance=1.5,
    scene="FloorPlan3",

    # step sizes
    gridSize=0.25,
    snapToGrid=True,
    rotateStepDegrees=90,

    # image modalities
    renderDepthImage=False,
    renderInstanceSegmentation=False,

    # camera properties
    width=960,
    height=540,
    fieldOfView=100
)

last_scene = si.Scene(controller)
task1 = ServeCoffee()
task2 = ServeEgg()

scene_model = Problem(last_scene, [task1, task2])
scene_model.InitStates()
print(scene_model.GetStates())