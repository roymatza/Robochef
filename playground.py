
from ai2thor.controller import Controller
from jinja2.loaders import FileSystemLoader
from tasks import ServeCoffee, ServeEgg
from problem import Problem
from jinja2 import Environment
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
pddl_data = scene_model.GenerateDataForPDDL()

env = Environment(loader=FileSystemLoader("templates"),
    trim_blocks=True,
    lstrip_blocks=True
)

template = env.get_template("pfile.pddl")
output_from_parsed_template = template.render(data=pddl_data)

# to save the results
with open("pddl/{}.pddl".format(pddl_data["pname"]), "w") as fh:
    fh.write(output_from_parsed_template)